/*
 * DIN_DOUT.h
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */

#ifndef INC_DIN_DOUT_H_
#define INC_DIN_DOUT_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#define KEY_CHANNEL  4
#define DIN_CHANNEL  9
#define DIN_COUNT    (DIN_CHANNEL - KEY_CHANNEL)

#define DEF_H_FRONT 10U
#define DEF_L_FRONT 10U

#define up_key             0x01U
#define down_key           0x02U
#define enter_key          0x04U
#define exit_key           0x08U



typedef struct {
	uint32_t Pin;
	GPIO_TypeDef * GPIOx;
} PIN_CONFIG;

typedef enum {
	DIN_CONFIG_NEGATIVE = 0U,
	DIN_CONFIG_POSITIVE = 1U,
	RPM_CONFIG = 2U
} DIN_INPUT_TYPE;

typedef struct DinConfigDef_t
{
uint32_t      ulCounter;
uint32_t 	  ulLowCounter;
uint32_t 	  ulHighCounter;
uint8_t 	  ucValue;
uint8_t 	  ucTempValue;
DIN_INPUT_TYPE eInputType;

} DinConfig_t;

typedef enum
{
 DIN_CONFIG_OK = 0,
 DIN_WRONG_CHANNEL_NUMBER  =1
} DIN_FUNCTION_ERROR_t;


typedef enum {
		INPUT_1 = 0U,
		INPUT_2 = 1U,
		INPUT_3 = 2U,
		INPUT_4 = 3U,
		INPUT_5 = 4U,
		INPUT_6 = 5U,
		INPUT_7 = 6U,
		INPUT_8 = 7U,
		INPUT_9 = 8U,
		INPUT_10 = 9U,
		INPUT_11 = 10U,
		INPUT_12 = 11U,
		INPUT_13 = 12U,
		INPUT_14 = 13U,
		INPUT_15 = 14U,
		INPUT_16 = 15U,
		INPUT_17 = 16U,
		INPUT_18 = 17U,
		INPUT_19 = 18U,
		INPUT_20 = 19U
} DIN_INPUT_NAME;

#define KL1 INPUT_6
#define KL2 INPUT_7
#define KL3 INPUT_8
#define KL4 INPUT_9
#define DOOR INPUT_1
#define REMOTE INPUT_2
#define FIRE INPUT_3
#define LOCAL_C INPUT_4
#define REMOTE_ACT INPUT_5

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
#define KEY_PEREOD         10U
#define SWITCHONDELAY      2U
#define DefaultDelay       10U
#define DefaultRepeatRate  40U
#define BRAKECODE          0x40U
#define MAKECODE           0x80U


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
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void StartDIN_DOUT(void *argument);

#endif /* INC_DIN_DOUT_H_ */
