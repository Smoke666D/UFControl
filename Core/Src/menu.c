/*
 * menu.c
 *
 *  Created on: Jun 26, 2023
 *      Author: i.dymov
 */
#include "menu.h"
//static xScreenSetObject* pCurrMenu        = 0;
static xScreenObjet*     pCurObject       = 0;

void vMenuInit()
{
	//pCurrMenu = &xMainMenu;
}


void vGetDataForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{

}

void vMenu( void)
{
	while(1)
	{
		//vDrawObject( ( xScreenObjet * )pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].pScreenCurObjets );
		    vLCDRedraw();
	}

}
