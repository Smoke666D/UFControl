/*
 * menu.h
 *
 *  Created on: Aug 16, 2023
 *      Author: i.dymov
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include "main.h"
#include "data_model.h"

#define MAX_SCREEN_OBJECT  20
#define LAST_OBJECT        1

#define KEY_UP_BREAK        (up_key | BRAKECODE) //1U
#define KEY_UP              (up_key | MAKECODE) //2U
#define KEY_DOWN_BREAK      (down_key | BRAKECODE) //3U
#define KEY_DOWN            (down_key | MAKECODE)  //4U
#define KEY_ENTER            (enter_key | MAKECODE)  //5U
#define KEY_ENTER_BREAK      (enter_key | BRAKECODE) // 6U
#define KEY_EXIT            (exit_key | MAKECODE)  //9U
#define KEY_EXIT_BREAK       (exit_key | BRAKECODE)//7U
#define KEY_ENTER_DELAY      (enter_key | DELAYCODE)



#define   CENTER_ALIGN        1U
#define   RIGTH_ALIGN         2U
#define   LEFT_ALIGN          3U
#define   NO_ALIGN            0U
#define ADD_ZERO			  0x01
#define NO_ZERO				  0x00


#define SCREEN_INDEX_ID		         0x13
#define TOTAL_LAMP_DATA_ID   		 0x01
#define LAMP_RES_DATA_ID    		 0x02
#define LAMP_RES_PROCENT_DATA_ID     0x03
#define TOTAL_LAMP_ERROR_ID          0x04
#define LAMP_ERROR_DATA_ID           0X05
#define TOTAL_ERROR_ID				 0x06
#define ERROR_DATA_ID			     0X07
#define STATUS_ID					 0x08
#define WORK_ID						 0x09
#define OPEN_ID						 0x10
#define ALARM_ID					 0x11
#define CUR_TIME_ID					 0x12
#define TIME_EDIT					 0x16
#define DATE_EDIT					 0x17
#define FBO_SIZE_ID					 0x18
#define LAMP_RESET_DATA_ID			 0x19
#define GET_V220					 0x20
#define GET_V187					 0x21
#define GET_V187_ON					 0x22
#define GET_V197					 0x23
#define GET_V197_ON					 0x24
#define GET_V250					 0x25
#define GET_V250_ON					 0x26
#define A_SIZE						 0x27
#define B_SIZE						 0x28

typedef enum
{
  FLAG_RESET,
  FLAG_SET,
} FLAG;
/*------------
 *
 */
typedef enum
{
  TEXT_STRING,
  DATA,
  EDIT_DATA,
  MULTI_EDIT_DATA,
} OBJECT_TYPE;


typedef enum
{
  SCREEN_VIEW,
  PARAMETR_SELECT,
  MULTI_PARAM_SELECT,
  PARAMENT_EDIT,
} SCREEN_EDIT_STATUS;

typedef enum
{
  VIEW_SCREEN,
  ONE_PARAMETR_EDIT,
  MULTI_PARAMETR_EDIT,
  COMMAND_EDIT,
} SCREEN_TYPE;

typedef enum
{
	DATA_SCREEN,
	MENU_SCREEN,
	LIST_SCREEN,
	MENU_1_LINE_STRING,
	EDIT_SCREEN,
} SCREEN_TYPE_t;

typedef struct __packed
{
  uint8_t obect_id;
  uint8_t cursor_x;
  uint8_t cursor_y;
  uint8_t last_object;
} xEditScrren_t;

typedef struct __packed
{
  uint8_t     last;
  uint8_t     x;
  uint8_t     y;
  uint8_t     Width;
  OBJECT_TYPE xType;
  char*       pStringParametr;
  void       ( *GetDtaFunction )();
  uint16_t    DataID;
  uint8_t *   data_parametr;
} xScreenObjet;



typedef struct __packed
{
  const xScreenObjet*  pScreenCurObjets;
  void*        pUpScreenSet;
  void*        pDownScreenSet;
  xScreenObjet*pEitObject;
  uint8_t	   DataIndex;
  uint8_t	   MaxMultiEditParametr;
  SCREEN_TYPE  ScreenType;
} xScreenType;

typedef struct __packed
{
  xScreenType*  pHomeMenu;               /* Указатель на массив экранов верхнего уровня. */
  uint8_t       pMaxIndex;	             /* Индекс домашнего экрана в маасиве экранов верхнего уровня */
  uint8_t       pCurrIndex;	             /* Индексе текущего экрана */
  void          (* pFunc)(void *,char);  /* Функция обработки нажатий */
} xScreenSetObject;


uint16_t usGetDataViewIndex();
void vSetDataViewIndex( uint16_t new_index);
void xMainScreenCallBack ( xScreenSetObject* menu, char key );
void xStatusScreenCallBack ( xScreenSetObject* menu, char key );
void xEditScreenCallBack ( xScreenSetObject* menu, char key );
void xYesNoScreenKeyCallBack( xScreenSetObject* menu, char key );
void xPassScreenCallBack( xScreenSetObject* menu, char key );
void vMenu( void);
void vMenuInit();
void StartMenuTask(void *argument);
#endif /* SRC_MENU_H_ */
