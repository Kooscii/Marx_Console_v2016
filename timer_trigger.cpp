#include "task.h"


//static uint16_t trigger_cnt=0;
//static uint8_t mode=0, status=0, i=0, j=0, k=0;

//uint8_t len;
//uint8_t ptr[4];

inline void pulse_delay() {
	uint16_t t=300;
	while (t--) __nop();
}

inline void TRIGGER() {
	GPIO_SetBits(GPIO_TRG);
	pulse_delay();
	GPIO_ResetBits(GPIO_TRG);
}

void timer_Trigger(void const *arg) {
	if (Voltage_Source.getValue(MP_TRIGGER) || Current_Source.getValue(MP_TRIGGER)) {
		if (!Main_Console.inTrigger()) {
			Main_Console.Times_Cnt = Main_Console.getValue(CP_TIMES);
			Main_Console.setRepetition(!Main_Console.Times_Cnt);
			Main_Console.setTrigger(true);

			LED_Set(GPIO_LED_RD, LED_ON);
			TRIGGER();
			
			Voltage_Source.isMarxUpdated = true;
			Current_Source.isMarxUpdated = true;
			 
			// if repetition mode, times always assigned to 1
			if (Main_Console.Repetition()) 
				Main_Console.Times_Cnt = 1;
			else 
				Main_Console.Times_Cnt--;

			if (Main_Console.Times_Cnt==0) {
				Main_Console.setRepetition(false);
				Main_Console.setTrigger(false);
				osTimerStop(id_tmr_trigger);
			}
			else
				osTimerStart(id_tmr_trigger, (uint32_t)(100000.0/Main_Console.getValue(CP_FREQ))-1);
		}
		else {
			if (Main_Console.Times_Cnt==0) {
				Main_Console.setRepetition(false);
				Main_Console.setTrigger(false);
				osTimerStop(id_tmr_trigger);
			}
			else {
				LED_Set(GPIO_LED_RD, LED_ON);
				TRIGGER();
				
				Voltage_Source.isMarxUpdated = true;
				Current_Source.isMarxUpdated = true;
				 
				// if repetition mode, times always assigned to 1
				if (Main_Console.Repetition()) 
					Main_Console.Times_Cnt = 1;
				else 
					Main_Console.Times_Cnt--;

				if (Main_Console.Times_Cnt==0) {
					Main_Console.setRepetition(false);
					Main_Console.setTrigger(false);
					osTimerStop(id_tmr_trigger);
				}
				else
					osTimerStart(id_tmr_trigger, (uint32_t)(100000.0/Main_Console.getValue(CP_FREQ))-1);
			}
		}
	}
	else {
		Main_Console.Times_Cnt = 0;
		Main_Console.setRepetition(false);
		Main_Console.setTrigger(false);
		osTimerStop(id_tmr_trigger);
	}
	osDelay(200);
	LED_Set(GPIO_LED_RD, LED_OFF);
}
