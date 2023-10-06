/*
 * lamp.c
 *
 *  Created on: Aug 1, 2023
 *      Author: i.dymov
 */

#include "main.h"
#include "data_model.h"

static  EventGroupHandle_t system_event = NULL;

 void HAL_RTCEx_RTCEventCallback(RTC_HandleTypeDef *hrtc)
 {
	 static portBASE_TYPE xHigherPriorityTaskWoken;
	 xHigherPriorityTaskWoken = pdFALSE;
	 if ( system_event!= NULL)
	 xEventGroupSetBitsFromISR( system_event, RTC_STAMP, &xHigherPriorityTaskWoken );
	 portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	 return;
 }


void LAMPstart(void *argument)
{
	static 	LAMP_FSM_SATE lamp_fsm = WAIT_FOR_INIT;

	static  uint8_t lamp_count = 0;
	for(;;)
	{
		switch (lamp_fsm)
		{
			case WAIT_FOR_INIT:
				system_event = xGetSystemUpdateEvent();
				if (system_event !=NULL)
				{
					xEventGroupWaitBits(system_event, SYSTEM_READY ,  pdTRUE, pdTRUE, portMAX_DELAY );
					lamp_fsm  = INIT;
				}
				break;
			case INIT:
				lamp_count = (uint8_t)int8GetRegister( LAMP_COUNT );
				lamp_fsm   = WORK;
				break;
			case WORK:
				xEventGroupWaitBits(system_event, RTC_STAMP ,  pdTRUE, pdTRUE, portMAX_DELAY );
				if  (xEventGroupGetBits(system_event) & SYSTEM_IDLE)
				{
					lamp_fsm   = LAMP_IDLE;
				}
				else
		        {
					uint32_t error_lsb = int32GetData(LAM_ERROR_REG_LSB);
					uint32_t error_msb = int32GetData(LAM_ERROR_REG_MSB);
					uint8_t error_flag = 0;
					uint8_t error_count = 0;
					uint8_t remain_resourse_3 = 0;
					uint8_t remain_resourse_0 = 0;
					for (uint8_t i = 0; i <  lamp_count; i++ )
					{
						//Получаем флаг ошиибки
						if (i < MAX_LAMP_COUNT/2)
						{
							error_flag = error_lsb & 0x01;
							error_lsb =error_lsb>>1;
						}
						else
						{
							error_flag = error_msb & 0x01;
														error_msb =error_msb>>1;

						}
						if ( int8GetRegister(DEVICE_OUTPUT_REG) & (0x01<<WORK_OUT_FLAG ))
						{
							if (!error_flag)
							{
								uint32_t new_recource_data = int32GetData(LAMP_WORK_HOURS_INDEX + i)+1;
								int32SetData (LAMP_WORK_HOURS_INDEX + i,  new_recource_data );   //Если флага ошибок нет, инкриментируем счетчик времени работы
								uint8_t resource = (uint8_t)  ( (new_recource_data/360.0) / ((float) int8GetRegister(LAMP_MAX_TIME_INDEX + i) * 1000 ) )*100;
								int8SetRegister( LAMP_RESURSE_INDEX + i , resource);
								if (resource >=97) remain_resourse_3++;
								if (resource >=100) remain_resourse_0++;
							}
							else
							{
								error_count++;
							}
						}
					}
					int8SetRegisterBit(DEVICE_ALARM_REG, REMAIN_RESOURS_3, (uint8_t)(remain_resourse_3 > 0 ) );
					int8SetRegisterBit(DEVICE_ALARM_REG, REMAIN_RESOURS_0, (uint8_t)(remain_resourse_0 > 0 ) );
					int8SetRegisterBit(DEVICE_ALARM_REG, ONE_LAMP_ERROR, (uint8_t)( error_count ==1 ) );
					int8SetRegisterBit(DEVICE_ALARM_REG, TWO_LAMP_ERROR, (uint8_t)( error_count >=2 ) );
					int8SetRegister( LAPM_ERROR_COUNT  , error_count );
		        }
				break;
			case LAMP_IDLE:
				xEventGroupWaitBits(system_event, SYSTEM_REINIT  , pdFALSE, pdTRUE, portMAX_DELAY );
				lamp_fsm   = WAIT_FOR_INIT;
				break;
			default:
				break;
		}
	}
	return;
}
