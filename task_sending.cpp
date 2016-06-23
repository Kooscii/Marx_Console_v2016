#include "task.h"
#include <stdarg.h>

#define I2C_TIMEOUT_OFFSET 30

bool flag_i2c_err = false;

extern "C" {
	
	void DMA1_Channel6_IRQHandler(void){
		if (DMA_GetITStatus(DMA1_IT_TC6)) {
			DMA_ClearITPendingBit(DMA1_IT_TC6);
			flag_i2c_err = false;
			osSignalSet(id_tsk_sending, 0x01);
		}
	}
	
}

inline bool i2c_timeout(uint8_t _timeout) {
	flag_i2c_err = true;
	osSignalWait(0x01, _timeout);
	if (flag_i2c_err) return true;
	else return false;
}

static uint16_t j=0, k =0, e=0;

void i2c_error() {
	uint16_t i, t;
	
	if (!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_ACK_FAILURE)) {
		j++;
		GPIO_Set(GPIO_I2C_SCL, GPIO_Mode_Out_OD);
		GPIO_SetBits(GPIO_I2C_SCL);
		for(t=0; t<0xffff; t--);
		GPIO_ResetBits(GPIO_I2C_SCL);
		for(t=0; t<0xffff; t--);
		GPIO_SetBits(GPIO_I2C_SCL);
		GPIO_Set(GPIO_I2C_SCL, MODE_I2C);
	}
	e++;
	I2C_SoftwareResetCmd(I2C1, ENABLE);
	I2C_Configure();
	DMA_Cmd(DMA1_Channel6, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC6);

}

bool I2C_WriteBytes(uint8_t _addr, uint8_t _size, ...) {
	va_list ap;
	uint16_t i, timeout;
	
	for(;;) {
		/* 0. Wait I2C idle */
		timeout = 10 + I2C_TIMEOUT_OFFSET;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && --timeout); 
		if (!timeout) break;		// if timeout, reset i2c
		
		/* 1. Generate I2C Start Signal */
		I2C_GenerateSTART(I2C1,ENABLE);										
		
		/* 2. Set Tx Buffer (this part of code also performs as a delay function for the time between start signal's generating and checking)*/
		DMA1_Channel6->CNDTR = _size;		// DMA buffer size
		va_start(ap, _size);
		for (i=0; i<_size; i++) I2C1_Buffer_Tx[i] = va_arg(ap, int);
		va_end(ap);
		
		/* 3. Check I2C Start Signal */
		//if (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) break;
		timeout = 10 + I2C_TIMEOUT_OFFSET;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && --timeout); 
		if (!timeout) break;	
		
		/* 4. Send Slave Address */
		I2C_Send7bitAddress(I2C1, _addr<<1, I2C_Direction_Transmitter);
		timeout = 10 + I2C_TIMEOUT_OFFSET;			// Address Timeout
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && --timeout);
		if (!timeout) break;		// if timeout, reset i2c
		
		/* 5. Send Data using DMA */
		DMA_Cmd(DMA1_Channel6, ENABLE);
		if (i2c_timeout(1)) break;
		timeout = 10 + I2C_TIMEOUT_OFFSET;			// Last Data Sending Timeout
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && --timeout);
		if (!timeout) break;		// if timeout, reset i2c
		
		/* 6. Generate Stop Signal and reset DMA */
		I2C_GenerateSTOP(I2C1, ENABLE);
		DMA_Cmd(DMA1_Channel6, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_TC6);
		k++;
		
		osDelay(1);
		
		return 0;
	}
	/* Error */
	i2c_error();
	return 1;
}

inline bool send_data(uint8_t addr, uint8_t cmd, uint16_t data) {
	uint8_t hdata, ldata;
	
	hdata = (uint8_t)(data>>8)&0x00FF;
	ldata = (uint8_t)(data)&0x00FF;
	
	return I2C_WriteBytes(0x27, 3, cmd, hdata, ldata);
}

bool send_data(uint8_t addr, uint8_t cmd, uint8_t hdata, uint8_t ldata) {
	return I2C_WriteBytes(0x27, 3, cmd, hdata, ldata);
}

void task_Sending (void const * arg)
{
	static float intv=0, freq=0;
	static uint8_t i;
	static uint16_t chop_dura=20*50, dead_zone[6]={25}, delay[6]={0}, width[6]={0}, freq_hex;
	
	GPIO_SetBits(GPIO_TRG);
	for (;;) {
		
		/* FREQ */
		freq = Main_Console.getValue(CP_FREQ);
		freq_hex = (uint16_t)freq;
		if (Main_Console.getValue(CP_FREQ_UNIT)) {
			freq_hex |= 0x8000;
			freq *= 1000.0;
		}
		send_data(0x27, 0xB2, freq_hex);
		
		
		intv = 100000000.0/freq-50;
		
//		/* Pulse Width */
//		for (i=4; i>0; i--) {
//			width[i-1] = (intv>1300? 1300:(intv>0? intv:0))*50;
//			intv -= 1300;
//		}
		/* Pulse Width */
		if (intv>1000) width[3] = 1000*50;
		else if (intv<0) width[3] = 0;
		else width[3] = intv*50;
		width[4] = Voltage_Source.getValue(MP_PULSEWIDTH)*5;	// Pulse Width
		width[5] = chop_dura;									// Chop Width
		
		/* Pulse Delay */
		delay[0] = 0;
		dead_zone[4] = dead_zone1._get_val()/2;
		dead_zone[5] = dead_zone2._get_val()/2;
		for (i=1; i<6; i++)
			delay[i] = delay[i-1] + width[i-1] + (width[i-1]? dead_zone[i]:0);
		
		/* PWM */ 
		send_data(0x27, 0xE1, 100, (uint8_t)(Voltage_Source.getValue(MP_PULSEVOLTAGE)/10));
		
		/* Pulse 1 : Trigger */
		send_data(0x27, 0xC1, width[4]);
		send_data(0x27, 0xD1, delay[4]);
		
		/* Pulse 2 : Chop */
		send_data(0x27, 0xC2, width[5]);
		send_data(0x27, 0xD2, delay[5]);
		
//		/* Pulse 3 : Pre-charge */
//		for (i=0; i<4; i++) {
//			send_data(0x27, 0xC3+i, width[i]);
//			send_data(0x27, 0xD3+i, delay[i]);
//		}
		send_data(0x27, 0xC3, width[3]);
		send_data(0x27, 0xD3, delay[3]);
		

		/* Lock */
		if (Voltage_Source.getValue(MP_TRIGGER)) {
			switch (Main_Console.getValue(CP_TIMES)) {
				case 1:		// single mode
					send_data(0x27, 0xA0, 0x07, 0x07);
					send_data(0x27, 0xA1, 872);
					send_data(0x27, 0xA2, 2921);
					break;
				default:	// rep
					send_data(0x27, 0xA0, 0x07, 0x07);
					send_data(0x27, 0xA1, 872);
					send_data(0x27, 0xA2, 1292);
					break;
			}
		}
		else {
			send_data(0x27, 0xA0, 0x0000);
			send_data(0x27, 0xA1, 278);
		}
		
		osDelay(200);
	}
}
