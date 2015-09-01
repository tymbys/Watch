//#include "oled.h"
//#include "stdlib.h"
//#include "oledfont.h"

#include "Font/timesNewRoman.h"
#include "Font/WatchFont.h"
#include "delay.h"
#include "stm32l1xx_rcc.h"

typedef unsigned char u8;
typedef unsigned int u32;

/* Page address set */
#define lcd_set_row(row) OLED_WR_Byte(0xB0 | ((row) & 0x0F), 0)

#define lcd_set_col(col) do { \
        OLED_WR_Byte(0x10 | ((col)>>4), 0); /* Sets the DDRAM column address - upper 3-bit */ \
        OLED_WR_Byte(0x00 | ((col) & 0x0F), 0); /* lower 4-bit */ \
    } while(0)

#define  lcdWidth 128
#define  lcdHeight 64

#define LCD_CACHSIZE          lcdWidth*(int)(lcdHeight/8+8)
//#define LCD_CACHSIZE          96*(int)(68/8+1)

//char lcd_buf[16];		        // òåêñòîâûé áóôåğ äëÿ âûâîäà íà LCD
unsigned char  LcdCache [LCD_CACHSIZE];


unsigned char FontHeight=10,FontWidth=10;


//////////////////////////////////////////////////////////////////////////////////
//imodule
//////////////////////////////////////////////////////////////////////////////////

//OLEDÄ£Ê½ÉèÖÃ
//0:4Ïß´®ĞĞÄ£Ê½
//1:²¢ĞĞ8080Ä£Ê½
#define OLED_MODE 0

//-----------------OLED¶Ë¿Ú¶¨Òå----------------



#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_7)// SS_Oled Âûáîğ êğèñòàëà
#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)// RST Ñáğîñ
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define OLED_RS_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)// DC Âûáîğ äàííûå/êîìàíäû
#define OLED_RS_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



//PC0~7,×÷ÎªÊı¾İÏß
#define DATAOUT(x) GPIO_Write(GPIOC,x);//Êä³ö



#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4) // CSK Òàêòèğîâàíèí
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)


#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)// MOSI Ïîñëåäîâàòåëüíàÿ ïåğåäà÷à
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)


#define OLED_CMD  0	//Ğ´ÃüÁî
#define OLED_DATA 1	//Ğ´Êı¾İ




//#include "RTC.h"
//////////////////////////////////////////////////////////////////////////////////	 
//imodule	
//////////////////////////////////////////////////////////////////////////////////	

//OLEDµÄÏÔ´æ
//´æ·Å¸ñÊ½ÈçÏÂ.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
u8 OLED_GRAM[128][8];	 

//¸üĞÂÏÔ´æµ½LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
		OLED_WR_Byte (0x00,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
		OLED_WR_Byte (0x10,OLED_CMD);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
#if OLED_MODE==1
//ÏòSSD1306Ğ´ÈëÒ»¸ö×Ö½Ú¡£
//dat:ÒªĞ´ÈëµÄÊı¾İ/ÃüÁî
//cmd:Êı¾İ/ÃüÁî±êÖ¾ 0,±íÊ¾ÃüÁî;1,±íÊ¾Êı¾İ;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_RS_Set();	 
} 	    	    
#else
//ÏòSSD1306Ğ´ÈëÒ»¸ö×Ö½Ú¡£
//dat:ÒªĞ´ÈëµÄÊı¾İ/ÃüÁî
//cmd:Êı¾İ/ÃüÁî±êÖ¾ 0,±íÊ¾ÃüÁî;1,±íÊ¾Êı¾İ;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_RS_Set();

	else 
	  OLED_RS_Clr();


	//delay_ms(1);

	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 
#endif
	  	  
//¿ªÆôOLEDÏÔÊ¾    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//¹Ø±ÕOLEDÏÔÊ¾     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDCÃüÁî
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//ÇåÆÁº¯Êı,ÇåÍêÆÁ,Õû¸öÆÁÄ»ÊÇºÚÉ«µÄ!ºÍÃ»µãÁÁÒ»Ñù!!!	  





void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//¸üĞÂÏÔÊ¾
}
//»­µã 
//x:0~127
//y:0~63
//t:1 Ìî³ä 0,Çå¿Õ				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//³¬³ö·¶Î§ÁË.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 Ìî³äÇøÓòµÄ¶Ô½Ç×ø±ê
//È·±£x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,Çå¿Õ;1,Ìî³ä	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//¸üĞÂÏÔÊ¾
}
//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//mode:0,·´°×ÏÔÊ¾;1,Õı³£ÏÔÊ¾				 
//size:Ñ¡Ôñ×ÖÌå 16/12 
/*
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //µ÷ÓÃ1206×ÖÌå
		else temp=oled_asc2_1608[chr][t];		 //µ÷ÓÃ1608×ÖÌå 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
*/
//////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



/*
void OLED_ShowChar_test(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;

	unsigned char i;
	unsigned char a = 0, b = 0, c = 0;

	chr=chr-' ';//µÃµ½Æ«ÒÆºóµÄÖµ


    for(t=0;t<size;t++)
    {
		if(size==12)temp=oled_asc2_1206[chr][t];  //µ÷ÓÃ1206×ÖÌå
		if(size==16)	temp=oled_asc2_1608[chr][t];		 //µ÷ÓÃ1608×ÖÌå
		//if(size==12)	temp=oled_font_test[chr][t];		 //µ÷ÓÃ1608×ÖÌå
		//if(size==8)			temp=lcd_font_table[chr][t];

        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
    }


}
*/
////////////////////////////////////////////////////////////////////////////////////////////



//ÏÔÊ¾2¸öÊı×Ö
//x,y :Æğµã×ø±ê	 
//len :Êı×ÖµÄÎ»Êı
//size:×ÖÌå´óĞ¡
//mode:Ä£Ê½	0,Ìî³äÄ£Ê½;1,µş¼ÓÄ£Ê½
//num:ÊıÖµ(0~4294967295);
/*
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
*/
//ÏÔÊ¾×Ö·û´®
//x,y:Æğµã×ø±ê  
//*p:×Ö·û´®ÆğÊ¼µØÖ·
//ÓÃ16×ÖÌå
/*
void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   
*/
//³õÊ¼»¯SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	

 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);



	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;	 //PD3,PD6ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;

 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//GPIO_Speed_50MHz;//ËÙ¶È50MHz
 	//GPIO_Init(GPIOD, &GPIO_InitStructure);	  //³õÊ¼»¯GPIOD3,6
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	//GPIO_SetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_6);	//PD3,PD6 Êä³ö¸ß
 	GPIO_SetBits(GPIOA,GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3);
/*
 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUTÍÆÍìÊä³ö
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7Êä³ö¸ß

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUTÍÆÍìÊä³ö
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  Êä³ö¸ß

 #else*/
/*
 	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUTÍÆÍìÊä³ö
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
 	//GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	//GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  Êä³ö¸ß
 	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_6);

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUTÍÆÍìÊä³ö	  RST
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	//GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	//GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  Êä³ö¸ß
 	GPIO_SetBits(GPIOB,GPIO_Pin_5);
*/

 //#endif
  							  
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //¹Ø±ÕÏÔÊ¾
	OLED_WR_Byte(0xD5,OLED_CMD); //ÉèÖÃÊ±ÖÓ·ÖÆµÒò×Ó,Õğµ´ÆµÂÊ
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],·ÖÆµÒò×Ó;[7:4],Õğµ´ÆµÂÊ
	OLED_WR_Byte(0xA8,OLED_CMD); //ÉèÖÃÇı¶¯Â·Êı
	OLED_WR_Byte(0X3F,OLED_CMD); //Ä¬ÈÏ0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //ÉèÖÃÏÔÊ¾Æ«ÒÆ
	OLED_WR_Byte(0X00,OLED_CMD); //Ä¬ÈÏÎª0

	OLED_WR_Byte(0x40,OLED_CMD); //ÉèÖÃÏÔÊ¾¿ªÊ¼ĞĞ [5:0],ĞĞÊı.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //µçºÉ±ÃÉèÖÃ
	OLED_WR_Byte(0x14,OLED_CMD); //bit2£¬¿ªÆô/¹Ø±Õ


	OLED_WR_Byte(0x20,OLED_CMD); //Set Memory Addressing Mode
	///
	//   Default => 0x02
		//     0x00 => Horizontal Addressing Mode
		//     0x01 => Vertical Addressing Mode
		//     0x02 => Page Addressing Mode
	//OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00£¬ÁĞµØÖ·Ä£Ê½;01£¬ĞĞµØÖ·Ä£Ê½;10,Ò³µØÖ·Ä£Ê½;Ä¬ÈÏ10;
	OLED_WR_Byte(0x02,OLED_CMD);
	///


	//OLED_WR_Byte(0xA1,OLED_CMD); //¶ÎÖØ¶¨ÒåÉèÖÃ,bit0:0,0->0;1,0->127;
	//OLED_WR_Byte(0xC0,OLED_CMD); //ÉèÖÃCOMÉ¨Ãè·½Ïò;bit3:0,ÆÕÍ¨Ä£Ê½;1,ÖØ¶¨ÒåÄ£Ê½ COM[N-1]->COM0;N:Çı¶¯Â·Êı
	//OLED_WR_Byte(0xDA,OLED_CMD); //Set COM Pins Hardware Configuration
	//OLED_WR_Byte(0x12,OLED_CMD); //[5:4]ÅäÖÃ

	///---------------
	OLED_WR_Byte(0xA0,OLED_CMD); //îòçåğêàëèòü 0xA0/0xA1 ëåâî-ïğàâî
	OLED_WR_Byte(0xC8,OLED_CMD); //Set COM Output Scan Direction îòçåğêàëèòü âåòõ-íèç C0h/C8h
	OLED_WR_Byte(0xDA,OLED_CMD); //Set COM Pins Hardware Configuration
	OLED_WR_Byte(0x18,OLED_CMD); //[5:4] îòçåğêàëèòü âåòõ-íèç
	//-----------------
		 
	OLED_WR_Byte(0x81,OLED_CMD); //¶Ô±È¶ÈÉèÖÃ
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ä¬ÈÏ0X7F (ÁÁ¶ÈÉèÖÃ,Ô½´óÔ½ÁÁ)
	OLED_WR_Byte(0xD9,OLED_CMD); //ÉèÖÃÔ¤³äµçÖÜÆÚ
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //ÉèÖÃVCOMH µçÑ¹±¶ÂÊ
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //âêëş÷èòü äèñïëåé

	OLED_WR_Byte(0xA6,OLED_CMD); //íîğìàëüíîå îòîáğàæåíèå
	//OLED_WR_Byte(0xA7,OLED_CMD); //èíâåğòèğîâàíîå îòîáğàæåíèå

	//OLED_WR_Byte(0xC8,OLED_CMD);

	OLED_WR_Byte(0xAF,OLED_CMD); //¿ªÆôÏÔÊ¾	 
	OLED_Clear();
}  

//contrast 0.. ff
void OLED_SetContrast(unsigned char contrast)
{
	OLED_WR_Byte(0x81,OLED_CMD); //¶Ô±È¶ÈÉèÖÃ
	OLED_WR_Byte(contrast,OLED_CMD);
}

/************Graph*************/

void OLED_drawbitmap(unsigned char x, unsigned char y, const unsigned char bitmap[], unsigned char w,unsigned char h, unsigned char mode)
{
	 unsigned char i,j;


	for ( j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			//if (*(bitmap + i + (j / 8) * w) & TO_BIT(j%8)) {
			if ((bitmap[i + (j / 8) * w]) & (0x1 << (j%8))) {
				OLED_DrawPoint(x + i, y + j, mode);



			}
		}
	}

}


void OLED_line (int x1, int y1, int x2, int y2, unsigned char mode)  	//Draws a line between two points on the display - ïî Áğåçåíõåéìó
{
  signed int dy = 0;
  signed int dx = 0;
  signed int stepx = 0;
  signed int stepy = 0;
  signed int fraction = 0;

 // if (x1>lcdWidth() || x2>lcdWidth() || y1>lcdHeight() || y2>lcdHeight()) return;

  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else stepy = 1;
  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else stepx = 1;
  dy <<= 1;
  dx <<= 1;
  OLED_DrawPoint(x1,y1,mode);
  if (dx > dy)
  {
    fraction = dy - (dx >> 1);
    while (x1 != x2)
    {
      if (fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;
      OLED_DrawPoint(x1,y1,mode);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      OLED_DrawPoint(x1,y1,mode);
    }
  }
}



void OLED_DrawPoint_fast(unsigned char x, unsigned char y, unsigned char mode)
{
	int index;
	unsigned char offset=0, data=0;

	if ( x >= lcdWidth ) return;	//åñëè ïåğåäàëè â ôóíêöèş ìóòü - âûõîäèì
	if ( y >= lcdHeight ) return;


	index = (((int)(y)/8)*lcdWidth)+x;    //ñ÷èòàåì íîìåğ áàéòà â ìàññèâå ïàìÿòè äèñïëåÿ
	offset  = y-((y/8)*8);          //ñ÷èòàåì íîìåğ áèòà â ıòîì áàéòå
	lcd_set_col(x);
	lcd_set_row(((unsigned char)(y/8)));

	data = LcdCache[index];         //áåğåì áàéò ïî íàéäåííîìó èíäåêñó

	//data = ( 0x01 << offset );

	if ( mode == 0 ) data &= ( ~( 0x01 << offset ) );	//ğåäàêòèğóåì áèò â ıòîì áàéòå
	else if ( mode == 1 ) data |= ( 0x01 << offset );
	//else if ( mode  == PIXEL_XOR ) data ^= ( 0x01 << offset );

	OLED_WR_Byte(data, 1);		//çàãğóæàåì áàéò íàçàä
	LcdCache[index] = data;
}

void OLED_DrawLine_fast (int x1, int y1, int x2, int y2, unsigned char mode)  	//Draws a line between two points on the display - ïî Áğåçåíõåéìó
{
  signed int dy = 0;
  signed int dx = 0;
  signed int stepx = 0;
  signed int stepy = 0;
  signed int fraction = 0;

  if (x1>lcdWidth || x2>lcdWidth || y1>lcdHeight || y2>lcdHeight) return;

  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else stepy = 1;
  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else stepx = 1;
  dy <<= 1;
  dx <<= 1;
  OLED_DrawPoint_fast(x1,y1,mode);
  if (dx > dy)
  {
    fraction = dy - (dx >> 1);
    while (x1 != x2)
    {
      if (fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;
      OLED_DrawPoint_fast(x1,y1,mode);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      OLED_DrawPoint_fast(x1,y1,mode);
    }
  }
}



void OLED_DrawStringXY_fast(unsigned char x, unsigned char y, const unsigned char *p, unsigned char size)
{
//#define MAX_CHAR_POSX 122
//#define MAX_CHAR_POSY 58
	unsigned char i=0;
    while(*p!='\0')
    {
        //if(x>lcdWidth-FontWidth){x=0;y-=FontHeight;}
        //if(y>lcdHeight-lcdHeight){y=x=0;OLED_Clear(); OLED_Refresh_Gram();}

        //OLED_DrawChrXY_fast(x,y,*p,h,1);
        //OLED_DrawChr20XY_fast(x,y,*p,font,w,h);

        OLED_DrawChrXY_fast(x,y,*p,size);

        x=x+FontWidth;
        i++;
        p++;
    }
}


void OLED_DrawBitmap_fast(unsigned char x, unsigned char y, const unsigned char bitmap[], unsigned char w,unsigned char h)
{
	u8 i,n;


	lcd_set_col(x);
	lcd_set_row(((unsigned char)(y/8)));
/*
			for(i=0;i<=(int)(h/(8+1));i++)
			{
				lcd_set_row (((unsigned char)(y/8))+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
				lcd_set_col (x);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
				//lcd_set_col (0);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
				for(n=0;n<w;n++)OLED_WR_Byte(bitmap[n+(int)(h/(8+1)-i)*w],OLED_DATA);

			}
*/

	for(i=0;i<(int)(h/(8));i++)
	{
		lcd_set_row (((unsigned char)(y/8))+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
		lcd_set_col (x);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
		//lcd_set_col (0);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
		for(n=0;n<w;n++)
		{
			OLED_WR_Byte(bitmap[((n+(int)(h/(8+1)-i)*w))],OLED_DATA);
		}


	}
}




void OLED_DrawChrXY_fast(unsigned char x, unsigned char y, int ch, unsigned char size)
{
	//unsigned char font[];

	unsigned char i,n;

	unsigned char *Font_en;
	unsigned char *Font_ru;
	unsigned char status=0;


	if (size==20)
	{
		FontHeight=Font20Height;
		FontWidth=Font20Width;
		Font_en=&Font_timesNewRoman_20pt_en;
		Font_ru=&Font_timesNewRoman_20pt_ru;
		status = 1;

	}else
	if(size==14)
	{
		FontHeight=Font14Height;
		FontWidth=Font14Width;
		Font_en=&Font_timesNewRoman_14pt_en;
		Font_ru=&Font_timesNewRoman_14pt_ru;
		status = 1;
	}else
	if(size==12)
	{
		//FontHeight=Font12Height;
		//FontWidth=Font12Width;
		//Font_en=&Font_timesNewRoman_12pt_en;
		//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
		//status = 1;
	}else
	if(size==10)
	{
		FontHeight=Font10Height;
		FontWidth=Font10Width;
		Font_en=&Font_timesNewRoman_10pt_en;
		Font_ru=&Font_timesNewRoman_10pt_ru;
		status = 1;
	}else
	if(size==7)
	{

		FontHeight=Font7Height;
		FontWidth=Font7Width;
		Font_en=&Font_timesNewRoman_7pt_en;
		Font_ru=&Font_timesNewRoman_7pt_ru;
		status = 1;
	}
	else status = 0;

if(status==1)
{

	lcd_set_col(x);
	lcd_set_row(((unsigned char)(y/8)));

			//for(i=0;i<=(int)(FontHeight/(8+1));i++)
			for(i=0;i<(int)(FontHeight/(8));i++)
			{
				lcd_set_row (((unsigned char)(y/8))+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
				lcd_set_col (x);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
				//lcd_set_col (0);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
				for(n=0;n<FontWidth;n++)
				{
					  if(ch>'À')
					  {
						  //OLED_WR_Byte[(ch*5+(i)-0x3C0)];	//âûäåëÿåì áàéò-ñòîëáèê èç ñèìâîëà è ãğóçèì â ìàññèâ - 5 ğàç
						  OLED_WR_Byte(Font_ru[((n+(int)(FontHeight/(8+1)-i)*FontWidth))+FontWidth*(ch-'À')*FontHeight/8],OLED_DATA);
					  }else{
						  //OLED_WR_Byte[(ch*5+(i)-0xA0)];	//âûäåëÿåì áàéò-ñòîëáèê èç ñèìâîëà è ãğóçèì â ìàññèâ - 5 ğàç
						  OLED_WR_Byte(Font_en[((n+(int)(FontHeight/(8+1)-i)*FontWidth))+FontWidth*(ch-' ')*FontHeight/8],OLED_DATA);
					  }

					//OLED_WR_Byte(font[((n+(int)(h/(8+1)-i)*w))+w*(ch-' ')*h/8],OLED_DATA);
				}
			}



}

}

void OLED_DrawChrToCharAnimXY_fast(unsigned char x, unsigned char y, int ch, int ch0, unsigned char size, unsigned char shift/*0..3*/)
{
	unsigned char i,n,k;


	unsigned char *Font_en;
	unsigned char *Font_ru;
	unsigned char status=0;
	unsigned char until_shift=0;


	unsigned char anim[5*Font20Width*Font20Height/8]=
			{
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			};



	//unsigned char temp[FontWidth*FontHeight/8];



	if (size==20)
		{
			FontHeight=Font20Height;
			FontWidth=Font20Width;
			Font_en=&Font_timesNewRoman_20pt_en;
			Font_ru=&Font_timesNewRoman_20pt_ru;
			status = 1;

		}else
		if(size==14)
		{
			FontHeight=Font14Height;
			FontWidth=Font14Width;
			Font_en=&Font_timesNewRoman_14pt_en;
			Font_ru=&Font_timesNewRoman_14pt_ru;
			status = 1;
		}else
		if(size==10)
		{
			FontHeight=Font10Height;
			FontWidth=Font10Width;
			Font_en=&Font_timesNewRoman_10pt_en;
			Font_ru=&Font_timesNewRoman_10pt_ru;
			status = 1;
		}else
		if(size==7)
		{

			FontHeight=Font7Height;
			FontWidth=Font7Width;
			Font_en=&Font_timesNewRoman_7pt_en;
			Font_ru=&Font_timesNewRoman_7pt_ru;
			status = 1;
		}
		else status = 0;

	if(status==1)
	{

		for(i=0;i<(FontWidth*FontHeight/8);i++)
		{
			anim[i]=	Font_en[i+FontWidth*(ch-' ')*FontHeight/8];
			anim[i+FontWidth*FontHeight/8]=	Font_en[i+FontWidth*(ch0-' ')*FontHeight/8];

			//anim[4*FontWidth+1+i*FontWidth+n]=	Font_ru[n+FontWidth*(ch0-'À')*FontHeight/8+FontWidth*i];
		}


		lcd_set_col(x);
			lcd_set_row(((unsigned char)(y/8)));

				for(i=0;i<=(int)(FontHeight/(8)-1);i++)
				{
					lcd_set_row (((unsigned char)(y/8))+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
					lcd_set_col (x);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
					//lcd_set_col (0);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
					for(n=0;n<FontWidth;n++)
					{
						if(ch>'À')
						{
							//OLED_WR_Byte[(ch*5+(i)-0x3C0)];	//âûäåëÿåì áàéò-ñòîëáèê èç ñèìâîëà è ãğóçèì â ìàññèâ - 5 ğàç
							//OLED_WR_Byte(Font_20pt_ru[((n+(int)(FontHeight/(8+1)-i)*FontWidth))+FontWidth*(ch-'À')*FontHeight/8],OLED_DATA);
						}else{
							//OLED_WR_Byte[(ch*5+(i)-0xA0)];	//âûäåëÿåì áàéò-ñòîëáèê èç ñèìâîëà è ãğóçèì â ìàññèâ - 5 ğàç
							//OLED_WR_Byte(Font_20pt_en[((n+(int)(FontHeight/(8+1)-i)*FontWidth))+FontWidth*(ch-' ')*FontHeight/8],OLED_DATA);
							OLED_WR_Byte(anim[((n+(FontHeight/8-i-1)*FontWidth+FontWidth*shift))],OLED_DATA);
						}

									//OLED_WR_Byte(font[((n+(int)(h/(8+1)-i)*w))+w*(ch-' ')*h/8],OLED_DATA);
					}
				}



	}


}

void OLED_DrawStringToStringAnimXY_fast(unsigned char x, unsigned char y, const unsigned char *p, const unsigned char *p0, unsigned char size,unsigned char shift)
{
//#define MAX_CHAR_POSX 122
//#define MAX_CHAR_POSY 58
	unsigned char i=0;
    while(*p!='\0')
    {
        //if(x>lcdWidth-FontWidth){x=0;y-=FontHeight;}
        //if(y>lcdHeight-lcdHeight){y=x=0;OLED_Clear(); OLED_Refresh_Gram();}

        //OLED_DrawChrXY_fast(x,y,*p,h,1);
        //OLED_DrawChr20XY_fast(x,y,*p,font,w,h);

    	OLED_DrawChrToCharAnimXY_fast(x,y,*p, *p0,size,shift);

        x=x+FontWidth;
        i++;
        p++;
        p0++;
    }
}


void OLED_DrawString_fast(unsigned char x, unsigned char y, const unsigned char *p, unsigned char SizeChar)
{
//#define MAX_CHAR_POSX 122
//#define MAX_CHAR_POSY 58
	unsigned char i=0;
    while(*p!='\0')
    {
        //if(x>lcdWidth-FontWidth){x=0;y-=FontHeight;}
        //if(y>lcdHeight-lcdHeight){y=x=0;OLED_Clear(); OLED_Refresh_Gram();}

        //OLED_DrawChrXY_fast(x,y,*p,h,1);
        //OLED_DrawChr20XY_fast(x,y,*p,font,w,h);

    	OLED_DrawChrXY_fast(x,y,*p,SizeChar);

        x=x+FontWidth;
        i++;
        p++;
    }
}



/*******Utilit********/
/*
void IntToStr(char *byf, u32 num,u8 len)
{
	u8 t,temp,n;
	u8 enshow=0;
	u8 i =0;
	n=num;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				//OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				byf[i]=' ';

				continue;
			}else enshow=1;

		}
	 	//OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
		byf[i]=temp+'0';
		i++;

	}
}
*/
int IntToStr_rait(char *byf, u32 num, unsigned char nol )
{

	char t,k,r;
	char i =0;
	char by[]="0000000000000000";


		while(num>0)
		{
			//by[i]=num%10+'0';
			by[nol-i]=num%10+'0';
			num=num/10;
			r=i;
			i++;
		}
/*


		i=r-1;
		//byf[i]=0;
		//byf[i+1]='0';
*/



/*

		for(i=0;i<r;i++)
		{
			//t=byf[0];
			//for(k=0;k<i;k++) byf[k]=byf[k+1];
			//byf[i-1]=t;
			by[nol-i]=by[i];
			by[i]='0';

		}
*/

		i=0;
		while(i<=nol)
		{
			byf[i]=by[i];
			i++;
		}
		byf[i]=0;

/*
		i=r+1;
		while(i<nol)
		{
			byf[i]='0';
			i++;
		}
*/

/*
		//if(nol>(r-1))
		if(nol>r)
		{
			for(i=r;i>0;i--)
			{
				//t=byf[0];
				//for(k=0;k<i;k++) byf[k]=byf[k+1];

				nol--;
				byf[nol]=byf[i];
				byf[i]='0';


			}

			//byf[nol]=byf[0];
			//byf[0]='0';


		}

*/

		return r;
}



int IntToStr(char *byf, u32 num )
{
	char t,k,r;
	u8 i =0;


	while(num>0)
	{
		byf[i]=num%10+'0';
		num=num/10;
		i++;
	}
	byf[i]=0;
	//byf[i+1]='0';
	r=i;

	for(i;i>0;i--)
	{
		t=byf[0];
		for(k=0;k<i;k++) byf[k]=byf[k+1];
		byf[i-1]=t;
	}

	return r;

}

u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}


void ClrByf(char *byf, char num)
{
	char i;
	for(i=0;i<num;i++)byf[i]=0;

}

/******×àñû*******/
/*
typedef struct
{
  unsigned char  hour;     //÷àñû
  unsigned char  min;	   //ìèíóòû
  unsigned char  sec;	   //ñåêóíäû
} RTC_Time;
*/

typedef struct{
	unsigned  int year;
	unsigned  char month;
	unsigned  char day;
	unsigned  char hour;
	unsigned  char minute;
	unsigned  char second;
} ftime_t;

int IntToTimeStr(char *byf, char num)
{
	char t,k,r;
	char b1,b2;
	u8 i =0;
if(num>=10)
{
	byf[1]=num%10+'0';
	num=num/10;
	byf[0]=num%10+'0';
	byf[2]=0;
}else
{
	byf[1]=num%10+'0';
	byf[0]='0';
	byf[2]=0;

}
}

void OLED_DrawChrTimeXY_fast(unsigned char x, unsigned char y, int ch, unsigned char size)
{
	//unsigned char font[];

	unsigned char i,n;

	unsigned char *Font_en;
	//unsigned char *Font_ru;
	unsigned char status=0;


	if(size==17)
	{
		FontHeight=Font17Height;
		FontWidth=Font17Width;
		Font_en=&FontWatch_17pt;
		//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
		status = 1;
	}else
	if(size==25)
	{
		FontHeight=Font25Height;
		FontWidth=Font25Width;
		Font_en=&FontWatch_25pt;
			//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
			status = 1;
	}else
	if(size==8)
	{
		FontHeight=Font8Height;
		FontWidth=Font8Width;
		Font_en=&FontWatch_8pt;
		//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
		status = 1;
		}
	else status = 0;

if(status==1)
{

	lcd_set_col(x);
	lcd_set_row(((unsigned char)(y/8)));

			//for(i=0;i<=(int)(FontHeight/(8+1));i++)
			for(i=0;i<(int)(FontHeight/(8));i++)
			{
				lcd_set_row (((unsigned char)(y/8))+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
				lcd_set_col (x);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
				//lcd_set_col (0);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
				for(n=0;n<FontWidth;n++)
				{
					  if(ch>='.')
					  {
						  //OLED_WR_Byte[(ch*5+(i)-0x3C0)];	//âûäåëÿåì áàéò-ñòîëáèê èç ñèìâîëà è ãğóçèì â ìàññèâ - 5 ğàç
						  OLED_WR_Byte(Font_en[((n+(int)(FontHeight/(8+1)-i)*FontWidth))+FontWidth*(ch-'.')*FontHeight/8],OLED_DATA);
					  }

					//OLED_WR_Byte(font[((n+(int)(h/(8+1)-i)*w))+w*(ch-' ')*h/8],OLED_DATA);
				}
			}



}

}




void OLED_DrawChrToCharTimeAnimXY_fast(unsigned char x, unsigned char y, int ch, int ch0, unsigned char size, unsigned char shift/*0..3*/)
{
	unsigned char i,n,k;


	unsigned char *Font_en;
	unsigned char *Font_ru;
	unsigned char status=0;
	unsigned char until_shift=0;


	unsigned char anim[5*Font20Width*Font20Height/8]=
			{
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			};



	//unsigned char temp[FontWidth*FontHeight/8];



	if(size==17)
		{
			FontHeight=Font17Height;
			FontWidth=Font17Width;
			Font_en=&FontWatch_17pt;
			//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
			status = 1;
		}else
		if(size==25)
		{
			FontHeight=Font25Height;
			FontWidth=Font25Width;
			Font_en=&FontWatch_25pt;
				//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
				status = 1;
		}else
		if(size==8)
		{
			FontHeight=Font8Height;
			FontWidth=Font8Width;
			Font_en=&FontWatch_8pt;
			//Font_ru=&Font_timesNewRoman_12pt_en;///!!!!!
			status = 1;
		}
		else status = 0;

	if((status==1)&&(ch!=ch0))
	{

		for(i=0;i<(FontWidth*FontHeight/8);i++)
		{
			anim[i]=	Font_en[i+FontWidth*(ch-'.')*FontHeight/8];
			anim[i+FontWidth*FontHeight/8]=	Font_en[i+FontWidth*(ch0-'.')*FontHeight/8];

			//anim[4*FontWidth+1+i*FontWidth+n]=	Font_ru[n+FontWidth*(ch0-'À')*FontHeight/8+FontWidth*i];
		}


		lcd_set_col(x);
		lcd_set_row(((unsigned char)(y/8)));

				for(i=0;i<=(int)(FontHeight/(8)-1);i++)
				{
					lcd_set_row (((unsigned char)(y/8))+i);    //ÉèÖÃÒ³µØÖ·£¨0~7£©			// CMD_SET_PAGE
					lcd_set_col (x);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞµÍµØÖ·		//CMD_SET_COLUMN_LOWER
					//lcd_set_col (0);      //ÉèÖÃÏÔÊ¾Î»ÖÃ¡ªÁĞ¸ßµØÖ·  	//CMD_SET_COLUMN_UPPER
					for(n=0;n<FontWidth;n++)
					{
						if(ch>='0')
						{
							OLED_WR_Byte(anim[((n+(FontHeight/8-i-1)*FontWidth+FontWidth*shift))],OLED_DATA);
						}


					}
				}
	}


}




void OLED_DrawTime(unsigned char x,unsigned char y, const unsigned char *p, unsigned char SizeChar)
{
	/*
	char lcd_h[2],lcd_m[2],lcd_s[2];
	char lcd_year[4],lcd_mouth[2],lcd_dey[2];

	ClrByf(lcd_h,2);
	ClrByf(lcd_m,2);
	ClrByf(lcd_s,2);

	ClrByf(lcd_year,4);
	ClrByf(lcd_mouth,2);
	ClrByf(lcd_dey,2);

	IntToTimeStr(lcd_h,tim.hour);
	IntToTimeStr(lcd_m,tim.minute);
	IntToTimeStr(lcd_s,tim.second);

	IntToTimeStr(lcd_year,tim.year);
	IntToTimeStr(lcd_mouth,tim.month);
	IntToTimeStr(lcd_dey,tim.day);

	OLED_DrawChrTimeXY_fast(x,y,*p,SizeChar);
	x=x+FontWidth;
	p++;
	OLED_DrawChrTimeXY_fast(x,y,*p,SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,':',SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,lcd_m[0],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_m[1],SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,':',SizeChar);
	x=x+FontWidth;



	OLED_DrawChrTimeXY_fast(x,y,lcd_s[0],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_s[1],SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,' ',SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,lcd_year[2],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_year[3],SizeChar);
	x=x+FontWidth;


	OLED_DrawChrTimeXY_fast(x,y,'.',SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,lcd_mouth[0],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_mouth[1],SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,'.',SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,lcd_dey[0],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_dey[1],SizeChar);
		//x=x+FontWidth;
*/




	unsigned char i=0;
	    while(*p!='\0')
	    {
	        //if(x>lcdWidth-FontWidth){x=0;y-=FontHeight;}
	        //if(y>lcdHeight-lcdHeight){y=x=0;OLED_Clear(); OLED_Refresh_Gram();}

	        //OLED_DrawChrXY_fast(x,y,*p,h,1);
	        //OLED_DrawChr20XY_fast(x,y,*p,font,w,h);

	    	OLED_DrawChrTimeXY_fast(x,y,*p,SizeChar);

	        x=x+FontWidth;
	        i++;
	        p++;
	    }


}


void OLED_DrawTimeAnim(unsigned char x, unsigned char y, ftime_t tim, ftime_t tim0, unsigned char SizeChar)
{
	char lcd_h[2],lcd_m[2],lcd_s[2];
	char lcd_h0[2],lcd_m0[2],lcd_s0[2];
	char b;
	char f_m,f_h;

	ClrByf(lcd_h,2);
	ClrByf(lcd_m,2);
	ClrByf(lcd_s,2);

	IntToTimeStr(lcd_h,tim.hour);
	IntToTimeStr(lcd_m,tim.minute);
	IntToTimeStr(lcd_s,tim.second);

	IntToTimeStr(lcd_h0,tim0.hour);
	IntToTimeStr(lcd_m0,tim0.minute);
	IntToTimeStr(lcd_s0,tim0.second);

	if(tim.hour!=tim0.hour)	f_h=1; else f_h=0;
	if(tim.minute!=tim0.minute)	f_m=1; else f_m=0;



for (b=0;b<=FontHeight/8;b++)
{
	/*
	OLED_DrawChrTimeXY_fast(x,y,lcd_h[0],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_h[1],SizeChar);
	x=x+FontWidth;

	OLED_DrawChrTimeXY_fast(x,y,':',SizeChar);
	x=x+FontWidth;



		OLED_DrawChrTimeXY_fast(x,y,lcd_m[0],SizeChar);
		x=x+FontWidth;
		OLED_DrawChrTimeXY_fast(x,y,lcd_m[1],SizeChar);
		x=x+FontWidth;

		OLED_DrawChrTimeXY_fast(x,y,':',SizeChar);

**/

if(f_h==1)
{
	OLED_DrawChrToCharTimeAnimXY_fast(x,y,lcd_h0[0],lcd_h[0],SizeChar,b);
	OLED_DrawChrToCharTimeAnimXY_fast(x+FontWidth,y,lcd_h0[1],lcd_h[1],SizeChar,b);
}else
{
	OLED_DrawChrTimeXY_fast(x,y,lcd_h[0],SizeChar);
	OLED_DrawChrTimeXY_fast(x+FontWidth,y,lcd_h[1],SizeChar);
}
	OLED_DrawChrTimeXY_fast(x+2*FontWidth,y,':',SizeChar);


if(f_m==1)
{
	OLED_DrawChrToCharTimeAnimXY_fast(x+3*FontWidth,y,lcd_m0[0],lcd_m[0],SizeChar,b);
	OLED_DrawChrToCharTimeAnimXY_fast(x+4*FontWidth,y,lcd_m0[1],lcd_m[1],SizeChar,b);
}else
{
	OLED_DrawChrTimeXY_fast(x+3*FontWidth,y,lcd_m[0],SizeChar);
	OLED_DrawChrTimeXY_fast(x+4*FontWidth,y,lcd_m[1],SizeChar);
}
	OLED_DrawChrTimeXY_fast(x+5*FontWidth,y,':',SizeChar);


	OLED_DrawChrToCharTimeAnimXY_fast(x+6*FontWidth,y,lcd_s0[0],lcd_s[0],SizeChar,b);
	//x=x+FontWidth;
	OLED_DrawChrToCharTimeAnimXY_fast(x+7*FontWidth,y,lcd_s0[1],lcd_s[1],SizeChar,b);
	delay_ms(40);

	}
	/*
	OLED_DrawChrTimeXY_fast(x,y,lcd_s[0],SizeChar);
	x=x+FontWidth;
	OLED_DrawChrTimeXY_fast(x,y,lcd_s[1],SizeChar);
	//x=x+FontWidth;

	 */

}


void OLED_LcdCache_Clear(void)
{
	u32 i;
	for(i=0;i<LCD_CACHSIZE;i++)LcdCache[i]=0X00;
}

void OLED_Draw0(void)
{
	unsigned char i,j;

	lcd_set_col(0);
	lcd_set_row(0);



	for (i=1;i<4;i++)
	{
		//lcd_set_col(0);
		//lcd_set_row(i);

		for(j=0;j<128;j++)
		{

			lcd_set_col(j);
			lcd_set_row(i);
			OLED_WR_Byte(0xf0, 1);

			lcd_set_col(j);
			lcd_set_row(i-1);
			OLED_WR_Byte(0x00, 1);



			lcd_set_col(j);
			lcd_set_row(7-i);
			OLED_WR_Byte(0x0f, 1);

			lcd_set_col(j);
			lcd_set_row(7-i+1);
			OLED_WR_Byte(0x00, 1);


			//delay_ms(10);
/*
	lcd_set_col(j);
	lcd_set_row(i-1);
	OLED_WR_Byte(0x0, 1);

	lcd_set_col(j);
	lcd_set_row(63-i);
	OLED_WR_Byte(0xff, 1);

	lcd_set_col(j);
	lcd_set_row(63-i+1);
	OLED_WR_Byte(0x0, 1);
*/

	}



	}
}

