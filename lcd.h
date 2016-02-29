#ifndef LCD_H_
#define LCD_H_

#include "peripheral.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LCD_REG 	0xF8
#define LCD_DAT 	0xFA

#define LCD_DELAY_RST_LOW	1000
#define LCD_DELAY_RST		3000	//CAUTION!!! after rst change to high, a long delay must be done !!!
#define LCD_DELAY_REG		30
#define LCD_DELAY_CLR		1500
#define LCD_DELAY_DAT		30

#define LCD_NONE 	0x00
#define LCD_1 	 	0x01
#define LCD_2 	 	0x02
#define LCD_BOTH 	0x03

class LCD {
	private:
		char format[7];
		char dsp_str[24];
		int8_t len_all;
		int8_t len_dec;
		int8_t len_val;
		void Delay ( uint16_t n );
		void SPI_Write ( uint8_t cmd, uint8_t dat );
		void ConvertFmt ( char *prefix, int8_t *len_all, int8_t *len_dec );
		void int2str ( int val );
		void float2str ( double val );
		void str2str ( char *val );
		void hex2str ( char val );
		void Select ( uint8_t sel );
	public:
		bool isInitialized;
		LCD() {Init();};
		~LCD() {};
		void Init(void); 
		void Print(uint8_t line, uint8_t pos, const char *str, ...);
		void Clear(void);
};

#endif /* LCD_H_ */
