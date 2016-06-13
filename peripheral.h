#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include "stm32f10x.h"
#include "pins.h"
#include "type.h"
#include "configuration.h"

#define LED_OFF	false
#define LED_ON	true

void GPIO_Set(GPIO_TypeDef* _GPx, uint16_t _Pin, GPIOMode_TypeDef _Mode);
void RCC_Configure(void);
void GPIO_Configure(void);
void DMA_Configure(void);
void SPI_Configure(void);
void I2C_Configure(void);
void EXTI_Configure(void);
void TIM_Configure(void);
void USART_Configure(void);
void NVIC_Configure(void);
void Peripheral_Configure(void);

void LED_Set(GPIO_TypeDef* _GPx, uint16_t _Pin, bool _led);

extern uint8_t I2C1_Buffer_Tx[5];
extern uint8_t SPI1_Buffer_Tx[5];

#endif /* PERIPHERAL_H_ */
