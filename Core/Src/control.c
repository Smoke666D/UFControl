/*
 * control.c
 *
 *  Created on: Aug 25, 2023
 *      Author: i.dymov
 */


#include "control.h"
#include "data_model.h"


uint8_t GetLowPowerState()
{
	return 0;
}

void StartControlTask(void *argument)
{
	CONTROLLER_STATE_t state =  CONTROLLER_IDLE;
	EventGroupHandle_t system_event = xGetSystemUpdateEvent();
	uint8_t control_type = 0;
	uint8_t LOW_POWER = 0;
	uint8_t START = 0;
	uint8_t WARNNING_MASK = 0;
	xEventGroupSetBits(system_event, SYSTEM_REINIT);
	while(1)
	{
	   vTaskDelay(1);
	   if ( GetLowPowerState() )
	   {
		   xEventGroupSetBits(system_event,  LCD_OFF);
		   xEventGroupSetBits(system_event,SYSTEM_IDLE);
		   state = CONTROLLER_IDLE;
		   if (START == 1)
		   {
			   START = 0;
			   vLAMWorkHoursWrite();
		   }
	   }
	   else
	   {
		   if (START == 0)
		   {
			   START = 1;
			   xEventGroupClearBits(system_event,  LCD_OFF);
			   state = CONTROLLER_INIT;
		   }
	   }
	   if  (( xEventGroupGetBits(system_event) & SYSTEM_RESTART) && (state==CONTROLLER_IDLE))
	   {
		  state = CONTROLLER_INIT;
		  xEventGroupClearBits(system_event,   SYSTEM_STOP);
		  xEventGroupSetBits(system_event, SYSTEM_REINIT);
	   }
	   if  (( xEventGroupGetBits(system_event) & SYSTEM_STOP) && (state!=CONTROLLER_IDLE))
	   {
		   xEventGroupSetBits(system_event,SYSTEM_IDLE);
		   state = CONTROLLER_IDLE;
	   }
	   int8SetRegisterBit( DEVICE_OUTPUT_REG, ALARM_OUT_FLAG,  int8GetRegister(DEVICE_ALARM_REG) & ERROR_LAM_FLAG );


		switch (state)
		{
			case CONTROLLER_INIT:
				xEventGroupClearBits(system_event,   SYSTEM_IDLE);
				InitDataModel();
				xEventGroupWaitBits(system_event,    DIN_SYSTEM_READY,  pdTRUE, pdTRUE, portMAX_DELAY );
				xEventGroupClearBits(system_event,   SYSTEM_REINIT);
				xEventGroupSetBits(system_event,     SYSTEM_READY);
				xEventGroupSetBits(system_event,     MB_START);
				state = CONTROLLER_WORK;
				control_type =  int8GetRegister(CONTROL_TYPE_REG );
				break;
			case CONTROLLER_WORK:
				if (WARNNING_MASK != int8GetRegister(DEVICE_ALARM_REG))
				{
					uint8_t mask = (( WARNNING_MASK ^ int8GetRegister(DEVICE_ALARM_REG) ) & int8GetRegister(DEVICE_ALARM_REG));
					if (mask)
					{
						vADDRecord(mask);
					}
					WARNNING_MASK = int8GetRegister(DEVICE_ALARM_REG);
				}
				if (int8GetRegister(DEVICE_ALARM_REG) & DEVICE_ERROR_MASK )
				{
					state = CONTROLLER_ALARM;

				}
				else
				{
					uint8_t status = int8GetRegister(DEVICE_STATUS_REG);

					if ( status & ( 0x01<<LOCAL_FLAG ) )
					{
						int8SetRegisterBit(DEVICE_OUTPUT_REG,WORK_OUT_FLAG, 1 );
						int8SetRegisterBit(DEVICE_OUTPUT_REG,LOCAL_OUT_FLAG,1 );
						break;
					}
					if (status  & ( 0x01<<REMOTE_FLAG ))
					{
						int8SetRegisterBit(DEVICE_OUTPUT_REG,LOCAL_OUT_FLAG,0);
						if (control_type == SCADA_TYPE )
						{
							int8SetRegisterBit(DEVICE_OUTPUT_REG,WORK_OUT_FLAG, int8GetRegister(SCADA_CONTROL_REG) );

						}
						else
						{
							int8SetRegisterBit(DEVICE_OUTPUT_REG,WORK_OUT_FLAG, (status  & ( 0x01<< REMOTE_ACT_FLAG)) );
						}
						break;
					}
					int8SetRegisterBit(DEVICE_OUTPUT_REG,WORK_OUT_FLAG, 0 );
					int8SetRegisterBit(DEVICE_OUTPUT_REG,LOCAL_OUT_FLAG,0);
				}
				break;
			case CONTROLLER_ALARM:
				if (( int8GetRegister(DEVICE_ALARM_REG)  & DEVICE_ERROR_MASK )== 0 )
				{
						state = CONTROLLER_WORK;
				}
				int8SetRegisterBit(DEVICE_OUTPUT_REG,WORK_OUT_FLAG, 0 );
				int8SetRegisterBit(DEVICE_OUTPUT_REG,LOCAL_OUT_FLAG,0);
				break;
			case CONTROLLER_IDLE:
			default:
				break;
			/*case CONTROLLER_SHOTDOWN:
				xEventGroupSetBits(system_event,SYSTEM_IDLE);
				vLAMWorkHoursWrite();
				break;*/
		}
	}
}
