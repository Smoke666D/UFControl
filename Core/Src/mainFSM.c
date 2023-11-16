#include "MainFSM.h"
#include "mb.h"
#include "main.h"
#include "data_model.h"
#include "stdio.h"
extern RTC_HandleTypeDef hrtc;
static uint16_t system_regs[DEVICE_HOLDING  ];
static uint8_t MB_FSM = 0;
static EventGroupHandle_t system_event;
static RTC_TimeTypeDef time;
static RTC_DateTypeDef date;
static CONFIG_ENABLE_t ConfigEnable = 0;
/*
 *
 */
uint16_t usGetRegInput( uint16_t reg_addr)
{
	uint16_t usRes=0;
	switch (reg_addr)
	{
		case 0:
		case 1: usRes = int16GetRegister(FBO_SIZE_A + reg_addr*2); break;
		case 2: usRes = int8GetRegister(LAMP_COUNT);      		   break;
		case 3: usRes = int8GetRegister(CONTROL_TYPE_REG); 		   break;
		case 4: usRes = int8GetRegister(LAPM_ERROR_COUNT); 		   break;
		case 5:
		case 6:
		case 7:
		case 8:if (int8GetRegister( DEVICE_OUTPUT_REG ) && (0x01 << WORK_OUT_FLAG))
			   {
			   uint64_t temp = int32GetData( LAM_ERROR_REG_LSB ) | (((uint64_t)int32GetData( LAM_ERROR_REG_MSB ))<<22);
					usRes =  temp >>( 11* (reg_addr-5) )  & 0x7FF;
			   }

				break;
		case 98:  usRes =int8GetRegister( DEVICE_ALARM_REG ); 	   break;
		case 99:  usRes =int8GetRegister( DEVICE_INPUT_REG );	   break;
		case 100: usRes =int8GetRegister( DEVICE_OUTPUT_REG );	   break;
		case 101: usRes =int16GetRegister( RECORD_COUNT );		   break;
		case 102: HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN); usRes = date.Date; break;
		case 103: HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN); usRes = date.Month; break;
		case 104: HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN); usRes = date.Year; break;
		case 105: HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);usRes = time.Hours; break;
		case 106: HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);usRes = time.Minutes; break;
		case 107:HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);usRes = time.Seconds; break;
		case 108: usRes = 0x5434; break;
		default:
			if ((reg_addr >=9) && (reg_addr <53)) usRes = int8GetRegister( LAMP_MAX_TIME_INDEX + reg_addr-9)*1000;
			if ((reg_addr>= 53) && (reg_addr< 97))   usRes = int8GetRegister( LAMP_RESURSE_INDEX + reg_addr-53);
			break;
	}

    return  (usRes);
}

/*
 *
 */
void vSetReg(REGS_t reg_addr, uint16_t data)
{
	switch ( reg_addr)
	{
		case  SCADA_CONTROL_ADDR:
			int8SetRegister(SCADA_CONTROL_REG, (uint8_t)data);
			break;
		case  CONFIG_ENABLE_REG:
			if ( data == 0x00 )
			{
				xEventGroupClearBits(system_event,MB_EDIT);
				xEventGroupSetBits(system_event,SYSTEM_RESTART);
				ConfigEnable =  CONFIG_DISABLE;
			}
			else if ( data == 0x01 )
			{
				if ( ( xEventGroupGetBits(system_event) & MENU_EDIT ) != MENU_EDIT )
				{
					xEventGroupSetBits(system_event,SYSTEM_IDLE | MB_EDIT);
					ConfigEnable =  CONFIG_ENABLE;
				}
				else
					data = 0;

			}
			break;

		case COMMAND_REG:

		   if (data == READ_RECORD)
			{
					vGetRecord(system_regs[ADDRESS_OFFSET],((uint8_t *)&system_regs[DATA_OFFSET]),&time,&date );
					system_regs[DATE_OFFSET] = date.Date;
					system_regs[MOUNTH_OFFSET] = date.Month;
					system_regs[YEAR_OFFSET] = date.Year;
					system_regs[HOUR_OFFSET] = time.Hours;
					system_regs[MINUTE_OFFSET] = time.Minutes;
					system_regs[SECOND_OFFSET] = time.Seconds;
			}
			if ( ConfigEnable ==  CONFIG_ENABLE )
			{
					switch (data)
					{
						case RESET_JOURNAL:
							JournalClear();
							break;
						case RESET_RESOURSE:
							if (system_regs[ADDRESS_OFFSET] > 0 )  vResetLampRecource(system_regs[ADDRESS_OFFSET]);
							break;
						case RESET_RESOIRSE_ALL:
							vResetLampRecource( 0 );
							break;
						case SET_RESOURCE:
							if ( ( system_regs[ ADDRESS_OFFSET ] > 0) && ( system_regs[ ADDRESS_OFFSET ] <= 44 ) )
							{
								vSetLampRecource( (uint8_t)system_regs[ADDRESS_OFFSET] , system_regs[SET_LAM_DATA_OFFSET ]/1000 );
							}
							break;
						case SET_RESOURCE_ALL:
							vSetLampRecource( 0 , system_regs[SET_LAM_DATA_OFFSET ]/1000 );
							break;
						default:
							break;
					}
				}
			break;
		case SET_DATE_REG:
			if ( ( ConfigEnable ==  CONFIG_ENABLE ) && (data <= 31) && ( data >0 ) )
			{
				HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
				date.Date = data;
				HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
				int8SetRegister(DAY,data);

			}
			break;
		case SET_MOUNTH_REG:
			if ( ( ConfigEnable ==  CONFIG_ENABLE ) && (data <= 12) && ( data >0) )
			{
				HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
				date.Month = data;
				HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
				int8SetRegister(MOUNTH,data);

			}
			break;
		case SET_YEAR_REG:
			if ( ( ConfigEnable ==  CONFIG_ENABLE ) && (data <= 99) )
			{
				HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
				date.Year = data;
				HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
				int8SetRegister(YEAR,data);
			}
			break;
		case SET_HOUR_REG:
			if ( ( ConfigEnable ==  CONFIG_ENABLE ) && (data <= 23) )
			{
				HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
				time.Hours  = data;
				HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
			}
			break;
		case SET_MIN_REG :
			if ( ( ConfigEnable ==  CONFIG_ENABLE ) && (data <= 59) )
			{
				HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
				time.Minutes  = data;
				HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
			}
			break;
		case SET_SEC_REG	:
			if ( ( ConfigEnable ==  CONFIG_ENABLE ) && (data <= 59) )
			{
				HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
				time.Seconds  = data;
				HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
			}
			break;
		default:
			break;
	}
	system_regs[ reg_addr] = data;
	return;
}
/*
 *
 */
uint16_t usGetReg( uint16_t reg_addr)
{
	return  (system_regs[ reg_addr ]);
}

/*
 *
 */
void StartMb(void *argument)
{
	 system_event = xGetSystemUpdateEvent();
	 memset(system_regs,0, DEVICE_HOLDING * 2 );
     while (1)
     {
    	 switch (MB_FSM)
    	 {
    	 	 case 0:
    	 		 eMBDisable();
    	 		 xEventGroupClearBits(system_event, MB_RESTART );
    	 		 xEventGroupWaitBits(system_event, MB_START, pdTRUE, pdTRUE, portMAX_DELAY );
    	 		 uint8_t addres  = int8GetRegister(MODBUS_ADDRES );
    	 		 eMBInit(MB_RTU,addres,0,19200,MB_PAR_ODD );
    	 		// vSetReg(11,0x5434);
    	 		 eMBEnable();
    	 		 MB_FSM = 1;
    	 		 break;
    	 	 default:
    	 		 eMBPoll();
    	 		 vTaskDelay(10);
    	 		 if (xEventGroupGetBits(system_event) &  MB_RESTART )
    	 		 {
    	 			 MB_FSM = 0;
    	 		 }
    	 		 break;
    	 }
	 }
}
