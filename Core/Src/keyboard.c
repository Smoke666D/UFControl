/*
 * keyboard.c
 *
 *  Created on: Oct 16, 2023
 *      Author: i.dymov
 */
#include "keyboard.h"


static xKeyDataType 	 KEYS[KEYBOARD_COUNT];
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
/*
 *
 */
static void vConfigKey( uint8_t index, GPIO_TypeDef* Port_,  uint16_t  Pin_, key_type type_, unsigned char code )
{
	if ( index < KEYBOARD_COUNT )
	{
		KEYS[index].port.KeyPort = Port_;
		KEYS[index].port.KeyPin  = Pin_;
		KEYS[index].ToDo 		  = type_;
		KEYS[index].COUNTERS	  = 0;
		KEYS[index].STATUS 	 	  = 0;
		KEYS[index].CODES		  = code;
	}
}
/*
 *
 */
static void vConfigKeyboard()
{
	vConfigKey(0,KL1_GPIO_Port,KL1_Pin,REPEAT, kl1_key);
	vConfigKey(1,KL2_GPIO_Port,KL2_Pin,REPEAT, kl2_key);
	vConfigKey(2,KL3_GPIO_Port,KL3_Pin,DELAY,  kl3_key);
	vConfigKey(3,KL4_GPIO_Port,KL4_Pin,REPEAT, kl4_key);
}

void vKeyboardTask( void * argument )
{
  KeyEvent      TEvent;
  uint16_t 		key_no_press_timeout = 0;
  vConfigKeyboard();
  for(;;)
  {
	  vTaskDelay(KEY_PEREOD);
	  for ( uint8_t i= 0U; i < KEYBOARD_COUNT; i++ )
	  {
		  KEYS[i].TK =  HAL_GPIO_ReadPin( KEYS[i].port.KeyPort, KEYS[i].port.KeyPin ); /* Считываем текущее состояние портов клавиатуры */
		  TEvent.KeyCode = KEYS[i].CODES;
		  if ( ( KEYS[i].STATUS != KEY_OFF ) && ( KEYS[i].TK == KEY_OFF_STATE ) ) /*Фиксируем отжатие клавищи (BRAKECODE)*/
		  {
			  KEYS[i].STATUS      = KEY_OFF; /*Состоянии клавиши ВЫКЛ*/
			  KEYS[i].COUNTERS    = 0U;      /*Сбрасываем счетчик*/
			  TEvent.Status 	  = BRAKECODE;
			  xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
			  key_no_press_timeout = 0;
		  }
		  else
		  {
			  if ( KEYS[i].TK == KEY_ON_STATE )
			  {
				  KEYS[i].COUNTERS++;
				  key_no_press_timeout = 0;
				  if ( (KEYS[i].STATUS == KEY_OFF) &&   ( KEYS[i].COUNTERS >=  SWITCHONDELAY  ) )
				  {
					  KEYS[i].COUNTERS    = 0U;
					  KEYS[i].STATUS      = KEY_ON;
					  TEvent.Status  	  = MAKECODE;
					  xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
				  }
				  else if  (KEYS[i].STATUS != KEY_OFF)
				  {
					  switch ( KEYS[i].STATUS )
				  	  {
				  			case KEY_ON:
				  					if (KEYS[i].ToDo == REPEAT)
				  					{
				  						if ( KEYS[i].COUNTERS >=  DefaultDelay  )
				  						{
				  							KEYS[i].STATUS      = KEY_ON_REPEAT;
				  				  			KEYS[i].COUNTERS    = 0U;
				  				  			TEvent.Status  = MAKECODE;
				  				  			xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
				  				  		}
				  				  	}
				  				  	else
				  				  	{
				  				  		if ( KEYS[i].COUNTERS >=  PressDelay   )
				  				  		{
				  				  			TEvent.Status  = DELAYCODE;
				  				  			xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
				  				  		}
				  				  	}
				  				  	break;
				  			case KEY_ON_REPEAT:
				  				  	 if ( KEYS[i].COUNTERS >= DefaultRepeatRate )
				  				  	 {
				  				  		KEYS[i].COUNTERS    = 0U;
				  				  		TEvent.Status  = MAKECODE;
				  				  		xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
				  				  	 }
				  				  	 break;
				  			default:
				  				  	 break;
				  	}
			  }
		  }
		}
    }
    if (++key_no_press_timeout > KEYPAD_TIME_OUT )
    {
    	key_no_press_timeout = 0U;
    	TEvent.KeyCode     = time_out;
    	TEvent.Status      = MAKECODE;
    	xQueueSend( pKeyboardQueue, &TEvent, portMAX_DELAY );
    }
  }
  return;
}
