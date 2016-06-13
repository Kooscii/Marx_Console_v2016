#include "task.h"

extern uint8_t I2C1_Buffer_Tx[5];

//static uint16_t trigger_cnt=0;
//static uint8_t mode=0, status=0, i=0, j=0, k=0;

//uint8_t len;
//uint8_t ptr[4];

//const uint16_t list_freq_uint[2] = {1, 1000};

inline void pulse_delay() {
	uint16_t t=76;	// delay 10us
	while (t--) __nop();
}

void TRIGGER() {
	GPIO_SetBits(GPIO_TRG);
	pulse_delay();
	GPIO_ResetBits(GPIO_TRG);
}

uint8_t I2C1_Buffer_Rx[3] = {0x12, 0x34, 0x56};

void timer_Trigger(void const *arg) {
	//osTimerStart(id_tmr_trigger, 1000);
//	static uint32_t trg_freq, a, b;
//	
//	if (Voltage_Source.getValue(MP_TRIGGER) || Current_Source.getValue(MP_TRIGGER)) {
//		if (!Main_Console.inTrigger()) {
//			Main_Console.Times_Cnt = Main_Console.getValue(CP_TIMES);
//			Main_Console.setRepetition(!Main_Console.Times_Cnt);
//			Main_Console.setTrigger(true);

//			LED_Set(GPIO_LED_YL, LED_ON);
//			//TRIGGER();
//			
//			Voltage_Source.isMarxUpdated = true;
//			Current_Source.isMarxUpdated = true;
//			 
//			// if repetition mode, times always assigned to 1
//			if (Main_Console.Repetition()) 
//				Main_Console.Times_Cnt = 1;
//			else 
//				Main_Console.Times_Cnt--;

//			if (Main_Console.Times_Cnt==0) {
//				Main_Console.setRepetition(false);
//				Main_Console.setTrigger(false);
//				osTimerStop(id_tmr_trigger);
//			}
//			else {
//				a=Main_Console.getValue(CP_FREQ);
//				b=list_freq_uint[Main_Console.getValue(CP_FREQ_UNIT)];
//				trg_freq = (uint32_t)(10000.0/a/b)-1;
//				osTimerStart(id_tmr_trigger, trg_freq);
//			}
//		}
//		else {
//			if (Main_Console.Times_Cnt==0) {
//				Main_Console.setRepetition(false);
//				Main_Console.setTrigger(false);
//				osTimerStop(id_tmr_trigger);
//			}
//			else {
//				LED_Set(GPIO_LED_YL, LED_ON);
//				TRIGGER();
//				
//				Voltage_Source.isMarxUpdated = true;
//				Current_Source.isMarxUpdated = true;
//				 
//				// if repetition mode, times always assigned to 1
//				if (Main_Console.Repetition()) 
//					Main_Console.Times_Cnt = 1;
//				else 
//					Main_Console.Times_Cnt--;

//				if (Main_Console.Times_Cnt==0) {
//					Main_Console.setRepetition(false);
//					Main_Console.setTrigger(false);
					//osTimerStop(id_tmr_trigger);
//				}
//				else
//					osTimerStart(id_tmr_trigger, trg_freq);
//			}
//		}
//	}
//	else {
//		Main_Console.Times_Cnt = 0;
//		Main_Console.setRepetition(false);
//		Main_Console.setTrigger(false);
//		osTimerStop(id_tmr_trigger);
//	}
//	osDelay(10);
//	LED_Set(GPIO_LED_YL, LED_OFF);
}
