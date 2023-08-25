/*
 * menudata.h
 *
 *  Created on: Aug 16, 2023
 *      Author: i.dymov
 */

#ifndef SRC_MENUDATA_H_
#define SRC_MENUDATA_H_

#include "menu.h"

#define MAIN_MENU_COUNT 3
#define DEVOLOPER_MENU_COUNT 2
#define USER_MENU_COUNT	     7
#define CONTROL_TYPE         8



#define LIST_LAMP_STATUS_ID    0x01
#define LIST_LAMP_STATUS_COUNT  0x02

extern xScreenSetObject xMainMenu;
extern xScreenSetObject xUserMenu;
extern xScreenSetObject xDevoloperMenu;

#endif /* SRC_MENUDATA_H_ */
