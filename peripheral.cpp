#include "peripheral.h"

#define I2C1_DR_Address	0x40005410
#define SPI1_DR_Address	0x4001300C

uint8_t I2C1_Buffer_Tx[5];
uint8_t SPI1_Buffer_Tx[100];

void LED_Set(GPIO_TypeDef* _GPx, uint16_t _Pin, bool _led) {
	if (_led) GPIO_ResetBits(_GPx, _Pin);
	else GPIO_SetBits (_GPx, _Pin);
}

void RCC_Configure(void)													
{
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA	|
	                         RCC_APB2Periph_GPIOB	|
	                         RCC_APB2Periph_GPIOC	|
	#ifdef USE_LCD
	                         RCC_APB2Periph_SPI1	|
	#endif
	#ifdef USE_USART1
	                         RCC_APB2Periph_USART1	|
	#endif
	                         RCC_APB2Periph_AFIO	|
							 0, ENABLE );

	RCC_APB1PeriphClockCmd ( 
	#ifdef USE_USART2
							 RCC_APB1Periph_USART2	|
	#endif
	#ifdef USE_USART3
							 RCC_APB1Periph_USART3	|
	#endif
	                         RCC_APB1Periph_TIM2	|
	#ifdef USE_I2C
							 RCC_APB1Periph_I2C1	|
	#endif
							 0, ENABLE );
							 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	GPIO_PinRemapConfig ( GPIO_Remap_SWJ_JTAGDisable, ENABLE );
}

void GPIO_Set( GPIO_TypeDef* _GPx, uint16_t _Pin, GPIOMode_TypeDef _Mode )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin   = _Pin;
	GPIO_InitStructure.GPIO_Mode  = _Mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init ( _GPx, &GPIO_InitStructure );
}

void GPIO_Set( GPIO_TypeDef* _GPx, uint16_t _Pin, GPIOMode_TypeDef _Mode, GPIOSpeed_TypeDef _Speed)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin   = _Pin;
	GPIO_InitStructure.GPIO_Mode  = _Mode;
	GPIO_InitStructure.GPIO_Speed = _Speed;

	GPIO_Init ( _GPx, &GPIO_InitStructure );
}

void GPIO_Configure(void)
{
#ifdef USE_USART1
	GPIO_Set(GPIO_TX1, MODE_TX);
	GPIO_Set(GPIO_RX1, MODE_RX);
#endif

#ifdef USE_USART2
	GPIO_Set(GPIO_TX2, MODE_TX);
	GPIO_Set(GPIO_RX2, MODE_RX);
#endif
	
#ifdef USE_USART3
	GPIO_Set(GPIO_TX3, MODE_TX);
	GPIO_Set(GPIO_RX3, MODE_RX);
#endif
	
#ifdef USE_I2C
	GPIO_Set(GPIO_I2C_SCL, MODE_I2C, GPIO_Speed_2MHz);
	GPIO_Set(GPIO_I2C_SDA, MODE_I2C, GPIO_Speed_2MHz);
	GPIO_SetBits(GPIO_I2C_SCL);
	GPIO_SetBits(GPIO_I2C_SDA);
#endif

#ifdef USE_LCD
	GPIO_Set(GPIO_LCD_RST, MODE_LCD_RST);
	GPIO_Set(GPIO_LCD_CLK, MODE_LCD_CLK);
	GPIO_Set(GPIO_LCD_SDA, MODE_LCD_SDA);
	GPIO_Set(GPIO_LCD_EN1, MODE_LCD_EN1);
	GPIO_Set(GPIO_LCD_EN2, MODE_LCD_EN2);
#endif

#ifdef USE_ENCODER
	GPIO_Set(GPIO_ENC_A, MODE_ENC, GPIO_Speed_2MHz);
	GPIO_Set(GPIO_ENC_B, MODE_ENC, GPIO_Speed_2MHz);
#endif

#ifdef USE_BUTTON
	GPIO_Set(GPIO_BTN_ENC, MODE_BTN);
	GPIO_Set(GPIO_BTN_LT, MODE_BTN);
	GPIO_Set(GPIO_BTN_RT, MODE_BTN);
	GPIO_Set(GPIO_BTN_BCK, MODE_BTN);
	GPIO_Set(GPIO_BTN_SET, MODE_BTN);
#endif

#ifdef USE_LED
	GPIO_Set(GPIO_LED_GR, MODE_LED);
	GPIO_Set(GPIO_LED_YL, MODE_LED);
	GPIO_Set(GPIO_LED_RD, MODE_LED);
	GPIO_Set(GPIO_LED_BL, MODE_LED);

	GPIO_SetBits ( GPIO_LED_GR );	//set default value at 0
	GPIO_SetBits ( GPIO_LED_BL );
	GPIO_SetBits ( GPIO_LED_YL );
	GPIO_SetBits ( GPIO_LED_RD );
#endif

	/* SIGNAL CTRL FPGA */
	GPIO_Set(GPIO_TFB, MODE_TFB);
	GPIO_Set(GPIO_TRG, MODE_TRG);

	GPIO_ResetBits(GPIO_TRG);
}

void DMA_Configure(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	/* DMA CH6 for I2C1 */
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)I2C1_Buffer_Tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE); // Enable IT
	
	/* DMA CH3 for SPI1 */
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI1_Buffer_Tx;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);	// Enable IT
}

void USART_Configure(void)
{
#if defined(USE_USART1) || defined(USE_USART2) || defined(USE_USART3)
	USART_InitTypeDef USART_InitStructure;
#endif
	
#ifdef USE_USART1
	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init ( USART1, &USART_InitStructure );
	USART_Cmd ( USART1, ENABLE );
	USART_ITConfig ( USART1, USART_IT_RXNE, ENABLE ); 	// enable usart1 rx interrupt
#endif

#ifdef USE_USART2
	USART_InitStructure.USART_BaudRate            = 19200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_2;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init ( USART2, &USART_InitStructure );
	USART_Cmd ( USART2, ENABLE );
#endif

#ifdef USE_USART3
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init ( USART3, &USART_InitStructure );
	USART_Cmd ( USART3, ENABLE );
//	USART_ITConfig ( USART3, USART_IT_RXNE, ENABLE ); 	// enable usart3 rx interrupt
#endif
}


void SPI_Configure(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

#ifdef USE_LCD
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init ( SPI1, &SPI_InitStructure );
	SPI_Cmd ( SPI1, ENABLE );
#endif
}

void I2C_Configure(void)
{
	I2C_InitTypeDef  I2C_InitStructure;

#ifdef USE_I2C
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
	I2C_InitStructure.I2C_OwnAddress1 = 0xE0;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd ( I2C1, ENABLE );
	
	I2C_DMACmd(I2C1, ENABLE);
#endif
}


void TIM_Configure(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

	/* ENCODER */
	TIM_TimeBaseStructure.TIM_Prescaler     = 3;
	TIM_TimeBaseStructure.TIM_Period        = 0xff;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
	TIM_TimeBaseInit ( TIM2, &TIM_TimeBaseStructure );

	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICFilter    = 0;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit ( TIM2, &TIM_ICInitStructure );

	TIM_ICInitStructure.TIM_Channel     = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICFilter    = 0;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit ( TIM2, &TIM_ICInitStructure ) ;

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);

	TIM_Cmd(TIM2, ENABLE);
	
	TIM2->CNT = 0x8f;
}

void EXTI_Configure( void )
{
//	EXTI_InitTypeDef EXTI_InitStructure;

//	/* binding B11 to EXTI line */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
//	
//	/* configure EXTI line 11 */
//	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);

}

void NVIC_Configure(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_0 );

//#ifdef USE_USART1
//	//interrupt priority
//	NVIC_InitStructure.NVIC_IRQChannel            = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
//	NVIC_Init ( &NVIC_InitStructure );
//#endif
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel            = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
//	NVIC_Init ( &NVIC_InitStructure );
  
	/* Enable the USARTy_DMA1_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_0 );
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void Peripheral_Configure( void )
{
	RCC_Configure();
	GPIO_Configure();
	DMA_Configure();
	USART_Configure();
	SPI_Configure();
	I2C_Configure();
	NVIC_Configure();
	TIM_Configure();
	EXTI_Configure();
}
