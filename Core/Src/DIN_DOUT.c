/*
 * DIN_DOUT.c
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */


#include "main.h"
#include "data_model.h"

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_rx;


static   EventGroupHandle_t pREGEvent;
static   StaticEventGroup_t xREGCreatedEventGroup;
void PL_SET()
{
	HAL_GPIO_WritePin(LampNPL_GPIO_Port,LampNPL_Pin, GPIO_PIN_SET);
}
void PL_RESET()
{
	HAL_GPIO_WritePin(LampNPL_GPIO_Port,LampNPL_Pin, GPIO_PIN_RESET);
}

#define DATA_LOAD_READY 0x01
#define DATA_VALID_TIMES 3U
static uint8_t PL_STATE = 0;
static uint8_t pDATA[6];
static uint8_t data_check_counter = 0;

uint8_t data[6];
uint8_t mask[6][DATA_VALID_TIMES];
uint8_t dc_mask[6];



void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	hspi->ErrorCode = HAL_SPI_ERROR_NONE;
	 static portBASE_TYPE xHigherPriorityTaskWoken;
		 xHigherPriorityTaskWoken = pdFALSE;
		 xEventGroupSetBitsFromISR( pREGEvent, DATA_LOAD_READY, &xHigherPriorityTaskWoken );
		 portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
		 return;
}

void RegisteDATALoadInit()
{
	 pREGEvent = xEventGroupCreateStatic(&xREGCreatedEventGroup );
}

static uint8_t RegisterDATALoad()
{
	uint8_t res = 0;
	uint8_t i,j;
	switch (PL_STATE)
	{
		case 0:
			PL_RESET();
			PL_STATE = 1;
			if (data_check_counter >= DATA_VALID_TIMES)
			{
				data_check_counter = 0;
				for (i=0;i<6;i++)
				{
					    dc_mask[i ] = mask[i][0];
						for (j=1 ;j<DATA_VALID_TIMES;j++)
						{
							dc_mask[i ] &= mask[i][j];
						}
				}
				for (i=0;i<6;i++)
				{
					data[i] ^=  dc_mask[i];
				}
				res = 1;
			}
		break;
		case 1:
			PL_SET();
			PL_STATE =2;

			break;
		case 2:
			HAL_SPI_Receive_DMA(&hspi2,pDATA,6U);
			PL_STATE = 3;
			break;
		case 3:
			EventBits_t bits =  xEventGroupGetBits( pREGEvent );
			if (bits & DATA_LOAD_READY)
			{
				xEventGroupClearBits(pREGEvent,DATA_LOAD_READY);
				PL_STATE = 0;
				for (i=0;i <6 ;i++)
				{
					mask[i][data_check_counter] = data[i] ^ pDATA[i];

				}
				data_check_counter++;
			}
			break;


	}
	return (res);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	 static portBASE_TYPE xHigherPriorityTaskWoken;
	 xHigherPriorityTaskWoken = pdFALSE;
	 xEventGroupSetBitsFromISR( pREGEvent, DATA_LOAD_READY, &xHigherPriorityTaskWoken );
	 portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	 return;

}

const  PIN_CONFIG xDinPortConfig[DIN_CHANNEL]= {
												{nDOOR_sens_Pin,nDOOR_sens_GPIO_Port},
												{nRemote_activatio_Pin,nRemote_activatio_GPIO_Port},
												{Fire_alarm_Pin,Fire_alarm_GPIO_Port},
												{local_Pin,local_GPIO_Port},
												{remote_Pin,remote_GPIO_Port},
												{KL1_Pin,KL1_GPIO_Port},
												{KL2_Pin,KL2_GPIO_Port},
												{KL3_Pin,KL3_GPIO_Port},
												{KL4_Pin,KL4_GPIO_Port},
};


#define POW1	   0
#define POW2	   1
#define POW_OUT	   2
#define POW_ON	   6
#define ALARM_OUT  3
#define ALARM_LAMP 4
#define LOCAL_LAMP 5
const PIN_CONFIG xDoutPortConfig[DOUT_CHANNEL] = {
		{POW_OUT1_Pin,POW_OUT1_GPIO_Port},
		{POW_OUT2_Pin,POW_OUT2_GPIO_Port},
		{Relay_Work_Pin,Relay_Work_GPIO_Port},
		{Relay_Crash_Pin,Relay_Crash_GPIO_Port},
		{LedR_FBO_accident_Pin,LedR_FBO_accident_GPIO_Port},
		{LedY_Local_Control_Pin,LedY_Local_Control_GPIO_Port},
		{LedG_FBO_ON_Pin,LedG_FBO_ON_GPIO_Port},
};


static DinConfig_t xDinConfig[ DIN_COUNT];

DIN_FUNCTION_ERROR_t eDinConfig( uint8_t ucCh, DIN_INPUT_TYPE inType, uint32_t ulHFront, uint32_t ulLFront)
{
	DIN_FUNCTION_ERROR_t eRes = DIN_WRONG_CHANNEL_NUMBER ;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if ( ucCh <DIN_COUNT)
	{
		xDinConfig[ucCh].eInputType = inType;
		xDinConfig[ucCh].ucValue 	= (xDinConfig[ucCh].eInputType == DIN_CONFIG_POSITIVE ) ? 0U : 1U;
		GPIO_InitStruct.Pin 		= xDinPortConfig[ucCh].Pin;

		if ( xDinConfig[ucCh].eInputType == RPM_CONFIG )
		{
			xDinConfig[ucCh].eInputType = DIN_CONFIG_POSITIVE;
		}
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(xDinPortConfig[ucCh].GPIOx,&GPIO_InitStruct);
		xDinConfig[ucCh].ulHighCounter = ulHFront;
		xDinConfig[ucCh].ulLowCounter = ulLFront;
		xDinConfig[ucCh].ucTempValue = 0U;
		eRes = DIN_CONFIG_OK;
	}
	return ( eRes );
}

static void vDINInit()
{
	RegisteDATALoadInit();
	eDinConfig( INPUT_1, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	eDinConfig( INPUT_2, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	eDinConfig( INPUT_3, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	eDinConfig( INPUT_4, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	eDinConfig( INPUT_5, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	PL_SET();
}

static void vSetOut( uint8_t out, uint8_t state)
{
	HAL_GPIO_WritePin(xDoutPortConfig[out].GPIOx,xDoutPortConfig[out].Pin, state!=0 ? GPIO_PIN_SET:GPIO_PIN_RESET);
	return;
}

void StartDIN_DOUT(void *argument)
{
	uint8_t init_state = 0;

	EventGroupHandle_t system_event = xGetSystemUpdateEvent();
	vDINInit();
	while(1)
	{
		vTaskDelay(10);
		if (xEventGroupGetBits(system_event) & SYSTEM_IDLE)
		{
			vSetOut(ALARM_OUT,  0 );
			vSetOut(ALARM_LAMP, 0 );
			vSetOut(POW1,       0 );
			vSetOut(POW2,       0 );
			vSetOut(POW_OUT,    0 );
			vSetOut(POW_ON,     0 );
			vSetOut(LOCAL_LAMP, 0 );
			xEventGroupWaitBits(system_event, SYSTEM_REINIT, pdTRUE, pdTRUE, portMAX_DELAY );
			init_state = 0;
		}
		else
		{
			for (uint8_t i = 0; i <DIN_COUNT; i++)
			{
				if ( xDinConfig[i].eInputType != RPM_CONFIG )
				{
					uint8_t uсDinState = HAL_GPIO_ReadPin( xDinPortConfig[i].GPIOx, xDinPortConfig[i].Pin);
					if (uсDinState != xDinConfig[i].ucTempValue )
					{
							xDinConfig[i].ulCounter ++ ;
							if (xDinConfig[i].ulCounter > ( (xDinConfig[i].ucTempValue == GPIO_PIN_RESET) ? xDinConfig[i].ulHighCounter : xDinConfig[i].ulLowCounter ) )
							{
									xDinConfig[i].ucValue = uсDinState  ^ ( (~xDinConfig[i].eInputType) & 0x1);

								    xDinConfig[i].ucTempValue = uсDinState ;
							}
					}
					else
					{
						xDinConfig[i].ulCounter = 0U;
					}
				}
			}
			if   (RegisterDATALoad() == 1)
			{
				uint32_t bdata = data[5] | data[4]<<8 | (data[3] & 0x3F)<<16;
				int32SetData(LAM_ERROR_REG_LSB, ~bdata & 0x003FFFFF);
				bdata = data[3]>>6 | data[1]<<10 | (data[0] & 0x0F)<<18 | data[2] <<2;
				int32SetData(LAM_ERROR_REG_MSB, ~bdata & 0x003FFFFF);
				if ( init_state == 0 )
				{
					init_state = 1;
					xEventGroupSetBits(system_event,   DIN_SYSTEM_READY );
				}
			}
			int16SetRegisterBit(DEVICE_ALARM_REG,  DOOR_ALARM ,   0);//  (uint16_t)xDinConfig[DOOR].ucValue );
			int8SetRegisterBit(DEVICE_INPUT_REG,  IN_DOOR_ALARM,  0);// (uint16_t)xDinConfig[DOOR].ucValue );
			int16SetRegisterBit(DEVICE_ALARM_REG,  FIRE_FLAG,       (uint16_t)xDinConfig[FIRE].ucValue);
			int8SetRegisterBit(DEVICE_INPUT_REG,  IN_FIRE_ALARM,   (uint16_t)xDinConfig[FIRE].ucValue );
			int8SetRegisterBit(DEVICE_STATUS_REG, REMOTE_FLAG,     (uint16_t)xDinConfig[REMOTE].ucValue);
			int8SetRegisterBit(DEVICE_INPUT_REG,  IN_REMOTE_FLAG,   (uint16_t)xDinConfig[REMOTE].ucValue );
			int8SetRegisterBit(DEVICE_STATUS_REG, LOCAL_FLAG,      (uint16_t) xDinConfig[LOCAL_C].ucValue);
			int8SetRegisterBit(DEVICE_INPUT_REG,  IN_LOCAL_FLAG,   (uint16_t)xDinConfig[LOCAL_C].ucValue );
			int8SetRegisterBit(DEVICE_STATUS_REG, REMOTE_ACT_FLAG, (uint16_t)xDinConfig[REMOTE_ACT].ucValue);
			int8SetRegisterBit(DEVICE_INPUT_REG  ,IN_REMOTE_ACT,   (uint16_t)xDinConfig[REMOTE_ACT].ucValue );
		    //управление выходами
			uint8_t output_status = int8GetRegister(DEVICE_OUTPUT_REG);
			vSetOut(ALARM_OUT,  output_status & (0x01<<ALARM_OUT_FLAG) );
			vSetOut(ALARM_LAMP, output_status & (0x01<<ALARM_OUT_FLAG) );
			vSetOut(POW1,       output_status & (0x01<<WORK_OUT_FLAG ) );
			vSetOut(POW2,       output_status & (0x01<<WORK_OUT_FLAG ) );
			vSetOut(POW_OUT,    output_status & (0x01<<WORK_OUT_FLAG ) );
			vSetOut(POW_ON,     output_status & (0x01<<WORK_OUT_FLAG ) );
			vSetOut(LOCAL_LAMP, output_status & (0x01<<LOCAL_OUT_FLAG) );
		}
	}
}


static unsigned char CODES[KEYBOARD_COUNT]                      = { kl1_key, kl2_key, kl3_key, kl4_key };
static unsigned char STATUS[KEYBOARD_COUNT]                     = { 0U };
static unsigned int  COUNTERS[KEYBOARD_COUNT]                   = { 0U };
static StaticQueue_t      xKeyboardQueue;
static QueueHandle_t      pKeyboardQueue;

uint8_t              KeyboardBuffer[ 16U * sizeof( KeyEvent ) ] = { 0U };
/*---------------------------------------------------------------------------------------------------*/
void vSetupKeyboard( void )
{
  pKeyboardQueue  = xQueueCreateStatic( 16U, sizeof( KeyEvent ), KeyboardBuffer, &xKeyboardQueue );
  xQueueReset( pKeyboardQueue );
  return;
}
/*---------------------------------------------------------------------------------------------------*/
QueueHandle_t pGetKeyboardQueue( void )
{
  return pKeyboardQueue;
}

#define KEYPAD_TIME_OUT  1000
static uint16_t key_no_press_timeout = 0;
void vKeyboardTask( void * argument )
{
  KeyEvent      TEvent;
  GPIO_PinState TK[KEYBOARD_COUNT];
  for(;;)
  {
    vTaskDelay(KEY_PEREOD);
    for ( uint8_t i=0U; i<KEYBOARD_COUNT; i++ )                                          /* Считываем текущее состояние портов клавиатуры */
    {
      TK[i]=  HAL_GPIO_ReadPin( xDinPortConfig[i+DIN_COUNT].GPIOx, xDinPortConfig[i+DIN_COUNT].Pin );
      TEvent.KeyCode = CODES[i];
	  /*Фиксируем отжатие клавищи (BRAKECODE)*/
      if ( STATUS[i] && ( TK[i] == KEY_OFF_STATE ) )
      {
        STATUS[i]      = KEY_OFF; /*Состоянии клавиши ВЫКЛ*/
        COUNTERS[i]    = 0U;      /*Сбрасываем счетчик*/

        TEvent.Status  = BRAKECODE;
        //xQueueReset( pKeyboardQueue );
        xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
        key_no_press_timeout = 0;
      }
      else
      {
        /*Если текущие состояние потрта ВКЛ, а предидущие было ВЫКЛ
        //то запускаме счеткик нажатий*/
        if ( !STATUS[i] && ( TK[i] == KEY_ON_STATE ) )
        {
          COUNTERS[i]++;
          /*если счетчик превысил значение SWITCHONDELAY то фиксируем нажатие*/
          if ( COUNTERS[i] >=  SWITCHONDELAY  )
          {
            COUNTERS[i]    = 0U;
            STATUS[i]      = KEY_ON;
            TEvent.Status  = MAKECODE;
            xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
            key_no_press_timeout = 0;
          }
        }
        else if ( (STATUS[i] != KEY_OFF)  && ( TK[i] == KEY_ON_STATE ) )
        {
          COUNTERS[i]++;
          switch ( STATUS[i] )
          {
            case KEY_ON:
              if ( COUNTERS[i] >=  DefaultDelay  )
              {
                STATUS[i]      = KEY_ON_REPEAT;
                COUNTERS[i]    = 0U;
                TEvent.Status  = MAKECODE;
                xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
                key_no_press_timeout  = 0;
              }
              break;
            case KEY_ON_REPEAT:
              if ( COUNTERS[i] >= DefaultRepeatRate )
              {
                COUNTERS[i]    = 0U;
                TEvent.Status  = MAKECODE;
                xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
                key_no_press_timeout  = 0;
              }
              break;
            default:
    	      break;
          }
        }
      }
    }
    if (++key_no_press_timeout > KEYPAD_TIME_OUT )
    {
    	key_no_press_timeout = 0;
    	key_no_press_timeout = 0U;
    	TEvent.KeyCode     = time_out;
    	TEvent.Status      = MAKECODE;
    	xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
    }
  }
  return;
}
