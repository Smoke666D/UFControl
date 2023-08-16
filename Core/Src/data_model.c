/*
 * data_model.c
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */

#include "main.h"

uint16_t DATA_MODEL_REGISTER[REGISTER_COUNT];


void InitDataModel()
{


}

void SetData(uint16_t index, uint16_t data)
{
	if (index < REGISTER_COUNT)
	{
		DATA_MODEL_REGISTER[index] = data;
	}

}
uint16_t GetData(uint16_t index)
{
	return (index < REGISTER_COUNT) ? DATA_MODEL_REGISTER[index] : 0U;

}
 void vUPLOADDataToEEPROM()
 {

 }

 void vDOWNLOADDataFromEEPROM()
 {

 }
