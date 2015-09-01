#include "stm32l1xx.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_syscfg.h"

//#include "math.h"

#include "RTC.h"
//#include "oled.h"
#include "TACT.h"
#include "WProgram.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "icon.h"


//void RTC_Initilithahion(void);
//void RTC_Set_Time(void);
//int RTC_Set_Alarm(void);

typedef unsigned char u8;



//название меню
//const char menu[5][16] = {"Часы\0","Календарь\0","Будильник\0","Настройки\0"};
//команды к каждому пункту
//const unsigned char menu_cmd[5] = {1,2,3,4};

//int curent=0;
//unsigned char globa_menu=0; ////глобальное меню 0...и дальше
//unsigned char f_work_comand=0;
unsigned char f_aktive_menu=0;
unsigned char f_clearScrin = 0;


//volatile unsigned char key = 0;
volatile unsigned char key_up, key_down, key_ok;





//unsigned char TimeCount=60;

//char f_DrawTime=0;
//char f_DetectRadiathion=0;
//char f_Anim=0;

//volatile unsigned char curent_cmd = 0;



//char bbb[]="0000000000000000";

//char IndicationRadiation[64];


void RTC_Alarm_IRQHandler()
{
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
    {
    	//RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    	PWR_RTCAccessCmd(ENABLE); // Доступ в RTC
    	RTC_ClearITPendingBit(RTC_IT_ALRA);
    	RTC_ClearFlag(RTC_FLAG_ALRAF);
    	PWR_RTCAccessCmd(DISABLE);

        EXTI_ClearITPendingBit(EXTI_Line17);
        //OLED_DrawString_fast(0,0,"alarm",10);
    }

    /////////////////////////?????????????????????????????????????????????????????????????????????????????????????
    if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) != RESET)
    {
    	PWR_RTCAccessCmd(ENABLE);
    	RTC_ClearFlag(RTC_FLAG_ALRAF);
    	PWR_RTCAccessCmd(DISABLE);
    	//f_WakeupToAlarm=1;
    	curent_cmd = 5;

    }
/////////////////////////



}




void SysTick_Handler(void)
{

}

/*
void EXTI15_10_IRQHandler(void) {

   if (EXTI_GetITStatus(EXTI_Line12) != RESET)
   {
	   key = 12;

	   if (globa_menu<4) globa_menu++;
	   else globa_menu = 4;

	   //IntToStr(bbb,globa_menu);
	   //OLED_DrawString_fast(0,0,bbb,10);

   }



EXTI_ClearITPendingBit(EXTI_Line12);
//EXTI_ClearITPendingBit(EXTI_Line13);
//EXTI_ClearITPendingBit(EXTI_Line14);

}
*/

/*
void EXTI2_IRQHandler(void) {

   if (EXTI_GetITStatus(EXTI_Line2) != RESET)
   {

	   key = 2;

	   if (globa_menu>1) globa_menu--;
	   else globa_menu = 1;

	   //IntToStr(bbb,globa_menu);
	   //OLED_DrawString_fast(0,0,bbb,10);


   }


EXTI_ClearITPendingBit(EXTI_Line2);

}
*/
/*
void EXTI0_IRQHandler(void) {

   if (EXTI_GetITStatus(EXTI_Line0) != RESET)
   {
	   key = 1;

	   //OLED_DrawString_fast(0,0,"000000000",10);
	   //f_aktive_menu = 1;


	   if (f_aktive_menu == 1)
	   {
		   f_work_comand = 1;
		   f_aktive_menu = 0;
		   f_clearScrin = 1;
		   curent_cmd = menu_cmd[globa_menu-1];

	   }
	   	else curent_cmd = 255 ;

   }


EXTI_ClearITPendingBit(EXTI_Line0);

}

*/

/*
void RTC_IRQHandler(void)
{

	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{

		RtcGet();


		f_DrawTime=1;



		SBM20_Count++;

		RTC_ClearITPendingBit(RTC_IT_SEC);
		RTC_WaitForLastTask();
	}
}
*/



/*
void SpeakerDetekt()
{
	char i;

	if(f_DetectRadiathion==1)
	{

		GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		delay_ms(1);
		GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
    	f_DetectRadiathion=0;
	}
}

*/

/*
void ShowWatch()
{

		char temp_str[16];
		unsigned int temp=0;
		//char ch[]=":";
		//OLED_DrawTimeAnim(10,23,getTime,getTime0,25);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//sprintf (bbb,"%002i:%002i:%002i %002i/%002i/%002i",getTime.hour,getTime.minute,getTime.second,2000-getTime.year,getTime.month,getTime.day);
		//sprintf (bbb,"%002i:%002i:%002i" ,getTime.hour,getTime.minute,getTime.second);

		//ClrByf(bbb,16);
		ClrByf(temp_str,16);

		IntToStr_rait(bbb,getTime.hour,1);
		strcat(temp_str,bbb);

		strcat(temp_str,":");

		IntToStr_rait(bbb,getTime.minute,1);
		strcat(temp_str,bbb);

		strcat(temp_str,":");

		IntToStr_rait(bbb,getTime.second,1);
		//IntToStr(bbb,1234567891);
		strcat(temp_str,bbb);

		//IntToStr(bbb," ");
		strcat(temp_str," ");


		temp=getTime.year-2000;
		IntToStr_rait(bbb,temp,1);
		strcat(temp_str,bbb);

		//IntToStr(bbb,0);
		//strcat(temp_str,bbb);




		strcat(temp_str,"/");

		IntToStr_rait(bbb,getTime.month,1);
		strcat(temp_str,bbb);


		strcat(temp_str,"/");

		IntToStr_rait(bbb,getTime.day,1);
		strcat(temp_str,bbb);


		OLED_DrawTime(0,56,temp_str,8);
		//OLED_DrawString_fast(0,56,temp_str,7);
		//OLED_DrawTime(0,56,bbb,8);


		//sprintf (bbb,"%002i/%002i/%002i" ,2000-getTime.year,getTime.month,getTime.day);
		//OLED_DrawTime(59,47,bbb,8);
		//ClrByf(bbb,8);
		//getTime0=getTime;




}
*/


/*
void CapasitiAk(char akProcent)
{
	char i;
	OLED_DrawLine_fast(104,55,104,63,1);
	OLED_DrawLine_fast(104,55,120,55,1);
	OLED_DrawLine_fast(104,63,120,63,1);
	OLED_DrawLine_fast(120,55,120,63,1);

	//OLED_DrawLine_fast(120,57,120,61,1);
	OLED_DrawLine_fast(120,57,124,57,1);
	OLED_DrawLine_fast(120,61,124,61,1);
	OLED_DrawLine_fast(124,57,124,61,1);

	for(i=1;i<16;i++)
	{
		if (i<akProcent) OLED_DrawLine_fast(104+i,56,104+i,62,1);
		if (i>=akProcent) OLED_DrawLine_fast(104+i,56,104+i,62,0);
	}

}
*/

//char strDisp[]="000000000000000000000000000000\0";
//char a[]="1111\0";
//char b[]="2222\0";
RCC_ClocksTypeDef Frequency;




typedef unsigned char  bool;
bool WkupFlag=0;
unsigned char WainToAlarm = 0;


//ftime_t CurentTimeAndData, BackTimeAndData;
u8 t;
int f;

//RTC_DateTypeDef RTCDateStr;
//RTC_TimeTypeDef RTCTimeStr;


int main(void)
{


	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);//!!!!

	// тактирование ядра низкоскоростным внутренним генератором 4мгц
	RCC->ICSCR &= ~RCC_ICSCR_MSIRANGE;//!!!!
	RCC->ICSCR |= RCC_ICSCR_MSIRANGE_6;//!!!!
	//RTC_Initilithahion();//!!!!
	////////////////////////////////////////////////////////////////////

	delay_ms(1000);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



	curent_cmd = 0;
	PCount_Start = 0;
	/////////////////// настройка пробуждения
		RCC_HSICmd(DISABLE);
		PWR_PVDCmd(DISABLE);
		PWR_UltraLowPowerCmd(ENABLE);
		PWR_WakeUpPinCmd(PWR_WakeUpPin_1,ENABLE);
		PWR_UltraLowPowerCmd(ENABLE);

	////////////// настройка прирывания для будильника в неспящем режиме
		EXTI_InitTypeDef exti;
		NVIC_InitTypeDef NVIC_InitStructure;

		EXTI_ClearITPendingBit(EXTI_Line17);
		exti.EXTI_Line = EXTI_Line17;
		exti.EXTI_Mode = EXTI_Mode_Interrupt;
		exti.EXTI_Trigger = EXTI_Trigger_Rising;
		exti.EXTI_LineCmd = ENABLE;
		EXTI_Init(&exti);

		NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		//////////////////




	if(PWR_GetFlagStatus(PWR_FLAG_SB)!=RESET)//Если МК вышел из режима standby
	{
		WkupFlag=1;
		curent_cmd = 1;


		if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)!=RESET)//Если МК вышел из режима ALARM
		{
							//RTC_Initilithahion();//!!!!
						//PWR_RTCAccessCmd(ENABLE);
						//RTC_ClearFlag(RTC_FLAG_ALRAF);
						//PWR_RTCAccessCmd(DISABLE);
							//f_WakeupToAlarm=1;
						curent_cmd = 5;
		}




		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		PWR_ClearFlag(PWR_FLAG_SB); //Сброс флага, который указывает на то, что МК вышел из режима standby


		PWR_RTCAccessCmd(ENABLE); // Доступ в RTC
		//RTC_ClearITPendingBit(RTC_IT_WUT);
		//RTC_ClearFlag(RTC_FLAG_WUTF);
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		PWR_RTCAccessCmd(DISABLE);





	}
	else
	{


		WkupFlag=0;

		RTC_TimeTypeDef alarmTime;
		alarmTime.RTC_H12 = RTC_H12_PM;//!!!!;
		alarmTime.RTC_Hours = 20;
		alarmTime.RTC_Minutes = 0;
		alarmTime.RTC_Seconds = 20;


		RTC_Initilithahion();//!!!!
		//RTC_Set_Alarm(alarmTime);/// Магическая последовательность, 1. RTC_Set_Alarm, 2. RTC_Set_Time иначе не работает будильник


		RTC_TimeTypeDef RTC_Time;
		RTC_DateTypeDef RTC_Date;

		RTC_Time.RTC_H12 = RTC_H12_PM;//!!!!;
		RTC_Time.RTC_Hours = 20;
		RTC_Time.RTC_Minutes = 00;
		RTC_Time.RTC_Seconds = 00;


		RTC_Date.RTC_WeekDay = RTC_Weekday_Sunday;

		RTC_Date.RTC_Year = 14;
		RTC_Date.RTC_Month = 8;
		RTC_Date.RTC_Date = 17;

		RTC_Set_TimeDate(RTC_Time, RTC_Date);///


	}





	//else curent_cmd = 0;//f_WakeupToAlarm=0;











	//RTC_Initilithahion();//!!!!
	//RTC_Set_Time();






OLED_Init();
OLED_Clear();
char i,j;






/*
RCC_GetClocksFreq(&Frequency);
f=Frequency.SYSCLK_Frequency/1000;
IntToStr(a,f);
OLED_DrawString_fast(30,30,a,10);
*/


OLED_LcdCache_Clear();



//if(0==WkupFlag) //Если МК до этого не находился в режиме STANDBY!!!!!!!!!!!!!!!!!
//PWR_EnterSTANDBYMode(); //Перейди в режим STANDBY!!!!!!!!!!!!!!!!!!!


//OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);
//delay_ms(10000);


WaitToSleep = 0;



TACT_Config();

BackTimeAndData.hour=0;
BackTimeAndData.minute=0;
BackTimeAndData.second=0;

globa_menu = 1;
f_aktive_menu = 0;
//curent_cmd = 0;

PWatch_Setup_Step = 0;

key_up = 0;
key_down = 0;
key_ok = 0;

PCount_Start = 0;

while(1)
{

	key = 0;

	key_ok = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	key_down = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
	key_up = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);


/*
	OLED_DrawString_fast(0,0,"            ",10);

	IntToStr(bbb,key_ok);
	OLED_DrawString_fast(0,0,bbb,10);

	IntToStr(bbb,key_down);
	OLED_DrawString_fast(30,0,bbb,10);

	IntToStr(bbb,key_up);
	OLED_DrawString_fast(60,0,bbb,10);
*/

	if (key_ok!=0)
	{
		key_ok = 0;
		key = 1;



		//OLED_DrawString_fast(0,0,"000000000",10);
		//f_aktive_menu = 1;


		/*
		if (f_aktive_menu == 1)
		{
			//f_work_comand = 1;
			f_aktive_menu = 0;
			f_clearScrin = 1;
			curent_cmd = menu_cmd[globa_menu-1];

		}
		else curent_cmd = 255 ;
		*/
	}


	if (key_down==0)
	{
		key_down = 0;
		key = 2;

		/*
		if ((globa_menu>1)&&(f_work_comand == 0)) globa_menu--;
		else globa_menu = 1;

		//IntToStr(bbb,globa_menu);
		//OLED_DrawString_fast(0,0,bbb,10);

		 */

	}


	if (key_up==0)
	{

		key_up = 0;
		key = 12;

		/*
		if ((globa_menu<4)&&(f_work_comand == 0)) globa_menu++;
		else globa_menu = 4;

		//IntToStr(bbb,globa_menu);
		//OLED_DrawString_fast(0,0,bbb,10);

		 */

	}


	if (key!=0)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			delay_ms(30);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}



/*
	if (f_clearScrin==1)
	{
		OLED_Clear();
		f_clearScrin=0;
	}
*/


	//if (f_aktive_menu==0)
	//{

	switch(curent_cmd)
	{
		case 0:			//нет команды

				PWatch_Menu();

				break;
		case 1:			//Программа показа часов (главная страница)

				//OLED_Clear();
				//f_work_comand = 1;
				PWatch_Anim();
				break;
		case 2:			//Программа настройки даты/времени и др.

				//OLED_Clear();
				OLED_DrawString_fast(0,0,"Календарь",10);
				curent_cmd = 0;
				break;

		case 3:

				//OLED_Clear();
				PWatch_Set_Alarm();
				break;

		case 4:

				//OLED_Clear();
				//OLED_DrawString_fast(0,0,"Настройки",10);

				//f_work_comand = 1;
				PWatch_Setup();

				break;

		case 5:

				PWatch_Vibration();

				break;


		case 254:

				OLED_Clear();

				break;

		/*case 255:

				OLED_Clear();
				OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);
				f_aktive_menu = 1;
				//f_work_comand = 0;
				//key=0;
				break;
		*/
		default: 		//выполнить, если ни один вариант не подошел

				break;
	}
	//}
	//else
	//{
		//PWatch_Menu();
	//}

/*
				if(WainToAlarm == 1)
				{
					OLED_DrawString_fast(0,0,"alarm1",10);
					WainToAlarm =0;
				}
*/


				//if(WaitToSleep>=5)
				//{
					//WaitToSleep=0;


					//PWR_ClearFlag(PWR_FLAG_SB); //Сброс флага, который указывает на то, что МК вышел из режима standby


/*
					for (i=255; i>0; i--)
					{
						OLED_SetContrast(i);
						IntToStr(a,i);
						OLED_DrawString_fast(0,0,a,10);
						//delay_ms(500);
					}*/




					//OLED_DrawBitmap_fast(32,10,Fallout_Bitmaps,58,60);
					//delay_ms(1000);

					//OLED_Display_Off();!!!!!!!!!!!!!!!!!!!!!!!!1

					//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, DISABLE);
					//походу стабилитрон кушает 0.1 мА, общее потребление 0.15мА


					//PWR_EnterSTANDBYMode();!!!!!!!!!!!!!!!!!!!!!



				//}


				//WaitToSleep++;

/*
				RCC_GetClocksFreq(&Frequency);
				f=Frequency.SYSCLK_Frequency/1000;
				IntToStr(a,f);
				OLED_DrawString_fast(0,0,a,10);
*/






	delay_ms(250);



}

}



/*
void ShowWatch(ftime_t Time)
{

		char temp_str[16];
		unsigned int temp=0;
		//char ch[]=":";
		//OLED_DrawTimeAnim(10,23,getTime,getTime0,25);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//sprintf (bbb,"%002i:%002i:%002i %002i/%002i/%002i",getTime.hour,getTime.minute,getTime.second,2000-getTime.year,getTime.month,getTime.day);
		//sprintf (bbb,"%002i:%002i:%002i" ,getTime.hour,getTime.minute,getTime.second);

		//ClrByf(bbb,16);
		ClrByf(temp_str,16);

		IntToStr_rait(bbb,Time.hour,1);
		strcat(temp_str,bbb);

		strcat(temp_str,":");

		IntToStr_rait(bbb,Time.minute,1);
		strcat(temp_str,bbb);

		strcat(temp_str,":");

		IntToStr_rait(bbb,Time.second,1);
		//IntToStr(bbb,1234567891);
		strcat(temp_str,bbb);

		//IntToStr(bbb," ");
		strcat(temp_str," ");


		temp=Time.year-2000;
		IntToStr_rait(bbb,temp,1);
		strcat(temp_str,bbb);

		//IntToStr(bbb,0);
		//strcat(temp_str,bbb);




		strcat(temp_str,"/");

		IntToStr_rait(bbb,Time.month,1);
		strcat(temp_str,bbb);


		strcat(temp_str,"/");

		IntToStr_rait(bbb,Time.day,1);
		strcat(temp_str,bbb);


		OLED_DrawTime(0,56,temp_str,8);
		//OLED_DrawString_fast(0,56,temp_str,7);
		//OLED_DrawTime(0,56,bbb,8);


		//sprintf (bbb,"%002i/%002i/%002i" ,2000-getTime.year,getTime.month,getTime.day);
		//OLED_DrawTime(59,47,bbb,8);
		//ClrByf(bbb,8);
		//getTime0=getTime;




}
*/


/*

void PWatch_Anim(void)
{

	RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
	//IntToStr(a,RTCTimeStr.RTC_Hours);
	CurentTimeAndData.hour = RTCTimeStr.RTC_Hours;
	CurentTimeAndData.minute = RTCTimeStr.RTC_Minutes;
	CurentTimeAndData.second = RTCTimeStr.RTC_Seconds;



	//ShowWatch(CurentTimeAndData);



	if ((CurentTimeAndData.second>BackTimeAndData.second)||(BackTimeAndData.second>58))
	{

	OLED_DrawTimeAnim(10,23,CurentTimeAndData,BackTimeAndData,25);
	BackTimeAndData.hour=CurentTimeAndData.hour;
	BackTimeAndData.minute=CurentTimeAndData.minute;
	BackTimeAndData.second=CurentTimeAndData.second;
	}

}

void PWatch_Menu(void)
{
	if(key!=0)
			{
				//key=0;

			IntToStr(bbb,globa_menu-1);
			OLED_DrawString_fast(0,0,"      ",10);
			OLED_DrawString_fast(0,0,bbb,10);



			OLED_DrawString_fast(0,54,menu[globa_menu-1],10);

			//curent_cmd = menu[globa_menu-1];

			OLED_DrawLine_fast(0,10,128,10,0);
			OLED_DrawLine_fast(32*globa_menu-32,10,32*globa_menu,10,1);

			//if (f_work_comand==1)
			//{
			//	f_aktive_menu = 0;
			//}


			}
}

void PWatch_Setup(void)
{
	switch(curent_cmd)
		{
			case 0:			//нет команды

					break;
			case 1:			//Программа показа часов (главная страница)

					//OLED_Clear();
					PWatch_Anim();
					break;
			case 2:			//Программа настройки даты/времени и др.

					//OLED_Clear();
					OLED_DrawString_fast(0,0,"Календарь",10);
					break;

			case 3:

					//OLED_Clear();
					OLED_DrawString_fast(0,0,"Будильник",10);
					break;

			case 4:

					//OLED_Clear();
					OLED_DrawString_fast(0,0,"Настройки",10);
					break;


			case 254:

					OLED_Clear();

					break;

			case 255:

					OLED_Clear();
					OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);
					f_aktive_menu = 1;
					f_work_comand = 0;
					//key=0;
					break;
			default: 		//выполнить, если ни один вариант не подошел

					break;
		}


}
*/
/*
void PWatch_Menu(void)
{

	if(PCount_Start == 0)
	{
		OLED_Clear();
		OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);

		//f_aktive_menu = 1;
		PCount_Start ++;
	}




	if (key == 1)
		{

			//OLED_DrawString_fast(0,0,"000000000",10);
			//f_aktive_menu = 1;




				//f_work_comand = 1;
				//f_aktive_menu = 0;
				//f_clearScrin = 1;
				curent_cmd = menu_cmd[globa_menu-1];
				PCount_Start = 0;
		}


		if (key == 2)
		{


			if (globa_menu>1) globa_menu--;
			else globa_menu = 1;

			//IntToStr(bbb,globa_menu);
			//OLED_DrawString_fast(0,0,bbb,10);



		}


		if (key == 12)
		{


			if (globa_menu<4) globa_menu++;
			else globa_menu = 4;

			//IntToStr(bbb,globa_menu);
			//OLED_DrawString_fast(0,0,bbb,10);

		}



	if(key!=0)
			{
				//key=0;

			IntToStr(bbb,globa_menu-1);
			OLED_DrawString_fast(0,0,"      ",10);
			OLED_DrawString_fast(0,0,bbb,10);



			OLED_DrawString_fast(0,54,menu[globa_menu-1],10);

			//curent_cmd = menu[globa_menu-1];

			OLED_DrawLine_fast(0,10,128,10,0);
			OLED_DrawLine_fast(32*globa_menu-32,10,32*globa_menu,10,1);

			//if (f_work_comand==1)
			//{
			//	f_aktive_menu = 0;
			//}


			}

	//f_work_comand = 0;


}*/

