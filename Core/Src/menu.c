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
static uint8_t current_line 			  = 0;
static xScreenObjet*     pCurDrawScreen   = NULL;
static uint8_t cur_cursor_owener  = 0;
static uint8_t key_ready = 0;
static uint8_t enter_count = 0;
static QueueHandle_t     pKeyboard        = NULL;
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
 /* uint8_t  i         = 0U;
  uint8_t  x_offset  = 0U;
  uint8_t  y_offset  = 0U;*/
  uint8_t  Redraw    = 0U;
  uint8_t  CursorPos = 0U;
  uint8_t  ListCount = 0;

  //Проверяем состояние экрна. Если экран изменлися или изменилась строка вывода, то надо пперерисовать экраа
  /*if ( ( pCurDrawScreen != pScreenObjects )
	  && ( pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].CurLine != current_line )
		  && (pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].CursorOwener!= cur_cursor_owener ) )//Если экран изменился
  {
    pCurDrawScreen = pScreenObjects;
    current_line   = pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].CurLine;
    cur_cursor_owener  = pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].CursorOwener;
    Redraw         = 1U;
  }
  else*/                     //Если тот же самый экран
  {
    for (uint8_t i=0U; i<MAX_SCREEN_OBJECT; i++ ) //Проверяем есть ли на экране динамические объекты
    {
      switch ( pScreenObjects[i].xType )
      {
        case DATA:
        case EDIT_DATA_ENUM:
        case EDIT_DATA_NUM:
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
    		        	LCD_SetString(TEXT, pScreenObjects[i].x, pScreenObjects[i].y );
    		        	//LCD_SetStringDrawUTF8( TEXT, pScreenObjects[i].x, pScreenObjects[i].y );
    		        	break;
    		    /* case CURSOR:
    		    	 	 Text[0] = ' ';
    		    	     Text[1] = 0;
    		    	 	 if (CursorPos == cur_cursor_owener)
    		    	 	 {
    		    	 		 Text[0]= '>';
    		    	 	 }
    		    	 	LCD_SetStringDrawUTF8( TEXT, pScreenObjects[i].x, pScreenObjects[i].y );
    		    	 	CursorPos++;
    		    	 	break;
    		     case LIST_DATA: /*Вывод данных списка
    		     	 	 функции вывода передаем в качестве парамерта текущие значение курсора
    		    	    */
    		    	 /*   pScreenObjects[i].GetDtaFunction( mREAD, &Text, pScreenObjects[i].DataID, cur_cursor_owener + ListCount );
    		    	    TEXT = Text;
    		    	    if ((++ListCount == 2) && Text[0] == 0)
    		    	    {
    		    	    	if (cur_cursor_owener > 0)
    		    	    	{
    		    	    		pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].CursorOwener--;
    		    	    	}
    		    	    }
    		    	    LCD_SetStringDrawUTF8( TEXT, pScreenObjects[i].x, pScreenObjects[i].y );*?\/
    		   /*  case EDIT_DATA_NUM:
    		    	    pScreenObjects[i].GetDtaFunction( mREAD, &Text, pScreenObjects[i].DataID , pScreenObjects[i].pStringParametr);
    		    	    TEXT = Text;
    		    	    break;*/
    		     default: break;
    		}


   //   Insert = 0U;
   /*   switch ( pScreenObjects[i].xType )
      {

        case INPUT_HW_DATA:
        case HW_DATA:

        case DATA_STRING:
          if ( ( pScreenObjects[i].xType == INPUT_HW_DATA ) || ( pScreenObjects[i].xType ==DATA_STRING ) )
          {
            if ( pScreenObjects[i].ObjectParamert[3U] > 0U )
            {
              Insert = 1U;
              Blink = ( Blink > 0U ) ? 0U : 1U;
            }
            if ( !Insert )
            {
              u8g2_SetDrawColor( u8g2, pScreenObjects[i].ObjectParamert[1U]?0U:1U );
            }
            else
            {
              u8g2_SetDrawColor( u8g2, Blink?0U:1U );
            }
            u8g2_DrawBox( u8g2, pScreenObjects[i].x, pScreenObjects[i].y, pScreenObjects[i].Width, pScreenObjects[i].Height );
          }
          u8g2_SetFontMode( u8g2, pScreenObjects[i].ObjectParamert[0U] );
          if ( !Insert )
          {
            u8g2_SetDrawColor( u8g2, pScreenObjects[i].ObjectParamert[1U] );
          }
          else
          {
            u8g2_SetDrawColor( u8g2, Blink );
          }
          switch ( pScreenObjects[i].xType )
          {
            case STRING:
              break;

            case HW_DATA:
            case INPUT_HW_DATA:
              ( pScreenObjects[i].DataID > 0U ) ? pScreenObjects[i].GetDtaFunction( mREAD, &Text, pScreenObjects[i].DataID ) : pScreenObjects[i].GetDtaFunction( mREAD, &Text );
              TEXT = (char*)Text;
              break;
            default:
              break;
          }

          if ( pScreenObjects[i].xType != TEXT_STRING )
          {
            switch ( pScreenObjects[i].ObjectParamert[2U] )
            {
              case RIGTH_ALIGN:
                x_offset =  pScreenObjects[i].x + ( pScreenObjects[i].Width - u8g2_GetUTF8Width( u8g2, TEXT ) ) - 1U;
                break;
              case LEFT_ALIGN:
                x_offset =  pScreenObjects[i].x + 1U;
                break;
              default:
                x_offset =  pScreenObjects[i].x + ( pScreenObjects[i].Width - u8g2_GetUTF8Width( u8g2, TEXT ) ) / 2U;
                break;
            }
            y_offset =  pScreenObjects[i].y + ( pScreenObjects[i].Height / 2U ) + ( u8g2_GetAscent( u8g2 ) / 2U );
            LCD_SetStringDrawUTF8( TEXT, x_offset, y_offset);
          }
          else
          {
			LCD_SetStringDrawUTF8( TEXT, pScreenObjects[i].x, pScreenObjects[i].y );
          }
          break;
        default:
           break;
      }*/
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

void vGetDataForList( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID, uint8_t index )
{
  switch (ID)
  {
  	  case LIST_LAMP_STATUS_ID:
  		  GetNumberToString(Data,index,2);
  		  break;
  	  case LIST_LAMP_STATUS_COUNT:
  		  GetNumberToString(Data,ucGetLampProc(index),2);
  		  break;
  	  default:
  		  break;
  }
}




void vListScreenCallBack ( xScreenSetObject* menu, char key )
{
	 /*  switch ( key )
	   {
		     case KEY_UP:
		       		  if (menu->pHomeMenu[menu->pCurrIndex].CursorOwener > 0 )
		       		   			 menu->pHomeMenu[menu->pCurrIndex].CursorOwener--;

		     case KEY_DOWN:
		       		  menu->pHomeMenu[menu->pCurrIndex].CursorOwener++;
		       		  break;
		     case KEY_EXIT:
		    	 menu->pHomeMenu[menu->pCurrIndex].CurLine = 0;
		    	 menu->pHomeMenu[menu->pCurrIndex].CursorOwener = 0;
		    	 pCurrMenu = menu->pHomeMenu[menu->pCurrIndex].pUpScreenSet;
		    	 break;
		     defualt:

		    	 break;
	   }*/
}

//Объект редактирования
typedef struct __packed
{
    uint8_t editmode;
    uint8_t max_edit_object;
    uint8_t edit_object_index;
    xEditScrren_t edit_data[MAX_SCREEN_OBJECT];

} xEditObjectType ;


static xEditObjectType EditObject = { 0, 0 ,0,{}};

static void ExitEditMode()
{
	EditObject.editmode = 0;
	EditObject.max_edit_object = 0;

}
static uint8_t isEditMode()
{
	return (EditObject.editmode);
}
static uint8_t isEditObjectConfig()
{
	return (EditObject.max_edit_object == 0) ? 0 : 1;
}
static void ConfigEditMode(xScreenSetObject* menu)
{
	 uint8_t k = 0;
	 EditObject.edit_object_index = 0;
	 for (uint8_t i = 0; i< MAX_SCREEN_OBJECT;i++)
	 {
			switch ( menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[i].xType)
			{
			   case EDIT_DATA_ENUM:
			   case EDIT_DATA_NUM:
			    		EditObject.edit_data[k].obect_id = i;
			    		EditObject.edit_data[k].last_object = 1;
			    	    EditObject.edit_data[k].cursor_x = menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[i].x
			    		    		       + menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[i].Width;
			    		 EditObject.edit_data[k++].cursor_y = menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[i].y;
			    		break;
			    	default:
			    		break;
			  }
			  if (menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[i].last == LAST_OBJECT)
			  {
			       EditObject.edit_data[k].last_object = 0;
			       break;
			  }
	 }
	return;
}
static void incrementCursor()
{
	EditObject.edit_object_index ++;
	if (EditObject.edit_object_index >= EditObject.max_edit_object)
	{
		EditObject.edit_object_index  = 0;
	}
	return;
}
static uint8_t getXCursor( void )
{
	return (EditObject.edit_data[EditObject.edit_object_index].cursor_x);
}
static uint8_t getYCursor( void)
{
	return (EditObject.edit_data[EditObject.edit_object_index].cursor_y);
}
static uint8_t getObjectIndex()
{
	return (EditObject.edit_data[EditObject.edit_object_index].obect_id);
}



void vDataScreenCallBack ( xScreenSetObject* menu, char key )
{
	 /*  switch ( key )
	   {
		     case KEY_UP:
		    	// edit_mode = 1;
		    	 break;
		     case KEY_DOWN:
		    //	 edit_mode = 1;
		       	 break;
		     case KEY_EXIT:
		    	 if (isEditMode())
		    	 {
		    		 ExitEditMode();
		    		 LCD_SET_Cursor( 0 , 0, 0);
		    	 }
		    	 else
		    	 {
		    		 menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[getObjectIndex()].GetDtaFunction(
		    		 		    				 mESC,NULL,menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[getObjectIndex()].DataID);
		    		 menu->pHomeMenu[menu->pCurrIndex].CurLine = 0;
		    		 menu->pHomeMenu[menu->pCurrIndex].CursorOwener = 0;
		    		 pCurrMenu = menu->pHomeMenu[menu->pCurrIndex].pUpScreenSet;
		    	 }
		    	 break;
		     case KEY_ENTER:
		    	 if (isEditObjectConfig() == 0)
		    	 {
		    		 ConfigEditMode(menu);
		    	 }
		    	 if (isEditMode())
		    	 {
		    		 menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[getObjectIndex()].GetDtaFunction(
		    				 mSAVE,NULL,menu->pHomeMenu[menu->pCurrIndex].pScreenCurObjets[getObjectIndex()].DataID);
		    	 }
		    	 else
		    	 {
		    		 incrementCursor();
		    	 }
		    	 LCD_SET_Cursor( 1 , getXCursor(),getYCursor());
		    	 break;
		     default:
		    	 break;
	   }*/
}

void xCommonScreenCallBack ( xScreenSetObject* menu, char key )
{
	/*switch (menu->pHomeMenu[menu->pCurrIndex].scteen_type)
    {
				case DATA_SCREEN:
	       		  if ( key == KEY_EXIT)
	       			vMenuScreenCallBack (menu, key,1);
	       		    break;
				case EDIT_SCREEN:
	       			vDataScreenCallBack( menu, key);
	       		   break;
	       	   case MENU_SCREEN:
	       		   vMenuScreenCallBack (menu, key,1);
	       		   break;
	       	   case LIST_SCREEN:
	       		   vListScreenCallBack (menu, key );
	       		   break;
	       	   case MENU_1_LINE_STRING:
	       		   vMenuScreenCallBack (menu, key, 2);
	       		   break;
	       	   default:
	       		    break;
	   }
*/
}




void vMenuScreenCallBack ( xScreenSetObject* menu, char key , uint8_t inc_index)
{

  return;
}
#define UP_KEY_READY 0x01
#define DOWN_KEY_READY 0x02
static FLAG              fDownScreen      = FLAG_RESET;

static KeyEvent          TempEvent        = { 0U };
static KeyEvent          BufferEvent      = { 0U };

void xMainScreenCallBack ( xScreenSetObject* menu, char key )
{

	uint8_t           index = menu->pCurrIndex;
	xScreenSetObject* pMenu = menu;


	  switch ( key )
	  {
	    case KEY_UP:
	    	if ( fDownScreen == FLAG_RESET )
	    	{
	          menu->pCurrIndex++;
	          if (menu->pCurrIndex > menu->pMaxIndex)
	          {
	        	 pCurrMenu->pCurrIndex = 0U;
	          }
	        }
	    	else
	    	{
	    		if (menu->pHomeMenu[menu->pCurrIndex].DataIndex<255) menu->pHomeMenu[menu->pCurrIndex].DataIndex++ ;
	    	}
	      break;
	    case KEY_DOWN:
	    	if ( fDownScreen == FLAG_RESET )
	    	{
	    		  if (menu->pCurrIndex > 0)
	    		         {
	    		        	 pCurrMenu->pCurrIndex--;
	    		         }
	    		  else
	    			  pCurrMenu->pCurrIndex = 0;
	    	}
	    	else
	    	{
	    		if (menu->pHomeMenu[menu->pCurrIndex].DataIndex>0) menu->pHomeMenu[menu->pCurrIndex].DataIndex-- ;
	    	}
	       break;
	    case  KEY_ENTER:
	       if ( fDownScreen == FLAG_SET )
	            {
	              fDownScreen = FLAG_RESET;
	              if ( menu->pHomeMenu[index].pUpScreenSet != NULL )
	              {
	                pCurrMenu = menu->pHomeMenu[index].pUpScreenSet;
	                menu->pHomeMenu[menu->pCurrIndex].DataIndex = 0;
	                pMenu     = pCurrMenu;
	              }
	            }
	            pMenu->pCurrIndex = ( pMenu->pCurrIndex == pMenu->pMaxIndex ) ? 0U : pMenu->pCurrIndex + 1;
	         //   uCurrentAlarm =0;
	         //   key_ready &= ~SET_MENU_READY;
	            break;
	          case KEY_EXIT:
	              //Если нажата клавиша вниз, проверяем флаг, сигнализурующий что мы листаем
	              //карусель вложенных экранов

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

void vMenu( void)
{
	static uint8_t           key              = 0U;
	//vDrawObject( ( xScreenObjet * )pCurrMenu->pHomeMenu[pCurrMenu->pCurrIndex].pScreenCurObjets );
	 vDrawObject( pCurrMenu );
	 if ( xQueueReceive( pKeyboard, &TempEvent, 0U ) == pdPASS )
	  {

	       BufferEvent = TempEvent;
	      //Если зафиксировано нажатие клавиши
	      if ( TempEvent.Status == MAKECODE )
	      {
	          key = TempEvent.KeyCode | MAKECODE;
	      }
	      else
	    	  key = 0;
	      if (key!= 0)


	      pCurrMenu->pFunc( pCurrMenu, key );
	  }


}
