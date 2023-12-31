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
#define DOUT_CHANNEL  7

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
#define REMOTE INPUT_5
#define FIRE INPUT_3
#define LOCAL_C INPUT_4
#define REMOTE_ACT INPUT_2



uint8_t GetLowPowerState();
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void StartDIN_DOUT(void *argument);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
#endif /* INC_DIN_DOUT_H_ */
