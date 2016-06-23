#include "task.h"


/* this funtion realizes the delta of encoder being non-linear 
 * the faster the encoder being rotated, the more will delta's increase be accelerated */ 
inline int16_t Non_Linear(int16_t val) {
	int8_t s, v;
	
	s = (val>0)? 1:-1;
	v = s*val;
	while(--v) val*=s*val;
	val = ((val*s)/3+1)*s;
	
	return (s*val)>100? s*100:val;
};

void task_Button(void const * arg)
{
	static bool isPressed=false;
	static uint8_t isHolding=0;
	static uint16_t Encoder_CNT;
	static int16_t Encoder_Delta;
//	static bool isTFB=false;
	static bool flag_debug = false;
	
	for (;;)
	{
		// Buttons
		if (!isPressed || isHolding>15) { // Check that all buttons have been released before pressing another button
			/* Button LEFT or Button RIGHT */
			if ((GPIO_ReadInputDataBit(GPIO_BTN_LT)==RESET || GPIO_ReadInputDataBit(GPIO_BTN_RT)==RESET)) {
				if (GPIO_ReadInputDataBit(GPIO_BTN_LT)==RESET) {
					Main_Menu.onButton(BTN_LEFT);
				}
				else {
					Main_Menu.onButton(BTN_RIGHT);
				}
				isPressed = true;
			}
			/* Button BACK */
			else if (GPIO_ReadInputDataBit(GPIO_BTN_BCK)==RESET) {
				if (Main_Console.inTrigger()) {
					Main_Console.Times_Cnt = 0;
					osTimerStart(id_tmr_trigger, 1);
				}
				else {
					Voltage_Source.setValue(MP_TRIGGER, 0);
					flag_debug = true;
				}
				isPressed = true; 
			}
			/* Button SET */
			else if (GPIO_ReadInputDataBit(GPIO_BTN_SET)==RESET) {
				Voltage_Source.setValue(MP_TRIGGER, 1);
				isPressed = true;
			}
			/* Button ENCODER */
			else if (GPIO_ReadInputDataBit(GPIO_BTN_ENC)==RESET) {
	//			Main_Menu.setSpec();
	////			if (GPIO_ReadInputDataBit(GPIO_BTN_SET)==RESET && !isTFB) {
	////				isTFB=true;
	////				osTimerStart(id_tmr_trigger, 1);
	////			}
	////			else if (GPIO_ReadInputDataBit(GPIO_BTN_SET)==SET)
	////				isTFB=false;
				Main_Menu.onButton(BTN_OK);
				isPressed = true;
			}
		}
		else {
			if (!flag_debug) isHolding++;
		}
		
		if (GPIO_ReadInputDataBit(GPIO_BTN_LT)==SET && GPIO_ReadInputDataBit(GPIO_BTN_RT)==SET && GPIO_ReadInputDataBit(GPIO_BTN_BCK)==SET && GPIO_ReadInputDataBit(GPIO_BTN_SET)==SET && GPIO_ReadInputDataBit(GPIO_BTN_ENC)==SET) {
			isPressed = false;
			flag_debug = false;
			isHolding=0;
		}
		else if (flag_debug) {
			// Edit key combination here
			if (GPIO_ReadInputDataBit(GPIO_BTN_RT)==RESET) {
				flag_debug = false;
				Main_Menu.onButton(BTN_RIGHT_DEBUG);
			}
		}
		
		// Encoder
		Encoder_CNT = TIM2->CNT;			// get the current encoder cnt, it should be 0x8f by default
		TIM2->CNT = 0x8f;					// set encoder cnt to default
		Encoder_Delta = Encoder_CNT - 0x8f;	// the delta between CNT and default value shows the angle of the rotation
		if (Encoder_Delta) Main_Menu.onEncoder(Non_Linear(Encoder_Delta));
		
		
		// Trigger Signal Feedback
//		if (GPIO_ReadInputDataBit(GPIO_TFB)==RESET && !isTFB) {// && !Main_Console.inTrigger()) {
//			isTFB=true;
//			osTimerStart(id_tmr_trigger, 1);
//		}
//		else if (GPIO_ReadInputDataBit(GPIO_TFB)==SET) {
//			isTFB=false;
//		}
		
		osDelay(100);
	}
}
