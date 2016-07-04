/*
  
  u8g_arm.h
  
  Header file for u8g LPC122x utility procedures 

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

*/


#ifndef _U8G_ARM_H
#define _U8G_ARM_H

#include "stm32f10x.h"
#include "cmsis_os.h"
#include "configuration.h"
#include "pins.h"
#include <stdarg.h>
#include <stdio.h>
#include "u8g.h"


#define LCD_NONE 	0x00
#define LCD_1 	 	0x01
#define LCD_2 	 	0x02
#define LCD_BOTH 	0x03

#define LCD_REG 	0xF8
#define LCD_DAT 	0xFA

#define SPI1_DR_Address	0x4001300C
extern uint8_t *ptr_SPI1_Buffer_Tx;

/* system helper procedures */
void delay_system_ticks(uint32_t sys_ticks);	
void delay_micro_seconds(uint32_t us);

/* SPI */
void LCD_SPI_Write(uint8_t _size, uint8_t* _ptr);

void Select ( uint8_t sel );

void u8g_BufferStr(u8g_t *u8g, uint8_t x, uint8_t y, const char * fmt, ... );
void u8g_PrintStr(u8g_t *u8g);

uint8_t u8g_com_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);

#endif


