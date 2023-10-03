/*
 * registers.c
 *
 *  Created on: Aug 22, 2023
 *      Author: i.dymov
 */
#include "main.h"
#include "registers.h"
#include "datastorage.h"

static uint32_t REGISTER[REGISTER_COUNT];

void vInitRegister()
{
	REGISTE_DATA_TYPE_t data_type;
	//Во первых инициализируем блок данных
	 uint8_t record_format[2]={RECORD_TIME_STAMP,RECORD_BYTE};
	 eCreateDataStorage(FLASH_REGISTER_COUNT,record_format,2);
	 for (uint8_t i= 0; i< FLASH_REGISTER_COUNT;i++ )
	 {
		data_type = eEEPROMReadRegister(FIRST_EEPROM_REG_ADDR + i, &REGISTER[FIRST_REGISTER_ADDR + i]);
		if (data_type!= INTEGER_DATA)
		{
			 REGISTER[FIRST_REGISTER_ADDR + i] = 0;
			 eEEPROMRegTypeWrite(FIRST_EEPROM_REG_ADDR + i,&REGISTER[FIRST_REGISTER_ADDR + i], INTEGER_DATA);
		}
	 }
}

void vSetRegister(uint8_t addres, uint32_t data)
{
	if ( addres < REGISTER_COUNT )
	{
		REGISTER[addres] = data;
	}
}
void vSetRegisterBit(uint8_t addres, uint8_t bits, uint8_t data)
{
	if ( addres < REGISTER_COUNT )
	{
		if (data != 0)
			REGISTER[addres] |= 0x1 << bits;
		else
			REGISTER[addres] &= ~(0x1 << bits);
	}

}
uint8_t getRegisterBit(uint8_t addres, uint8_t bits)
{

}

uint32_t uGetRegister(uint8_t addres)
{
	return  (( addres < REGISTER_COUNT ) ? REGISTER[addres] : 0);

}
void vDecRegister(uint8_t addres, uint32_t step)
{

}
void vIncRegister(uint8_t addres, uint32_t step)
{

}
void vSaveRegister( uint8_t first_addres, uint8_t dest_addres, uint8_t len)
{

}

