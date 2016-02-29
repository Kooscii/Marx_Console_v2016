#include "lcd.h"


void LCD::Delay ( uint16_t n){
	u16 i = 0;
	while ( n-- ) {
		i = 10;
		while ( i-- ) ;
	}
}


void LCD::Select ( uint8_t sel )
{
	GPIO_WriteBit ( GPIO_LCD_EN1, (BitAction)(sel & LCD_1));
	GPIO_WriteBit ( GPIO_LCD_EN2, (BitAction)(sel & LCD_2));
}


void LCD::SPI_Write ( uint8_t cmd, uint8_t dat )
{
	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET)
		;
	SPI_I2S_SendData ( SPI1, cmd );
	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET)
		;
	SPI_I2S_SendData ( SPI1, (uint8_t)( dat & 0xf0 ) );
	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET )
		;
	SPI_I2S_SendData ( SPI1, (uint8_t)( dat << 4 & 0xf0 ) );
}


void LCD::Init ()
{
	Select ( LCD_BOTH );

	GPIO_WriteBit ( GPIO_LCD_RST, Bit_RESET );
	Delay ( LCD_DELAY_RST_LOW );
	GPIO_WriteBit ( GPIO_LCD_RST, Bit_SET );
	Delay ( LCD_DELAY_RST );

	SPI_Write ( LCD_REG, 0x38 );
	Delay ( LCD_DELAY_REG );
	SPI_Write ( LCD_REG, 0x0c );
	Delay ( LCD_DELAY_REG );
	SPI_Write ( LCD_REG, 0x06 );
	Delay ( LCD_DELAY_REG );
	SPI_Write ( LCD_REG, 0x02 );
	Delay ( LCD_DELAY_REG );
	SPI_Write ( LCD_REG, 0x01 );
	Delay ( LCD_DELAY_CLR );
	
	isInitialized = true;
}


void LCD::Clear ()
{
	Select ( LCD_BOTH );
	SPI_Write ( LCD_REG, 0x01 );
	Delay ( LCD_DELAY_CLR );
}



void LCD::Print ( uint8_t line, uint8_t pos, const char * fmt, ... )
{
	va_list ap;
	char buf[24];
	uint8_t i=0;

	// select screen (line 0,1: LCD_1; line 2,3: LCD_2)
	if (line < 2) Select(LCD_1);
	else Select(LCD_2);

	// set the position address
	if (line % 2) SPI_Write ( LCD_REG, 0x90 + pos );
	else SPI_Write ( LCD_REG, 0x80 + pos );
	Delay(LCD_DELAY_REG);

	// just ignore the error here
	va_start(ap, fmt);
	std::vsprintf(buf, fmt, ap);
	va_end(ap);
	
	while (buf[i]!='\0') {
		SPI_Write ( LCD_DAT, buf[i] );
		Delay ( LCD_DELAY_DAT );
		i++;
	}
}
