/*
 * keyboard.h
 *
 *  Created on: Oct 16, 2023
 *      Author: i.dymov
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
/*------------------------------------Работа с коавиатурой---------------------*/

/*------------------------ Define --------------------------------------*/
/* Константа определяющая количесвто клавиш, которые обрабатываем драйвер */

#define KEYBOARD_COUNT     4U
#define KEY_READY	       0x0001U
#define KEY_ON_MESSAGE     0x0001U
#define KEY_OFF_MESSAGE    0x0002U
#define KEY_ON_STATE       ( GPIO_PIN_RESET )
#define KEY_OFF_STATE      ( GPIO_PIN_SET )
#define KEY_OFF            0x00U
#define KEY_ON    	       0X01U
#define KEY_ON_REPEAT      0x02U
#define KEY_PEREOD         20U
#define SWITCHONDELAY      2U
#define DefaultDelay       50U
#define DefaultRepeatRate  10U
#define PressDelay         100U
#define BRAKECODE          0x40U
#define MAKECODE           0x80U
#define DELAYCODE		   0x20U


#define kl1_key            0x01U
#define kl2_key            0x02U
#define kl3_key            0x04U
#define kl4_key            0x08U
#define kl5_key            0x10U
#define kl6_key            0x20U
#define kl7_key            0x40U
#define kl8_key            0x80U
#define time_out           0xFFU

#define USE_KEY_TIME_OUT       0
#define KEYPAD_TIME_OUT  1000
#define  KEY_TIME_OUT      1000U
/*---------------------------- Structures --------------------------------------*/
typedef struct __packed
{
  GPIO_TypeDef* KeyPort;
  uint16_t      KeyPin;
} xKeyPortStruct;
typedef struct __packed
{
  unsigned int  KeyCode;
  unsigned char Status;
} KeyEvent;
typedef struct __packed
{
  unsigned int  sKeyCode;
  unsigned char cStatus;
} xKeyEvent;

typedef enum
{
  REPEAT = 0,
  DELAY = 1,
} key_type;

typedef struct __packed
{
  xKeyPortStruct port;
  key_type       ToDo;
  uint16_t       COUNTERS;
  uint8_t        STATUS;
  GPIO_PinState  TK;
  unsigned char  CODES;
} xKeyDataType;


void vKeyboardTask( void * argument );
void vSetupKeyboard( void );
QueueHandle_t pGetKeyboardQueue( void );

#endif /* INC_KEYBOARD_H_ */
