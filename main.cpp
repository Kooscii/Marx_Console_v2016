#include "cmsis_os.h"
#include "peripheral.h"
#include "task.h"
#include "menu.h"
#include "variables.h"

/* TODO list */
/*  
 * Marx Function
 * Automatically generate menu item
 nvic registeration, swinterrupt
*/


int main (void) 
{	
	static uint8_t a=0;
	
	
	FLASH_Status fx;
	
	Peripheral_Configure();
	Menu_Init();
		
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP| FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	fx = FLASH_ProgramHalfWord(0x0800fc00, 0xF14a);
	fx = FLASH_ProgramHalfWord(0x0800fc02, 0x1234);
	fx = FLASH_ErasePage(0x0800fc00);
	FLASH_Lock();
	
	osKernelInitialize ();
	osTaskInitialize();
	osKernelStart ();
	
	
	for (;;) {
		if (!(a%64)) 
			Main_Menu.isMenuUpdated = true;
		a++;
		osDelay(1000);
	}
}
