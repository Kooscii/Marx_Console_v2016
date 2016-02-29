#include "task.h"

/* Task: LCD */
osThreadDef ( task_LCD , osPriorityBelowNormal, 1, 1000 );
osThreadId id_tsk_lcd;

/* Task: Button */
osThreadDef(task_Button, osPriorityNormal , 1, 0);
osThreadId id_tsk_btn;

/* Task: Sending */
osThreadDef(task_Sending, osPriorityHigh , 1, 0);
osThreadId id_tsk_sending;

///* Timer: Trigger */
//osTimerDef (tmr_Trigger, timer_Trigger);
//osTimerId id_tmr_trigger; 

void osTaskInitialize() {
	id_tsk_btn     = osThreadCreate ( osThread(task_Button), NULL );
	id_tsk_lcd     = osThreadCreate ( osThread(task_LCD), NULL );
	id_tsk_sending = osThreadCreate ( osThread(task_Sending), NULL);
	
//	id_tmr_trigger = osTimerCreate (osTimer(tmr_Trigger), osTimerOnce, NULL);
//	osTimerStop(id_tmr_trigger);
}
