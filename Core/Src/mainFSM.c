#include "MainFSM.h"
#include "mb.h"


static uint16_t system_regs[DEVICE_HOLDING  ];
static uint8_t MB_FSM = 0;
extern RTC_HandleTypeDef hrtc;
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
		case 8:
			   uint64_t temp = int32GetData( LAM_ERROR_REG_LSB ) | (((uint64_t)int32GetData( LAM_ERROR_REG_MSB ))<<22);
					usRes =  temp >>( 11* (reg_addr-5) )  & 0x7FF; break;
		case 98:  usRes =int8GetRegister( DEVICE_ALARM_REG ); 	   break;
		case 99:  usRes =int8GetRegister( DEVICE_INPUT_REG );	   break;
		case 100: usRes =int8GetRegister( DEVICE_OUTPUT_REG );	   break;
		case 101: usRes =int16GetRegister( RECORD_COUNT );		   break;
		default:
			if ((reg_addr >=9) && (reg_addr <53)) usRes = int8GetRegister( LAMP_MAX_TIME_INDEX + reg_addr-9)*1000;
			if ((reg_addr>= 53) && (reg_addr< 97))   usRes = int8GetRegister( LAMP_RESURSE_INDEX + reg_addr-53);
			break;
	}

    return  (usRes);
}

static RTC_TimeTypeDef time;
static RTC_DateTypeDef date;
void vSetReg(REGS_t reg_addr, uint16_t data)
{
    if (reg_addr == SCADA_CONTROL_ADDR) int8SetRegister(SCADA_CONTROL_REG, (uint8_t)data);
    if (reg_addr == COMMAND_REG )
    {
    	switch (data)
    	{
    		case READ_DATE:
    			HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    			HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
    			system_regs[DATE_OFFSET] = date.Date;
    			system_regs[MOUNTH_OFFSET] = date.Month;
    			system_regs[YEAR_OFFSET] = date.Year;
    			system_regs[HOUR_OFFSET] = time.Hours;
    			system_regs[MINUTE_OFFSET] = time.Minutes;
    			system_regs[SECOND_OFFSET] = time.Seconds;
    			break;
    		case SET_DATE:
    			date.Date    = system_regs[DATE_OFFSET];
    			date.Month   = system_regs[MOUNTH_OFFSET];
    			date.Year 	 = system_regs[YEAR_OFFSET];
    			time.Hours 	 = system_regs[HOUR_OFFSET];
    			time.Minutes = system_regs[MINUTE_OFFSET];
    			time.Seconds = system_regs[SECOND_OFFSET];
    			HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
    			HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
    			break;
    		case RESET_JOURNAL:
    			int16SetRegister( RECORD_COUNT , 0);
    			break;
    		case READ_RECORD:
    			vGetRecord(system_regs[ADDRESS_OFFSET],((uint8_t *)&system_regs[DATA_OFFSET]),&time,&date );

    			system_regs[DATE_OFFSET] = date.Date;
    			system_regs[MOUNTH_OFFSET] = date.Month;
    			system_regs[YEAR_OFFSET] = date.Year;
    			system_regs[HOUR_OFFSET] = time.Hours;
    			system_regs[MINUTE_OFFSET] = time.Minutes;
    			system_regs[SECOND_OFFSET] = time.Seconds;
    			break;
    		case RESET_RESOURSE:
    			if (system_regs[ADDRESS_OFFSET] > 0 )
    				vResetLampRecource(system_regs[ADDRESS_OFFSET]);
    			break;
    		case RESET_RESOIRSE_ALL:
    			vResetLampRecource( 0 );
    			break;
    		case SET_RESOURCE:
    			if ( system_regs[ADDRESS_OFFSET]!= 0)
    			{
    				vSetLampRecource( system_regs[ADDRESS_OFFSET] , system_regs[DATA_OFFSET]/1000 );
    			}
    			break;
    		case SET_RESOURCE_ALL:
    			vSetLampRecource( 0 , system_regs[DATA_OFFSET]/1000 );
    			break;
    	}
    }
	system_regs[ reg_addr] = data;
}

uint16_t usGetReg( uint16_t reg_addr)
{
	return  (system_regs[ reg_addr ]);
}


void StartMb(void *argument)
{
	 EventGroupHandle_t system_event = xGetSystemUpdateEvent();
     while (1)
     {
    	 switch (MB_FSM)
    	 {

    	 	 case 0:
    	 		 eMBDisable();
    	 		 xEventGroupWaitBits(system_event, MB_START, pdTRUE, pdTRUE, portMAX_DELAY );
    	 		 uint8_t addres  = int8GetRegister(MODBUS_ADDRES );
    	 		 eMBInit(MB_RTU,addres,0,19200,MB_PAR_ODD );


    	 		 vSetReg(11,0x5434);
    	 		 eMBEnable();
    	 		 MB_FSM = 1;
    	 		 break;
    	 	 default:
    	 		 eMBPoll();
    	 		 vTaskDelay(10);
    	 		 if (xEventGroupGetBits(system_event) & SYSTEM_IDLE)
    	 		 {
    	 			 MB_FSM = 0;
    	 		 }
    	 		 break;
    	 }
	 }
}
