/*
 * data_model.h
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */
#ifndef INC_DATA_MODEL_H_
#define INC_DATA_MODEL_H_

#include "main.h"
#include "EEPROM.h"
#include "menu.h"


#define VALID_CODE      0x38
/* описание регистра в регисторвой модели*/
#define VALID_CODE_ADDRES   0
#define MODBUS_ADDRES      ( VALID_CODE_ADDRES +1  )
#define FBO_SIZE_W         ( MODBUS_ADDRES + 1 )
#define FBO_SIZE_H         ( FBO_SIZE_W + 2)
#define FBO_SIZE_L         ( FBO_SIZE_H + 2 )
#define LAMP_COUNT         ( FBO_SIZE_L  +2)
#define CONTROL_TYPE_REG   ( LAMP_COUNT +1 )
	#define SCADA_TYPE     1
	#define LOCAL_TYPE 	   0
#define LAMP_MAX_TIME_INDEX      ( CONTROL_TYPE_REG +1 )
#define LAMP_WORK_HOURS_INDEX   (LAMP_MAX_TIME_INDEX + 44 )
	#define LAMP_WORK_HOURS_SIZE       (44*4)
#define LAMP_RESURSE_INDEX     (LAMP_WORK_HOURS_INDEX + LAMP_WORK_HOURS_SIZE)
#define LAM_ERROR_REG_LSB (LAMP_RESURSE_INDEX + 44)
#define LAM_ERROR_REG_MSB (LAM_ERROR_REG_LSB +4)
//8 битный регист аварий
#define SCADA_CONTROL_REG    (LAM_ERROR_REG_MSB  + 4)


#define DEVICE_ALARM_REG      (SCADA_CONTROL_REG  + 1)

    #define DOOR_ALARM            0
    #define FIRE_FLAG             1
    #define LAMP_ERROR 		      2
	#define REMAIN_RESOURS_3      3
	#define REMAIN_RESOURS_0      4
    #define VOLT_187			  5
    #define VOLT_196			  6
    #define VOLT_250			  7
#define ERROR_LAM_FLAG       ( (0x1<< FIRE_FLAG) | (0x1<<VOLT_187) | (0x1<<VOLT_250) | (0x1<<LAMP_ERROR) | (0x1<<REMAIN_RESOURS_0))
#define  DEVICE_ERROR_MASK   ( (0x1<< FIRE_FLAG) | (0x1<<VOLT_187) | (0x1<<VOLT_250) | (0x1<<DOOR_ALARM) )
#define DEVICE_STATUS_REG    (DEVICE_ALARM_REG + 1)
    #define REMOTE_FLAG     0
	#define LOCAL_FLAG      1
    #define REMOTE_ACT_FLAG 2
#define DEVICE_INPUT_REG   (DEVICE_STATUS_REG + 1)
    #define IN_DOOR_ALARM   0
	#define IN_FIRE_ALARM 	1
	#define IN_REMOTE_FLAG  2
	#define IN_LOCAL_FLAG   3
    #define IN_REMOTE_ACT   4
#define DEVICE_OUTPUT_REG   (DEVICE_INPUT_REG + 1)
   #define ALARM_OUT_FLAG   0
   #define WORK_OUT_FLAG    1
   #define LOCAL_OUT_FLAG   2

#define LAPM_ERROR_COUNT     DEVICE_OUTPUT_REG   + 1
#define TOTAL_REGISTER_COUNT    (LAPM_ERROR_COUNT  + 1)
#define EEPROM_REGISER_COUNT   DEVICE_ALARM_REG

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
  CONFIG_NOT_READ,
  CONFIG_READ,
  CONFIG_EDIT,
} ConfigStatus_t;

typedef enum
{
  BYTE_DATA,
  SHORT_DATA,
  REGISTER_DATA,
  TIME_STEM
} DataType_t;

typedef enum
{
  REGISTER_TYPE,
  ENUM_TYPE,
} ViewModelDataType;



#define MOD_BUS_ADDRES  0x01



#define LAMP_DATA_SIZE 5
#define DOOR_ADDR 0x01
#define REMOTE_ADDR 0x02
#define FIRE_ADDR 0x03
#define LOCAL_ADDR 0x04
#define REMOTE_ACT_ADDR 0x05
#define ERROR_LAMP_RE1_ADDR 0x06
#define ERROR_LAMP_RE2_ADDR 0x07



#define REGISTER_LAST_ADR (LAMP44   + 1)
void vEditLampCount(DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
//void int16SetRegisterBit(uint16_t addres, uint8_t bits, uint8_t data);
void int16SetRegister(uint16_t addres, uint16_t data);
uint16_t int16GetRegister(uint16_t addres);
void int8SetRegisterBit(uint16_t addres, uint8_t bits, uint8_t data);
void int8SetRegister( uint16_t addres , uint8_t data );
uint8_t int8GetRegister(uint16_t addres);
uint32_t int32GetData( uint16_t index );
void int32SetData( uint16_t index, uint32_t data);
uint32_t GetModelData(uint8_t index,uint8_t offset);
void SetData(uint16_t index, uint16_t data);
uint16_t GetData(uint16_t index);
void vSetTimeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vSetDateForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetDataForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetRecourceForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vSetRecourceForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetLapmErrorForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetErrorForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetFBOHSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetFBOWSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetFBOLSizeForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vResetRecourceLamp( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID );
void vGetMbTypeMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID);
void vGetControlTypeMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID);
void vSetAllResLampMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t index);
uint8_t ucGetLampProc(uint8_t lamp_id);
void vLAMWorkHoursWrite();
#endif /* INC_DATA_MODEL_H_ */
