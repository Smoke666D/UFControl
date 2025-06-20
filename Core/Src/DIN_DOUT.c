/*
 * DIN_DOUT.c
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */
#include "main.h"
#include "data_model.h"
#include "math.h"

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_rx;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc3;
extern TIM_HandleTypeDef htim8;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc3;
static uint8_t ADC_3_Convert = 0;
static uint8_t ADC_1_Convert = 0;
#define CONVERSION_NUMBER 10
#define AC_CONVERION_NUMBER 1500
#define ADC_CHANNEL 2
#define R2   75
#define R1   470
static uint16_t ADC3_DMABuffer[CONVERSION_NUMBER];
static int16_t ADC1_DMABuffer[AC_CONVERION_NUMBER*ADC_CHANNEL];
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
#define ADC3_DATA_READY 0x02
#define ADC1_DATA_READY 0x04
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
#define POW_ON	   4
#define ALARM_OUT  3
#define ALARM_LAMP 6
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
	eDinConfig( INPUT_1, DIN_CONFIG_NEGATIVE , 100, 100 );
	eDinConfig( INPUT_2, DIN_CONFIG_NEGATIVE , 100, 100 );
	eDinConfig( INPUT_3, DIN_CONFIG_POSITIVE , 100, 100 );
	eDinConfig( INPUT_4, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	eDinConfig( INPUT_5, DIN_CONFIG_NEGATIVE , DEF_H_FRONT, DEF_L_FRONT );
	PL_SET();
}

static void vSetOut( uint8_t out, uint8_t state)
{
	HAL_GPIO_WritePin(xDoutPortConfig[out].GPIOx,xDoutPortConfig[out].Pin, state!=0 ? GPIO_PIN_SET:GPIO_PIN_RESET);
	return;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC3)
	{
	 static portBASE_TYPE xHigherPriorityTaskWoken;
	 xHigherPriorityTaskWoken = pdFALSE;
	 xEventGroupSetBitsFromISR( pREGEvent, ADC3_DATA_READY, &xHigherPriorityTaskWoken );
	 portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}
	else
	{
		 static portBASE_TYPE xHigherPriorityTaskWoken;
		xHigherPriorityTaskWoken = pdFALSE;
		xEventGroupSetBitsFromISR( pREGEvent, ADC1_DATA_READY, &xHigherPriorityTaskWoken );
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	}
		 return;
}

static void vDecNetural(int16_t * data)
{
 for (int16_t i = 0;i<AC_CONVERION_NUMBER*ADC_CHANNEL;i=i+ADC_CHANNEL)
 {
   data[i]   = data[i]   - data[i+1];
 }
  return;
}

float  xADCRMS(int16_t * source, uint16_t size, uint8_t cc )
{
  uint64_t sum =0;
  for (uint16_t i=0;i<size*cc;i=i+cc)
  {
    sum =sum+ source[i]*source[i];
  }
  sum =sum/size;
  return  sqrt(sum);

}


/*
 * Сервисная функция для вычисления максимального значения
 *  source  -адрес буфера DMA
 *  off     -номер канала, скоторым работает функция
 *  size    -размер буфера для обработки
 */
int16_t  xADCMax( int16_t * source,  uint16_t * delay )
{

  int16_t max =0;
  uint16_t del =0,i;

  for (i=0;i<AC_CONVERION_NUMBER*ADC_CHANNEL;i=i+ADC_CHANNEL)
  {
      if ( source[i ] >  max)
      {
        max = source[i];
        del = i;
      }
  }
  *delay = (del/ADC_CHANNEL);
  return ( max );

}

void  vADCSetFreqDetectParam(int16_t AMP,uint8_t * del,uint8_t * fd)
{
  switch (AMP/100)
         {
           case 0:
             *del = 0;
             *fd =  70;
             break;
           case 1:
             *del = 2;
             *fd =  40;
             break;
           case 2:
             *del = 4;
             *fd =  25;
             break;
           case 3:
             *del = 5;
             *fd =  20;
             break;
           case 4:
             *del = 7;
             *fd =  15;
             break;
           case 5:
             *del = 8;
             *fd =  10;
             break;
           case 6:
             *del = 11;
             *fd =  8;
             break;
           case 7:
             *del = 12;
             *fd =  7;
             break;
           case 8:
             *del = 14;
             *fd =  6;
             break;
           case 9:
             *del = 15;
             *fd =  5;
             break;
           case 10:
             *del = 16;
             *fd =  4;
             break;
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
           case 16:
           case 17:
           case 18:
           case 19:
           case 20:
           default:
             *del = 20;
             *fd =  4;
             break;
         }
}

#define MAX_ZERO_POINT 20
#define FASE_DETECT_HISTERESIS  30

void vADCFindFreq(int16_t * data, uint16_t * count, uint8_t off, int16_t AMP)
{
  uint8_t AMP_DELTA = 15,
                 FD = 15,
                 F1 = 0,
                 F2 = 0,
                 CNT = 0,
                 index = 0;

  uint16_t tt=0;
  uint16_t PER[MAX_ZERO_POINT];

  vADCSetFreqDetectParam(AMP,&AMP_DELTA,&FD);

  for (uint16_t i=FD<<1;i<(AC_CONVERION_NUMBER-FD)<<1;i=i+2)
  {
    //Если значение попадет в корридор окло нуля
    if ((data[i] > -AMP_DELTA) && (data[i] < AMP_DELTA))
    {
        //то прверяем текущую фазу
        if ((data[i] > data[i-FD*4]) || (data[i] < data[i+FD*4]))
            F2  = 1U;
        else
            F2 = 0U;
        if (CNT > 0)
        {
          if (((F2==1) && (F1 == 1)) || ((F2==0) &&(F1==0)))
          {
            index = 5;
            break;
          }
        }
        F1 = F2;
        PER[index++] =i/2;
        i = i+ FD*4;
        CNT++;
        if (index> MAX_ZERO_POINT) break;
    }
  }
  tt =0;
  if (index>1)
  {
    for (uint8_t i =1;i<index;i++)
    {
      tt =tt+ + PER[i]-PER[i-1];
    }
    tt = (tt/(index-1))*2;
  }
  *count = tt;

  return;
}
#define A 220

uint16_t vRCFilter( uint16_t input,uint16_t * old_output)

{

	volatile uint32_t new = input;
	volatile uint32_t old = *old_output;


	volatile uint16_t  output =  ( A * old + (256-A)*new )>>8;
	//*old_input = input;
	*old_output = output;
	return output;
}

static uint8_t V250_ALARM = 0;
static uint8_t V187_ALARM = 0;
static uint8_t V198_ALARM = 0;

#define DELAY_START 200
uint8_t switch_off = 0;
uint8_t GetLowPowerState()
{
	return switch_off;
}


void StartDIN_DOUT(void *argument)
{
	uint8_t init_state = 0;
	uint8_t start_flag  = 0;
	uint16_t uCurPeriod = AC_CONVERION_NUMBER-1;
	int16_t iMax =0;
    uint16_t old= 0;
	uint16_t DF1;
    uint16_t power_on_dealy = 0;
	uint16_t delay = 0;
	uint8_t ac_ready = 0;
	uint16_t start_delay = 0;
	EventGroupHandle_t system_event = xGetSystemUpdateEvent();
	vDINInit();
	HAL_TIM_Base_Start(&htim8);
	while(1)
	{
		vTaskDelay(10);
		if (ADC_3_Convert == 0)
		{
			ADC_3_Convert = 1;
			HAL_ADC_Start_DMA(&hadc3, (uint32_t* )&ADC3_DMABuffer, CONVERSION_NUMBER );
		}
		if ((start_flag) && (delay <( DELAY_START+10)))
		{
			delay++;
		}
		if ((ADC_1_Convert == 0) && (start_flag))
		{
			if ( (ac_ready == 0) &&  (++start_delay > 250) )
			{
				xEventGroupSetBits(system_event,AC_CONTROL_READY);
				ac_ready = 1;
			}
			ADC_1_Convert = 0;
			HAL_ADC_Start_DMA(&hadc1, (uint32_t* )&ADC1_DMABuffer, AC_CONVERION_NUMBER*ADC_CHANNEL );

		}
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
			ac_ready = 0;
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
							xDinConfig[i].ulCounter ++;
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
				/*
				 * Для исключения ложны срабатываний данные обновляем только через 5 с. после включения ламп
				 */
				uint8_t out_on = int8GetRegister(DEVICE_OUTPUT_REG) & (0x01<<WORK_OUT_FLAG);
				if (out_on )
				{
					if (++power_on_dealy> 60) power_on_dealy = 130;
					if (power_on_dealy  >= 60 )
					{
						uint32_t bdata = data[5] | data[4]<<8 | (data[3] & 0x3F)<<16;
						int32SetData(LAM_ERROR_REG_LSB, bdata & 0x003FFFFF);
						bdata = data[3]>>6 | data[1]<<10 | (data[0] & 0x0F)<<18 | data[2] <<2;
						int32SetData(LAM_ERROR_REG_MSB, bdata & 0x003FFFFF);
					}
				}
				else
				{
					power_on_dealy =0;
					int32SetData(LAM_ERROR_REG_LSB, 0);
					int32SetData(LAM_ERROR_REG_MSB, 0);
				}
				if ( init_state == 0 )
				{
					init_state = 1;
					xEventGroupSetBits(system_event,   DIN_SYSTEM_READY );
				}
			}
			int8SetRegisterBit(DEVICE_ALARM_REG,  DOOR_ALARM ,(uint16_t)xDinConfig[DOOR].ucValue );
        	int8SetRegisterBit(DEVICE_INPUT_REG,  IN_DOOR_ALARM,  (uint16_t)xDinConfig[DOOR].ucValue );
			int8SetRegisterBit(DEVICE_ALARM_REG,  FIRE_FLAG,       (uint16_t)xDinConfig[FIRE].ucValue);
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

			EventBits_t bits =  xEventGroupGetBits( pREGEvent );
			if (bits & ADC3_DATA_READY)
			{
				HAL_ADC_Stop_DMA(&hadc3);
				ADC_3_Convert = 0;
				xEventGroupClearBits(pREGEvent,ADC3_DATA_READY);
				uint32_t buf_data = 0;
				for (uint8_t i=0;i <CONVERSION_NUMBER;i++)
				{
					buf_data = buf_data+ADC3_DMABuffer[i];
				}
				buf_data = buf_data/CONVERSION_NUMBER;
				float VDD = buf_data*(3.3/4095);
				VDD = VDD*(R1+R2)/R2;
				if (VDD < 10)
				{
					if (switch_off ==0)
					{
						switch_off = 1;
						vLAMWorkHoursWrite();
					}
				}
				else
				{
					switch_off = 0;
					start_flag = 1;
				}
			}
			if (bits & ADC1_DATA_READY)
			{
				HAL_ADC_Stop_DMA(&hadc1);
				xEventGroupClearBits(pREGEvent,ADC1_DATA_READY);
				vDecNetural(&ADC1_DMABuffer[0]); //частота дискритизации 40КHZ
				iMax=xADCMax((int16_t *)&ADC1_DMABuffer[0], &DF1);
				vADCFindFreq((int16_t *)&ADC1_DMABuffer[0], &uCurPeriod,2,iMax);
				if ((uCurPeriod < AC_CONVERION_NUMBER-1) && (uCurPeriod!=0) )
				{
				    uint16_t data1 = xADCRMS(&ADC1_DMABuffer[0],uCurPeriod,2);
					data1 = vRCFilter(data1, &old);
					data1 = (float)data1 * ( 401U * 3.3 / 4095U );
					int8SetRegister(V220,(uint8_t)data1 );
					if ( data1 >= int8GetRegister(VHIGH) )
					{
						V250_ALARM= 1;
					}
					if ((data1 <= int8GetRegister(VHIGH_ON) ) && (V250_ALARM) )
					{
						V250_ALARM = 0;
					}
					int8SetRegisterBit(DEVICE_ALARM_REG,VOLT_250 ,  V250_ALARM );
					if ( (data1 >= int8GetRegister( VLOW_ON ) ) && (V187_ALARM) ) V187_ALARM = 0;
					if  (data1 <= int8GetRegister( VLOW ) )
					{
						V187_ALARM++;
						if (V187_ALARM >=10) V187_ALARM=10;
						if ((switch_off) || (delay <DELAY_START)) V187_ALARM = 0;
					}
					int8SetRegisterBit(DEVICE_ALARM_REG,VOLT_187 , (V187_ALARM>=3)?1:0);
					if  ((data1 <= int8GetRegister(WWAR)) && (V187_ALARM == 0))
					{
						V198_ALARM++;
						if (V198_ALARM >=10) V198_ALARM=10;
						if ((switch_off)  || (delay <DELAY_START)) V198_ALARM = 0;
					}
					if ((data1 >= int8GetRegister(WWAR_ON)) && (V198_ALARM)) V198_ALARM = 0;
					int8SetRegisterBit(DEVICE_ALARM_REG,VOLT_198 , (V198_ALARM>=3)?1:0 );

				}

			}

		}
	}
}




