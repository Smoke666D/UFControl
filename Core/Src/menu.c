/*
 * menu.c
 *
 *  Created on: Jun 26, 2023
 *      Author: i.dymov
 */
#include "menu.h"
#include "LCD.h"

#include "math.h"
#include "menudata.h"
//static xScreenSetObject* pCurrMenu        = 0;
static xScreenObjet*     pCurObject       = 0;
static xScreenSetObject* pCurrMenu        = NULL;
static xScreenObjet*     pCurDrawScreen   = NULL;
static uint8_t cur_cursor_owener  = 0;
static uint8_t key_ready = 0;
static uint8_t enter_count = 0;
static QueueHandle_t     pKeyboard        = NULL;
static KeyEvent          TempEvent        = { 0U };
static KeyEvent          BufferEvent      = { 0U };
static uint16_t 		 DataViewIndex    = 0;
static EventGroupHandle_t system_event;

void vMenuInit()
{
	Init16X2LCD();
	pCurrMenu = &xMainMenu;
	vSetupKeyboard();
	pKeyboard  = pGetKeyboardQueue();
}

void vDrawObject( xScreenSetObject* menu)
{

  xScreenObjet * pScreenObjects =  ( xScreenObjet * )pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].pScreenCurObjets;

  char* TEXT      = NULL;
 // uint8_t  Insert    = 0U;
  char  Text[20U];
  char  TextBuf[40];
 /* uint8_t  i         = 0U;
  uint8_t  x_offset  = 0U;
  uint8_t  y_offset  = 0U;*/
  uint8_t  Redraw    = 0U;
  uint8_t  CursorPos = 0U;
  uint8_t  ListCount = 0;

  //Проверяем состояние экрна. Если экран изменлися или изменилась строка вывода, то надо пперерисовать экраа
  if ( pCurDrawScreen != pScreenObjects )
  {
    pCurDrawScreen = pScreenObjects;
    Redraw         = 1U;
  }
  else                    //Если тот же самый экран
  {
    for (uint8_t i=0U; i<MAX_SCREEN_OBJECT; i++ ) //Проверяем есть ли на экране динамические объекты
    {
      switch ( pScreenObjects[i].xType )
      {
        case DATA:
        case EDIT_DATA:
          Redraw = 1U;
          break;
        default:
          break;
      }
      if ( ( pScreenObjects[i].last > 0U ) || ( Redraw != 0U ) )
      {
        break;
      }
    }
  }

  if ( Redraw > 0U )   //Если экран нужно перерисовывать
  {
	ClearScreenBuffer();

    for (uint8_t  i=0U; i<MAX_SCREEN_OBJECT; i++ )
    {

    		switch ( pScreenObjects[i].xType )
    		{
    		 	 //Если текущий объект - строка
    		     case TEXT_STRING:
    		        	TEXT = pScreenObjects[i].pStringParametr;
    		        	convertUtf8ToCp1251(TEXT,Text);
    		        	LCD_SetString(Text, pScreenObjects[i].x, pScreenObjects[i].y );
    		        	break;
    		     case DATA:
    		     case EDIT_DATA:
    		     case MULTI_EDIT_DATA:
    		    	   uint8_t x_offset;
    		    	   pScreenObjects[i].GetDtaFunction( mREAD, TextBuf, pScreenObjects[i].DataID );
    		    	   convertUtf8ToCp1251(TextBuf,Text);
    		    	   x_offset = pScreenObjects[i].x;
    		    	   if ( pScreenObjects[i].data_parametr!=NULL )
    		    	   {
    		    		   switch (pScreenObjects[i].data_parametr[0])
    		    		   {
    		    		   	   case CENTER_ALIGN:
    		    		   		   volatile uint8_t len =  strlen(Text);
    		    		   		   if ( pScreenObjects[i].x > (strlen(Text)/2))
    		    		   		   {
    		    		   			   x_offset = pScreenObjects[i].x - strlen(Text)/2;
    		    		   		   }
    		    		   		   break;
    		    		   	   case RIGTH_ALIGN :
    		    		   		    if ( pScreenObjects[i].x > (strlen(Text)-1))
    		    		   		    {
    		    		   		    	x_offset =    pScreenObjects[i].x-(strlen(Text)-1);
    		    		   		    }
    		    		   		   break;
    		    		   	   default:
    		    		   		 x_offset = pScreenObjects[i].x;
    		    		   		 break;
    		    		   }
    		    		   uint8_t cursor_offset = x_offset;
    		    		   if  (pScreenObjects[i].xType == MULTI_EDIT_DATA)
    		    		   {
    		    			   cursor_offset = x_offset + pScreenObjects[i].data_parametr[2]*DataViewIndex;
    		    		   }
    		    		   LCD_SET_Cursor(pScreenObjects[i].data_parametr[1],cursor_offset,pScreenObjects[i].y  );
    		    	   }
    		    	   else
    		    	   {
    		    		 x_offset =    pScreenObjects[i].x;
    		    	   }
    		    	   if ( strlen(Text)+x_offset > 21)
    		    	   {
    		    		   Text[20-x_offset] = 0;
    		    	   }
    		    	   LCD_SetString(Text, x_offset, pScreenObjects[i].y );
    		    	   break;

    		     default: break;
    		}

      if ( pScreenObjects[i].last == LAST_OBJECT )
      {
    	  break;
      }
    }
  }
  return;
}


void GetNumberToString(char * text, int number, uint8_t count )
{
 uint8_t k = 0;
 int buf_number = number;
 int koof =(int) pow(10,count-1);
 for (uint8_t i = count; i>0; i--)
 {
	 text[k++] = buf_number/koof + '0';
	 buf_number /=10;
	 koof /=10;
 }
}


void vMenuScreenCallBack ( xScreenSetObject* menu, char key , uint8_t inc_index)
{

  return;
}
#define UP_KEY_READY 0x01
#define DOWN_KEY_READY 0x02
static FLAG              fDownScreen      = FLAG_RESET;



uint16_t usGetDataViewIndex()
{
	return (DataViewIndex);
}
void vSetDataViewIndex( uint16_t new_index)
{
	DataViewIndex = new_index;
}


SCREEN_EDIT_STATUS edit_data_flag =SCREEN_VIEW;
void xEditScreenCallBack ( xScreenSetObject* menu, char key )
{
	uint8_t           index = menu->pCurrIndex;
	xScreenSetObject* pMenu = menu;
	  switch ( key )
	  {
	    case KEY_UP:
	    	switch (edit_data_flag)
	    	{
	    		case SCREEN_VIEW:
	    			if (++(pMenu->pCurrIndex) > pMenu->pMaxIndex)  {  pMenu->pCurrIndex = 0U; }
	    			break;
	    		case PARAMETR_SELECT:
	    		case  MULTI_PARAM_SELECT:
	    			DataViewIndex ++;
	    			if  (edit_data_flag ==  MULTI_PARAM_SELECT)
	    			{
	    				if (DataViewIndex >= pMenu->pHomeMenu[index].pEitObject->data_parametr[3])
	    				{
	    					DataViewIndex = pMenu->pHomeMenu[index].pEitObject->data_parametr[3] - 1;
	    				}
	    			}
	    			break;
	    		case PARAMENT_EDIT:
	    			pMenu->pHomeMenu[index].pEitObject->GetDtaFunction(mINC,NULL,NULL);
	    			break;
	    	}
	      break;
	    case KEY_DOWN:
	    	switch (edit_data_flag)
	    	{
	    		case SCREEN_VIEW:
	    			 if (pMenu->pCurrIndex > 0)
	    				    pCurrMenu->pCurrIndex--;
	    			 else
	    				    pCurrMenu->pCurrIndex = menu->pMaxIndex;
	    		    break;
	    		case PARAMETR_SELECT:
	    		case  MULTI_PARAM_SELECT:
	    			if (DataViewIndex > 1)  DataViewIndex -- ;
	    		    break;
	    		case PARAMENT_EDIT:
	    			pMenu->pHomeMenu[index].pEitObject->GetDtaFunction(mDEC,NULL,NULL);
	    		    break;
	    	}
	       break;
	    case  KEY_EXIT:
	    	if ( edit_data_flag != SCREEN_VIEW)
	    	{
	    		pMenu->pHomeMenu[index].pEitObject->GetDtaFunction(mESC,NULL,NULL);
	    		pMenu->pHomeMenu[index].pEitObject->data_parametr[1] = 0;
	    		edit_data_flag = SCREEN_VIEW;
	    	}
	    	else
	    	{
	           if ( pMenu->pHomeMenu[index].pUpScreenSet != NULL )
	           {
	               pCurrMenu = menu->pHomeMenu[index].pUpScreenSet;
	               menu->pHomeMenu[menu->pCurrIndex].DataIndex = 0;
	               pMenu     = pCurrMenu;
	               xEventGroupSetBits(system_event,SYSTEM_RESTART);
	           }
	       }
	    	DataViewIndex =0;
	       break;
	   case KEY_ENTER:
		    switch (edit_data_flag )
		    {
		          xEventGroupSetBits(system_event,SYSTEM_STOP);
		          case SCREEN_VIEW:
		        		  for (uint8_t  i=0U; i<MAX_SCREEN_OBJECT; i++ )
		        		  {
		        		  	   if ( (menu->pHomeMenu[index].pScreenCurObjets[i].xType == EDIT_DATA) ||
		        		  			  (menu->pHomeMenu[index].pScreenCurObjets[i].xType == MULTI_EDIT_DATA) )
		        		  	   {
		        		  	       menu->pHomeMenu[index].pEitObject = & (menu->pHomeMenu[index].pScreenCurObjets[i]);
		        		  	       break;
		        		  	   }
		        		  	   if ( menu->pHomeMenu[index].pScreenCurObjets[i].last == LAST_OBJECT ) break;
		        		  }
		           if (menu->pHomeMenu[index].ScreenType == ONE_PARAMETR_EDIT)
		           {
		        	   edit_data_flag  = PARAMENT_EDIT;
		        	   menu->pHomeMenu[index].pEitObject ->data_parametr[1] = 1;
		           }
		           else
		           {
		        	   if (menu->pHomeMenu[index].pEitObject->xType == MULTI_EDIT_DATA)
		        	   {
		        		   edit_data_flag  = MULTI_PARAM_SELECT;
		        	   }
		        	   else
		        	   {
		        		   edit_data_flag  = PARAMETR_SELECT;
		        	   }
		           }
		           break;
		          case  MULTI_PARAM_SELECT:
		          case  PARAMETR_SELECT:
		        	  edit_data_flag = PARAMENT_EDIT;
		        	  break;
		          case PARAMENT_EDIT:
		        	  menu->pHomeMenu[index].pEitObject->GetDtaFunction(mSAVE,NULL,NULL);
		        	  menu->pHomeMenu[index].pEitObject->data_parametr[1] = 0;
		        	  edit_data_flag = SCREEN_VIEW;
		        	  DataViewIndex = 0;
		        	  break;
		    }
	        break;
	    default:
	      break;
	  }
	  return;
}

void xStatusScreenCallBack ( xScreenSetObject* menu, char key )
{

	uint8_t           index = menu->pCurrIndex;
	xScreenSetObject* pMenu = menu;
	  switch ( key )
	  {
	    case KEY_UP:
	    	if ( fDownScreen == FLAG_RESET )
	          if (++menu->pCurrIndex > menu->pMaxIndex)  {  pCurrMenu->pCurrIndex = 0U; }
	    	else
	    		DataViewIndex ++ ;
	      break;
	    case KEY_DOWN:
	    	if ( fDownScreen == FLAG_RESET )
	    	{
	    		  if (menu->pCurrIndex > 0)
	    			  pCurrMenu->pCurrIndex--;
	    		  else
	    			 pCurrMenu->pCurrIndex = menu->pMaxIndex;
	    	}
	    	else
	    	{
	    		if (DataViewIndex > 0)  DataViewIndex --;
	    	}
	       break;
	    case  KEY_EXIT:
	       if ( fDownScreen == FLAG_SET )
	       {
	           fDownScreen = FLAG_RESET;
	           if ( menu->pHomeMenu[index].pUpScreenSet != NULL )
	           {
	               pCurrMenu = menu->pHomeMenu[index].pUpScreenSet;
	               menu->pHomeMenu[menu->pCurrIndex].DataIndex = 0;
	               pMenu     = pCurrMenu;
	           }
	           DataViewIndex = 0;
	       }
	       break;
	   case KEY_ENTER:
	        if ( fDownScreen == FLAG_RESET )
	        {
	            if ( menu->pHomeMenu[index].pDownScreenSet != NULL )
	            {
	                pCurrMenu             = menu->pHomeMenu[index].pDownScreenSet;
	                fDownScreen            = FLAG_SET;
	                pCurrMenu->pCurrIndex = 0U;
	            }
	        }
	        else
	        {
	            pMenu->pCurrIndex = ( pMenu->pCurrIndex == pMenu->pMaxIndex ) ? 0U : pMenu->pCurrIndex + 1;
	        }
	        break;
	    default:
	      break;
	  }
	  return;
}

void xMainScreenCallBack ( xScreenSetObject* menu, char key )
{
	uint8_t           index = menu->pCurrIndex;
	xScreenSetObject* pMenu = menu;
	  switch ( key )
	  {
	    case KEY_UP:
	       if (++(pMenu->pCurrIndex) > pMenu->pMaxIndex)  {  pMenu->pCurrIndex = 0U; }
	      break;
	    case KEY_DOWN:
	        if (pMenu->pCurrIndex > 0)
	        	(pMenu->pCurrIndex)--;
	        else
	        	pMenu->pCurrIndex = (pMenu->pMaxIndex-1);
	       break;
	    case  KEY_EXIT:
	       if ( pMenu->pHomeMenu[index].pUpScreenSet != NULL )
	        {
	             pCurrMenu = menu->pHomeMenu[index].pUpScreenSet;
	             pCurrMenu->pCurrIndex = 0;
	        }
	    case KEY_ENTER:
	    	 if (menu->pHomeMenu[index].pDownScreenSet!=NULL)
	    	 {
	    		 pCurrMenu  = menu->pHomeMenu[index].pDownScreenSet;
	    		 pCurrMenu->pCurrIndex = 0U;
	    	 }
	       break;
	    default:
	      break;
	  }
	  return;
}

#define SETINGS_MENU_TIMEOUT 30000

void StartMenuTask(void *argument)
{
	static uint8_t           key              = 0U;
	static uint32_t timer = 0;
	static uint8_t flag;
	static uint8_t SET_MENU = 0;
	vMenuInit();
	system_event = xGetSystemUpdateEvent();
	for (;;)
	{
	 osDelay(1);
	 if (++timer > SETINGS_MENU_TIMEOUT) timer = SETINGS_MENU_TIMEOUT;
	 vDrawObject( pCurrMenu );
	 if ( xQueueReceive( pKeyboard, &TempEvent, 0U ) == pdPASS )
	 {
	      BufferEvent = TempEvent;
	      if (BufferEvent.KeyCode == time_out)
	      {
	    	  LCD_LED_OFF();
	      }
	      else
	      {
	    	  LCD_LED_ON();
	    	  key = TempEvent.KeyCode | TempEvent.Status;
	    	  //Если зафиксировано нажатие клавиши
	    	if ( SET_MENU == 1 )
	    	{
	    		    		  timer = SETINGS_MENU_TIMEOUT;
	    		    		  pCurrMenu = &xDevoloperMenu;
	    		    		  xEventGroupSetBits(system_event,SYSTEM_STOP);
	    	}


	    	  if ((timer < SETINGS_MENU_TIMEOUT) && (SET_MENU == 0))
	    	  {
	    		  switch (key)
	    		  {
	    		  	    case KEY_UP:
	    		  	    	flag |= 0x01;
	    		  	    	break;
	    		  	    case KEY_UP_BREAK:
	    		  	    	flag = 0;
	    		  	    	break;
	    		  	     case KEY_DOWN:
	    		  	  	    flag |= 0x02;
	    		  	  	    break;
	    		  	  	 case KEY_DOWN_BREAK:
	    		  	  	    flag = 0;
	    		  	  	    break;
	    		  	  	 case KEY_ENTER:
	    		  	  		 if (flag == 0x0F)
	    		  	  		 {
	    		  	  			SET_MENU = 1;
	    		  	  			break;
	    		  	  		 }
	    		  	  		 if ((flag & 0x03) == 0x03)
	    		  	  			 flag =0x07;
	    		  	  		 break;
	    		  	  	 case KEY_ENTER_BREAK:
	    		  	  		 if ((flag & 0x07) == 0x07)
	    		  	  		 {
	    		  	  			 flag = 0x0F;
	    		  	  		 }
	    		  	  		 break;
	    		  }

	    	  }

	    	  pCurrMenu->pFunc( pCurrMenu, key );
	      }
	  }

	}
}
