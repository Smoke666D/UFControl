/*
 * lamp.c
 *
 *  Created on: Aug 1, 2023
 *      Author: i.dymov
 */

#include "main.h"
#include "registers.h"

LAMP_t lamp_data[MAX_LAMP_COUNT];


static void vResetLamp()
{
	for (uint8_t i = 0; i < MAX_LAMP_COUNT; i++)
		{
			lamp_data[i].state = 0;
			lamp_data[i].timer = 0;
			lamp_data[i].work_time = 0;
		}
}


void InitData()
{
	for (uint8_t i = 0; i < MAX_LAMP_COUNT; i++)
	{
		lamp_data[i].state = 0;
		lamp_data[i].timer = 0;
	}
	//for (uint8_t i = 0; i < MAX_LAMP_COUNT; i++)
	//{
	//	lamp_data[i].index =
	//}
}
void vSaveData()
{
//	uint8_t data_buffer[REGISTER_SIZE*MAX_LAMP_COUNT];
	for (uint8_t i = 0; i< MAX_LAMP_COUNT; i++)
	{

	}

}


static 	LAMP_FSM_SATE lamp_fsm = WAIT_FOR_INIT;

void LAMPstart(void *argument)
{
	EventGroupHandle_t system_event = NULL;
	uint8_t lamp_count = 0;

	for(;;)
	{
		switch (lamp_fsm)
		{
			case WAIT_FOR_INIT:
				system_event = xGetSystemUpdateEvent();
				if (system_event !=NULL)
				{
					xEventGroupWaitBits(system_event,   WORK_READY,  pdFALSE, pdTRUE, portMAX_DELAY );
					lamp_fsm  = INIT;
				}
				break;
			case INIT:
				lamp_count = uGetRegister(LAMP_REG);
				lamp_fsm   = WORK;
				break;
			case WORK:
				if (xEventGroupWaitBits(system_event, WORK_READY |  LAMP_REINIT, pdFALSE, pdTRUE, portMAX_DELAY ) & LAMP_REINIT )
				{
					lamp_fsm = INIT;
				}
				else
				{
					uint32_t error_lsb = uGetRegister(LAM_ERROR_REG_LSB);
					uint32_t error_msb = uGetRegister(LAM_ERROR_REG_MSB);
					uint8_t error_flag = 0;
					uint8_t error_count = 0;
					for (uint8_t i = 0; i <  lamp_count; i++ )
					{
							if (i < MAX_LAMP_COUNT/2 )
							{
								error_flag = (error_lsb >> i) & 0x01;
							}
							else
							{
								error_flag = (error_msb >> i) & 0x01;
							}
							if (!error_flag)
							{
								 vIncRegister(LAMP1_REG + i,1);
							}
							else
							{
								error_count++;
							}
					}
					if (error_count ==1 )
					{
						vSetRegisterBit(DEVICE_ALARM_REG, ONE_LAMP_ERROR, 1);
					}
					if (error_count >=2 )
					{
						vSetRegisterBit(DEVICE_ALARM_REG, TWO_LAMP_ERROR, 1);
					}
				}
			break;

		}
		vTaskDelay(1000);
	}
}
