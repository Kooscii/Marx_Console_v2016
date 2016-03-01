#include "task.h"


/* this funtion realizes the delta of encoder being non-linear 
 * the faster the encoder being rotated, the more will delta's increase be accelerated */ 
inline int16_t Non_Linear(int16_t val) { 
	if (val>=0)
		return (int16_t)((val*val));
	else 
		return (int16_t)(-(val*val));
};

void task_Button(void const * arg)
{
	static bool isPressed=false;
	static uint16_t Encoder_CNT;
	static int16_t Encoder_Delta;
	static bool isTFB=false;
	
	for (;;)
	{
		/* Button LEFT or Button RIGHT */
		if ((GPIO_ReadInputDataBit(GPIO_BTN_LT)==RESET || GPIO_ReadInputDataBit(GPIO_BTN_RT)==RESET) && !isPressed) {
			if (GPIO_ReadInputDataBit(GPIO_BTN_LT)==RESET) {
				Main_Menu.onButton(BTN_LEFT);
			}
			else {
				Main_Menu.onButton(BTN_RIGHT);
			}
			isPressed = true;
		}
		/* Button BACK */
		else if (GPIO_ReadInputDataBit(GPIO_BTN_BCK)==RESET && !isPressed) {
			if (Main_Console.inTrigger()) {
				Main_Console.Times_Cnt = 0;
				osTimerStart(id_tmr_trigger, 1);
			}
			else
				Main_Menu.onButton(BTN_CANCEL);
			isPressed = true;
		}
		/* Button SET */
		else if (GPIO_ReadInputDataBit(GPIO_BTN_SET)==RESET && !isPressed) {
			Main_Menu.onButton(BTN_OK);
			isPressed = true;
		}
		/* Button ENCODER */
		else if (GPIO_ReadInputDataBit(GPIO_BTN_ENC)==RESET) {
			Main_Menu.setSpec();
		}
		else if (GPIO_ReadInputDataBit(GPIO_BTN_ENC)!=RESET) {
			Main_Menu.resetSpec();
		}
		
		Encoder_CNT = TIM2->CNT;			// get the current encoder cnt, it should be 0x8f by default
		TIM2->CNT = 0x8f;					// set encoder cnt to default
		Encoder_Delta = Encoder_CNT - 0x8f;	// the delta between CNT and default value shows the angle of the rotation
		if (Encoder_Delta) Main_Menu.onEncoder(Non_Linear(Encoder_Delta));
		
		if (GPIO_ReadInputDataBit(GPIO_TFB)==RESET && !isTFB) {// && !Main_Console.inTrigger()) {
			isTFB=true;
			osTimerStart(id_tmr_trigger, 1);
		}
		else if (GPIO_ReadInputDataBit(GPIO_TFB)==SET) {
			isTFB=false;
		}
		
		// reducing scanning speed if the user hold the button down
		if (GPIO_ReadInputDataBit(GPIO_BTN_LT)==SET && GPIO_ReadInputDataBit(GPIO_BTN_RT)==SET && GPIO_ReadInputDataBit(GPIO_BTN_BCK)==SET && GPIO_ReadInputDataBit(GPIO_BTN_SET)==SET) {
			isPressed = false;
		}
		
		osDelay(50);
	}
}
