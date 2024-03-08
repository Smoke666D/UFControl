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
static uint16_t step = 1;
uint8_t bytebuffer;
extern RTC_HandleTypeDef hrtc;

//static void vSetAllResLampMenu( DATA_COMMNAD_TYPE cmd, char* Data);
static void vSetLampCount(DATA_COMMNAD_TYPE cmd, char* Data);
char const AllOk[] = "Все исправны";
static const char * ErrorStrings[] ={ "Аварий нет",
										"Открыта крышка",
										"Пожарная тревога",
										"Отказ ламп",
										"Наработка 97%",
										"Наработка 100%",
										"Напряжение <187В",
										"Напряжение<198В",
										"Напряжение>250В"
	};
/*
 *
 */
void vGetErrorForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
    uint8_t error_count = 0;
    int16_t index = usGetDataViewIndex();
    uint16_t error_reg = int8GetRegister(DEVICE_ALARM_REG);
    for (uint8_t i=0;i <= VOLT_250 ;i++)
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
    		uint8_t i;
    		uint8_t k= 0;
    		for (i=0;i< VOLT_250;i++)
    		{
    			if (error_reg>>i & 0x01)
    			{
    				k++;
    				if ((k-1) == index) break;
    			}
    		}

    		sprintf(Data,"%s",ErrorStrings[(error_count ==0 ) ? 0: i + 1]);
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
	 			 if ( i!=int8GetRegister(LAMP_COUNT ))
	 				 sprintf(Data,"Отказ лампы N%02u",i+1);
	 			 else
	 				sprintf(Data,"Все исправны");
	 		 }

	 		 break;
	 	 default:
	 		 break;
	 }
 }

/*
 * callback функция для пароля
 */
void vGetPassword( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
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
			    if (EditFlag == 0) EditDATA = 0 ;
				sprintf(Data,"%04u",(uint16_t) EditDATA );
				break;
		case mINC:
				EditFlag = 1;
				EditDATA = EditDATA + step;
				if ( EditDATA > 9999 )  EditDATA = 9999;
				break;
		case mDEC:
				EditFlag = 1;
				if ((EditDATA -step)> 0 )
					EditDATA = EditDATA - step;
				break;
		case mSAVE:
			    if (int16GetRegister( PASSWORD  ) == EditDATA )
			    {
			    	Data[0] = 0x55;
			    }
			    else
			    {
			    	Data[0] = 0x55;
			    }
		case mESC:
				EditDATA = 0;
				EditFlag = 0;
				step = 1;
				break;
	}
}

void vGetVoltForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{

	uint8_t INDEX;
	switch (ID)
	{
	case  GET_V187: INDEX = VLOW; break;
	case  GET_V187_ON	: INDEX = VLOW_ON	; break;
	case  GET_V197: INDEX = WWAR; break;
	case  GET_V197_ON: INDEX =  WWAR_ON	; break;
	case  GET_V250: INDEX = VHIGH; break;
	case  GET_V250_ON: INDEX = VHIGH_ON; break;
}
	switch (cmd)
	{
		case mREAD:
			if (EditFlag == 0) EditDATA = int8GetRegister( INDEX ) ;

				sprintf(Data,"%03u",(uint16_t) EditDATA );
				break;
		case mINC:
			    if (EditFlag == 0) EditDATA = int8GetRegister( INDEX) ;
				EditFlag = 1;
			    if (++EditDATA > 255 )  EditDATA = 255;
			    switch (INDEX)
			    {
			    	case VLOW_ON:
			    		if (EditDATA <= int8GetRegister(VLOW))
			    			EditDATA = int8GetRegister(VLOW)+1 ;
			    		break;
			    	case WWAR_ON:
			    		if (EditDATA <= int8GetRegister(WWAR))
			    			EditDATA = int8GetRegister(WWAR)+1 ;
			    		break;
			    	case VHIGH_ON:
			    		if (EditDATA >= int8GetRegister(VHIGH))
			    				EditDATA = int8GetRegister(VHIGH)-1 ;
			    		break;

			    }
				break;
		case mDEC:
			if (EditFlag == 0) EditDATA = int8GetRegister( INDEX) ;
				EditFlag = 1;
				if (EditDATA  > 0)  EditDATA--;
				 switch (INDEX)
							    {
							    	case VLOW_ON:
							    		if (EditDATA <= int8GetRegister(VLOW))
							    			EditDATA = int8GetRegister(VLOW)+1 ;
							    		break;
							    	case WWAR_ON:
							    		if (EditDATA <= int8GetRegister(WWAR))
							    			EditDATA = int8GetRegister(WWAR)+1 ;
							    		break;
							    	case VHIGH_ON:
							    		if (EditDATA >= int8GetRegister(VHIGH))
							    				EditDATA = int8GetRegister(VHIGH)-1 ;
							    		break;

							    }
				break;
		case mSAVE:
				int8SetRegister(INDEX, (uint16_t)EditDATA );
				eEEPROMWr(INDEX ,&DATA_MODEL_REGISTER[INDEX],1);
				switch (INDEX)
				{
						case VLOW:
							  if (EditDATA>= int8GetRegister(VLOW_ON))
							  {
								  int8SetRegister(VLOW_ON, (uint16_t)EditDATA +1 );
								  eEEPROMWr(VLOW_ON ,&DATA_MODEL_REGISTER[VLOW_ON],1);
							  }
							  break;
						case WWAR:
							 if (EditDATA>= int8GetRegister(WWAR_ON))
							 {
								 int8SetRegister(WWAR_ON, (uint16_t)EditDATA +1 );
								 eEEPROMWr(WWAR_ON ,&DATA_MODEL_REGISTER[WWAR_ON],1);
							 }
							 break;
						case VHIGH:
							if (EditDATA <= int8GetRegister(VHIGH_ON))
							{
									int8SetRegister(VHIGH_ON, (uint16_t)EditDATA - 1 );
									eEEPROMWr(VHIGH_ON ,&DATA_MODEL_REGISTER[VHIGH_ON],1);
							}
							break;
						default:
							break;
				}
		case mESC:
				EditDATA = 0;
				EditFlag = 0;
				EditFlag = 0;
				break;
	}
}


void vGetFBOSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{

	uint16_t index = usGetDataViewIndex();
	uint16_t DataIndex = (ID==A_SIZE) ? FBO_SIZE_A : FBO_SIZE_B;
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
			if (EditFlag == 0) EditDATA = int16GetRegister( DataIndex ) ;
				sprintf(Data,"%04u",(uint16_t) EditDATA );
				break;
		case mINC:
			if (EditFlag == 0) EditDATA = int16GetRegister( DataIndex ) ;
				EditFlag = 1;
				EditDATA = EditDATA + step;
				if ( EditDATA > 9999 )  EditDATA = 9999;
				break;
		case mDEC:
			if (EditFlag == 0) EditDATA = int16GetRegister( DataIndex ) ;
				EditFlag = 1;
				if ((EditDATA -step)> 0 )
					EditDATA = EditDATA - step;
				break;
		case mSAVE:
				int16SetRegister( DataIndex , (uint16_t)EditDATA );
				eEEPROMWr( DataIndex  ,&DATA_MODEL_REGISTER[ DataIndex ],2);
		case mESC:
				EditDATA = 0;
				EditFlag = 0;
				step = 1;
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
		case LAMP_RES_PROCENT_DATA_ID:
			sprintf(Data,"%u ч", int16GetRegister( LAMP_RESURSE_INDEX  + index*2) );
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
	case   GET_V220:
		sprintf(Data,"%u",int8GetRegister( V220));
		break;
		case SCREEN_INDEX_ID:
			sprintf(Data,"%u",( max_index ==0 ) ? 0: index + 1 );
			break;
		case TOTAL_LAMP_DATA_ID:
			vSetLampCount(cmd,Data);
			break;
		case LAMP_RES_DATA_ID:
			sprintf(Data,"%u ч", (uint16_t)int8GetRegister(LAMP_MAX_TIME_INDEX + index) * res_mul );
			//vSetResLampMenu(cmd, Data, index);

			break;
		case LAMP_RES_PROCENT_DATA_ID:
			sprintf(Data,"%u ч", (uint16_t)int16GetRegister( LAMP_RESURSE_INDEX  + index*2) );

		default:
			break;
	}
}

void vGetJournal( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	static RTC_TimeTypeDef time;
	static RTC_DateTypeDef date;
	uint16_t index = usGetDataViewIndex();
	uint16_t max_index = int16GetRegister( RECORD_COUNT);
	uint16_t flag;
	if  (index  >= max_index )
	{
		index = ( max_index == 0 ) ? 0 : max_index -1 ;
		vSetDataViewIndex( index );
	}
	switch (ID)
	{
		case TOTAL_RECORD:
			sprintf(Data,"%u",int16GetRegister( RECORD_COUNT));
			break;
		case CUR_RECORD:
			if (int16GetRegister( RECORD_COUNT)!=0)
			{
				sprintf(Data,"N%u",(max_index == 0) ? 0 : index +1);
			}
			else
				Data[0]=0;
			break;
		case ALARM_STATUS:
			if (int16GetRegister( RECORD_COUNT)!=0)
			{
				uint8_t i;
				vGetRecord( index ,(uint8_t *)&flag,&time,&date );
				for (i=0;i< VOLT_250;i++)
				{
				if (flag>>i & 0x1) break;
				}
				sprintf(Data,"%s",ErrorStrings[i+1]);
			}
			else
				sprintf(Data,"%s","Нет записей!");
			break;
		case ALARM_TIME_STATUS:
			if (int16GetRegister( RECORD_COUNT)!=0)
			{
			vGetRecord( index ,(uint8_t *)&flag,&time,&date );
			sprintf(Data,"%02u:%02u:%02u  %02u.%02u.20%02u",time.Hours,time.Minutes,time.Seconds,date.Date,date.Month,date.Year);
			}
			else Data[0]=0;
			break;
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
					  if (EditFlag==0)
					  {
						  EditDATA = int8GetRegister(LAMP_MAX_TIME_INDEX + index);
					  }
					  sprintf(Data,"%u ч", (uint8_t)EditDATA * res_mul);
					  break;
				  case mINC:
					  if (EditFlag == 0) EditDATA = int8GetRegister(LAMP_MAX_TIME_INDEX + index);
					  EditFlag = 1;
					  if ( ++EditDATA > MAX_RESOURCE )  EditDATA= 1;
					 break;
				  case mDEC:
					  if (EditFlag == 0) EditDATA = int8GetRegister(LAMP_MAX_TIME_INDEX + index);
					  EditFlag = 1;
					  if (EditDATA > 0 )
					  {
						  if ( --EditDATA == 0)  EditDATA= 1;
					  }
					  break;
				  case mSAVE:
					  EditFlag = 0;
					  vSetLampRecource( index + 1, (uint8_t)EditDATA);
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


void vSetLampRecource( uint8_t lamp_index, uint16_t recource)
{
	if ( lamp_index == 0 )
	{
		for (uint8_t i= 0;i<(uint8_t) int8GetRegister( LAMP_COUNT) ;i++)
		{
			int8SetRegister(LAMP_MAX_TIME_INDEX + i, (uint8_t)recource );
		}
	    eEEPROMWr(LAMP_MAX_TIME_INDEX ,&DATA_MODEL_REGISTER[LAMP_MAX_TIME_INDEX],(uint8_t) int8GetRegister( LAMP_COUNT));
	}
	else
	{
		if ( lamp_index <= MAX_LAMP_COUNT )
		{
			int8SetRegister(LAMP_MAX_TIME_INDEX + lamp_index - 1 , (uint8_t)recource );
			eEEPROMWr(LAMP_MAX_TIME_INDEX + lamp_index -1 ,&DATA_MODEL_REGISTER[LAMP_MAX_TIME_INDEX + lamp_index - 1 ], 1 );
		}
	}

	return;
}

void vResetJournal( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	if (cmd == mSAVE)
	{
	    JournalClear();
	}
	else
	{
		if  (GetEditFlag() == SCREEN_VIEW)
		{
			Data[0] = 0;
		}
		else
			sprintf(Data,"Очистить?" );
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
				sprintf(Data,"лампы %02u",(uint8_t)EditDATA );
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
			vResetLampRecource(EditDATA);
		case mESC:
			EditDATA = 0;
			EditFlag = 0;
			break;

	}

}
/*
 *
 */
void vResetLampRecource(uint8_t lamp_index)
{
	if (lamp_index == 0)
	{
		memset(&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX],0,MAX_LAMP_COUNT*4);
		eEEPROMWr(LAMP_WORK_HOURS_INDEX ,&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX],MAX_LAMP_COUNT*4);
	}
	else
	{
		if (lamp_index <= MAX_LAMP_COUNT)
		{
			*((uint32_t *)&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX + (lamp_index- 1)*4]) = 0;
			eEEPROMWr(LAMP_WORK_HOURS_INDEX + lamp_index - 1 ,&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX + (lamp_index - 1)*4],4);
		}
	}
	return;
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
				    if ( ++EditDATA > MAX_LAMP_COUNT )  EditDATA = MAX_LAMP_COUNT;
				    break;
				 case mDEC:
					EditFlag = 1;
					if ( EditDATA > 1)  EditDATA--;
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
				    if ( ++EditDATA > MAX_LAMP_COUNT )  EditDATA = MAX_LAMP_COUNT;
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
						if (++date.Date > 31) date.Date = 0;
						break;
					case 1:
						if (++date.Month > 12) date.Month = 0;
						break;
					case 2:
						if (++date.Year > 99) date.Year = 0;
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
/*
 *
 */
 void vSetAllResLampMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t index)
{
	switch (cmd)
	{
	  case mREAD:
		  uint8_t temp_data;
		  if (EditFlag == 1)
			  temp_data = EditDATA;
		  else
			  temp_data = 1;
		  sprintf(Data,"%u ч", (uint8_t)temp_data * res_mul);
		  break;
	  case mINC:
		  if ( EditFlag == 0 ) EditDATA = 1;
		  EditFlag = 1;
		  if ( ++EditDATA > MAX_RESOURCE )  EditDATA= 1;
		 break;
	  case mDEC:
		  if ( EditFlag == 0 ) EditDATA = 1;
		  EditFlag = 1;
		  if ( --EditDATA == 0)  EditDATA= 1;
		  break;
	  case mSAVE:
		  if (EditFlag == 0) EditDATA = 1;
		  vSetLampRecource( 0 , EditDATA );
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
   		  if  ( int8GetRegister(DEVICE_ALARM_REG) & ( 0x01<<DOOR_ALARM)  )
   		   	 sprintf(Data,"Откр");
   		  else
   		   	 sprintf(Data,"Закр");
   		   break;
   	   case ALARM_ID:
   		   if  ( int8GetRegister(DEVICE_ALARM_REG)== 0 )
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
    	   sprintf(Data,"%04ux%04u",int16GetRegister(FBO_SIZE_A ),int16GetRegister(FBO_SIZE_B ));
    	   break;
   	   default:
   		   break;
   }
}


void vSetContrMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID)
{
	uint8_t data =0;
		switch (cmd)
		{
		  case mREAD:
			  data = (EditFlag == 0) ? int8GetRegister(CONTRAST_REG ) : (uint8_t)EditDATA;
			  sprintf(Data,"%u", data);
			  break;
		  case mINC:
			  if (EditFlag == 0) EditDATA = int8GetRegister(CONTRAST_REG );
			  EditFlag = 1;

			  if ( ++EditDATA > 10 )  EditDATA= 1;
			  int8SetRegister(CONTRAST_REG, (uint8_t)EditDATA );
			 break;
		  case mDEC:
			  if (EditFlag == 0) EditDATA = int8GetRegister(CONTRAST_REG );
			  EditFlag = 1;
			  if ( --EditDATA == 0)  EditDATA= 1;
			  int8SetRegister(CONTRAST_REG, (uint8_t)EditDATA );
			  break;
		  case mSAVE:
			  EditFlag = 0;
			  int8SetRegister(CONTRAST_REG, (uint8_t)EditDATA );
			  data = (uint8_t)EditDATA;
			  eEEPROMWr(CONTRAST_REG, &data,1);
		  case mESC:
			  EditDATA = 0;
			  EditFlag = 0;
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
		  sprintf(Data,"%s",(data ? "SCADA": "ЩИТ" ));
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
			*((uint16_t*)&DATA_MODEL_REGISTER[PASSWORD ]) = 7643;
			DATA_MODEL_REGISTER[CONTROL_TYPE_REG ] = (0x01<<LOCAL_TYPE);
			DATA_MODEL_REGISTER[MODBUS_ADDRES  ] = 0x01;
			DATA_MODEL_REGISTER[LAMP_COUNT]= MAX_LAMP_COUNT;
			*((uint16_t *)&DATA_MODEL_REGISTER[RECORD_INDEX  ])	= 0;
			*((uint16_t *)&DATA_MODEL_REGISTER[RECORD_COUNT ])	= 0;
			*((uint16_t *)&DATA_MODEL_REGISTER[FBO_SIZE_A ]) 	= 1000;
			*((uint16_t *)&DATA_MODEL_REGISTER[FBO_SIZE_B   ]) 	= 800;
			DATA_MODEL_REGISTER[VLOW]    	=  187;
			DATA_MODEL_REGISTER[VLOW_ON] 	=  197;
			DATA_MODEL_REGISTER[WWAR] 		=  198;
			DATA_MODEL_REGISTER[WWAR_ON] 	=  210;
			DATA_MODEL_REGISTER[VHIGH] 		=  250;
			DATA_MODEL_REGISTER[VHIGH_ON] 	=  240;
			DATA_MODEL_REGISTER[DAY] 	= 1;
			DATA_MODEL_REGISTER[MOUNTH] = 12;
			DATA_MODEL_REGISTER[YEAR] 	= 23;
			DATA_MODEL_REGISTER[CONTRAST_REG] = 7;
			for (int i=0;i<MAX_LAMP_COUNT;i++)
			{
				DATA_MODEL_REGISTER[i+LAMP_MAX_TIME_INDEX] = 9;
			}
			eEEPROMWr(VALID_CODE_ADDRES,DATA_MODEL_REGISTER,EEPROM_REGISER_COUNT);
			memset(DATA_MODEL_REGISTER,0,EEPROM_REGISER_COUNT);
			eEEPROMRd(0x00 ,DATA_MODEL_REGISTER , EEPROM_REGISER_COUNT);
		}
		else
		{
			static RTC_DateTypeDef date;
			hrtc.DateToUpdate.Date = DATA_MODEL_REGISTER[DAY];
			hrtc.DateToUpdate.Month = DATA_MODEL_REGISTER[MOUNTH];
			hrtc.DateToUpdate.Year = DATA_MODEL_REGISTER[YEAR];
			HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
		}
	}
}

/*
 * Функция записи данных о времени работы ламп в EEPROM
 */
void vLAMWorkHoursWrite()
{
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	DATA_MODEL_REGISTER[DAY] = date.Date;
	DATA_MODEL_REGISTER[MOUNTH]= date.Month;
	DATA_MODEL_REGISTER[YEAR] = date.Year;
	eEEPROMWr(DAY,&DATA_MODEL_REGISTER[DAY],3);
	eEEPROMWr(LAMP_WORK_HOURS_INDEX,&DATA_MODEL_REGISTER[LAMP_WORK_HOURS_INDEX],LAMP_WORK_HOURS_SIZE );
	HAL_NVIC_SystemReset();
}
/*
 *   Получить запись из журнала
 */
void vGetRecord( uint16_t addr,uint8_t * flag, RTC_TimeTypeDef * time, RTC_DateTypeDef * date)
{
  uint16_t total     =  int16GetRegister( RECORD_COUNT );
  uint16_t cur_index =  int16GetRegister( RECORD_INDEX);
  uint8_t pData[RECORD_DATA_SIZE];
  uint16_t index = 0;
  if ( addr  <= total )
  {
	  if ( (cur_index - total) >= 0  )
	  {
		  index = addr;
	  }
	  else
	  {
		  if ((cur_index + index) < total)
		  {
			  index = cur_index + index;
		  }
		  else
		  {
			  index = addr - (total - cur_index + 1);
		  }
	  }

	  eEEPROMRd(EEPROM_REGISER_COUNT +  index*RECORD_SIZE  ,pData , RECORD_SIZE );
      *flag = 		  pData[0];
      date->Date	= pData[1];
      date->Month 	= pData[2];
      date->Year 	= pData[3];
      time->Hours 	= pData[4];
      time->Minutes = pData[5];
      time->Seconds = pData[6];
  }
  else
  {
	  *flag = 0;
  }
}
/*
 * Добавить запись в журнал
 */
void vADDRecord( uint8_t flag)
{

	RTC_TimeTypeDef time_buffer;
	RTC_DateTypeDef date_buffer;
	uint16_t  index = int16GetRegister(RECORD_INDEX);
	uint8_t DataBuffer[RECORD_SIZE];
	DataBuffer[0] =  flag;
	HAL_RTC_GetTime(&hrtc, &time_buffer,  RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_buffer,  RTC_FORMAT_BIN);
	DataBuffer[1] =  date_buffer.Date;
	DataBuffer[2] =  date_buffer.Month;
	DataBuffer[3] =  date_buffer.Year;
    DataBuffer[4] =  time_buffer.Hours;
    DataBuffer[5] =  time_buffer.Minutes;
    DataBuffer[6] =  time_buffer.Seconds;
    eEEPROMWr(EEPROM_REGISER_COUNT +  index* RECORD_SIZE, DataBuffer, RECORD_SIZE );
    if ( ++index >= RECORD_DATA_SIZE ) index = 0;
    int16SetRegister(RECORD_INDEX, index);
    if (int16GetRegister(RECORD_COUNT) < ( RECORD_DATA_SIZE)  )
    {
    	int16SetRegister(RECORD_COUNT,	int16GetRegister(RECORD_COUNT) +1 );
    }
    eEEPROMWr(RECORD_COUNT , &DATA_MODEL_REGISTER[RECORD_COUNT], 4 );

}
/*
 * Очистить журнал
 */
void JournalClear()
{
	 int16SetRegister(RECORD_INDEX, 0);
	 int16SetRegister(RECORD_COUNT, 0);
	 eEEPROMWr(RECORD_COUNT , &DATA_MODEL_REGISTER[RECORD_COUNT], 4 );
}



