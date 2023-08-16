/*
 * lamp.c
 *
 *  Created on: Aug 1, 2023
 *      Author: i.dymov
 */

#include "main.h"


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

void LAMPstart(void *argument)
{
	InitData();
	for(;;)
	{
		vTaskDelay(1000);

		for (uint8_t i = 0; i < MAX_LAMP_COUNT; i++)
		{
			//lamp_data[i].state =
			if (lamp_data[i].state == ACTIVE )
			{
				lamp_data[i].timer++;
			}
		}
	}
}
