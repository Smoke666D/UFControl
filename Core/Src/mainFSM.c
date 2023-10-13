#include "MainFSM.h"
#include "mb.h"

uint16_t input_regs[REG_COUNT];
uint16_t system_regs[REG_COUNT ];


static uint8_t MB_FSM = 0;


uint16_t usGetRegInput( REGS_t reg_addr)
{
	uint16_t usRes;
	switch (reg_addr)
	{
	    case 0:
	    case 1:
	    	 usRes = int16GetRegister(FBO_SIZE_A + reg_addr);
	    	break;
		case 3:
			usRes = int8GetRegister(LAMP_COUNT);
			break;
		case 4:
			usRes = int8GetRegister(DEVICE_INPUT_REG);
			break;
		default:
			usRes = 0;
			break;
	}


	return  (usRes);
}
void vSetRegInput(REGS_t reg_addr, uint16_t data )
{
	if (reg_addr < REG_COUNT)
	{
		input_regs[reg_addr] = data;
	}
}
void vSetRegInputBit(REGS_t reg_addr,uint16_t offset,  uint16_t data )
{
	if ( reg_addr < REG_COUNT )
		{
			if (data != 0)
				input_regs[reg_addr]  |= 0x1 << offset;
			else
				input_regs[reg_addr]  &= ~(0x1 << offset);
		}
}
void vUPDATECoils( uint8_t rw)
{

}
void vSetReg(REGS_t reg_addr, uint16_t data)
{
			  if (reg_addr == MODE)
			  {
				  if (system_regs[reg_addr] != data)
				  {
					//  mode_restart = 1;
				  }
			  }
			  system_regs[reg_addr] = data;
}

uint16_t usGetReg( REGS_t reg_addr)
{
	uint16_t usRes;

     if (reg_addr == MODE)
	 {
			//connection = 1;
		}
		usRes = system_regs[reg_addr];

	return  (usRes);
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
