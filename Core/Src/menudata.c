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

static char LEFT_DATA[]= {LEFT_OFFSET};

//Описание экранов главонго меню
static xScreenObjet const StatusMainScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 6U, TEXT_STRING,  "Статус", 0U, 0U, NULL  },
 // { 0U, 8U		   , LINE1, 4U, DATA,  0U, (void*)vGetDataForMenu, 0U },
  { 0U, 12U		   , LINE1, 7U, TEXT_STRING,  "/Работа", 0U, 0U, NULL  },
 // { 0U, 19U		   , LINE1, 1U, DATA,  0U, (void*)vGetDataForMenu, 1U },
  { 0U, LEFT_OFFSET, LINE2, 6U, TEXT_STRING,  "Крышка", 0U, 0U, NULL  },
 // { 0U, 8U		   , LINE2, 4U, DATA,  0U, (void*)vGetDataForMenu, 2U },
  { 0u, 12U		   , LINE2, 7U, TEXT_STRING,  "/Аварии", 0U, 0U, NULL  },
  { LAST_OBJECT, 19U		   , LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, 3U, NULL  },
};

static xScreenObjet const UserMenuScreen[]=
{
 // { 0U, LEFT_OFFSET, LINE1, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE1, 5U, TEXT_STRING,  "Лампы", 0U, 0U, NULL  },
 // { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U },
  { 0U, 1U		   , LINE2, 11U, TEXT_STRING,  "Ресурс ламп", 0U, 0U, NULL  },
  { LAST_OBJECT, 19U		   , LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, 3U, NULL  },
};

static xScreenObjet const DevoloperMenuScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 1U, CURSOR,  0U, 0U, 0U, NULL  },
  { 0U, 1U		   , LINE1, 0U, TEXT_STRING,  "Количество ламп", 0U, 0U, NULL  },
  { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U, NULL  },
  { LAST_OBJECT, 1U		   , LINE2, 0U, TEXT_STRING,  "Типоразмер ФБО", 0U, 0U , NULL },
};

static xScreenObjet const LampErrorMenuScreen[]=
{
  { 0U, 0U		   , LINE1, 0U, TEXT_STRING,  " Ламп. Ошибок", 0U, 0U, NULL  },
  { 0u, 19U		   , LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, 3U , NULL },
  { LAST_OBJECT, 19U, LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, 3U, NULL  },
};




static xScreenObjet const FBOSizeScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "Типоразмер ФБО", 0U, 0U , NULL },
  { 0U, LEFT_OFFSET, LINE1, 17U,  EDIT_DATA_NUM,    0U,(void*)vGetDataForMenu, 4U, NULL  },
  { 0U, 4U,          LINE2, 1U,  TEXT_STRING,  "x",  0U, 0U },
  { LAST_OBJECT, 15U, LINE2, 5U,  EDIT_DATA_NUM,    0U,(void*)vGetDataForMenu, 5U, NULL  },
};

static xScreenObjet const LampCountScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 0U, TEXT_STRING,  "Установка количества", 0U, 0U, NULL  },
  { 0U, LEFT_OFFSET, LINE2, 0U,  TEXT_STRING,  "ламп:",  0U, 0U, NULL  },
  { LAST_OBJECT, 6U, 		 LINE2, 2U,  EDIT_DATA_NUM,  0U,(void*)vGetDataForMenu, 7U, NULL  },

};

static xScreenObjet const LampResursScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Текущие аварии", 0U, 0U, NULL  },
   { LAST_OBJECT, 6U, 		 LINE2, 2U,  EDIT_DATA_NUM,  0U,(void*)vGetDataForMenu, 7U, NULL },
};
static xScreenObjet const LampMaxResursScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Ресурс лампы:", 0U, 0U, NULL  },
   { 0U, 15U, LINE1, 2U, DATA,  0U, (void*)vGetDataForMenu,  SCREEN_INDEX_ID, &LEFT_DATA  },
   { 0U, 17U, LINE1, 0U, TEXT_STRING,  "/", 0U, 0U , NULL },
   { 0U, 18U, LINE1, 2, DATA,  0U, (void*)vGetDataForMenu,  TOTAL_LAMP_DATA_ID, &LEFT_DATA  },
   { LAST_OBJECT, 6U, LINE2, 2U,  DATA,  0U,(void*)vGetDataForMenu, LAMP_RES_DATA_ID, NULL  },
};

static xScreenObjet const  LampErrorScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Ресурс лампы %", 0U, 0U, NULL  },
   { LAST_OBJECT, 6U, 		 LINE2, 2U,  EDIT_DATA_NUM,  0U,(void*)vGetDataForMenu, 7U, NULL  },
};

static xScreenObjet const LampRepairScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Назначить ресурс", 0U, 0U , NULL },
   { 0U, LEFT_OFFSET, LINE2, 1U, CURSOR,  0U, 0U, 0U , NULL },
   { 0U, LEFT_OFFSET, LINE2, 5U, TEXT_STRING,  "всем лампам", 0U, 0U , NULL },
   { 0U, LEFT_OFFSET, LINE3, 5U, TEXT_STRING,  "Назначить ресурс", 0U, 0U, NULL  },
   { 0U, LEFT_OFFSET, LINE4, 1U, CURSOR,  0U, 0U, 0U, NULL  },
   { LAST_OBJECT, LEFT_OFFSET, LINE4, 5U, TEXT_STRING,  "лампам индвидуально", 0U, 0U, NULL  },
};


static xScreenObjet const SettingsScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Адрес MODBUS", 0U, 0U , NULL },
   { 0U, 17, LINE1, 2U, EDIT_DATA_NUM,  "%02d", (void*)vGetDataForMenu, MOD_BUS_ADDRES , NULL },
   { 0U, LEFT_OFFSET, LINE2, 5U, TEXT_STRING,  "Tип управления", 0U, 0U , NULL },
   { LAST_OBJECT, 16, LINE4, 5U,  EDIT_DATA_ENUM,  NULL , (void*)vGetDataForMenu, CONTROL_TYPE, NULL  },
};


static xScreenObjet const LampStatusScreen[] =
{
   { 0    , LEFT_OFFSET , 		         LINE1, 20U,  LIST_DATA,  0U,(void*)vGetDataForMenu, 8U , NULL },
   { LAST_OBJECT, LEFT_OFFSET , 		 LINE2, 20U,  LIST_DATA,  0U,(void*)vGetDataForMenu, 8U , NULL },
};


//Описание экранов главонго меню
xScreenType  xMainMenuScreens[MAIN_MENU_COUNT]=
{
  { StatusMainScreen,         &xMainMenu, &xStatusMenu           ,0,0,0,0 },
  { UserMenuScreen,           &xMainMenu, NULL            ,0,0,0,0  },

};

//Описание экранов главонго меню
xScreenType  xStatusMenuScreens[STATUS_MENU_COUNT]=
{
  {LampErrorMenuScreen,      &xMainMenu, NULL    ,0,0,0,0  },
  {LampResursScreen,         &xMainMenu, NULL    ,0,0,0,0  },
  {LampErrorScreen,          &xMainMenu, NULL    ,0,0,0,0  },
  {LampMaxResursScreen,      &xMainMenu, NULL    ,0,0,0,0  }

};


xScreenSetObject xStatusMenu =
{
   xStatusMenuScreens,
  ( STATUS_MENU_COUNT - 1U ),
  0U,
  ( void* )&xMainScreenCallBack,
};



xScreenSetObject xMainMenu =
{
   xMainMenuScreens,
  ( MAIN_MENU_COUNT - 1U ),
  1U,
  ( void* )&xMainScreenCallBack,
};
