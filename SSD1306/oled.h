#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
#include "stm32l1xx_gpio.h"
#include "stdlib.h"
#include "core_cm3.h"
/*
typedef unsigned char u8;
typedef unsigned int u32;

// Page address set
#define lcd_set_row(row) OLED_WR_Byte(0xB0 | ((row) & 0x0F), 0)

#define lcd_set_col(col) do { \
        OLED_WR_Byte(0x10 | ((col)>>4), 0); // Sets the DDRAM column address - upper 3-bit // \
        OLED_WR_Byte(0x00 | ((col) & 0x0F), 0); //lower 4-bit // \
    } while(0)

#define  lcdWidth 128
#define  lcdHeight 64

#define LCD_CACHSIZE          lcdWidth*(int)(lcdHeight/8+8)
//#define LCD_CACHSIZE          96*(int)(68/8+1)

//char lcd_buf[16];		        // текстовый буфер для вывода на LCD
unsigned char  LcdCache [LCD_CACHSIZE];


unsigned char FontHeight=10,FontWidth=10;


//////////////////////////////////////////////////////////////////////////////////	 
//imodule	
//////////////////////////////////////////////////////////////////////////////////	

//OLEDДЈКЅЙиЦГ
//0:4ПЯґ®РРДЈКЅ
//1:ІўРР8080ДЈКЅ
#define OLED_MODE 0
		    						  
//-----------------OLED¶ЛїЪ¶ЁТе----------------  					   



#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_7)// SS_Oled Выбор кристала
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)// RST Сброс
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define OLED_RS_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)// DC Выбор данные/команды
#define OLED_RS_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



//PC0~7,ЧчОЄКэѕЭПЯ
#define DATAOUT(x) GPIO_Write(GPIOC,x);//Кдіц  



#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4) // CSK Тактированин
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)


#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)// MOSI Последовательная передача
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

 		     
#define OLED_CMD  0	//РґГьБо
#define OLED_DATA 1	//РґКэѕЭ


*/



//OLEDїШЦЖУГєЇКэ
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_LcdCache_Clear(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	 
void OLED__line (int x1, int y1, int x2, int y2, unsigned char mode);

void OLED_ShowChar_test(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_drawbitmap(unsigned char x, unsigned char y, const unsigned char *bitmap, unsigned char w, unsigned char h, unsigned char mode);
#endif  
	 



