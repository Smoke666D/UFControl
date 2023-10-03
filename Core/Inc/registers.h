/*
 * registers.h
 *
 *  Created on: Aug 22, 2023
 *      Author: i.dymov
 */

#ifndef INC_REGISTERS_H_
#define INC_REGISTERS_H_

#define ONE_LAMP_ERROR  0x01
#define TWO_LAMP_ERROR  0x02

#define LAMP1_REG     (0)
#define LAMP2_REG     (LAMP1_REG + 1)
#define LAMP3_REG     (LAMP2_REG  + 1)
#define LAMP4_REG     (LAMP3_REG  + 1)
#define LAMP5_REG     (LAMP4_REG  + 1)
#define LAMP6_REG     (LAMP5_REG  + 1)
#define LAMP7_REG     (LAMP6_REG  + 1)
#define LAMP8_REG     (LAMP7_REG  + 1)
#define LAMP9_REG     (LAMP8_REG  + 1)
#define LAMP10_REG    (LAMP9_REG  + 1)
#define LAMP11_REG    (LAMP10_REG  + 1)
#define LAMP12_REG    (LAMP11_REG  + 1)
#define LAMP13_REG    (LAMP12_REG  + 1)
#define LAMP14_REG    (LAMP13_REG  + 1)
#define LAMP15_REG    (LAMP14_REG  + 1)
#define LAMP16_REG    (LAMP15_REG  + 1)
#define LAMP17_REG    (LAMP16_REG  + 1)
#define LAMP18_REG    (LAMP17_REG  + 1)
#define LAMP19_REG    (LAMP18_REG  + 1)
#define LAMP20_REG    (LAMP19_REG  + 1)
#define LAMP21_REG    (LAMP20_REG  + 1)
#define LAMP22_REG    (LAMP21_REG  + 1)
#define LAMP23_REG    (LAMP22_REG  + 1)
#define LAMP24_REG    (LAMP23_REG  + 1)
#define LAMP25_REG    (LAMP24_REG  + 1)
#define LAMP26_REG    (LAMP25_REG  + 1)
#define LAMP27_REG    (LAMP26_REG  + 1)
#define LAMP28_REG    (LAMP27_REG  + 1)
#define LAMP29_REG    (LAMP28_REG  + 1)
#define LAMP30_REG    (LAMP29_REG  + 1)
#define LAMP31_REG    (LAMP30_REG  + 1)
#define LAMP32_REG    (LAMP31_REG  + 1)
#define LAMP33_REG    (LAMP32_REG  + 1)
#define LAMP34_REG    (LAMP33_REG  + 1)
#define LAMP35_REG    (LAMP34_REG  + 1)
#define LAMP36_REG    (LAMP35_REG  + 1)
#define LAMP37_REG    (LAMP36_REG  + 1)
#define LAMP38_REG    (LAMP37_REG  + 1)
#define LAMP39_REG    (LAMP38_REG  + 1)
#define LAMP40_REG    (LAMP39_REG  + 1)
#define LAMP41_REG    (LAMP40_REG  + 1)
#define LAMP42_REG    (LAMP41_REG  + 1)
#define LAMP43_REG    (LAMP42_REG  + 1)
#define LAMP44_REG    (LAMP43_REG  + 1)
#define LAMP_REG		  (LAMP44_REG  + 1)
#define LAM_ERROR_REG_LSB (LAMP_REG +1)
#define LAM_ERROR_REG_MSB (LAM_ERROR_REG_LSB +1)
#define DEVICE_ALARM_REG ( LAM_ERROR_REG_MSB +1)
#define DEVICE_STATUS_REG (DEVICE_ALARM_REG +1)
#define H_VOLTAGE_REG  	   (DEVICE_STATUS_REG +1 )
#define L_VOLTAGE_REG	   ( DEVICE_STATUS_REG +1 )
#define REGISTER_COUNT ( L_VOLTAGE_REG +1)

#define DOOR_ALARM        1
#define MB_DOOR_ALARM     1
#define REMOTE_FLAG 	  1
#define FIRE_FLAG   	  2
#define MB_FIRE_ALARM     2
#define LOCAL_FLAG  	  2
#define REMOTE_ACT_FLAG   3
#define MB_REMOTE_FLAG    3
#define MB_LOCAL_FLAG     4
#define MB_REMOTE_ACT     5
#define ALARM_OUT_FLAG	  4
#define LOCAL_OUT_FLAG	  5
#define WORK_OUT_FLAG     6
#define SCADA_FLAG        7
#define SCADA_ON_FLAG	  8

#define FIRST_REGISTER_ADDR    0
#define FIRST_EEPROM_REG_ADDR  0
#define FLASH_REGISTER_COUNT  (LAMP_REG )

void vInitRegister();
uint32_t uGetRegister(uint8_t addres);
void vSetRegister(uint8_t addres, uint32_t data);
void vSetRegisterBit(uint8_t addres, uint8_t bits, uint8_t data);
uint8_t getRegisterBit(uint8_t addres, uint8_t bits);
#endif /* INC_REGISTERS_H_ */
