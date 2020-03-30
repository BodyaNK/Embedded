#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <misc.h>
 
#include <stm32f4xx_syscfg.h>
 
#include <stm32f4xx_exti.h>

uint16_t delay_c = 0;

void delay_ms(uint16_t delay_t){
  delay_c = delay_t;
  while(delay_c){};
}

void SysTick_Handler(void){
  if(delay_c > 0)
  delay_c--;
}

void ButtonInit() {
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
      GPIO_InitTypeDef gpio_b;
      GPIO_StructInit(&gpio_b);
      gpio_b.GPIO_Mode = GPIO_Mode_IN;
 
      GPIO_Init(GPIOA, &gpio_b);
}

const uint16_t LEDS = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
const uint16_t LED[] = {GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void leds_init() {
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
      GPIO_InitTypeDef gpio;
      GPIO_StructInit(&gpio);
      gpio.GPIO_OType = GPIO_OType_PP;
      gpio.GPIO_Mode = GPIO_Mode_OUT;
      gpio.GPIO_Pin = LEDS;
 
      GPIO_Init(GPIOD, &gpio);
}

void it_init() {
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = EXTI_Line0;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Rising;
    exti.EXTI_LineCmd = ENABLE;
 
    EXTI_Init(&exti);
 
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = EXTI0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
 
    NVIC_Init(&nvic);
}

u16 flag = 0;

void EXTI0_IRQHandler(void){
    
    if (EXTI_GetITStatus(EXTI_Line0) != RESET){
        flag += 1; 
        if (flag >= 4) flag = 0;
				//delay_ms(30);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

int main2(){
		//SysTick_Config(SystemCoreClock/1000);
    ButtonInit();
    it_init();
    leds_init();
  do {
      GPIO_Write(GPIOD, LED[flag]);
  } while(1);
}
