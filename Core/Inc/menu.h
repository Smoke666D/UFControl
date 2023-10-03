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
#define KEY_ENTER_BREAK      (etner_key | BRAKECODE) // 6U
#define KEY_EXIT            (exit_key | MAKECODE)  //9U
#define KEY_EXIT_BREAK       (exit_key | BRAKECODE)//7U



#define   CENTER_ALIGN        1U
#define   RIGTH_ALIGN         2U
#define   LEFT_ALIGN          3U
#define   NO_ALIGN            0U
#define ADD_ZERO			  0x01
#define NO_ZERO				  0x00


#define SCREEN_INDEX_ID		 0x00
#define TOTAL_LAMP_DATA_ID   0x01
#define LAMP_RES_DATA_ID     0x02

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
  EDIT_DATA_ENUM,
  EDIT_DATA_NUM,
  CURSOR,
  LIST_DATA,
} OBJECT_TYPE;

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
  uint8_t      ( *GetDtaFunction )();
  uint16_t    DataID;
  uint8_t *   data_parametr;
} xScreenObjet;



typedef struct __packed
{
  const xScreenObjet*  pScreenCurObjets;
  void*        pUpScreenSet;
  void*        pDownScreenSet;
  uint8_t      MaxLineNumber;
  uint8_t	   DataIndex;
  uint8_t	   CursorOwener;
  uint8_t      MaxCursorObject;
} xScreenType;

typedef struct __packed
{
  xScreenType*  pHomeMenu;               /* Указатель на массив экранов верхнего уровня. */
  uint8_t       pMaxIndex;	             /* Индекс домашнего экрана в маасиве экранов верхнего уровня */
  uint8_t       pCurrIndex;	             /* Индексе текущего экрана */
  void          (* pFunc)(void *,char);  /* Функция обработки нажатий */
} xScreenSetObject;


void vGetDataForList( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID, uint8_t index );
void xMainScreenCallBack ( xScreenSetObject* menu, char key );
void xCommonScreenCallBack ( xScreenSetObject* menu, char key );
void vMenuScreenCallBack ( xScreenSetObject* menu, char key , uint8_t inc_index);
void vMenu( void);
void vMenuInit();
#endif /* SRC_MENU_H_ */
