#include "task.h"


#define USART_CONTROL_BOX_FPGA USART3


inline void send_data(uint8_t addr, uint8_t cmd, uint16_t data)
{
	while(USART_GetFlagStatus(USART_CONTROL_BOX_FPGA, USART_FLAG_TC) == RESET);
	USART_SendData(USART_CONTROL_BOX_FPGA, addr);
	osDelay(1);
	while(USART_GetFlagStatus(USART_CONTROL_BOX_FPGA, USART_FLAG_TC) == RESET);
	USART_SendData(USART_CONTROL_BOX_FPGA, cmd);
	osDelay(1);
	while(USART_GetFlagStatus(USART_CONTROL_BOX_FPGA, USART_FLAG_TC) == RESET);
	USART_SendData(USART_CONTROL_BOX_FPGA, (data>>8)&0xff);
	osDelay(1);
	while(USART_GetFlagStatus(USART_CONTROL_BOX_FPGA, USART_FLAG_TC) == RESET);
	USART_SendData(USART_CONTROL_BOX_FPGA, data&0xff);
	osDelay(1);
	while(USART_GetFlagStatus(USART_CONTROL_BOX_FPGA, USART_FLAG_TC) == RESET);
	USART_SendData(USART_CONTROL_BOX_FPGA, 0x00);
	osDelay(1);
	while(USART_GetFlagStatus(USART_CONTROL_BOX_FPGA, USART_FLAG_TC) == RESET);
	USART_SendData(USART_CONTROL_BOX_FPGA, 0x00);
	osDelay(1);
}


void task_Sending (void const * arg)
{
	uint8_t i,j,k;
	//int16_t stage_polarity, stage_width, stage_delay, stage_pwm;
//	static uint16_t cf=1500, dz=500, stage_data; 
	static uint16_t pr=50, prc=30;
	uint8_t UpdatedCnt_V[2][2][4], UpdatedCnt_C[2][2][4];
	
	for (i=0; i<2; i++) {
		for (j=0; j<2; j++) {
			for (k=0; k<4; k++) {
			UpdatedCnt_V[i][j][k] = 0;
			UpdatedCnt_C[i][j][k] = 0;
			}
		}
	}
	
	for(;;)
	{
//		/* Setting Controlling GPIO */
//		if (Voltage_Source.getValue(MP_TRIGGER) || Current_Source.getValue(MP_TRIGGER)) {
//			GPIO_SetBits ( GPIO_FPGA_EN );
//			GPIO_SetBits ( GPIO_RLY_TRG );
//		}
//		else {
//			GPIO_ResetBits ( GPIO_FPGA_EN );
//			GPIO_ResetBits ( GPIO_RLY_TRG );
//		}
		
		/* Sending Voltage Source's Parameters */
		for (i=0; i<Voltage_Source.getQty(); i++) {
			// if parameters updated	
			/* Width */
			if (Voltage_Source.isUpdated(MP_PULSEWIDTH) || Voltage_Source.isMarxUpdated) {
				Voltage_Source.setUpdated(MP_PULSEWIDTH, false);
				UpdatedCnt_V[0][i][0] = 3;
			}
			if (UpdatedCnt_V[0][i][0]) {
				UpdatedCnt_V[0][i][0]--;
				send_data(0x80|i, 0xC2, Voltage_Source.getValue(MP_PULSEWIDTH)*5);
			}

			
			if (Voltage_Source.isUpdated(MP_PULSEDELAY) || Voltage_Source.isMarxUpdated) {
				Voltage_Source.setUpdated(MP_PULSEDELAY, false);
				UpdatedCnt_V[0][i][1] = 3;
			}
			if (UpdatedCnt_V[0][i][1]) {
				UpdatedCnt_V[0][i][1]--;
				send_data(0x80|i, 0xC6, Voltage_Source.getValue(MP_PULSEDELAY)*5);
			}
			
			/* fpga status updating */
			Voltage_Source.setUpdated(MP_TRIGGER, false);
			send_data(0x80|i, 0xCA, (Voltage_Source.getValue(MP_TRIGGER)||Current_Source.getValue(MP_TRIGGER))? 0x5500: 0x0000);
			
			/* fpga pwm updating */
			Voltage_Source.setUpdated(MP_PULSEVOLTAGE, false);
			send_data(0x80|i, 0xC1, Voltage_Source.getValue(MP_TRIGGER)? ((uint16_t)(Voltage_Source.getValue(MP_PULSEVOLTAGE)/Voltage_Source.getQty()))*pr: 0);
		}
		Voltage_Source.isMarxUpdated = false;

		/* Sending Current Source's Parameters */
		for (i=0; i<Current_Source.getQty(); i++)  {
			// if parameter updated
			if (Current_Source.isUpdated(MP_PULSEWIDTH) || Current_Source.isMarxUpdated) {
				Current_Source.setUpdated(MP_PULSEWIDTH, false);
				UpdatedCnt_C[0][i][0] = 3;
			}
			if (UpdatedCnt_C[0][i][0]) {
				UpdatedCnt_C[0][i][0]--;
				send_data(0x80|i, 0xC3, Current_Source.getValue(MP_PULSEWIDTH)*5);
			}
			
			if (Current_Source.isUpdated(MP_PULSEDELAY) || Current_Source.isMarxUpdated) {
				Current_Source.setUpdated(MP_PULSEDELAY, false);
				UpdatedCnt_C[0][i][1] = 3;
			}
			if (UpdatedCnt_C[0][i][1]) {
				UpdatedCnt_C[0][i][1]--;
				send_data(0x80|i, 0xC7, Current_Source.getValue(MP_PULSEDELAY)*5);
			}
			
			/* fpga status updating */
			Current_Source.setUpdated(MP_TRIGGER, false);
			send_data(0x80|i, 0xCA, (Voltage_Source.getValue(MP_TRIGGER)||Current_Source.getValue(MP_TRIGGER))? 0x5500: 0x0000);
			
			/* fpga pwm updating */
			Current_Source.setUpdated(MP_PULSEVOLTAGE, false);
			send_data(0x80|i, 0xC4, Voltage_Source.getValue(MP_TRIGGER)? ((uint16_t)(Current_Source.getValue(MP_PULSEVOLTAGE)/Current_Source.getQty()))*prc: 0);			
		}
		Current_Source.isMarxUpdated = false;
		
		Main_Console.setUpdated(CP_FREQ, false);
		Main_Console.setUpdated(CP_TIMES, false);
		
		osDelay(20);
	}
}
