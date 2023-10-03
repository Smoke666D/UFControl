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
#define  DEVICE_INPUT		   8
#define  DEVICE_HOLDING_START  (DEVICE_INPUT_START + DEVICE_INPUT)
#define  DEVICE_HOLDING        10
#define  DEVICE_HOLDING_FLASG  7
#define REG_COUNT 10



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
#endif /* INC_MAINFSM_H_ */
