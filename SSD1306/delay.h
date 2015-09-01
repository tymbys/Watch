#include<stm32l1xx_rcc.h>
#include<stm32l1xx_gpio.h>

#include "stm32l1xx.h"
//#include "stm32l1xx_conf.h"


void delay_ms(uint32_t ms);
void delay_us(uint32_t us);


void delay_ms(uint32_t ms)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
        RCC_GetClocksFreq (&RCC_Clocks);

        //nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
        nCount=(RCC_Clocks.SYSCLK_Frequency/10000)*ms;
        for (; nCount!=0; nCount--);
}

void delay_us(uint32_t us)
{
        SysTick->LOAD=us*9;
    SysTick->CTRL|=0x01;
    while(!(SysTick->CTRL&(1<<16)));
    SysTick->CTRL=0x00000000;
    SysTick->VAL=0x00000000;
}
