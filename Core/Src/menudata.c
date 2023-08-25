/*
 * menudata.c
 *
 *  Created on: Aug 16, 2023
 *      Author: i.dymov
 */
#include "menudata.h"
#include "data_model.h"

#define LEFT_OFFSET  0x00
#define LINE1		 0x00
#define LINE2		 0x01
#define LINE3		 0x03
#define LINE4		 0x04
#define LINE5		 0x05
#define LINE6		 0x05



//Описание экранов главонго меню
static xScreenObjet const StatusMainScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 6U, TEXT_STRING,  "Статус", 0U, 0U },
  { 0U, 8U		   , LINE1, 4U, DATA,  0U, (void*)vGetDataForMenu, 0U },
  { 0U, 12U		   , LINE1, 7U, TEXT_STRING,  "|Работа", 0U, 0U },
  { 0U, 19U		   , LINE1, 1U, DATA,  0U, (void*)vGetDataForMenu, 1U },
  { 0U, LEFT_OFFSET, LINE2, 6U, TEXT_STRING,  "Крышка", 0U, 0U },
  { 0U, 8U		   , LINE2, 4U, DATA,  0U, (void*)vGetDataForMenu, 2U },
  { 0U, 12U		   , LINE2, 7U, TEXT_STRING,  "|Аварии", 0U, 0U },
  { LAST_OBJECT, 19U		   , LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, 3U },
};

static xScreenObjet const UserMenuScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE1, 5U, TEXT_STRING,  "Лампы", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE2, 11U, TEXT_STRING,  "Ресурс ламп", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE3, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE3, 11U, TEXT_STRING,  "Замена ламп", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE4, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE4, 11U, TEXT_STRING,  "Текущие аварии", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE5, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE5, 11U, TEXT_STRING,  "Журнал аварий", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE6, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE6, 11U, TEXT_STRING,  "Дата и время", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE6, 1U, CURSOR,  0U, 0U, 0U },
  { LAST_OBJECT, 1U		   , LINE6, 11U, TEXT_STRING,  "Настройки", 0U, 0U },

};

static xScreenObjet const DevoloperMenuScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE1, 0U, TEXT_STRING,  "Количество ламп", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U },
  { LAST_OBJECT, 1U		   , LINE2, 0U, TEXT_STRING,  "Типоразмер ФБО", 0U, 0U },
};


static xScreenObjet const FBOSizeScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "Типоразмер ФБО", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE1, 17U,  EDIT_DATA_NUM,    0U,(void*)vGetDataForMenu, 4U },
  { 0U, 4U,          LINE2, 1U,  TEXT_STRING,  "x",  0U, 0U },
  { LAST_OBJECT, 15U, LINE2, 5U,  EDIT_DATA_NUM,    0U,(void*)vGetDataForMenu, 5U },
};

static xScreenObjet const LampCountScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 0U, TEXT_STRING,  "Установка количества", 0U, 0U },
  { 0U, LEFT_OFFSET, LINE2, 0U,  TEXT_STRING,  "ламп:",  0U, 0U },
  { LAST_OBJECT, 6U, 		 LINE2, 2U,  EDIT_DATA_NUM,  0U,(void*)vGetDataForMenu, 7U },

};

static xScreenObjet const LampResursScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Лампа", 0U, 0U },
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Лампа", 0U, 0U },
   { 0U, 9U, LINE1, 7U, TEXT_STRING,  "отработ", 0U, 0U },
   { 0U, 19U, LINE1, 7U, TEXT_STRING,  "%", 0U, 0U },
   { 0U, 9U, LINE2, 7U, TEXT_STRING,  "отработ", 0U, 0U },
   { 0U, 19U, LINE2, 7U, TEXT_STRING,  "%", 0U, 0U },
   { 0    , 6U , 		         LINE1, 2U,  LIST_DATA,  0U,(void*) vGetDataForList, LIST_LAMP_STATUS_ID  },
   { 0    , 6U , 		         LINE2, 2U,  LIST_DATA,  0U,(void*) vGetDataForList, LIST_LAMP_STATUS_ID  },
   { 0    , 17U , 		         LINE1, 2U,  LIST_DATA,  0U,(void*) vGetDataForList, LIST_LAMP_STATUS_COUNT },
   { LAST_OBJECT, 17 , 		     LINE2, 2U,  LIST_DATA,  0U,(void*) vGetDataForList, LIST_LAMP_STATUS_COUNT},
};

static xScreenObjet const LampRepairScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Назначить ресурс", 0U, 0U },
   { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U },
   { 0U, LEFT_OFFSET, LINE2, 5U, TEXT_STRING,  "всем лампам", 0U, 0U },
   { 0U, LEFT_OFFSET, LINE3, 5U, TEXT_STRING,  "Назначить ресурс", 0U, 0U },
   { 0U, LEFT_OFFSET, LINE4, 1U, CURSOR,  0U, 0U, 0U },
   { LAST_OBJECT, LEFT_OFFSET, LINE4, 5U, TEXT_STRING,  "лампам индвидуально", 0U, 0U },
};


static xScreenObjet const SettingsScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Адрес MODBUS", 0U, 0U },
   { 0U, 17, LINE1, 2U, EDIT_DATA_NUM,  "%02d", (void*)vGetDataForMenu, MOD_BUS_ADDRES },
   { 0U, LEFT_OFFSET, LINE2, 5U, TEXT_STRING,  "Tип управления", 0U, 0U },
   { LAST_OBJECT, 16, LINE4, 5U,  EDIT_DATA_ENUM,  NULL , (void*)vGetDataForMenu, CONTROL_TYPE },
};


static xScreenObjet const LampStatusScreen[] =
{
   { 0    , LEFT_OFFSET , 		         LINE1, 20U,  LIST_DATA,  0U,(void*)vGetDataForMenu, 8U },
   { LAST_OBJECT, LEFT_OFFSET , 		 LINE2, 20U,  LIST_DATA,  0U,(void*)vGetDataForMenu, 8U },
};


//Описание экранов главонго меню
xScreenType  xMainMenuScreens[MAIN_MENU_COUNT]=
{
  {DATA_SCREEN, StatusMainScreen,         &xMainMenu, NULL            ,0,0,0,0 },
  {MENU_SCREEN, UserMenuScreen,           &xMainMenu, &xUserMenu      ,0,LINE6,0,6 },
  {MENU_SCREEN, DevoloperMenuScreen,      &xMainMenu, &xDevoloperMenu ,0,LINE2,0,2 },

};


//Описание экранов главонго меню
xScreenType  xUserMenuScreens[USER_MENU_COUNT]=
{
  {LIST_SCREEN, LampStatusScreen,         &xUserMenu , NULL       ,0,0,0,0 },
  {LIST_SCREEN, LampResursScreen,         &xUserMenu , NULL      ,0,0,0,0  },
  {MENU_1_LINE_STRING, LampRepairScreen,  &xUserMenu , NULL      ,0,0,0,0  },
//  {LIST_SCREEN, Current_AlarmScreen,         &xUserMenu , NULL      ,0,0,0,0  },
 // {LIST_SCREEN, AlarmJournalScreen,         &xUserMenu , NULL      ,0,0,0,0  },
 // {LIST_SCREEN, DatesScreen,         &xUserMenu , NULL      ,0,0,0,0  },
  {EDIT_SCREEN, SettingsScreen,      &xUserMenu , NULL ,0,0,0,0 },

};


xScreenType  xDeviloperMenuScreens[DEVOLOPER_MENU_COUNT]=
{
  {DATA_SCREEN, LampCountScreen,          &xMainMenu, NULL     ,0,0,0,0 },
  {DATA_SCREEN, FBOSizeScreen,            &xMainMenu, NULL     ,0,0,0,0},


};


xScreenSetObject xUserMenu =
{
   xUserMenuScreens,
  ( MAIN_MENU_COUNT - 1U ),
  0U,
  ( void* )&xCommonScreenCallBack,
};

xScreenSetObject xDevoloperMenu =
{
   xDeviloperMenuScreens,
  ( DEVOLOPER_MENU_COUNT - 1U ),
  0U,
  ( void* )&xCommonScreenCallBack,
};

xScreenSetObject xMainMenu =
{
   xMainMenuScreens,
  ( MAIN_MENU_COUNT - 1U ),
  0U,
  ( void* )&xMainScreenCallBack,
};
