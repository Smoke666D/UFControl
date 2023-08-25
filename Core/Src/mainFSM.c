#include "MainFSM.h"
#include "mb.h"

uint16_t input_regs[REG_COUNT];
uint16_t system_regs[REG_COUNT ];
uint16_t usGetRegInput( REGS_t reg_addr)
{
	uint16_t usRes;
    usRes = input_regs[reg_addr];
	return  (usRes);
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


	 uint16_t addres = 0;
	// addres = (uiGetDinMask() & DEVICE_ADDR_MASK)>>DEVICE_ADDR_OFFSET;
	 eMBInit(MB_RTU,addres,0,19200,MB_PAR_ODD );
	 eMBEnable(  );
     while (1)
     {
		 eMBPoll();

		 vTaskDelay(10);
	 }
}
