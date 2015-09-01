#include "stm32l1xx_rtc.h"
#include "icon.h"

RTC_TimeTypeDef RTCTimeStr;
RTC_DateTypeDef RTCDateStr;

ftime_t CurentTimeAndData, BackTimeAndData;
volatile unsigned char key = 0;

char bbb[]="0000000000000000";


char temp_str[16];
unsigned int temp=0;


/////////////////
//название меню
const char menu[5][16] = {"Часы     \0","Календарь\0","Будильник\0","Настройки\0"};
//команды к каждому пункту
const unsigned char menu_cmd[5] = {1,2,3,4};
unsigned char globa_menu=0; ////глобальное меню 0...и дальше
////////////////

unsigned char PWatch_Setup_Step = 0;

volatile unsigned char PCount_Start = 0;

unsigned char f_work_comand=0;

volatile unsigned char curent_cmd = 0;

char WaitToSleep = 0;

unsigned char f_WakeupToAlarm = 0;


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

			//IntToStr(bbb,globa_menu-1);
			//OLED_DrawString_fast(0,0,"      ",10);
			//OLED_DrawString_fast(0,0,bbb,10);



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


}


void DataToString(ftime_t CurentTimeAndData)
{
	ClrByf(temp_str,16);

	temp=CurentTimeAndData.year;
	IntToStr_rait(bbb,temp,1);
	strcat(temp_str,bbb);

	strcat(temp_str,"/");

	IntToStr_rait(bbb,CurentTimeAndData.month,1);
	strcat(temp_str,bbb);

	strcat(temp_str,"/");

	IntToStr_rait(bbb,CurentTimeAndData.day,1);
	strcat(temp_str,bbb);
}

void PWatch_Anim(void)
{
	//IntToStr(bbb,PCount_Start);
	//OLED_DrawString_fast(0,0,"      ",10);
	//OLED_DrawString_fast(0,0,bbb,10);

	if(PCount_Start == 0)
		{
			OLED_Clear();
			//OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);

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
				curent_cmd = 0;
				PCount_Start = 0;
			}


	RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
	RTC_GetDate(RTC_Format_BIN, &RTCDateStr);

	//IntToStr(a,RTCTimeStr.RTC_Hours);
	CurentTimeAndData.hour = RTCTimeStr.RTC_Hours;
	CurentTimeAndData.minute = RTCTimeStr.RTC_Minutes;
	CurentTimeAndData.second = RTCTimeStr.RTC_Seconds;


	CurentTimeAndData.year =  RTCDateStr.RTC_Year;
	CurentTimeAndData.month =  RTCDateStr.RTC_Month;
	CurentTimeAndData.day =  RTCDateStr.RTC_Date;


////////


	DataToString(CurentTimeAndData);


	OLED_DrawTime(88,0,temp_str,8);

////////



	//if ((CurentTimeAndData.second>BackTimeAndData.second)||(BackTimeAndData.second>58))
	if (CurentTimeAndData.second != BackTimeAndData.second)
	{

	OLED_DrawTimeAnim(10,23,CurentTimeAndData,BackTimeAndData,25);
	BackTimeAndData.hour=CurentTimeAndData.hour;
	BackTimeAndData.minute=CurentTimeAndData.minute;
	BackTimeAndData.second=CurentTimeAndData.second;


	if(WaitToSleep >= 8)
	{
		PWR_EnterSTANDBYMode();
		WaitToSleep = 0;
	}

	WaitToSleep++;
	}

	//f_work_comand = 0;

	//if (key == 1) curent_cmd = 0;
}



void PWatch_Setup(void)
{
	if(PCount_Start == 0)
			{
				OLED_Clear();
				//OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);

				//f_aktive_menu = 1;

				RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
				RTC_GetDate(RTC_Format_BIN, &RTCDateStr);


				//IntToStr(a,RTCTimeStr.RTC_Hours);
				CurentTimeAndData.hour = RTCTimeStr.RTC_Hours;
				CurentTimeAndData.minute = RTCTimeStr.RTC_Minutes;
				CurentTimeAndData.second = RTCTimeStr.RTC_Seconds;

				CurentTimeAndData.year = RTCDateStr.RTC_Year;
				CurentTimeAndData.month = RTCDateStr.RTC_Month;
				CurentTimeAndData.day = RTCDateStr.RTC_Date;

				PCount_Start ++;

			}







	if((key==1)&&(PWatch_Setup_Step < 6))
	{
		PWatch_Setup_Step++;


		//OLED_DrawLine_fast(0,0,128,0,0);
		//OLED_DrawLine_fast(32*PWatch_Setup_Step-32,0,32*PWatch_Setup_Step,0,1);


	}

	//OLED_DrawStringXY_fast(0,0,"Наст. часов",10);
	//IntToStr(bbb,PWatch_Setup_Step);
	//OLED_DrawString_fast(0,0,bbb,10);







	switch(PWatch_Setup_Step)
		{
			case 0:			//нет команды
					//ShowWatch(CurentTimeAndData);
					if ((key==12)&&(CurentTimeAndData.hour < 24)) CurentTimeAndData.hour ++;
					if ((key==2)&&(CurentTimeAndData.hour > 0)) CurentTimeAndData.hour --;


					OLED_DrawLine_fast(0,10,128,10,0);
					OLED_DrawLine_fast(6,10,38,10,1);

					break;
			case 1:			//часы

					if ((key==12)&&(CurentTimeAndData.minute < 59)) CurentTimeAndData.minute ++;
					if ((key==2)&&(CurentTimeAndData.minute > 0)) CurentTimeAndData.minute --;

					OLED_DrawLine_fast(0,10,128,10,0);
					OLED_DrawLine_fast(50,10,82,10,1);

					break;
			case 2:			//минуты

					if ((key==12)&&(CurentTimeAndData.second < 59)) CurentTimeAndData.second ++;
					if ((key==2)&&(CurentTimeAndData.second > 0)) CurentTimeAndData.second --;

					OLED_DrawLine_fast(0,10,128,10,0);
					OLED_DrawLine_fast(96,10,128,10,1);

					break;


			case 3:			//
					//PCount_Start = 0;

					if ((key==12)&&(CurentTimeAndData.year < 99)) CurentTimeAndData.year ++;
					if ((key==2)&&(CurentTimeAndData.year > 0)) CurentTimeAndData.year --;

					OLED_DrawLine_fast(0,10,128,10,0);
					OLED_DrawLine_fast(6,10,38,10,1);

					break;

			case 4:			//

					if ((key==12)&&(CurentTimeAndData.month < 12)) CurentTimeAndData.month ++;
					if ((key==2)&&(CurentTimeAndData.month > 0)) CurentTimeAndData.month --;

					OLED_DrawLine_fast(0,10,128,10,0);
					OLED_DrawLine_fast(50,10,82,10,1);

					break;

			case 5:			//

					if ((key==12)&&(CurentTimeAndData.day < 31)) CurentTimeAndData.day ++;
					if ((key==2)&&(CurentTimeAndData.day > 0)) CurentTimeAndData.day --;

					OLED_DrawLine_fast(0,10,128,10,0);
					OLED_DrawLine_fast(96,10,128,10,1);

					break;

			case 6:

					//f_work_comand = 0;
					//PWatch_Setup_Step = 0;
					PCount_Start = 0;
					curent_cmd = 0;
					PWatch_Setup_Step = 0;

					RTC_TimeTypeDef RTC_Time;
					RTC_DateTypeDef RTC_Date;

					RTC_Time.RTC_H12 = RTC_H12_PM;//!!!!;
					RTC_Time.RTC_Hours = CurentTimeAndData.hour;
					RTC_Time.RTC_Minutes =  CurentTimeAndData.minute;
					RTC_Time.RTC_Seconds = CurentTimeAndData.second;

					RTC_Date.RTC_Year = CurentTimeAndData.year;
					RTC_Date.RTC_Month = CurentTimeAndData.month;
					RTC_Date.RTC_Date = CurentTimeAndData.day;

					//RTC_Set_Time(RTC_Time);
					RTC_Set_TimeDate(RTC_Time, RTC_Date);

					//OLED_Clear();
					//OLED_DrawString_fast(0,0,"Настройки",10);

					break;


			default: 		//выполнить, если ни один вариант не подошел

					break;
		}






	if (PWatch_Setup_Step < 3)
	{

					//OLED_DrawString_fast(0,54,"                 ",7);
					OLED_DrawString_fast(0,54,"Настройка времени",7);
					OLED_DrawTimeAnim(10,23,CurentTimeAndData,BackTimeAndData,25);
					BackTimeAndData.hour=CurentTimeAndData.hour;
					BackTimeAndData.minute=CurentTimeAndData.minute;
					BackTimeAndData.second=CurentTimeAndData.second;

					//OLED_DrawTime(BackTimeAndData.);
	}else
		if(PWatch_Setup_Step >= 3)

		{
					//OLED_DrawString_fast(0,54,"                 ",7);
					OLED_DrawString_fast(0,54,"Настройка даты   ",7);
					DataToString(CurentTimeAndData);
					OLED_DrawTime(10,23,temp_str,25);

		}

}


void PWatch_Set_Alarm(void)
{
	if(PCount_Start == 0)
				{
					OLED_Clear();
					//OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);

					//f_aktive_menu = 1;

					RTC_GetTime(RTC_Format_BIN, &RTCTimeStr);
					RTC_GetDate(RTC_Format_BIN, &RTCDateStr);


					//IntToStr(a,RTCTimeStr.RTC_Hours);
					CurentTimeAndData.hour = RTCTimeStr.RTC_Hours;
					CurentTimeAndData.minute = RTCTimeStr.RTC_Minutes;
					CurentTimeAndData.second = RTCTimeStr.RTC_Seconds;

					CurentTimeAndData.year = RTCDateStr.RTC_Year;
					CurentTimeAndData.month = RTCDateStr.RTC_Month;
					CurentTimeAndData.day = RTCDateStr.RTC_Date;

					PCount_Start ++;

				}







		if((key==1)&&(PWatch_Setup_Step < 6))
		{
			PWatch_Setup_Step++;


			//OLED_DrawLine_fast(0,0,128,0,0);
			//OLED_DrawLine_fast(32*PWatch_Setup_Step-32,0,32*PWatch_Setup_Step,0,1);


		}

		//OLED_DrawStringXY_fast(0,0,"Наст. часов",10);
		//IntToStr(bbb,PWatch_Setup_Step);
		//OLED_DrawString_fast(0,0,bbb,10);







		switch(PWatch_Setup_Step)
			{
				case 0:			//нет команды
						//ShowWatch(CurentTimeAndData);
						if ((key==12)&&(CurentTimeAndData.hour < 24)) CurentTimeAndData.hour ++;
						if ((key==2)&&(CurentTimeAndData.hour > 0)) CurentTimeAndData.hour --;


						OLED_DrawLine_fast(0,10,128,10,0);
						OLED_DrawLine_fast(6,10,38,10,1);

						break;
				case 1:			//часы

						if ((key==12)&&(CurentTimeAndData.minute < 59)) CurentTimeAndData.minute ++;
						if ((key==2)&&(CurentTimeAndData.minute > 0)) CurentTimeAndData.minute --;

						OLED_DrawLine_fast(0,10,128,10,0);
						OLED_DrawLine_fast(50,10,82,10,1);

						break;
				case 2:			//минуты

						if ((key==12)&&(CurentTimeAndData.second < 59)) CurentTimeAndData.second ++;
						if ((key==2)&&(CurentTimeAndData.second > 0)) CurentTimeAndData.second --;

						OLED_DrawLine_fast(0,10,128,10,0);
						OLED_DrawLine_fast(96,10,128,10,1);

						break;


				case 3:

						//f_work_comand = 0;
						//PWatch_Setup_Step = 0;
						PCount_Start = 0;
						curent_cmd = 0;
						PWatch_Setup_Step = 0;



						RTC_TimeTypeDef alarmTime;

						alarmTime.RTC_H12 = RTC_HourFormat_24;
						alarmTime.RTC_Hours = CurentTimeAndData.hour;
						alarmTime.RTC_Minutes = CurentTimeAndData.minute;
						alarmTime.RTC_Seconds = CurentTimeAndData.second;

						RTC_Set_Alarm(alarmTime);



						break;


				default: 		//выполнить, если ни один вариант не подошел

						break;
			}






		if (PWatch_Setup_Step < 3)
		{

						//OLED_DrawString_fast(0,54,"                 ",7);
						OLED_DrawString_fast(0,54,"Будильник        ",7);
						OLED_DrawTimeAnim(10,23,CurentTimeAndData,BackTimeAndData,25);
						BackTimeAndData.hour=CurentTimeAndData.hour;
						BackTimeAndData.minute=CurentTimeAndData.minute;
						BackTimeAndData.second=CurentTimeAndData.second;

						//OLED_DrawTime(BackTimeAndData.);
		}

}

void PWatch_Vibration(void)
{
	if(PCount_Start == 0)
	{
		OLED_Clear();
		//OLED_DrawBitmap_fast(3,12,menu1_sprait,123,40);

		OLED_DrawString_fast(10,38,"Это спарта!",10);

		//f_aktive_menu = 1;
		//PCount_Start ++;
	}


	PCount_Start ++;




	if ((key == 1)||(PCount_Start> 30))
	{
		curent_cmd = 1;
		PCount_Start = 0;
	}

	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	delay_ms(100);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	delay_ms(200);


}
