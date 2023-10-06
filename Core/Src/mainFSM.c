#include "MainFSM.h"
#include "mb.h"

uint16_t input_regs[REG_COUNT];
uint16_t system_regs[REG_COUNT ];


uint16_t usGetRegInput( REGS_t reg_addr)
{
	uint16_t usRes;
	switch (reg_addr)
	{
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


	 uint16_t addres = 1;
	// addres = (uiGetDinMask() & DEVICE_ADDR_MASK)>>DEVICE_ADDR_OFFSET;
	 eMBInit(MB_RTU,addres,0,19200,MB_PAR_ODD );
	 eMBEnable(  );
     while (1)
     {
		 eMBPoll();
		 vTaskDelay(10);
	 }
}
