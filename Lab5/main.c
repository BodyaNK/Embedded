#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <misc.h>
#include <stm32f4xx_syscfg.h>
#include "stm32f4xx_spi.h"
#include <stm32f4xx_exti.h>
#include "font6_8.h"
#include <string.h>
#define WIDTH 84
#define HEIGHT 48


void LCD5110_CS(unsigned char);
void LCD5110_RST(unsigned char);
void LCD5110_DC(unsigned char);
void LCD5110_MO(unsigned char);
void LCD5110_SCK(unsigned char);
void LCD5110_LCD_write_byte(unsigned char, unsigned char);

uint16_t delay_c = 0; 

void Delay_ms(uint16_t delay_t)
{
 delay_c = delay_t;
 while(delay_c){};
}

void LCD5110_clear() {   
    unsigned char i, j;   
    for (i = 0; i < 6; i++)   
        for (j = 0; j < 84; j++)   
            LCD5110_LCD_write_byte(0, 1);   
}

void LCD5110_GPIO_Config() 
{
	GPIO_InitTypeDef GPIOA_Init;  
	GPIO_InitTypeDef GPIOC_Init; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE); 
	
	GPIOA_Init.GPIO_Pin = GPIO_Pin_8;  
	GPIOA_Init.GPIO_Speed = GPIO_Speed_25MHz;  
	GPIOA_Init.GPIO_Mode = GPIO_Mode_OUT;  
	GPIOA_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIOA_Init.GPIO_OType = GPIO_OType_PP;  
	
	GPIOC_Init.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIOC_Init.GPIO_Speed = GPIO_Speed_25MHz;  
	GPIOC_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIOC_Init.GPIO_Mode = GPIO_Mode_OUT;  
	GPIOC_Init.GPIO_OType = GPIO_OType_PP;  
	
	GPIO_Init(GPIOA,&GPIOA_Init);  
	GPIO_Init(GPIOC,&GPIOC_Init); 
}

void LCD_init() 
{
	LCD5110_GPIO_Config();
	
	LCD5110_DC(1);
	LCD5110_MO(1);
	LCD5110_SCK(1);
	LCD5110_CS(1);
	
	LCD5110_RST(0);
	Delay_ms(10);
	LCD5110_RST(1);
	
	LCD5110_LCD_write_byte(0x21,0);  
	LCD5110_LCD_write_byte(0xc6,0);  
	LCD5110_LCD_write_byte(0x06,0);  
	LCD5110_LCD_write_byte(0x13,0);  
	LCD5110_LCD_write_byte(0x20,0);   
	LCD5110_clear();
	LCD5110_LCD_write_byte(0x0c,0); 
}

void LCD5110_CS(unsigned char OnOff) 
{ 
	if (OnOff)   
		GPIO_SetBits(GPIOC, GPIO_Pin_9); 
	else  
		GPIO_ResetBits(GPIOC, GPIO_Pin_9); 

} 

void LCD5110_RST(unsigned char OnOff)
{
	if (OnOff) 
		GPIO_SetBits(GPIOC, GPIO_Pin_8); 
	else  
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
}

void LCD5110_DC(unsigned char OnOff)
{
	if (OnOff) 
		GPIO_SetBits(GPIOC, GPIO_Pin_7); 
	else  
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
}

void LCD5110_MO(unsigned char OnOff)
{
	if (OnOff) 
		GPIO_SetBits(GPIOC, GPIO_Pin_6); 
	else  
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

void LCD5110_SCK(unsigned char OnOff)
{
	if (OnOff) 
		GPIO_SetBits(GPIOA, GPIO_Pin_8); 
	else  
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
}

void LCD5110_LCD_write_byte(unsigned char dat, unsigned char mode)
{
	unsigned char i; 
	
	LCD5110_CS(0);
	
	if (!mode) 
		LCD5110_DC(0); 
	else
		LCD5110_DC(1);

	for(i = 0; i < 8; i++)
	{   
		LCD5110_MO(dat &0x80); 
    dat = dat<<1;   
		LCD5110_SCK(0);
		LCD5110_SCK(1);
	}   	
	
	LCD5110_CS(1); 
}

void LCD5110_set_XY(unsigned char X,unsigned char Y) 
{  
	unsigned char x;  
	x = 6*X;  
	
	LCD5110_LCD_write_byte(0x40|Y,0); 
	LCD5110_LCD_write_byte(0x80|x,0); 
}
	
void LCD5110_write_char(unsigned char c) {   
    unsigned char line;   
    unsigned char ch = 0;   
   
    c = c - 32;   
   
    for (line = 0; line < 6; line++) {   
        ch = font6_8[c][line];   
        LCD5110_LCD_write_byte(ch, 1);   
   
    }   
} 

void LCD5110_write_string(char *s) {   
    unsigned char ch;   
    while (*s != '\0') {   
        ch = *s;   
        LCD5110_write_char(ch);   
        s++;   
    }   
} 

int main(void) {
	LCD_init();
	LCD5110_set_XY(0,HEIGHT/2 - 1);
	LCD5110_write_string("hello world");
}
