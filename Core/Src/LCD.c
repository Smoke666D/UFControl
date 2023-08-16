/*
 * LCD.c
 *
 *  Created on: Jun 26, 2023
 *      Author: i.dymov
 */


#include "main.h"


#define MAX_CHAR  40
static uint8_t LCD_BUFFER[MAX_CHAR];
static uint8_t LCD_OUT_BUFFER[LED_STRING_LEN ];
static IDICATOR_STRING_BUFFER LCD_DATA_CHNGE = NO_CHANGE;
static EventGroupHandle_t lcdFlags;
static uint32_t local_period = 40;
extern TIM_HandleTypeDef htim7;


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



/*
 * Функция ввывода сторки по нужному положения на индикаторе.
 */
static void LCD_SetString( char * data, uint8_t pos_x, uint8_t pos_y)
{
 uint8_t k = 0;
 for (uint8_t i = pos_x*pos_y;i< MAX_CHAR;i++)
 {
	 if (data[k]!=0)
	 {
		 if (data[k]!=LCD_BUFFER[i])
		 {
			 LCD_DATA_CHNGE |= (i >20)?DOWN_STRING:UP_STRING;
			 if (data[k]>='А')
			 {
				 LCD_BUFFER[i] = russian[data[k++]-'А'];
			 }
			 else
			 {
				 LCD_BUFFER[i] = data[k++];
			 }
		 }
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

static void WriteByte( uint8_t data)
{
	uint16_t buf_port =(uint16_t) LCD_0_GPIO_Port->ODR;
	buf_port = buf_port & 0xF0;
	buf_port = buf_port | data;
	LCD_0_GPIO_Port->ODR = buf_port;
}

static void ReadByte (uint8_t * pData)
{
	*pData = (uint16_t) LCD_0_GPIO_Port->ODR & 0x0F;
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
	DelayUS(1);
	RESET_E;
}

static void LCD_SendCommand(uint8_t command)
{
	RESET_RS;
	RESET_RW;
	RESET_E;
	WriteByte( command);
	Strob();
	DelayUS(40);
}

static void LCD_SendData(uint8_t data)
{
	SET_RS;
	RESET_RW;
	RESET_E;
	WriteByte( data);
	Strob();
	DelayUS(40);
}

void Init16X2LCD()
{
	osDelay(40);
	LCD_SendCommand(LCD_FUNSTION_SET  | DB_4);
}



void vRedrawLCD()
{
	EventBits_t redraw_flags;
	redraw_flags = xEventGroupWaitBits(lcdFlags, UP_STRING | DOWN_STRING ,pdTRUE,pdFALSE,0);
	if (redraw_flags & UP_STRING )
	{
		memcpy(LCD_OUT_BUFFER,LCD_BUFFER,LED_STRING_LEN);
		LCD_SendCommand(0x80);  //Устаналвиаем адрес в начало верхней строки
		for (uint8_t i = 0;i< LED_STRING_LEN;i++)
		{
			LCD_SendData(LCD_OUT_BUFFER[i]);
		}
	}
	if (redraw_flags & DOWN_STRING )
	{
		memcpy(LCD_OUT_BUFFER,&LCD_BUFFER[LED_STRING_LEN ],LED_STRING_LEN);
		LCD_SendCommand(0x80 | 0x40);  //Устаналвиаем адрес в начало нижней строки
		for (uint8_t i = 0;i< LED_STRING_LEN;i++)
		{
			LCD_SendData(LCD_OUT_BUFFER[i]);
		}
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
