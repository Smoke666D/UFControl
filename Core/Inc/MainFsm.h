/*
 * MainFsm.h
 *
 *  Created on: Aug 21, 2023
 *      Author: i.dymov
 */

#ifndef INC_MAINFSM_H_
#define INC_MAINFSM_H_

#include "main.h"
#include "FreeRTOS.h"
#include "event_groups.h"

#define  DEVICE_DINPUT_START   0
#define  DEVICE_DINPUT         0
#define  DEVICE_COIL_START     (DEVICE_DINPUT_START + DEVICE_DINPUT)
#define  DEVICE_COIL		   0
#define  DEVICE_INPUT_START    ( DEVICE_COIL_START + DEVICE_COIL)
#define  DEVICE_INPUT		   102
#define  DEVICE_HOLDING_START  (DEVICE_INPUT_START + DEVICE_INPUT)
#define  DEVICE_HOLDING        13


#define SCADA_CONTROL_OFFSET    0x00
#define CONFIG_OFFSET			0x01
#define ADDRESS_OFFSET			0x02
#define COMMAND_OSFFET			0x03
#define DATA_OFFSET 			0x04
#define DATE_OFFSET				0x05
#define MOUNTH_OFFSET           0x06
#define YEAR_OFFSET				0x07
#define HOUR_OFFSET				0x08
#define MINUTE_OFFSET           0x09
#define SECOND_OFFSET			0x0A
#define ID_OFFSET				0x0B


#define SCADA_CONTROL_ADDR  (0)
#define CONFIG_ENABLE_REG   (SCADA_CONTROL_ADDR +1)
#define ADDRESS_REG 		(CONFIG_ENABLE_REG  +1)
#define COMMAND_REG 		(ADDRESS_REG +1)
#define DATA_REG    		(COMMAND_REG +1)


#define RESET_RESOURSE 		0x5501
#define RESET_RESOIRSE_ALL  0x5500
#define SET_RESOURCE        0x4401
#define SET_RESOURCE_ALL	0x4400
#define RESET_JOURNAL       0x2200
#define READ_RECORD			0X2201
#define SET_DATE			0x3300
#define READ_DATE           0x3301

typedef enum
 {
   INPUT_STATUS = 0,
   DEVICE_STATUS = 1,
   IN_AIR_TEMP =2,
   FSM_STATUS = 3,
   WATER_VALVE = 4,
   FAN_SPEED = 5,
   DOOR_STATE_TRIGGER = 6,
   ERROR_STATUS = 7,
   MODE = 0,
   FAN_SPEED_CONFIG = 1,
   WORK_TEMP = 2,
   AIR_TEMP = 3,
#ifdef MASTER_MODE
   CONTROL_MODE = 4,
   DEVICE_COUNT = 5,
   DEVICE_TYPE = 6,
   ERROR_MASTER_STATUS = 7,
#endif
   PWM = 4,
   ADC1_DATA = 5,
   ADC2_DATA = 6,
 } REGS_t;
void StartMb(void *argument);
void vUPDATECoils( uint8_t rw);
void vSetRegInputBit(REGS_t reg_addr,uint16_t offset,  uint16_t data );
uint16_t usGetRegInput( uint16_t reg_addr);
uint16_t usGetReg( uint16_t reg_addr);
void rvvTIMERExpiredISR();
void vSetReg(REGS_t reg_addr, uint16_t data);
#endif /* INC_MAINFSM_H_ */
