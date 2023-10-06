/*
 * LCD.H
 *
 *  Created on: Jun 26, 2023
 *      Author: i.dymov
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "cmsis_os.h"

#define IDICATOR_BUS_MASK 0x07F
#define IND_RS  0x40
#define IND_RW 0x20
#define IND_E  0x10
#define DB_0   0x01
#define DB_1   0x02
#define DB_2   0x04
#define DB_3   0x08
#define DB_4   0x10
#define DB_5   0x20
#define DB_6   0x40
#define DB_7   0x80

typedef  enum
{
	NO_CHANGE = 0x00,
	UP_STRING  = 0x01,
	DOWN_STRING = 0x02,
	BOTH_STRING = 0x03,
} IDICATOR_STRING_BUFFER;

#define LED_STRING_LEN  20
#define DISPALY_REDRAW_TIME 50

typedef enum
{
  LCD_CLEAR  		 = DB_0,
  LCD_RETURN_HOME    = DB_1,
  LCD_ENTRY_MODE_SET = DB_2,
  LCD_ON_OFF		 = DB_3,
  LCD_CURRSOR_SHIFT =  DB_4,
  LCD_FUNSTION_SET   = DB_5,
  LCD_SET_CGRAM_ADDR = DB_6,
  LCD_SET_DDRAM_ADDR = DB_7,
} LCD_COMMAND;


#define SET_RS HAL_GPIO_WritePin(Ind_RS_GPIO_Port,Ind_RS_Pin,GPIO_PIN_SET)
#define RESET_RS HAL_GPIO_WritePin(Ind_RS_GPIO_Port,Ind_RS_Pin,GPIO_PIN_RESET)
#define SET_RW HAL_GPIO_WritePin(Ind_R_nW_GPIO_Port,Ind_R_nW_Pin,GPIO_PIN_SET)
#define RESET_RW HAL_GPIO_WritePin(Ind_R_nW_GPIO_Port,Ind_R_nW_Pin,GPIO_PIN_RESET)
#define SET_E HAL_GPIO_WritePin(IND_E_GPIO_Port,IND_E_Pin,GPIO_PIN_SET)
#define RESET_E HAL_GPIO_WritePin(IND_E_GPIO_Port,IND_E_Pin,GPIO_PIN_RESET)
#define LCD_DELAY_EVENT  0x0001
#define LCD_UPDATE_EVENT 0x0002
void vSetTimersEnvet(osEventFlagsId_t event);
void vTimeExpire();
void LCD_Task(void *argument);
void LCD_SetString( char * data, uint8_t pos_x, uint8_t pos_y);
void LCD_SET_Cursor( uint8_t status, uint8_t x, uint8_t y);
void ClearScreenBuffer();
void LCD_Cursor_Off();
void LCD_LED_ON();
void LCD_LED_OFF();
void Init16X2LCD();
#endif /* INC_LCD_H_ */
