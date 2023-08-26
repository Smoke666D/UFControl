/*
 * control.c
 *
 *  Created on: Aug 25, 2023
 *      Author: i.dymov
 */


#include "control.h"



void StartControlTask(void *argument)
{
	CONTROLLER_STATE_t state = CONTROLLER_INIT;
	EventGroupHandle_t system_event = NULL;
	while(1)
	{
		vTaskDelay(1);
		switch (state)
		{
			case CONTROLLER_INIT:
				system_event = xGetSystemUpdateEvent();
				if (system_event !=NULL)
				{
					vInitRegister();
					xEventGroupWaitBits(system_event,   REGISTER_SYSTEM_READY | DIN_SYSTEM_READY,  pdTRUE, pdTRUE, portMAX_DELAY );
					xEventGroupSetBits(system_event, WORK_READY);
					state = CONTROLLER_WORK;
				}
				break;
			case CONTROLLER_WORK:
				if (uGetRegister(DEVICE_ALARM_REG)  )
				{
					state = CONTROLLER_ALARM;
				}
				break;
			case CONTROLLER_ALARM:
				vSetRegisterBit(DEVICE_STATUS_REG, WORK_OUT_FLAG,  0);
				vSetRegisterBit(DEVICE_STATUS_REG, ALARM_OUT_FLAG, 1);
				vSetRegisterBit(DEVICE_STATUS_REG, LOCAL_OUT_FLAG, 0);
				break;
			case CONTROLLER_REINIT:
				break;
			case CONTROLLER_SHOTDOWN:
				xEventGroupClearBits(system_event, WORK_READY);
				break;
		}
	}
}
