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

bool i2c_timeout(uint8_t _timeout) {
	flag_i2c_err = true;
	osSignalWait(0x01, _timeout);
	if (flag_i2c_err) return true;
	else return false;
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
		if (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) break;
		
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
		
		return 0;
	}
	/* Error */
	I2C_SoftwareResetCmd(I2C1, ENABLE);
	I2C_Configure();
	return 1;
}

bool send_data(uint8_t addr, uint8_t cmd, uint16_t data) {
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
	GPIO_SetBits(GPIO_TRG);
	
	for (;;) {
		/* Lock */
		if (Voltage_Source.getValue(MP_TRIGGER)) {
			send_data(0x27, 0xA0, 0x0001);
			send_data(0x27, 0xA1, 0x3F3F);
		}
		else {
			send_data(0x27, 0xA0, 0x0000);
			send_data(0x27, 0xA1, 0x0000);
		}
		
		/* PWM */
		send_data(0x27, 0xE1, 100, (uint8_t)(Voltage_Source.getValue(MP_PULSEVOLTAGE)/10));
		
		/* Pulse Width 1 */
		send_data(0x27, 0xC1, Voltage_Source.getValue(MP_PULSEWIDTH)*5);
		
		/* Pulse Delay 1 */
		send_data(0x27, 0xD1, Voltage_Source.getValue(MP_PULSEDELAY)*5);
		
		osDelay(50);
	}
}
