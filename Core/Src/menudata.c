/*
 * menudata.c
 *
 *  Created on: Aug 16, 2023
 *      Author: i.dymov
 */
#include "menu.h"


#define LEFT_OFFSET  0x00
#define LINE1		 0x00
#define LINE2		 0x01

//Описание экранов главонго меню
static xScreenObjet const MainScreenData[]=
{
  { 0U, LEFT_OFFSET, LINE1, 6U, TEXT_STRING,  "Статус", 0U, 0U },
  { 0U, 8U		   , LINE1, 4U, DATA,  0U, (void*)vGetDataForMenu, 0U },
  { 0U, 12U		   , LINE1, 7U, TEXT_STRING,  "|Работа", 0U, 0U },
  { 0U, 19U		   , LINE1, 1U, DATA,  0U, (void*)vGetDataForMenu, 1U },
  { 0U, LEFT_OFFSET, LINE2, 6U, TEXT_STRING,  "Крышка", 0U, 0U },
  { 0U, 8U		   , LINE2, 4U, DATA,  0U, (void*)vGetDataForMenu, 2U },
  { 0U, 12U		   , LINE2, 7U, TEXT_STRING,  "|Аварии", 0U, 0U },
  { 1U, 19U		   , LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, 3U },
};

static xScreenType const MainScreen =
{
		DATA_SCREEN,
		MainScreenData,
		0,
		0,
		0,
		( void* )&xInfoScreenCallBack,
		0,
};

static xScreenObjet const UserMenu[]=
{
  { 0U, LEFT_OFFSET, LINE1, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE1, 5U, TEXT_STRING,  "Лампы", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE2, 11U, TEXT_STRING,  "Ресурс ламп", 0U, 0U },
};

