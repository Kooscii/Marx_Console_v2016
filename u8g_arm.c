/*
  
  u8g_arm.c
  

  u8g utility procedures for LPC122x

  Universal 8bit Graphics Library
  
  Copyright (c) 2013, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

 
  The following delay procedures must be implemented for u8glib. This is done in this file:

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds
  
  Additional requirements:
  
      SysTick must be enabled, but SysTick IRQ is not required. Any LOAD values are fine,
      it is prefered to have at least 1ms
      Example:
        SysTick->LOAD = (SystemCoreClock/1000UL*(unsigned long)SYS_TICK_PERIOD_IN_MS) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = 7;   // enable, generate interrupt (SysTick_Handler), do not divide by 2
*/

#include "u8g_arm.h"

/*========================================================================*/
/* Generic ARM delay procedure, based on the system timer (SysTick) */

/*
  Delay by the provided number of system ticks.
  The delay must be smaller than the RELOAD value.
  This delay has an imprecision of about +/- 20 system ticks.   
*/
static void _delay_system_ticks_sub(uint32_t sys_ticks)
{
  uint32_t start_val, end_val, curr_val;
  uint32_t load;
  
  start_val = SysTick->VAL;
  start_val &= 0x0ffffffUL;
  end_val = start_val;
  
  if ( end_val < sys_ticks )
  {
    /* check, if the operation after this if clause would lead to a negative result */
    /* if this would be the case, then add the reload value first */
    load = SysTick->LOAD;
    load &= 0x0ffffffUL;
    end_val += load;
  }
  /* counter goes towards zero, so end_val is below start value */
  end_val -= sys_ticks;		
  
  
  /* wait until interval is left */
  if ( start_val >= end_val )
  {
    for(;;)
    {
      curr_val = SysTick->VAL;
      curr_val &= 0x0ffffffUL;
      if ( curr_val <= end_val )
	break;
      if ( curr_val > start_val )
	break;
    }
  }
  else
  {
    for(;;)
    {
      curr_val = SysTick->VAL;
      curr_val &= 0x0ffffffUL;
      if ( curr_val <= end_val && curr_val > start_val )
	break;
    }
  }
}

/*
  Delay by the provided number of system ticks.
  Any values between 0 and 0x0ffffffff are allowed.
*/
void delay_system_ticks(uint32_t sys_ticks)
{
  uint32_t load4;
  load4 = SysTick->LOAD;
  load4 &= 0x0ffffffUL;
  load4 >>= 2;
  
  while ( sys_ticks > load4 )
  {
    sys_ticks -= load4;
    _delay_system_ticks_sub(load4);
  }
  _delay_system_ticks_sub(sys_ticks);
}

/*
  Delay by the provided number of micro seconds.
  Limitation: "us" * System-Freq in MHz must now overflow in 32 bit.
  Values between 0 and 1.000.000 (1 second) are ok.
*/
void delay_micro_seconds(uint32_t us)
{
  uint32_t sys_ticks;

  sys_ticks = SystemCoreClock;
  sys_ticks /=1000000UL;
  sys_ticks *= us;
  delay_system_ticks(sys_ticks);  
}

/*========================================================================*/
/*
  The following delay procedures must be implemented for u8glib

  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds

*/

void u8g_Delay(uint16_t val)
{ 
	osDelay(val);
}

void u8g_MicroDelay()
{
	delay_micro_seconds(1);
}

void u8g_10MicroDelay(void)
{
	delay_micro_seconds(10);
}

void u8g_xMicroDelay(uint16_t val)
{
	delay_micro_seconds(val);
}

/*========================================================================*/
/* SPI */

void LCD_SPI_Write(uint8_t _size, uint8_t * _ptr) {
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC3);
	
	DMA1_Channel3->CNDTR = _size;
	DMA1_Channel3->CMAR = (uint32_t)_ptr;
	
	DMA_Cmd(DMA1_Channel3, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
}

void Select ( uint8_t sel ) {
	GPIO_WriteBit ( GPIO_LCD_EN1, (BitAction)(sel & LCD_1));
	GPIO_WriteBit ( GPIO_LCD_EN2, (BitAction)(sel & LCD_2));
}

/*========================================================================*/
/* u8glib com procedure */
static uint8_t i=0;
static uint8_t addr;

extern osThreadId id_tsk_lcd;

void DMA1_Channel3_IRQHandler(void){
	if(DMA_GetITStatus(DMA1_IT_TC3))
	{	
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		osSignalSet(id_tsk_lcd, 0x01);
	}
}

static char str_buf[4][24]={" "}; 
uint8_t *ptr_SPI1_Buffer_Tx;

void u8g_PrintStr(u8g_t *u8g) {
	uint8_t i; 
	const uint8_t reg[4]={0x80,0x90,0x80,0x90}, sel[4]={LCD_1, LCD_1, LCD_2, LCD_2};
	
	for (i=0; i<4; i++) {
		/* Select LCD, LCD_1 for line 0 and 1, LCD_2 for line 3 and 4 */
		Select(sel[i]);
		/* LCD Register */
		u8g_com_hw_spi_fn(u8g, U8G_COM_MSG_ADDRESS, 0, NULL);
		u8g_com_hw_spi_fn(u8g, U8G_COM_MSG_WRITE_BYTE, 0x38, NULL);		// Set to Chinese Mode
		u8g_com_hw_spi_fn(u8g, U8G_COM_MSG_WRITE_BYTE, reg[i], NULL);	// Set line address
		/* LCD Data */
		u8g_com_hw_spi_fn(u8g, U8G_COM_MSG_ADDRESS, 1, NULL);			
		u8g_com_hw_spi_fn(u8g, U8G_COM_MSG_WRITE_SEQ_P, 24, str_buf[i]);
	}
}

void u8g_BufferStr(u8g_t *u8g, uint8_t x, uint8_t y, const char * fmt, ... ) {
	va_list ap;
	uint8_t i;
	char buf[25] = {0x00};
	
	if (x==0x0f && y==0x0f) {
		for (i=0;i<24;i++) {
			str_buf[0][i] = 0x20;
			str_buf[1][i] = 0x20;
			str_buf[2][i] = 0x20;
			str_buf[3][i] = 0x20;
		}
	}
	else {
		va_start(ap, fmt);
		vsprintf(buf, fmt, ap);
		va_end(ap);
		
		for (i=0; i<24; i++) {
			if (buf[i]==0x00) break;
			str_buf[x][y+i] = buf[i];
		}
	}
}

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
	switch(msg)
	{
	case U8G_COM_MSG_STOP:
	  break;

	case U8G_COM_MSG_INIT:
		break;

	case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
		if ( arg_val == 0 ) addr = LCD_REG;
		else addr = LCD_DAT;
	 break;

	case U8G_COM_MSG_CHIP_SELECT:
		if ( arg_val == 0 ) {}
		else {}
		break;

	case U8G_COM_MSG_RESET:
		Select ( LCD_BOTH );
		GPIO_WriteBit (GPIO_LCD_RST, (BitAction)arg_val);
		osDelay(10);
		break;

	case U8G_COM_MSG_WRITE_BYTE:
		osSignalClear(id_tsk_lcd, 0x01);
		ptr_SPI1_Buffer_Tx[0] = addr;
		ptr_SPI1_Buffer_Tx[1] = arg_val&0xF0;
		ptr_SPI1_Buffer_Tx[2] = (arg_val<<4)&0xF0;
		LCD_SPI_Write(3, ptr_SPI1_Buffer_Tx);
		osSignalWait(0x01, 1);
		u8g_xMicroDelay(50);
		break;

	case U8G_COM_MSG_WRITE_SEQ:
	case U8G_COM_MSG_WRITE_SEQ_P:
		{
			register uint8_t *ptr = arg_ptr;
			
			osSignalClear(id_tsk_lcd, 0x01);
			
			i=0;
			while(arg_val>0) {
				ptr_SPI1_Buffer_Tx[i] = addr;
				ptr_SPI1_Buffer_Tx[i+1] = (*ptr)&0xF0;
				ptr_SPI1_Buffer_Tx[i+2] = ((*ptr)<<4)&0xF0;
				arg_val--;
				ptr++;
				i+=3;
			}
			LCD_SPI_Write(i, ptr_SPI1_Buffer_Tx);
			osSignalWait(0x01, 5);
			
			u8g_xMicroDelay(100);
		}
		break;
	}
	return 1;
}

