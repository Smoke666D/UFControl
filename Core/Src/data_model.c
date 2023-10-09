/*
 * data_model.c
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */


#include "data_model.h"
#include "stdio.h"
#include "math.h"

#define STRING_BUFFER_SIZE 4U
extern RTC_HandleTypeDef hrtc;
uint32_t mod_bus_addr = 0;
uint32_t dev_mod = 0;
uint8_t DATA_MODEL_REGISTER[TOTAL_REGISTER_COUNT];
static uint8_t EditFlag = 0;
static uint32_t EditDATA = 0;
uint8_t bytebuffer;
static void vSetResLampMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t index);

//static void vSetAllResLampMenu( DATA_COMMNAD_TYPE cmd, char* Data);
static void vSetLampCount(DATA_COMMNAD_TYPE cmd, char* Data);
char const AllOk[] = "Все исправны";
static const char * ErrorStrings[] ={ "Аварий нет",
										"Открыта крышка",
										"Пожарная тревога",
										"Отказ одной лампы",
										"Отказ 2+ ламп",
										"Наработка 97%",
										"Наработка 100%",
										"Напряжение <198В",
										"Напряжение<187В",
										"Напряжение>250В"
	};

static void PrintString(uint8_t * dest,const char * source)
{
	for (uint8_t i=0;i<20;i++)
	{
		dest[i] = source[i];
		if (source[i] == 0) break;
	}
}
/*
 *
 */
void vGetErrorForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
    uint8_t error_count = 0;
    int16_t index = usGetDataViewIndex();
    uint16_t error_reg = int16GetRegister(DEVICE_ALARM_REG);
    for (uint8_t i=0;i< REMAIN_RESOURS_0;i++)
    {
    	if (error_reg & (0x1<<i)) error_count++;
    }
   	if  (index+1 > error_count )
   	{
   	 	index = (error_count == 0) ? 0 : error_count - 1;
   	 	vSetDataViewIndex(index );
   	}
    switch (ID)
    {
    	case  SCREEN_INDEX_ID:
    		sprintf(Data,"%u",(error_count ==0 ) ? 0: index + 1 );
    		break;
    	case TOTAL_ERROR_ID:
    		sprintf(Data,"%u",error_count);
    		break;
    	case ERROR_DATA_ID:
    		sprintf(Data,"%s",ErrorStrings[index]);
    		break;
    	default:

    		break;

    }
    return;
}
/*
 * Функция отобаражет ошибки работы ламп
 */
 void vGetLapmErrorForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
 {
	 uint16_t index = usGetDataViewIndex();
	 uint16_t max_index = int8GetRegister(LAPM_ERROR_COUNT );
	 if  (index >= max_index )
	 {
		 index = ( max_index == 0 ) ? 0 : max_index -1 ;
		 vSetDataViewIndex( index );
	 }
	 switch (ID)
	 {
	 	 case SCREEN_INDEX_ID:
	 		sprintf(Data,"%u",( max_index ==0 ) ? 0: index + 1 );
	 		 break;
	 	 case TOTAL_LAMP_ERROR_ID:
	 		 sprintf(Data,"%u",int8GetRegister(LAPM_ERROR_COUNT ));
	 		 break;
	 	 case LAMP_ERROR_DATA_ID:
	 		 if (int8GetRegister(LAPM_ERROR_COUNT ) == 0)
	 		 {
	 			sprintf(Data,"Все исправны");
	 		 }
	 		 else
	 		 {
	 			 uint8_t lamp_number =0;
	 			 uint64_t buffer = int32GetData(LAM_ERROR_REG_MSB);
	 			 buffer = buffer <<22;
	 			 buffer = buffer | int32GetData(LAM_ERROR_REG_LSB);
                  uint8_t i;
	 			 for ( i = 0; i<int8GetRegister(LAMP_COUNT );i++)
	 			 {
	 				 if (buffer & 0x0001)
	 				 {
	 					 if (++lamp_number == (index + 1))
	 						 break;
	 				 }
	 				 buffer =buffer >> 1;
	 			 }
	 			 sprintf(Data,"Отказ лампы N%02u",i+1);
	 		 }

	 		 break;
	 	 default:
	 		 break;
	 }
 }

void vGetFBOHSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{


	switch (cmd)
	{
		case mREAD:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_H) ;

				sprintf(Data,"%02u",(uint16_t) EditDATA );
				break;
		case mINC:
			    if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_H) ;
				EditFlag = 1;
				if ( ++EditDATA > 99 )  EditDATA = 99;
				break;
		case mDEC:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_H) ;
				EditFlag = 1;
				if (EditDATA  > 0)  EditDATA--;
				break;
		case mSAVE:
				int16SetRegister(FBO_SIZE_H, (uint16_t)EditDATA );
				eEEPROMWr(FBO_SIZE_H ,&DATA_MODEL_REGISTER[FBO_SIZE_H],2);
		case mESC:
				EditDATA = 0;
				EditFlag = 0;
				break;
	}
}

static uint16_t step = 1;
void vGetFBOWSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	uint16_t index = usGetDataViewIndex();
    switch (index)
    {
    	case 0:step = 1000;break;
    	case 1:step = 100;break;
    	case 2:step = 10;break;
    	case 3:step = 1;break;

    }

	switch (cmd)
	{
		case mREAD:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_W) ;
				sprintf(Data,"%04u",(uint16_t) EditDATA );
				break;
		case mINC:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_W) ;
				EditFlag = 1;
				EditDATA = EditDATA + step;
				if ( EditDATA > 9999 )  EditDATA = 9999;
				break;
		case mDEC:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_W) ;
				EditFlag = 1;
				if ((EditDATA -step)> 0 )
					EditDATA = EditDATA - step;
				break;
		case mSAVE:
				int16SetRegister(FBO_SIZE_W, (uint16_t)EditDATA );
				eEEPROMWr(FBO_SIZE_W ,&DATA_MODEL_REGISTER[FBO_SIZE_W],2);
		case mESC:
				EditDATA = 0;
				EditFlag = 0;

				break;
	}
}



void vGetFBOLSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{


	uint16_t index = usGetDataViewIndex();
	    switch (index)
	    {

	    	case 1:step = 100;break;
	    	case 2:step = 10;break;
	    	case 3:step = 1;break;

	    }
	switch (cmd)
	{
		case mREAD:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_L) ;
				sprintf(Data,"%03u",(uint16_t) EditDATA );
				break;
		case mINC:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_L) ;
				EditFlag = 1;
				EditDATA = EditDATA + step;
				if ( EditDATA > 999 )  EditDATA = 999;
				break;
		case mDEC:
			if (EditFlag == 0) EditDATA = int16GetRegister( FBO_SIZE_L) ;
				EditFlag = 1;
				if ((EditDATA -step) > 0 )
					EditDATA = EditDATA - step;
				break;
		case mSAVE:
				int16SetRegister(FBO_SIZE_L, (uint16_t)EditDATA );
				eEEPROMWr(FBO_SIZE_L ,&DATA_MODEL_REGISTER[FBO_SIZE_L],2);
		case mESC:
				EditDATA = 0;
				EditFlag = 0;

				break;
	}
}


void vGetRecourceEditForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	uint16_t index = usGetDataViewIndex();
	uint16_t max_index = int8GetRegister(LAMP_COUNT);
	if  (index  >= max_index )
	{
		index = ( max_index == 0 ) ? 0 : max_index -1 ;
		vSetDataViewIndex( index );
	}
	switch (ID)
	{

		case SCREEN_INDEX_ID:

			sprintf(Data,"%u",( max_index ==0 ) ? 0: index + 1 );
			break;
		case TOTAL_LAMP_DATA_ID:
			vSetLampCount(cmd,Data);
			break;
		case LAMP_RES_DATA_ID:
			vSetResLampMenu(cmd, Data, index);

			break;
		case LAMP_RES_PROCENT_DATA_ID:
			sprintf(Data,"%u %%", int8GetRegister( LAMP_RESURSE_INDEX  + index) );
		default:
			break;
	}
}


/*
 * Функция отображения на экранае максимального времени работы ламм
 */
void vGetRecourceForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	uint16_t index = usGetDataViewIndex();
	uint16_t max_index = int8GetRegister(LAMP_COUNT);
	if  (index  >= max_index )
	{
		index = ( max_index == 0 ) ? 0 : max_index -1 ;
		vSetDataViewIndex( index );
	}
	switch (ID)
	{
		case SCREEN_INDEX_ID:
			sprintf(Data,"%u",( max_index ==0 ) ? 0: index + 1 );
			break;
		case TOTAL_LAMP_DATA_ID:
			vSetLampCount(cmd,Data);
			break;
		case LAMP_RES_DATA_ID:
			sprintf(Data,"%u ч", (uint16_t)int8GetRegister(LAMP_MAX_TIME_INDEX + index) * 1000 );
			//vSetResLampMenu(cmd, Data, index);

			break;
		case LAMP_RES_PROCENT_DATA_ID:
			sprintf(Data,"%u %%", int8GetRegister( LAMP_RESURSE_INDEX  + index) );

		default:
			break;
	}
}



void vSetRecourceForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	uint16_t index = usGetDataViewIndex();
	uint16_t max_index = int8GetRegister(LAMP_COUNT);
	if  (index  >= max_index )
	{
		index = ( max_index == 0 ) ? 0 : max_index -1 ;
		vSetDataViewIndex( index );
	}
	switch (ID)
	{
		case SCREEN_INDEX_ID:
			sprintf(Data,"%u",( max_index ==0 ) ? 0: index + 1 );
			break;
		case TOTAL_LAMP_DATA_ID:
			sprintf(Data,"%0u",(uint8_t) int8GetRegister( LAMP_COUNT) );
			break;
		default:
			switch (cmd)
				{
				  case mREAD:
					  if (EditFlag)
						  sprintf(Data,"%u ч", (uint8_t)EditDATA * 1000);
					  else
						  sprintf(Data,"%u ч", (uint16_t)int8GetRegister(LAMP_MAX_TIME_INDEX + index) * 1000 );
					  break;
				  case mINC:
					  if (EditFlag == 0) EditDATA = int8GetRegister(LAMP_MAX_TIME_INDEX + index);
					  EditFlag = 1;
					  if ( ++EditDATA > 13 )  EditDATA= 1;
					 break;
				  case mDEC:
					  if (EditFlag == 0) EditDATA = int8GetRegister(LAMP_MAX_TIME_INDEX + index);
					  EditFlag = 1;
					  if ( --EditDATA == 0)  EditDATA= 1;
					  break;
				  case mSAVE:
					  EditFlag = 0;
					  int8SetRegister(LAMP_MAX_TIME_INDEX + index, (uint8_t)EditDATA );
					  eEEPROMWr(LAMP_MAX_TIME_INDEX+ index ,&DATA_MODEL_REGISTER[LAMP_MAX_TIME_INDEX + index],1);
					  break;

				  case mESC:
					  EditDATA = 0;
					  EditFlag = 0;
					  break;
				}
			//vSetResLampMenu(cmd, Data, index);
			break;
	}
}





void vResetRecourceLamp( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{

	switch ( cmd)
	{
		case  mREAD:
			if ( EditFlag ==0) EditDATA = 0;
			if ( EditDATA  == 0)
			{
				sprintf(Data,"Всех ламп" );
			}
			else
			{
				sprintf(Data,"лампы %02u",EditDATA );
			}
         	break;
		 case mINC:
				 EditFlag = 1;
				if ( ++EditDATA > int8GetRegister(LAMP_COUNT ))  EditDATA = int8GetRegister(LAMP_COUNT );
				break;
		case mDEC:
				EditFlag = 1;
				if ( EditDATA > 0)  EditDATA--;
			break;
		case mSAVE:
			if (EditDATA!= 0)
			{
				*((uint32_t *)&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX + EditDATA - 1]) = 0;
				eEEPROMWr(LAMP_WORK_HOURS_INDEX + EditDATA - 1 ,&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX + EditDATA - 1],4);
			}
			else
			{
				memset(&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX],0,44*4);
				eEEPROMWr(LAMP_WORK_HOURS_INDEX ,&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX],44*4);

			}
		case mESC:
			EditDATA = 0;
			EditFlag = 0;
			break;

	}

}
 void vEditLampCount(DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	switch (cmd)
		{
				case mREAD:
					if (EditFlag == 0) EditDATA = int8GetRegister( LAMP_COUNT) ;
					sprintf(Data,"%0u",(uint8_t) EditDATA );
					break;
				  case mINC:
				    EditFlag = 1;
				    if ( ++EditDATA > 44 )  EditDATA = 44;
				    break;
				 case mDEC:
					EditFlag = 1;
					if ( EditDATA > 0)  EditDATA--;
					break;
				case mSAVE:
					int8SetRegister(LAMP_COUNT, (uint8_t)EditDATA );
					eEEPROMWr(LAMP_COUNT ,&DATA_MODEL_REGISTER[LAMP_COUNT],1);
				case mESC:
					EditDATA = 0;
					EditFlag = 0;
					break;
		}
}


static void vSetLampCount(DATA_COMMNAD_TYPE cmd, char* Data)
{
	switch (cmd)
		{
				case mREAD:
					if (EditFlag == 0) EditDATA = int8GetRegister( LAMP_COUNT) ;
					sprintf(Data,"%0u",(uint8_t) EditDATA );
					break;
				  case mINC:
				    EditFlag = 1;
				    if ( ++EditDATA > 44 )  EditDATA = 44;
				    break;
				 case mDEC:
					EditFlag = 1;
					if ( --EditDATA == 0)  EditDATA= 1;
					break;
				case mSAVE:
					int8SetRegister(LAMP_COUNT, (uint8_t)EditDATA );
					eEEPROMWr(LAMP_COUNT ,&DATA_MODEL_REGISTER[LAMP_COUNT],1);
				case mESC:
					EditDATA = 0;
					EditFlag = 0;
					break;
		}
}

static RTC_TimeTypeDef time;
void vSetTimeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{

	switch (cmd)
	{
			case mREAD:
				if (EditFlag == 0) HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
				sprintf(Data,"%02u:%02u:%02u",time.Hours,time.Minutes,time.Seconds);
				break;
			case mINC:
				EditFlag = 1;
				switch ( usGetDataViewIndex())
				{
					case 0:
						if (++time.Hours >= 24) time.Hours = 0;
						break;
					case 1:
						if (++time.Minutes >= 59) time.Minutes = 0;
						break;
					case 2:
						if (++time.Seconds >= 59) time.Seconds = 0;
						break;
				}
				break;
			case mDEC:
				EditFlag = 1;
				switch ( usGetDataViewIndex())
				{
					case 0:
						if (time.Hours >= 1) time.Hours--;
						break;
					case 1:
						if (time.Minutes >= 1) time.Minutes--;
						break;
					case 2:
						if (time.Seconds >= 1) time.Seconds--;
						break;
				}
				break;
			 case mSAVE:

					HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);

			 case mESC:
					EditFlag = 0;
					EditDATA = 0;
					break;

	}
}

static  RTC_DateTypeDef date;
void vSetDateForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{

	switch (cmd)
	{
			case mREAD:
				if (EditFlag == 0) HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
				sprintf(Data,"%02u.%02u.%02u",date.Date,date.Month,date.Year );
				break;
			case mINC:
				EditFlag = 1;
				switch ( usGetDataViewIndex())
				{
					case 0:
						if (++date.Date >= 31) date.Date = 0;
						break;
					case 1:
						if (++date.Month >= 12) date.Month = 0;
						break;
					case 2:
						if (++date.Year >= 99) date.Year = 0;
						break;
				}
				break;
			case mDEC:
				EditFlag = 1;
				switch ( usGetDataViewIndex())
				{
					case 0:
						if (date.Date  >= 1) date.Date --;
						break;
					case 1:
						if (date.Month >= 1) date.Month--;
						break;
					case 2:
						if (date.Year >= 1) date.Year--;
						break;
				}
				break;
			 case mSAVE:
					HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
			 case mESC:
				    EditDATA = 0;
					EditFlag = 0;
					break;

	}
}






 void vSetAllResLampMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t index)
{
	switch (cmd)
	{
	  case mREAD:
		  uint8_t temp_data =1;
		  if (EditFlag == 1)
			  temp_data = EditDATA;
		  sprintf(Data,"%u ч", (uint8_t)temp_data * 1000);
		  break;
	  case mINC:
		  if ( EditFlag == 0 ) EditDATA = 1;
		  EditFlag = 1;
		  if ( ++EditDATA > 13 )  EditDATA= 1;
		 break;
	  case mDEC:
		  if ( EditFlag == 0 ) EditDATA = 1;
		  EditFlag = 1;
		  if ( --EditDATA == 0)  EditDATA= 1;
		  break;
	  case mSAVE:
		  EditFlag = 0;
		  for (uint8_t i= 0;i<(uint8_t) int8GetRegister( LAMP_COUNT) ;i++)
		  {
			  int8SetRegister(LAMP_MAX_TIME_INDEX + i, (uint8_t)EditDATA );
		  }
		  eEEPROMWr(LAMP_MAX_TIME_INDEX ,&DATA_MODEL_REGISTER[LAMP_MAX_TIME_INDEX],(uint8_t) int8GetRegister( LAMP_COUNT));
	  case mESC:
		  EditDATA = 0;
		  EditFlag = 0;
		  break;
	}
}

uint32_t int32GetData( uint16_t index )
{
	return  (*((uint32_t *)&DATA_MODEL_REGISTER[index]));
}
void int32SetData( uint16_t index, uint32_t data)
{

	(*((uint32_t *)&DATA_MODEL_REGISTER[index])) = data;
}


void int16SetRegisterBit(uint16_t addres, uint8_t bits, uint8_t data)
{
	if ( addres < TOTAL_REGISTER_COUNT  )
	{
		if (data != 0)
			*((uint16_t *)&DATA_MODEL_REGISTER[addres]) |= 0x1 << bits;
		else
			*((uint16_t *)&DATA_MODEL_REGISTER[addres])  &= ~(0x1 << bits);
	}
}

uint16_t int16GetRegister(uint16_t addres)
{
	return  (( addres < TOTAL_REGISTER_COUNT   ) ? *((uint16_t *)&DATA_MODEL_REGISTER[addres]) : 0);
}

void int16SetRegister(uint16_t addres, uint16_t data)
{
	(*((uint16_t *)&DATA_MODEL_REGISTER[addres])) = data;
}


void int8SetRegisterBit(uint16_t addres, uint8_t bits, uint8_t data)
{
	if ( addres < TOTAL_REGISTER_COUNT  )
	{
		if (data != 0)
			DATA_MODEL_REGISTER[addres] |= 0x1 << bits;
		else
			DATA_MODEL_REGISTER[addres] &= ~(0x1 << bits);
	}

}

uint8_t int8GetRegister(uint16_t addres)
{
	return  (( addres < TOTAL_REGISTER_COUNT   ) ? DATA_MODEL_REGISTER[addres] : 0);

}
void int8SetRegister( uint16_t addres , uint8_t data )
{
	if (addres < TOTAL_REGISTER_COUNT   )
		DATA_MODEL_REGISTER[addres]  = data;
	return;
}

void vGetDataForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID)
{
   switch (ID)
   {
   	   case STATUS_ID:
   		   switch ( int8GetRegister(DEVICE_STATUS_REG) & 0x03)
   		   {
   		   	   case 0:
   		   		sprintf(Data,"Выкл");
   		   		break;
   		   	   case 1:
   		   		 sprintf(Data,"Дист");
   		   		  break;
   		   	   case 2:
   		   	     sprintf(Data,"Мест");
   		   		  break;
   		   	   default:
   		   		   break;
   		   }
   		   break;
   	   case WORK_ID:
   		    if  (  int8GetRegister(DEVICE_OUTPUT_REG) & (0x01<<WORK_OUT_FLAG) )
   		    	sprintf(Data,"Д");
   		    else
   		    	sprintf(Data,"Н");
   		   break;
   	   case OPEN_ID:
   		  if  ( int16GetRegister(DEVICE_ALARM_REG) & ( 0x01<<DOOR_ALARM)  )
   		   	 sprintf(Data,"Закр");
   		  else
   		   	 sprintf(Data,"Откр");
   		   break;
   	   case ALARM_ID:
   		   if  ( int16GetRegister(DEVICE_ALARM_REG)== 0 )
   			   sprintf(Data,"Н");
   		   else
   			  sprintf(Data,"Д");
   		   break;
   	   case CUR_TIME_ID:
   		   RTC_TimeTypeDef time;
   		   HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
   		   RTC_DateTypeDef date;
   		   HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
   		   sprintf(Data,"%02u:%02u:%02u  %02u.%02u.20%02u",time.Hours,time.Minutes,time.Seconds,date.Date,date.Month,date.Year);
   		   break;
       case  FBO_SIZE_ID:
    	   sprintf(Data,"%04ux%03u-%02u ",int16GetRegister(FBO_SIZE_W ),int16GetRegister(FBO_SIZE_H ),int16GetRegister(FBO_SIZE_L ));
    	   break;
   	   default:
   		   break;
   }
}

 void vGetMbTypeMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID)
{
	uint8_t data =0;
	switch (cmd)
	{
	  case mREAD:
		  data = (EditFlag == 0) ? int8GetRegister(MODBUS_ADDRES ) : (uint8_t)EditDATA;
		  sprintf(Data,"%u", data);
		  break;
	  case mINC:
		  if (EditFlag == 0) EditDATA = int8GetRegister(MODBUS_ADDRES );
		  EditFlag = 1;

		  if ( ++EditDATA >= 255 )  EditDATA= 1;
		 break;
	  case mDEC:
		  if (EditFlag == 0) EditDATA = int8GetRegister(MODBUS_ADDRES );
		  EditFlag = 1;
		  if ( --EditDATA == 0)  EditDATA= 1;
		  break;
	  case mSAVE:
		  EditFlag = 0;
		  int8SetRegister(MODBUS_ADDRES, (uint8_t)EditDATA );
		  data = (uint8_t)EditDATA;
		  eEEPROMWr(MODBUS_ADDRES, &data,1);
	  case mESC:
		  EditDATA = 0;
		  EditFlag = 0;
		  break;
	}
}

void vGetControlTypeMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID)
{
	uint8_t data =0;
	switch (cmd)
	{
	  case mREAD:
		  data = (EditFlag == 0) ? int8GetRegister(CONTROL_TYPE_REG ) : (uint8_t)EditDATA;
		  sprintf(Data,"%s",(data ? "ЩИТ" : "SCADA"));
		  break;
	  case mINC:
		  if (EditFlag == 0) EditDATA =int8GetRegister(CONTROL_TYPE_REG );
		  EditFlag = 1;
		  if ( EditDATA== 0 )  EditDATA= 1;
		 break;
	  case mDEC:
		  if (EditFlag == 0) EditDATA =int8GetRegister(CONTROL_TYPE_REG );
		  if ( EditDATA == 1)  EditDATA= 0;
		  EditFlag = 1;
		  break;
	  case mSAVE:
		  EditFlag = 0;
		  int8SetRegister(CONTROL_TYPE_REG, (uint8_t)EditDATA  );
		  data = (uint8_t)EditDATA;
		  eEEPROMWr(CONTROL_TYPE_REG ,&DATA_MODEL_REGISTER[CONTROL_TYPE_REG],1);
		  break;
	  case mESC:
		  EditDATA = 0;
		  EditFlag = 0;
		  break;
	}

}



void InitDataModel()
{
	EditFlag = 0;
	if ( eEEPROMRd(0x00 ,DATA_MODEL_REGISTER , EEPROM_REGISER_COUNT) == EEPROM_OK)
	{
		if (DATA_MODEL_REGISTER[VALID_CODE_ADDRES]!=VALID_CODE )
		{
			memset(DATA_MODEL_REGISTER,0,EEPROM_REGISER_COUNT);
			DATA_MODEL_REGISTER[VALID_CODE_ADDRES] = VALID_CODE;
			DATA_MODEL_REGISTER[CONTROL_TYPE_REG ] = (0x01<<LOCAL_TYPE);
			DATA_MODEL_REGISTER[MODBUS_ADDRES  ] = 0x01;
			DATA_MODEL_REGISTER[LAMP_COUNT]= 44;
			*((uint16_t *)&DATA_MODEL_REGISTER[FBO_SIZE_W ]) =1000;
			*((uint16_t *)&DATA_MODEL_REGISTER[FBO_SIZE_H   ]) = 800;
			eEEPROMWr(VALID_CODE_ADDRES,DATA_MODEL_REGISTER,EEPROM_REGISER_COUNT);
			memset(DATA_MODEL_REGISTER,0,EEPROM_REGISER_COUNT);
			eEEPROMRd(0x00 ,DATA_MODEL_REGISTER , EEPROM_REGISER_COUNT);
		}
	}
}

/*
 * Функция записи данных о времени работы ламп в EEPROM
 */
void vLAMWorkHoursWrite()
{
	eEEPROMWr(LAMP_WORK_HOURS_INDEX,DATA_MODEL_REGISTER,LAMP_WORK_HOURS_SIZE );
}




