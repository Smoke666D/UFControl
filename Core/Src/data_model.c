/*
 * data_model.c
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */

#include "data_model.h"
#include "stdio.h"

#define STRING_BUFFER_SIZE 4U

static ViewBuffer_t StringBuffer[STRING_BUFFER_SIZE];

const DataLimit_t MODBUS_DATA =
{
	.MaxValue = 99,
	.MinValie = 1,
	.data_step = 1,
    .data_type = INTEGER_DATA,
	.data_size = 1,
} ;

const DataLimit_t DISCRET_STATE =
{
	.MaxValue = 1,
	.MinValie = 0,
	.data_step = 1,
    .data_type = INTEGER_DATA,
	.data_size = 1,
};

uint32_t mod_bus_addr = 0;
sDataModelUnit  MODBUS_REG =
{
	.limits = &MODBUS_DATA,
	.mode = CONFIG_NOT_READ,
	.data = &mod_bus_addr,
	.reg_id = MOD_BUS_ADDRES,
	.model_type = REGISTER_TYPE,
	.data_view = NULL,
};

uint32_t dev_mod = 0;
sDataModelUnit  DEV_MODE =
{
	.limits = &MODBUS_DATA,
	.mode = CONFIG_NOT_READ,
	.data = &mod_bus_addr,
	.reg_id = MOD_BUS_ADDRES,
	.model_type = ENUM_TYPE,
	.data_view = {"ЩИТ","SCADA"},
};

sDataModelUnit * DATA_MODEL_DATA[] = {&MODBUS_REG, &DEV_MODE };
static uint8_t free_data_pointer = 0;
uint16_t DATA_MODEL_REGISTER[REGISTER_COUNT];

uint8_t bytebuffer;






void vGetRegisteDataToString( uint8_t index, char * data )
{
	uint8_t buf[5];
	switch (GetRegisterData(index, buf) )
	{
		case INTEGER_DATA:

			 break;

	}

}


void vGeRegisterForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID )
{
	  switch (cmd)
	   {
	   	   	  case mREAD:

	   	   		   break;
	   	   	   case mSAVE:
	   	   		   break;
	   	   	   case mESC:
	   	   		   break;
	   	   	   case mINC:

	   	   		   break;
	   	   	   case mDEC:
	   	   		   break;
	   }




}


static void UpdateStringBuffer( uint8_t ID, char * format)
{
	uint8_t find = STRING_BUFFER_SIZE;
	for (uint8_t i= 0; i< STRING_BUFFER_SIZE; i++  )
    {
	     if (StringBuffer[i].ID == ID)
	     {
	    	 find  = i;
	    	 break;
		 }
	}
	if ( find == STRING_BUFFER_SIZE)
	{
		find = free_data_pointer;
		free_data_pointer++;
		if ( free_data_pointer>=STRING_BUFFER_SIZE )
		{
			free_data_pointer = 0;
		}
	}
	if (DATA_MODEL_DATA[ID]->mode == CONFIG_NOT_READ)
	{
			switch (DATA_MODEL_DATA[ID]->limits->data_type)
			{
				case BOOLEAN_DATA:
				case INTEGER_DATA:
					uint32_t bd;
					eEEPROMReadRegister(DATA_MODEL_DATA[ID]->reg_id,&bd);
					* ( (int *)DATA_MODEL_DATA[ID]->data ) = bd;
					if (DATA_MODEL_DATA[ID]->model_type == REGISTER_TYPE)
					{
						sprintf(StringBuffer[find].data,format,bd);
					}
					else
					{
						strcpy(StringBuffer[find].data,DATA_MODEL_DATA[ID]->data_view[bd]);
					}
				case NUMBER_DATA:
					float  fd;
					eEEPROMReadRegister(DATA_MODEL_DATA[ID]->reg_id,&fd);
					sprintf(StringBuffer[find].data,format,fd);
					* ((float *)DATA_MODEL_DATA[ID]->data) = fd;
					break;
				case TIME_STAMP:
				default:
					break;
			}
	}
	DATA_MODEL_DATA[ID]->mode = CONFIG_READ;
}

static void FreeStringBuffer(uint8_t ID)
{
	DATA_MODEL_DATA[ID]->mode = CONFIG_NOT_READ;
}

static void vIncData(uint8_t ID)
{

	switch (DATA_MODEL_DATA[ID]->limits->data_type)
	{
		case BOOLEAN_DATA:
		case INTEGER_DATA:
			int * pint = (int *)DATA_MODEL_DATA[ID]->data;
			(* pint)+=(int)DATA_MODEL_DATA[ID]->limits->data_step;
			if  ( (* pint )  > (int)DATA_MODEL_DATA[ID]->limits->MaxValue )
			{
				*pint = (int) DATA_MODEL_DATA[ID]->limits->MaxValue;
			}
			break;
		case NUMBER_DATA:
			float * fint = (float *)DATA_MODEL_DATA[ID]->data;
			(* fint)+=(float)DATA_MODEL_DATA[ID]->limits->data_step;
			if  ( (* fint )  > (float)DATA_MODEL_DATA[ID]->limits->MaxValue )
			{
				*fint = (float) DATA_MODEL_DATA[ID]->limits->MaxValue;
			}
			break;
		default:
			break;
	}
	DATA_MODEL_DATA[ID]->mode = CONFIG_EDIT;
}

static void vDecData(uint8_t ID)
{
	switch (DATA_MODEL_DATA[ID]->limits->data_type)
	{
		case BOOLEAN_DATA:
		case INTEGER_DATA:
			int * pint = (int *)DATA_MODEL_DATA[ID]->data;
			(* pint)-=(int)DATA_MODEL_DATA[ID]->limits->data_step;
			if  ( (* pint )  < (int)DATA_MODEL_DATA[ID]->limits->MaxValue )
			{
				*pint = (int) DATA_MODEL_DATA[ID]->limits->MaxValue;
			}
			break;
		case NUMBER_DATA:
			float * fint = (float *)DATA_MODEL_DATA[ID]->data;
			(* fint)-=(float)DATA_MODEL_DATA[ID]->limits->data_step;
			if  ( (* fint )  < (float)DATA_MODEL_DATA[ID]->limits->MaxValue )
			{
				*fint = (float) DATA_MODEL_DATA[ID]->limits->MaxValue;
			}
			break;
		default:
			break;
	}
	DATA_MODEL_DATA[ID]->mode = CONFIG_EDIT;
}


static void  vSaveData(uint8_t ID)
{
	eEEPROMRegTypeWrite(DATA_MODEL_DATA[ID]->reg_id,DATA_MODEL_DATA[ID]->data,DATA_MODEL_DATA[ID]->limits->data_type );
}
/*
 * Функция возвращает строку из буфера с текущими состоянием переменной
 */
static char * getBufRef( uint8_t ID)
{
	for (uint8_t i= 0; i< STRING_BUFFER_SIZE; i++  )
	{
		if (StringBuffer[i].ID == ID)
		{
			return StringBuffer[i].data;
		}
	}
	return 0;
}

void vGetDataForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID, char * format )
{
   /*switch (cmd)
   {
   	   	   case mREAD:
   	   		   if (DATA_MODEL_DATA[ID]->mode != CONFIG_READ)
   	   		   {
   	   			   UpdateStringBuffer( ID, format);
   	   		   }
   	   		   strcpy(Data,(const char *)  getBufRef(ID) );
   	   		   break;
   	   	   case mSAVE:
   	   		   vSaveData( ID );
   	   		   break;
   	   	   case mESC:
   	   		   FreeStringBuffer(ID);
   	   		   break;
   	   	   case mINC:
   	   		   vIncData( ID );
   	   		   break;
   	   	   case mDEC:
   	   		   vDecData( ID );
   	   		   break;
   }*/
}


void InitDataModel()
{


}

void SetData(uint16_t index, uint16_t data)
{
	if (index < REGISTER_COUNT)
	{
		DATA_MODEL_REGISTER[index] = data;
	}

}
uint16_t GetData(uint16_t index)
{
	return (index < REGISTER_COUNT) ? DATA_MODEL_REGISTER[index] : 0U;

}
 void vUPLOADDataToEEPROM()
 {

 }

 void vDOWNLOADDataFromEEPROM()
 {

 }
 uint8_t ucGetLampProc(uint8_t lamp_id)
 {
	 if (lamp_id > MAX_LAMP_COUNT)
	 {
		 return 0;
	 }
	 else
	 {
		 return (uint8_t)(GetData(lamp_id + LAMP1)/GetData(lamp_id + LAMP1_MAX_TIME ));
	 }

 }
