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
static IDICATOR_STRING_BUFFER LCD_DATA_CHNGE = NO_CHANGE;

static uint32_t local_period = 40;
static uint8_t cursor_pos = 0;
static uint8_t cur_cursor_pos = 0;
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

/*ventFlagsId_t event)
{
	osTimers = event;
}*/

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

 uint8_t Data[20];
 convertUtf8ToCp1251(data,Data);
 volatile char temp;
 for (uint8_t i = (pos_x+ (pos_y*20));i< MAX_CHAR;i++)
 {
	 if (Data[k] !=0)
	 {
		 if (Data[k] >=192)
			temp = russian[Data[k] -192];
		 else
		 {
			 if (Data[k]=='|') 			 temp = 0xB5;
			 else
				temp = Data[k];

		 }
	      LCD_DATA_CHNGE |= (i >20)?DOWN_STRING:UP_STRING;
		  LCD_BUFFER[i] = temp ;
		  k++;
	 }
	 else
	 {
		 break;
	 }
 }
 if (LCD_DATA_CHNGE !=NO_CHANGE)
 {
	 xEventGroupSetBits(lcdFlags,LCD_DATA_CHNGE );
 }
}
/*
 *
 *
 */
void LCD_SET_Cursor( uint8_t status, uint8_t x, uint8_t y)
{


}
void LCD_Cursor_Off()
{

}

static void WriteByte( uint8_t data)
{
	/*GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = LCD_0_Pin|LCD_1_Pin|LCD_2_Pin | LCD_3_Pin|LCD_4_Pin
	                          |LCD_5_Pin|LCD_6_Pin|LCD_7_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
*/

	LCD_0_GPIO_Port->ODR = (LCD_0_GPIO_Port->ODR & 0xFF00) | data;
}

static void ReadByte (uint8_t * pData)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	 GPIO_InitStruct.Pin = LCD_0_Pin|LCD_1_Pin|LCD_2_Pin | LCD_3_Pin|LCD_4_Pin
	                          |LCD_5_Pin|LCD_6_Pin|LCD_7_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	*pData = (uint16_t) LCD_0_GPIO_Port->ODR & 0x0F;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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

static uint8_t ReadByt()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
		 GPIO_InitStruct.Pin = LCD_0_Pin|LCD_1_Pin|LCD_2_Pin | LCD_3_Pin|LCD_4_Pin
		                          |LCD_5_Pin|LCD_6_Pin|LCD_7_Pin;
		  GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	RESET_RS;
	SET_RW;
	//Strob();
	SET_E;
	DelayUS(10);
	uint8_t data = HAL_GPIO_ReadPin(GPIOB,LCD_7_Pin)<<7 |
			HAL_GPIO_ReadPin(GPIOB,LCD_6_Pin)<<6 |
			 HAL_GPIO_ReadPin(GPIOB,LCD_5_Pin)<<5 |
			HAL_GPIO_ReadPin(GPIOB,LCD_4_Pin)<<4 |
			 HAL_GPIO_ReadPin(GPIOB,LCD_3_Pin)<<3 |
			 HAL_GPIO_ReadPin(GPIOB,LCD_2_Pin)<<2 |
			 HAL_GPIO_ReadPin(GPIOB,LCD_1_Pin)<<1 |
			 HAL_GPIO_ReadPin(GPIOB,LCD_0_Pin) ;
	RESET_E;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		 HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	return (uint8_t) data ;

}

void Init16X2LCD()
{
	volatile uint8_t temp;
	lcdFlags= xEventGroupCreateStatic(&lcdFlagCreatedEventGroup );
	HAL_GPIO_WritePin(Ind_LED_GPIO_Port,Ind_LED_Pin, GPIO_PIN_SET);
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
	EventBits_t redraw_flags;

	redraw_flags = xEventGroupWaitBits(lcdFlags, UP_STRING | DOWN_STRING ,pdTRUE,pdFALSE,0);
	if (redraw_flags & UP_STRING )
	{
		memcpy(ddd,LCD_BUFFER,LED_STRING_LEN);
		for (uint8_t i = 0;i < LED_STRING_LEN;i++)
		{
			if (ddd[i]!=LCD_OUT_BUFFER[i])
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
	}
	if (redraw_flags & DOWN_STRING )
	{
		memcpy(ddd,&LCD_BUFFER[LED_STRING_LEN ],LED_STRING_LEN);

		for (uint8_t i = 0;i< LED_STRING_LEN;i++)
		{
			if (ddd[i]!=LCD_OUT_BUFFER[i+ LED_STRING_LEN])
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
	}


}
/*
 * Таск перерисовывет данные на LCD
 */
void LCD_Task(void *argument)
{
	Init16X2LCD();
	while(1)
	{
		vTaskDelay(DISPALY_REDRAW_TIME);
		vRedrawLCD();
	}
}
