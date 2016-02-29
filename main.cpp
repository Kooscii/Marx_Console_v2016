#include "cmsis_os.h"
#include "peripheral.h"
#include "task.h"
#include "menu.h"
#include "variables.h"


/* TODO list */
/*  
 * Marx Function
 * Automatically generate menu item
*/

int main (void) 
{	
	Peripheral_Configure();
	Menu_Init();
		
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP| FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ProgramHalfWord(0x0800ff00, 0x1234);
	FLASH_ErasePage(0x0800ff00);
	FLASH_Lock();
	
	osKernelInitialize ();
	osTaskInitialize();
	osKernelStart ();
	
	static uint8_t a=0;
	for (;;)
	{
		if (a%2) 
			LED_Set(GPIO_LED_RD, LED_ON);
		else 
			LED_Set(GPIO_LED_RD, LED_OFF);
		
		if (!(a%64)) 
			Main_Menu.isMenuUpdated = true;
		
		a++;
		
		osDelay(1000);
	}
}
