/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "EEPROM.h"
#include "lcd.h"
#include "DIN_DOUT.H"
#include "lamp.h"
#include "cmsis_os.h"
#include "string.h"
#include "MainFSM.h"
#include "data_model.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define REGISTER_SYSTEM_READY 0x01
#define SYSTEM_REINIT           0x02
#define DIN_SYSTEM_READY      0x04
#define WORK_READY            0x08
#define REGISTER_SYSTEM_READY_1 0x10
#define SYSTEM_READY          0x20
#define DIN_SYSTEM_REINIT           0x40
#define SYSTEM_IDLE             0x80
#define SYSTEM_STOP				0x100
#define SYSTEM_RESTART			0x200
#define LCD_OFF					0x400
#define LCD_REDRAW				0x800
#define RTC_STAMP				0x1000

EventGroupHandle_t xGetSystemUpdateEvent();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define nDOOR_sens_Pin GPIO_PIN_13
#define nDOOR_sens_GPIO_Port GPIOC
#define nRemote_activatio_Pin GPIO_PIN_0
#define nRemote_activatio_GPIO_Port GPIOC
#define Fire_alarm_Pin GPIO_PIN_1
#define Fire_alarm_GPIO_Port GPIOC
#define local_Pin GPIO_PIN_2
#define local_GPIO_Port GPIOC
#define remote_Pin GPIO_PIN_3
#define remote_GPIO_Port GPIOC
#define KL1_Pin GPIO_PIN_4
#define KL1_GPIO_Port GPIOA
#define KL2_Pin GPIO_PIN_5
#define KL2_GPIO_Port GPIOA
#define KL3_Pin GPIO_PIN_6
#define KL3_GPIO_Port GPIOA
#define KL4_Pin GPIO_PIN_7
#define KL4_GPIO_Port GPIOA
#define IND_E_Pin GPIO_PIN_4
#define IND_E_GPIO_Port GPIOC
#define Ind_R_nW_Pin GPIO_PIN_5
#define Ind_R_nW_GPIO_Port GPIOC
#define LCD_0_Pin GPIO_PIN_0
#define LCD_0_GPIO_Port GPIOB
#define LCD_1_Pin GPIO_PIN_1
#define LCD_1_GPIO_Port GPIOB
#define LCD_2_Pin GPIO_PIN_2
#define LCD_2_GPIO_Port GPIOB
#define Ind_RS_Pin GPIO_PIN_10
#define Ind_RS_GPIO_Port GPIOB
#define Ind_LED_Pin GPIO_PIN_11
#define Ind_LED_GPIO_Port GPIOB
#define LampNPL_Pin GPIO_PIN_12
#define LampNPL_GPIO_Port GPIOB
#define POW_OUT1_Pin GPIO_PIN_6
#define POW_OUT1_GPIO_Port GPIOC
#define POW_OUT2_Pin GPIO_PIN_7
#define POW_OUT2_GPIO_Port GPIOC
#define Relay_Work_Pin GPIO_PIN_8
#define Relay_Work_GPIO_Port GPIOC
#define Relay_Crash_Pin GPIO_PIN_9
#define Relay_Crash_GPIO_Port GPIOC
#define EN485_Pin GPIO_PIN_8
#define EN485_GPIO_Port GPIOA
#define LedR_FBO_accident_Pin GPIO_PIN_10
#define LedR_FBO_accident_GPIO_Port GPIOC
#define LedY_Local_Control_Pin GPIO_PIN_11
#define LedY_Local_Control_GPIO_Port GPIOC
#define LedG_FBO_ON_Pin GPIO_PIN_12
#define LedG_FBO_ON_GPIO_Port GPIOC
#define LCD_3_Pin GPIO_PIN_3
#define LCD_3_GPIO_Port GPIOB
#define LCD_4_Pin GPIO_PIN_4
#define LCD_4_GPIO_Port GPIOB
#define LCD_5_Pin GPIO_PIN_5
#define LCD_5_GPIO_Port GPIOB
#define LCD_6_Pin GPIO_PIN_6
#define LCD_6_GPIO_Port GPIOB
#define LCD_7_Pin GPIO_PIN_7
#define LCD_7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
