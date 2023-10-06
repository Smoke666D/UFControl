/*
 * LCD.c
 *
 *  Created on: Jun 26, 2023
 *      Author: i.dymov
 */


#include "main.h"


#define MAX_CHAR  40
static uint8_t LCD_BUFFER[MAX_CHAR];
static uint8_t LCD_OUT_BUFFER[ MAX_CHAR  ];


extern TIM_HandleTypeDef htim7;
static   EventGroupHandle_t lcdFlags;
static   StaticEventGroup_t lcdFlagCreatedEventGroup;

const unsigned char russian[]={ 0x41, 0xA0, 0x42, 0xA1, 0xE0, 0x45,
0xA3, 0xA4, 0xA5,0xA6, 0x4B, 0xA7, 0x4D, 0x48, 0x4F, 0xA8, 0x50,0x43,
0x54, 0xA9, 0xAA, 0x58, 0xE1, 0xAB, 0xAC, 0xE2, 0xAD,0xAE, 0x62,
0xAF, 0xB0, 0xB1, 0x61, 0xB2, 0xB3, 0xB4, 0xE3, 0x65, 0xB6, 0xB7,
0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0x6F, 0xBE, 0x70, 0x63,0xBF,
0x79, 0xE4, 0x78, 0xE5, 0xC0, 0xC1, 0xE6, 0xC2,0xC3, 0xC4, 0xC5,
0xC6, 0xC7 };


static void LCD_SendCommand(uint8_t command);

void ClearScreenBuffer()
{
	memset(LCD_BUFFER,0,LED_STRING_LEN *2 );
}

/*
 * Функция ввывода сторки по нужному положения на индикаторе.
 */
void LCD_SetString( char * data, uint8_t pos_x, uint8_t pos_y)
{
 uint8_t k = 0;
 char temp;
 for (uint8_t i = (pos_x+ (pos_y*20));i< MAX_CHAR;i++)
 {
	 if (data[k] !=0)
	 {
		 if (data[k] >=192)
			temp = russian[data[k] -192];
		 else
		 {
			 if (data[k]=='|') 			 temp = 0xB5;
			 else
				temp = data[k];

		 }
		  LCD_BUFFER[i] = temp ;
		  k++;
	 }
	 else
	 {
		 break;
	 }
 }

}
/*
 *
 *
 */
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t cursor_satatus = 0;

void LCD_SET_Cursor( uint8_t status, uint8_t x, uint8_t y)
{
	cursor_x = x;
	cursor_y = y;
	cursor_satatus = status;

}
void LCD_Cursor_Off()
{

}

static void WriteByte( uint8_t data)
{
	LCD_0_GPIO_Port->ODR = (LCD_0_GPIO_Port->ODR & 0xFF00) | data;
}



static void DelayUS( uint16_t value)
{
	TIM6->ARR = value;
	TIM6->CNT = 0;//Обнуляем счётный регист
	TIM6->CR1 |= TIM_CR1_CEN;//Запускаем таймер, разрешаем его работу.
	while((TIM6->SR & TIM_SR_UIF)==0){} //Ждём установки флага UIF = счёт закончен
	TIM6->SR &=~ TIM_SR_UIF;//Сброс флага.
}

static void Strob()
{
	SET_E;
	DelayUS(3);
	RESET_E;
}

static void LCD_SendCommand(uint8_t command)
{
	RESET_RS;
	RESET_RW;
	RESET_E;
	WriteByte( command);
	Strob();
	DelayUS(45);
}

static void LCD_SendData	(uint8_t data)
{
	SET_RS;
	RESET_RW;
	RESET_E;
	WriteByte( data);
	Strob();
	DelayUS(50);
}

void Init16X2LCD()
{

	lcdFlags= xEventGroupCreateStatic(&lcdFlagCreatedEventGroup );
	//HAL_GPIO_WritePin(Ind_LED_GPIO_Port,Ind_LED_Pin, GPIO_PIN_SET);
	RESET_E;
	osDelay(100);
	LCD_SendCommand(LCD_FUNSTION_SET  | DB_4 );
	osDelay(6);
	LCD_SendCommand(LCD_FUNSTION_SET  | DB_4 );
	osDelay(1);
	LCD_SendCommand(LCD_FUNSTION_SET  | DB_4 );
	osDelay(1);
	LCD_SendCommand(LCD_FUNSTION_SET  | DB_4| DB_3  | DB_2 );
	LCD_SendCommand(LCD_CLEAR );
	LCD_SendCommand(LCD_ON_OFF  | DB_2 );
	osDelay(10);
	LCD_SendCommand(LCD_ENTRY_MODE_SET | DB_1 );
	LCD_SendCommand(LCD_CURRSOR_SHIFT| DB_3 | DB_2);
}




void vRedrawLCD()
{
    uint8_t ddd[20];


	memcpy(ddd,LCD_BUFFER,LED_STRING_LEN);
	for (uint8_t i = 0;i < LED_STRING_LEN;i++)
	{
		//if (ddd[i]!=LCD_OUT_BUFFER[i])
		{

			LCD_SendCommand(0x80 |  i);
			if (i==0)
			{
				LCD_SendCommand(LCD_RETURN_HOME);
				vTaskDelay(2);
			}
			else
			LCD_SendCommand(0x80 | i);
			if (ddd[i] == 0)
			{
				LCD_SendData(0x20);
			}
			else
			{
				LCD_SendData(ddd[i]);
			}
		}
	}
	memcpy(LCD_OUT_BUFFER,LCD_BUFFER,LED_STRING_LEN);
	memcpy(ddd,&LCD_BUFFER[LED_STRING_LEN ],LED_STRING_LEN);

	for (uint8_t i = 0;i< LED_STRING_LEN;i++)
	{
		//if (ddd[i]!=LCD_OUT_BUFFER[i+ LED_STRING_LEN])
		{
			LCD_SendCommand(0x80 | 0x40 | i);
			if (ddd[i] == 0)
			{
				LCD_SendData(0x20);
			}
			else
			{
				LCD_SendData(ddd[i]);
			}
		}
	}
	memcpy(&LCD_OUT_BUFFER[LED_STRING_LEN ],&LCD_BUFFER[LED_STRING_LEN ],LED_STRING_LEN);

	if (cursor_satatus == 1)
		  {
			  uint8_t com = 0x80 | cursor_x;
			  if (cursor_y==1)  com = com |  0x40;
			  LCD_SendCommand(com );
			  LCD_SendCommand(LCD_ON_OFF  | DB_2 | DB_1 | DB_0);
		  }
		  else
		  {
			  LCD_SendCommand(LCD_ON_OFF  | DB_2 );
		  }

}

static uint8_t LCD_LED_State = 0;
void LCD_LED_ON()
{
	LCD_LED_State = 1;
}
void LCD_LED_OFF()
{
	LCD_LED_State = 0;
}

/*
 * Таск перерисовывет данные на LCD
 */
void LCD_Task(void *argument)
{
	EventGroupHandle_t system_event = xGetSystemUpdateEvent();
	Init16X2LCD();
	LCD_LED_ON();
	while(1)
	{
		vTaskDelay(DISPALY_REDRAW_TIME);
		if (xEventGroupGetBits(system_event) & LCD_OFF)
		{
			HAL_GPIO_WritePin(Ind_LED_GPIO_Port,Ind_LED_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(Ind_LED_GPIO_Port,Ind_LED_Pin, (LCD_LED_State == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
			xEventGroupWaitBits(system_event, LCD_REDRAW,  pdTRUE, pdTRUE, portMAX_DELAY );
			vRedrawLCD();
		}
	}
}
