
void TACT_Config(void){


GPIO_InitTypeDef GPIO_InitStructure;
//EXTI_InitTypeDef EXTI_InitStructure;
//NVIC_InitTypeDef NVIC_InitStructure;

// Enable GPIOA clock //
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

// Configure PA0 pin as input floating //
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2  ; ///////////////////////////////
GPIO_Init(GPIOA, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0; ///////////////////////////////
GPIO_Init(GPIOA, &GPIO_InitStructure);


GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12  ; ///////////////////////////////
GPIO_Init(GPIOB, &GPIO_InitStructure);


//вибромоторчик
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1; ///////////////////////////////
GPIO_Init(GPIOA, &GPIO_InitStructure);


/*
//GPIO_SetBits(GPIOA, GPIO_Pin_2);

// Enable SYSCFG clock //
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
// Connect EXTI0 Line to PA0 pin //
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,   EXTI_PinSource2);
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,   EXTI_PinSource0);

SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB,   EXTI_PinSource12);

// Configure EXTI0 line //
EXTI_InitStructure.EXTI_Line = EXTI_Line2;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

EXTI_InitStructure.EXTI_Line = EXTI_Line0;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);


EXTI_InitStructure.EXTI_Line = EXTI_Line12;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);


// Enable and set EXTI0 Interrupt to the lowest priority //
NVIC_InitStructure.NVIC_IRQChannel =  EXTI2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
*/

}
