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

static uint8_t LEFT_DATA[]=   {LEFT_ALIGN,  0, 2, 3 };
static uint8_t RIGTH_DATA[]=  {RIGTH_ALIGN, 0, 2, 3 };
static uint8_t CENTER_DATA[]= {CENTER_ALIGN,0, 3, 3 };
static uint8_t CENTER_DATA3[]= {CENTER_ALIGN,0, 1, 3 };
static uint8_t CENTER_DATA4[]= {CENTER_ALIGN,0, 1, 4 };


//Описание экранов главонго меню
static xScreenObjet const StatusMainScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 6U, TEXT_STRING,  "Статус-", 0U, 0U, NULL  },
  { 0U, 10U		   , LINE1, 4U, DATA,  0U, (void*)vGetDataForMenu, STATUS_ID, RIGTH_DATA  },
  { 0U, 11U		   , LINE1, 7U, TEXT_STRING,  " Работа-", 0U, 0U, NULL  },
  { 0U, 19U		   , LINE1, 1U, DATA,  0U, (void*)vGetDataForMenu, WORK_ID ,RIGTH_DATA  },
  { 0U, LEFT_OFFSET, LINE2, 6U, TEXT_STRING,  "Крышка-", 0U, 0U, NULL  },
  { 0U, 10U		   , LINE2, 4U, DATA,  0U, (void*)vGetDataForMenu, OPEN_ID ,RIGTH_DATA },
  { 0u, 11U		   , LINE2, 7U, TEXT_STRING,  " Аварии-", 0U, 0U, NULL  },
  { LAST_OBJECT, 19U, LINE2, 1U, DATA,  0U, (void*)vGetDataForMenu, ALARM_ID, RIGTH_DATA   },
};

static xScreenObjet const SettingMenuScreen[]=
{

  { 0U, 0U		   , LINE1, 4U, DATA,  0U, (void*)vGetDataForMenu, CUR_TIME_ID, CENTER_DATA  },
  { 0U, 0U		   , LINE2, 5U, TEXT_STRING,  "Aдр:", 0U, 0U, NULL  },
  { 0U, 5U		   , LINE2, 1U, DATA,  0U, (void*)vGetMbTypeMenu,0 , RIGTH_DATA   },
  { 0U, 7U		   , LINE2, 11U, TEXT_STRING,  "Упр.", 0U, 0U, NULL  },
  { LAST_OBJECT, 19U, LINE2, 1U, DATA,  0U, (void*)vGetControlTypeMenu, 0, RIGTH_DATA    },
};


static xScreenObjet const DeviceInfoScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Количество ламп", 0U, 0U , NULL },
   { 0U, 19U, LINE1, 1, DATA,  0U, (void*)vGetRecourceForMenu,  TOTAL_LAMP_DATA_ID, RIGTH_DATA  },
   { 0U, LEFT_OFFSET, LINE2, 5U, TEXT_STRING,  "Размер:", 0U, 0U , NULL },
   { LAST_OBJECT, 19, LINE2, 2U, EDIT_DATA,  NULL, (void*)vGetDataForMenu, FBO_SIZE_ID , RIGTH_DATA    },
};

static xScreenObjet const DeviceHardInfoScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Напряжение сети:", 0U, 0U , NULL },
   { LAST_OBJECT, 10U, LINE2, 1, DATA,  0U, (void*)vGetRecourceForMenu, GET_V220, CENTER_DATA  },


};

static xScreenObjet const JournalInfoScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Записей в журнале:", 0U, 0U , NULL },
   { LAST_OBJECT, 10U, LINE2, 1, DATA,  0U, (void*)vGetJournal, TOTAL_RECORD, CENTER_DATA  },


};


static xScreenObjet const LampErrorScreen[]  =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Текущие аварии:", 0U, 0U, NULL  },
   { 0, 16U,  LINE1, 2U,  DATA,  0U,(void*)vGetErrorForMenu, SCREEN_INDEX_ID, RIGTH_DATA },
   { 0U, 17U, LINE1, 0U, TEXT_STRING,  "/", 0U, 0U , NULL },
   { 0U, 18U, LINE1, 2, DATA,  0U, (void*)vGetErrorForMenu,   TOTAL_ERROR_ID, LEFT_DATA  },
   { LAST_OBJECT, 10U, LINE2, 7U,  DATA,  0U,(void*)vGetErrorForMenu, ERROR_DATA_ID, CENTER_DATA },
};

static xScreenObjet const xJournalScreen[]=
{
	{ 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "N", 0U, 0U, NULL  },
	{0U, 2U, LINE1, 1, DATA,  0U, (void*)vGetJournal, CUR_RECORD, LEFT_DATA  },
	{ 0U, 19U, LINE1, 1, DATA,  0U, (void*)vGetJournal, ALARM_STATUS, RIGTH_DATA  },
	{ LAST_OBJECT, 0U, LINE2, 1, DATA,  0U, (void*)vGetJournal, ALARM_TIME_STATUS, LEFT_DATA  },
};

static xScreenObjet const LampErrorMenuScreen[]=
{
  { 0, 0U		   , LINE1, 0U, TEXT_STRING,  " Ламп. Ошибок", 0U, 0U, NULL  },
  { 0 , 16U, LINE1, 2U, DATA,  0U, (void*)vGetLapmErrorForMenu,  SCREEN_INDEX_ID, RIGTH_DATA  },
  { 0U, 17U, LINE1, 0U, TEXT_STRING,  "/", 0U, 0U , NULL },
  { 0U, 18U, LINE1, 2, DATA,  0U, (void*)vGetLapmErrorForMenu,   TOTAL_LAMP_ERROR_ID, LEFT_DATA  },
  { LAST_OBJECT, 10U, LINE2, 7U,  DATA,  0U,(void*)vGetLapmErrorForMenu, LAMP_ERROR_DATA_ID, CENTER_DATA },
};

static xScreenObjet const LampMaxResursScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Ресурс лампы:", 0U, 0U, NULL  },
   { 0U, 16U, LINE1, 2U, DATA,  0U, (void*)vGetRecourceForMenu,  SCREEN_INDEX_ID, RIGTH_DATA  },
   { 0U, 17U, LINE1, 0U, TEXT_STRING,  "/", 0U, 0U , NULL },
   { 0U, 18U, LINE1, 2, DATA,  0U, (void*)vGetRecourceForMenu,  TOTAL_LAMP_DATA_ID, LEFT_DATA  },
   { LAST_OBJECT, 10U, LINE2, 7U,  DATA,  0U,(void*)vGetRecourceForMenu, LAMP_RES_DATA_ID, CENTER_DATA },
};

static xScreenObjet const LampResursScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Ресурс лампы", 0U, 0U, NULL  },
   { 0U, 16U, LINE1, 2U, DATA,  0U, (void*)vGetRecourceForMenu,  SCREEN_INDEX_ID, RIGTH_DATA  },
   { 0U, 17U, LINE1, 0U, TEXT_STRING,  "/", 0U, 0U , NULL },
   { 0U, 18U, LINE1, 2, DATA,  0U, (void*)vGetRecourceForMenu,  TOTAL_LAMP_DATA_ID, LEFT_DATA  },
   { LAST_OBJECT, 10U, LINE2, 2U,  DATA,  0U,(void*)vGetRecourceForMenu, LAMP_RES_PROCENT_DATA_ID , CENTER_DATA },
};




static xScreenObjet const xYesNoScreen[]=
{
	{ 0U, 0, LINE1, 5U, TEXT_STRING,  "Применить изменения?", 0U, 0U, CENTER_DATA  },
	{ LAST_OBJECT, 0, LINE2, 5U, TEXT_STRING,  "  ДА          НЕТ", 0U, 0U, CENTER_DATA  },

};



static xScreenObjet const LampReSetRecourceScreen[] =
{
	{ 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Сбросить наработку", 0U, 0U, NULL  },
	{ LAST_OBJECT, 10U, LINE2, 2U,  EDIT_DATA,  0U,(void*)vResetRecourceLamp, LAMP_RESET_DATA_ID , CENTER_DATA },
};

static xScreenObjet const AllLampSetRecourceScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Назначить рес. всем", 0U, 0U , NULL },
   { LAST_OBJECT, 10, LINE2, 2U, EDIT_DATA,  NULL, (void*)vSetAllResLampMenu, 0 , CENTER_DATA  },
};

static xScreenObjet const LampSetRecourceScreen[] =
{
	{ 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Ресурс лампы", 0U, 0U, NULL  },
	{ 0U, 16U, LINE1, 2U, DATA,  0U, (void*)vSetRecourceForMenu,  SCREEN_INDEX_ID, RIGTH_DATA  },
	{ 0U, 17U, LINE1, 0U, TEXT_STRING,  "/", 0U, 0U , NULL },
	{ 0U, 18U, LINE1, 2, DATA,  0U, (void*)vSetRecourceForMenu,  TOTAL_LAMP_DATA_ID, LEFT_DATA  },
	{ LAST_OBJECT, 10U, LINE2, 2U,  EDIT_DATA,  0U,(void*)vSetRecourceForMenu, LAMP_RES_DATA_ID , CENTER_DATA },
};

static xScreenObjet const TimeSetScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Установить время", 0U, 0U , NULL },
   { LAST_OBJECT, 10, LINE2, 2U, MULTI_EDIT_DATA,  NULL, (void*)vSetTimeForMenu, NULL , CENTER_DATA },
};

static xScreenObjet const DateSetScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Установить дату", 0U, 0U , NULL },
   { LAST_OBJECT, 10, LINE2, 2U, MULTI_EDIT_DATA,  NULL, (void*)vSetDateForMenu, NULL , CENTER_DATA },
};

static xScreenObjet const ControlTypeScreen[]=
{
	{ 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Tип управления", 0U, 0U , NULL },
	{ LAST_OBJECT, 10, LINE2, 5U,  EDIT_DATA,  NULL , (void*)vGetControlTypeMenu, 0, CENTER_DATA },
};

static xScreenObjet const MBScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Адрес MODBUS", 0U, 0U , NULL },
   { LAST_OBJECT, 10, LINE2, 2U, EDIT_DATA,  NULL, (void*)vGetMbTypeMenu, 0 , CENTER_DATA  },
};



static xScreenObjet const LampCountScreen[] =
{
   { 0U, LEFT_OFFSET, LINE1, 5U, TEXT_STRING,  "Количество ламп", 0U, 0U , NULL },
   { LAST_OBJECT, 18U, LINE1, 2, EDIT_DATA,  0U, (void*)vEditLampCount,  TOTAL_LAMP_DATA_ID, RIGTH_DATA },
};

static xScreenObjet const FBOASizeScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "Типоразмер ФБО - A", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  MULTI_EDIT_DATA,    0U,(void*)vGetFBOSizeForMenu, A_SIZE, CENTER_DATA4  },
};

static xScreenObjet const FBOBSizeScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "Типоразмер ФБО -B", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  MULTI_EDIT_DATA,    0U,(void*)vGetFBOSizeForMenu, B_SIZE, CENTER_DATA4  },
};




static xScreenObjet const  VLOWScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "V нижний порог", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  EDIT_DATA,    0U,(void*)vGetVoltForMenu,  GET_V187, CENTER_DATA },
};
static xScreenObjet const  VLOWONScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "V отмены ниж. порог", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  EDIT_DATA,    0U,(void*)vGetVoltForMenu,  GET_V187_ON, CENTER_DATA },
};
static xScreenObjet const  VWARScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "V предупр.", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  EDIT_DATA,    0U,(void*)vGetVoltForMenu,  GET_V197, CENTER_DATA },
};
static xScreenObjet const VWARONScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "V отмены предупр", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  EDIT_DATA,    0U,(void*)vGetVoltForMenu,  GET_V197_ON, CENTER_DATA },
};
static xScreenObjet const  VHIGHScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "V верхний порог", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  EDIT_DATA,    0U,(void*)vGetVoltForMenu,  GET_V250, CENTER_DATA },
};
static xScreenObjet const  VHIGHONScreen[]=
{
  { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "V отмены верх. порог", 0U, 0U , NULL },
  { LAST_OBJECT, 10U, LINE2, 5U,  EDIT_DATA,    0U,(void*)vGetVoltForMenu,  GET_V250_ON, CENTER_DATA },
};

static xScreenObjet const xPasswordScreen[]=
{
	 { 0U, LEFT_OFFSET, LINE1, 14U, TEXT_STRING,  "Введите пароль:", 0U, 0U , NULL },
	  { LAST_OBJECT, 10U, LINE2, 5U,  MULTI_EDIT_DATA,    0U,(void*)vGetPassword, 0, CENTER_DATA4 },
};


//Описание экранов главонго меню

xScreenType  xMainMenuScreens[MAIN_MENU_COUNT]=
{
  { StatusMainScreen,         &xMainMenu, &xStatusMenu           ,0,0,0,VIEW_SCREEN },
  { SettingMenuScreen,        &xMainMenu, &xSettingMenu          ,0,0,0,VIEW_SCREEN},
  { DeviceInfoScreen,         &xMainMenu, NULL                   ,0,0,0,VIEW_SCREEN },
  { DeviceHardInfoScreen,     &xMainMenu, NULL					  ,0,0,0,VIEW_SCREEN },
  { JournalInfoScreen,        &xMainMenu, &xJournalView			  ,0,0,0,VIEW_SCREEN },

};

//Описание экранов главонго меню
xScreenType  xStatusMenuScreens[STATUS_MENU_COUNT]=
{
  {LampErrorMenuScreen,      &xMainMenu, NULL    ,0,0,0,VIEW_SCREEN  },
  {LampErrorScreen,          &xMainMenu, NULL    ,0,0,0,VIEW_SCREEN },
  {LampResursScreen,         &xMainMenu, NULL    ,0,0,0,VIEW_SCREEN },
  {LampMaxResursScreen,      &xMainMenu, NULL    ,0,0,0,VIEW_SCREEN  }

};
xScreenType  xSettingMenuScreens[SETTINGS_MENU_COUNT ]=
{
  {MBScreen	,     			  &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
  {ControlTypeScreen,         &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT },
  {AllLampSetRecourceScreen,  &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
  {LampSetRecourceScreen,     &xMainMenu, NULL    ,0,0,0,MULTI_PARAMETR_EDIT  },
  {LampReSetRecourceScreen,   &xMainMenu, NULL    ,0,0,0,COMMAND_EDIT  },
  {TimeSetScreen,  			  &xMainMenu, NULL    ,0,0,3,MULTI_PARAMETR_EDIT  },
  {DateSetScreen,             &xMainMenu, NULL    ,0,0,3,MULTI_PARAMETR_EDIT   },

};

xScreenType  xSecretMenuScreens[SECRET_MENU_COUNT] =
{
	{VLOWScreen,     			  &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
	{VLOWONScreen	,     		  &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
	{VWARScreen,     			  &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
	{VWARONScreen,     		      &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
	{VHIGHScreen,     		      &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
    {VHIGHONScreen,     		  &xMainMenu, NULL    ,0,0,0,ONE_PARAMETR_EDIT  },
};


xScreenType  xDevoloperMenuScreens[DEVOLOPER_MENU_COUNT ]=
{
  {LampCountScreen,    &xMainMenu, NULL    ,0,0,0, ONE_PARAMETR_EDIT   },
  {FBOASizeScreen,     &xMainMenu, NULL    ,0,0,0, MULTI_PARAMETR_EDIT },
  {FBOBSizeScreen,     &xMainMenu, NULL    ,0,0,0, MULTI_PARAMETR_EDIT  },

};

xScreenType xYesNoScreens[ YESNO_MENU_COUNT ]=
{
  {xYesNoScreen,    &xMainMenu, NULL    ,0,0,0, VIEW_SCREEN    },
};

xScreenType xJournalScreens[JOURNAL_MENU_COUNT]=
{
		{xJournalScreen,    &xMainMenu, NULL    ,0,0,0, VIEW_SCREEN    },
};

xScreenType xPasswordScreens[PASSWORD_MENU_COUNT]=
{
    {xPasswordScreen, &xMainMenu, &xSecretMenu ,0,0,0, MULTI_PARAMETR_EDIT },

};

xScreenSetObject xSecretMenu =
{
   xSecretMenuScreens,
  ( SECRET_MENU_COUNT  - 1U ),
  0U,
  ( void* )&xEditScreenCallBack,
};

xScreenSetObject xStatusMenu =
{
   xStatusMenuScreens,
  ( STATUS_MENU_COUNT - 1U ),
  0U,
  ( void* )&xStatusScreenCallBack,
};


xScreenSetObject xSettingMenu =
{
   xSettingMenuScreens,
  ( SETTINGS_MENU_COUNT  - 1U ),
  0U,
  ( void* )&xEditScreenCallBack,
};

xScreenSetObject xDevoloperMenu =
{
   xDevoloperMenuScreens,
  ( DEVOLOPER_MENU_COUNT  - 1U ),
  0U,
  ( void* )&xEditScreenCallBack,
};

xScreenSetObject xMainMenu =
{
   xMainMenuScreens,
  ( MAIN_MENU_COUNT - 1U ),
  0U,
  ( void* )&xMainScreenCallBack,
};

xScreenSetObject xYesNoMenu =
{
  xYesNoScreens,
  ( YESNO_MENU_COUNT - 1U ),
  0U,
  ( void* )&xYesNoScreenKeyCallBack,
};

xScreenSetObject xPasswordMenu =
{
  xPasswordScreens,
  ( PASSWORD_MENU_COUNT - 1U ),
  0U,
  ( void* )&xPassScreenCallBack,
};

xScreenSetObject  xJournalView =
{
	xJournalScreens,
   ( JOURNAL_MENU_COUNT - 1U ),
	0U,
   ( void* )&xJournalScreenCallBack,

};
