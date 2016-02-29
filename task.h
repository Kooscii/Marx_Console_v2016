#ifndef TASK_H_
#define TASK_H_

#include "peripheral.h"
#include "cmsis_os.h"

extern void task_LCD(void const * arg);
extern void task_Button(void const * arg);
extern void task_Sending(void const * arg);
extern void timer_Trigger(void const *arg);

/* Task: LCD */
extern osThreadId id_tsk_lcd;

/* Task: Button */
extern osThreadId id_tsk_btn;

/* Task: Sending */
extern osThreadId id_tsk_sending;

/* Timer: Trigger */
extern osTimerId id_tmr_trigger; 

void osTaskInitialize();

#endif /* TASK_H_ */
