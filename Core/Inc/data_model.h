/*
 * data_model.h
 *
 *  Created on: Jul 27, 2023
 *      Author: i.dymov
 */
#ifndef INC_DATA_MODEL_H_
#define INC_DATA_MODEL_H_

#include "main.h"
#include "datastorage.h"


/* описание регистра в регисторвой модели*/
#define MODBUS_ADDRES   0x01
#define FBO_SIZE_W      (MODBUS_ADDRES +1)  // 0x02
#define FBO_SIZE_H      (FBO_SIZE_W    +1)  // 0x03
#define FBO_SIZE_L      ( FBO_SIZE_H   +1)  // 0x04
#define LAMP_COUNT      ( FBO_SIZE_L   +1)  // 0x05
#define CONTROL_TYPE    ( LAMP_COUNT   +1 )  //0x06
#define LAMP1_MAX_TIME   (CONTROL_TYPE  + 1)
#define LAMP2_MAX_TIME   (LAMP1_MAX_TIME + 1)
#define LAMP3_MAX_TIME   (LAMP2_MAX_TIME + 1)
#define LAMP4_MAX_TIME   (LAMP3_MAX_TIME + 1)
#define LAMP5_MAX_TIME   (LAMP4_MAX_TIME + 1)
#define LAMP6_MAX_TIME   (LAMP5_MAX_TIME + 1)
#define LAMP7_MAX_TIME   (LAMP6_MAX_TIME + 1)
#define LAMP8_MAX_TIME   (LAMP7_MAX_TIME + 1)
#define LAMP9_MAX_TIME   (LAMP8_MAX_TIME + 1)
#define LAMP10_MAX_TIME   (LAMP9_MAX_TIME + 1)
#define LAMP11_MAX_TIME   (LAMP10_MAX_TIME + 1)
#define LAMP12_MAX_TIME   (LAMP11_MAX_TIME + 1)
#define LAMP13_MAX_TIME   (LAMP12_MAX_TIME + 1)
#define LAMP14_MAX_TIME   (LAMP13_MAX_TIME + 1)
#define LAMP15_MAX_TIME   (LAMP14_MAX_TIME + 1)
#define LAMP16_MAX_TIME   (LAMP15_MAX_TIME + 1)
#define LAMP17_MAX_TIME   (LAMP16_MAX_TIME + 1)
#define LAMP18_MAX_TIME   (LAMP17_MAX_TIME + 1)
#define LAMP19_MAX_TIME   (LAMP18_MAX_TIME + 1)
#define LAMP20_MAX_TIME   (LAMP19_MAX_TIME + 1)
#define LAMP21_MAX_TIME   (LAMP20_MAX_TIME + 1)
#define LAMP22_MAX_TIME   (LAMP21_MAX_TIME + 1)
#define LAMP23_MAX_TIME   (LAMP22_MAX_TIME + 1)
#define LAMP24_MAX_TIME   (LAMP23_MAX_TIME + 1)
#define LAMP25_MAX_TIME   (LAMP24_MAX_TIME + 1)
#define LAMP26_MAX_TIME   (LAMP25_MAX_TIME + 1)
#define LAMP27_MAX_TIME   (LAMP26_MAX_TIME + 1)
#define LAMP28_MAX_TIME   (LAMP27_MAX_TIME + 1)
#define LAMP29_MAX_TIME   (LAMP28_MAX_TIME + 1)
#define LAMP30_MAX_TIME   (LAMP29_MAX_TIME + 1)
#define LAMP31_MAX_TIME   (LAMP30_MAX_TIME + 1)
#define LAMP32_MAX_TIME   (LAMP31_MAX_TIME + 1)
#define LAMP33_MAX_TIME   (LAMP32_MAX_TIME + 1)
#define LAMP34_MAX_TIME   (LAMP33_MAX_TIME + 1)
#define LAMP35_MAX_TIME   (LAMP34_MAX_TIME + 1)
#define LAMP36_MAX_TIME   (LAMP35_MAX_TIME + 1)
#define LAMP37_MAX_TIME   (LAMP36_MAX_TIME + 1)
#define LAMP38_MAX_TIME   (LAMP37_MAX_TIME + 1)
#define LAMP39_MAX_TIME   (LAMP38_MAX_TIME + 1)
#define LAMP40_MAX_TIME   (LAMP39_MAX_TIME + 1)
#define LAMP41_MAX_TIME   (LAMP40_MAX_TIME + 1)
#define LAMP42_MAX_TIME   (LAMP41_MAX_TIME + 1)
#define LAMP43_MAX_TIME   (LAMP42_MAX_TIME + 1)
#define LAMP44_MAX_TIME   (LAMP43_MAX_TIME + 1)
#define LAMP1    (LAMP44_MAX_TIME + 1)
#define LAMP2   (LAMP1 + LAMP_DATA_SIZE)
#define LAMP3   (LAMP2 + LAMP_DATA_SIZE)
#define LAMP4   (LAMP3 + LAMP_DATA_SIZE)
#define LAMP5   (LAMP4 + LAMP_DATA_SIZE)
#define LAMP6   (LAMP5 + LAMP_DATA_SIZE)
#define LAMP7   (LAMP6 + LAMP_DATA_SIZE)
#define LAMP8   (LAMP7 + LAMP_DATA_SIZE)
#define LAMP9   (LAMP8 + LAMP_DATA_SIZE)
#define LAMP10   (LAMP9 + LAMP_DATA_SIZE)
#define LAMP11   (LAMP10 + LAMP_DATA_SIZE)
#define LAMP12   (LAMP11 + LAMP_DATA_SIZE)
#define LAMP13   (LAMP12 + LAMP_DATA_SIZE)
#define LAMP14   (LAMP13 + LAMP_DATA_SIZE)
#define LAMP15   (LAMP14 + LAMP_DATA_SIZE)
#define LAMP16   (LAMP15 + LAMP_DATA_SIZE)
#define LAMP17   (LAMP16 + LAMP_DATA_SIZE)
#define LAMP18   (LAMP17 + LAMP_DATA_SIZE)
#define LAMP19   (LAMP18 + LAMP_DATA_SIZE)
#define LAMP20   (LAMP19 + LAMP_DATA_SIZE)
#define LAMP21   (LAMP20 + LAMP_DATA_SIZE)
#define LAMP22   (LAMP21 + LAMP_DATA_SIZE)
#define LAMP23   (LAMP22 + LAMP_DATA_SIZE)
#define LAMP24   (LAMP23 + LAMP_DATA_SIZE)
#define LAMP25   (LAMP24 + LAMP_DATA_SIZE)
#define LAMP26   (LAMP25 + LAMP_DATA_SIZE)
#define LAMP27   (LAMP26 + LAMP_DATA_SIZE)
#define LAMP28   (LAMP27 + LAMP_DATA_SIZE)
#define LAMP29   (LAMP28 + LAMP_DATA_SIZE)
#define LAMP30   (LAMP29 + LAMP_DATA_SIZE)
#define LAMP31   (LAMP30 + LAMP_DATA_SIZE)
#define LAMP32   (LAMP31 + LAMP_DATA_SIZE)
#define LAMP33   (LAMP32 + LAMP_DATA_SIZE)
#define LAMP34   (LAMP33 + LAMP_DATA_SIZE)
#define LAMP35   (LAMP34 + LAMP_DATA_SIZE)
#define LAMP36   (LAMP35 + LAMP_DATA_SIZE)
#define LAMP37   (LAMP36 + LAMP_DATA_SIZE)
#define LAMP38   (LAMP37 + LAMP_DATA_SIZE)
#define LAMP39   (LAMP38 + LAMP_DATA_SIZE)
#define LAMP40   (LAMP39 + LAMP_DATA_SIZE)
#define LAMP41   (LAMP40 + LAMP_DATA_SIZE)
#define LAMP42   (LAMP41 + LAMP_DATA_SIZE)
#define LAMP43   (LAMP42 + LAMP_DATA_SIZE)
#define LAMP44   (LAMP43 + LAMP_DATA_SIZE)

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

typedef struct
{
	uint32_t MaxValue;
	uint32_t MinValie;
	uint32_t data_step;
	REGISTE_DATA_TYPE_t  data_type;
	uint8_t data_size;
} DataLimit_t;


typedef struct
{
 const DataLimit_t  * limits;
 ConfigStatus_t mode;
 void * data;
 uint16_t reg_id;
 ViewModelDataType model_type;
 char * data_view[20];
} sDataModelUnit;

typedef struct
{
	char data[20];
	uint8_t ID;
} ViewBuffer_t;


#define MOD_BUS_ADDRES  0x01


#define REGISTER_COUNT  REGISTER_LAST_ADR
#define LAMP_DATA_SIZE 5
#define DOOR_ADDR 0x01
#define REMOTE_ADDR 0x02
#define FIRE_ADDR 0x03
#define LOCAL_ADDR 0x04
#define REMOTE_ACT_ADDR 0x05
#define ERROR_LAMP_RE1_ADDR 0x06
#define ERROR_LAMP_RE2_ADDR 0x07



#define REGISTER_LAST_ADR (LAMP44_MAX_TIME + 1)

void SetData(uint16_t index, uint16_t data);
uint16_t GetData(uint16_t index);
void vGetDataForMenu( DATA_COMMNAD_TYPE cmd, char* Data, uint8_t ID, char * format );
uint8_t ucGetLampProc(uint8_t lamp_id);
#endif /* INC_DATA_MODEL_H_ */
