#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "stm32f10x.h"

//#define USE_USART1
//#define USE_USART2
#define USE_USART3
#define USE_ENCODER
#define USE_BUTTON
#define USE_LCD
#define USE_LED


/*********************************************
 *			    USART 1, 2, 3                *
 *********************************************
 *
 * Mode:	TX -> IN_FLOATING
 * 			RX -> AF_PP
 */
#define		MODE_TX		GPIO_Mode_AF_PP
#define		MODE_RX		GPIO_Mode_IN_FLOATING
/*
 * GPIO:	TX1 -> A9	RX1 -> A10
 * 			TX2 -> A2	RX2 -> A3
 * 			TX3 -> B10	RX3 -> B11
 */ 
#define		GPIO_TX1	GPIO_A9
#define		GPIO_RX1	GPIO_A10
#define		GPIO_TX2	GPIO_A2
#define		GPIO_RX2	GPIO_A3
#define		GPIO_TX3	GPIO_B10
#define		GPIO_RX3	GPIO_B11

/*********************************************/


/*********************************************
 *			         LCD                     *
 *********************************************
 *
 * Mode:	RST,EN1,EN2 -> Out_PP
 * 			CLK,SDA -> AF_PP
 */
#define		MODE_LCD_RST	GPIO_Mode_Out_PP
#define		MODE_LCD_EN1	GPIO_Mode_Out_PP
#define		MODE_LCD_EN2	GPIO_Mode_Out_PP
#define		MODE_LCD_CLK	GPIO_Mode_AF_PP
#define		MODE_LCD_SDA	GPIO_Mode_AF_PP
/*
 * GPIO:	RST -> A4
 * 			CLK -> A5
 * 			SDA -> A7
 *			EN1 -> B0
 *			EN2 -> B1
 */
#define		GPIO_LCD_RST	GPIO_A4
#define		GPIO_LCD_CLK	GPIO_A5
#define		GPIO_LCD_SDA	GPIO_A7
#define		GPIO_LCD_EN1	GPIO_B0
#define		GPIO_LCD_EN2	GPIO_B1

/*********************************************/


/*********************************************
 *			         LED                     *
 *********************************************
 *
 * Mode:	LED -> Out_OD
 */
#define		MODE_LED		GPIO_Mode_Out_OD
/*
 * GPIO:	Red -> B15
 * 			Yellow -> B14
 * 			Blue -> B13
 *			Green -> B12
 */
#define		GPIO_LED_RD	GPIO_B15
#define		GPIO_LED_YL	GPIO_B14
#define		GPIO_LED_BL	GPIO_B13
#define		GPIO_LED_GR	GPIO_B12

/*********************************************/


/*********************************************
 *			  Button & Encoder               *
 *********************************************
 *
 * Mode:	BTN -> IPU
 * 			ENC -> IN_FLOATING
 */
#define		MODE_BTN		GPIO_Mode_IPU
#define		MODE_ENC		GPIO_Mode_IN_FLOATING
/*
 * GPIO:	BTN_ENC -> B9
 *			BTN_LT -> B8
 * 			BTN_RT -> B7
 * 			BTN_BCK -> B6
 *			BTN_SET -> B5
 *			ENC_A -> A0
 * 			ENC_B -> A1
 */
#define		GPIO_BTN_ENC	GPIO_B9
#define		GPIO_BTN_LT		GPIO_B8
#define		GPIO_BTN_RT		GPIO_B7
#define		GPIO_BTN_BCK	GPIO_B6
#define		GPIO_BTN_SET	GPIO_B5
#define		GPIO_ENC_A		GPIO_A0
#define		GPIO_ENC_B		GPIO_A1

/*********************************************/


/*********************************************
 *			      Control                    *
 *********************************************
 *
 * Mode:	TRG -> Out_PP
 * 			TFB -> IN_FLOATING
 */
#define		MODE_TRG	GPIO_Mode_Out_PP
#define		MODE_TFB	GPIO_Mode_IN_FLOATING
/*
 * GPIO:	TRG -> C13
 * 			TFB -> B11
 */
#define		GPIO_TRG	GPIO_C13
#define		GPIO_TFB	GPIO_B11

/*********************************************/



#endif /* CONFIGURATION_H_ */
