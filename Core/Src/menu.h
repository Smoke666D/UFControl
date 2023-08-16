/*
 * menu.h
 *
 *  Created on: Aug 16, 2023
 *      Author: i.dymov
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include "main.h"

typedef enum
{
  mREAD,
  mSAVE,
  mESC,
  mINC,
  mDEC,
} DATA_COMMNAD_TYPE;

typedef enum
{
  TEXT_STRING,
  DATA,
  EDIT_DATA,
  CURSOR,
} OBJECT_TYPE;

typedef enum
{
	DATA_SCREEN,
	MENU_SCREEN,
} SCREEN_TYPE_t;

typedef struct __packed
{
  uint8_t     last;
  uint8_t     x;
  uint8_t     y;
  uint8_t     Width;
  OBJECT_TYPE xType;
  char*       pStringParametr;
  void        ( *GetDtaFunction )();
  uint16_t    DataID;
} xScreenObjet;



typedef struct __packed
{
  SCREEN_TYPE_t screen_type;
  const xScreenObjet*  pScreenCurObjets;
  void*         pUpScreen;
  uint8_t       pCurrIndex;
  uint8_t       pMaxIndex;
  void          ( *pFunc )( void*, char );
  void 			*xScreens;
} xScreenType;

tpyedex struct _

void vGetDataForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );

#endif /* SRC_MENU_H_ */
