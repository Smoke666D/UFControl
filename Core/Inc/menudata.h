/*
 * menudata.h
 *
 *  Created on: Aug 16, 2023
 *      Author: i.dymov
 */

#ifndef SRC_MENUDATA_H_
#define SRC_MENUDATA_H_

#include "menu.h"

#define MAIN_MENU_COUNT 5
#define DEVOLOPER_MENU_COUNT 3
#define SECRET_MENU_COUNT    6
#define USER_MENU_COUNT	     7
#define CONTROL_TYPE         8
#define STATUS_MENU_COUNT    4
#define SETTINGS_MENU_COUNT  8
#define YESNO_MENU_COUNT     1
#define PASSWORD_MENU_COUNT  1
#define JOURNAL_MENU_COUNT	 1

#define LIST_LAMP_STATUS_ID    0x01
#define LIST_LAMP_STATUS_COUNT  0x02



extern xScreenSetObject xMainMenu;
extern xScreenSetObject xSettingMenu;
extern xScreenSetObject xDevoloperMenu;
extern xScreenSetObject xStatusMenu;
extern xScreenSetObject xYesNoMenu;
extern xScreenSetObject xSecretMenu;
extern xScreenSetObject xPasswordMenu;
extern xScreenSetObject  xJournalView;
#endif /* SRC_MENUDATA_H_ */
