#include "stm32l1xx_rtc.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_exti.h"

#define JD0 2451911 // дней до 01 янв 2001 ПН

typedef struct{
	unsigned  int year;
	unsigned  char month;
	unsigned  char day;
	unsigned  char hour;
	unsigned  char minute;
	unsigned  char second;
} ftime_t;


ftime_t setTime;          //структура для работы с типом "время"
unsigned  int tmp;           //вспомогательная переменная
ftime_t getTime;
ftime_t getTime0;


void RTC_Initilithahion(void)
{
	uint32_t RTC_Form;


	/* Allow access to the RTC */
	 PWR_RTCAccessCmd(ENABLE);

	 /* Reset RTC Backup Domain */
	 RCC_RTCResetCmd(ENABLE);
	 RCC_RTCResetCmd(DISABLE);

	 /* LSE Enable */
	 RCC_LSEConfig(RCC_LSE_ON);

	 /* Wait until LSE is ready */
	 while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

	  /* RTC Clock Source Selection */
	 RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	 /* Enable the RTC */
	 RCC_RTCCLKCmd(ENABLE);



	 /*
//
	 RTC_TimeTypeDef RTC_Time;
	 RTC_DateTypeDef RTC_Date;

	 //PWR_RTCAccessCmd(ENABLE);

	 RTC_Time.RTC_H12 = RTC_H12_PM;//!!!!;
	 RTC_Time.RTC_Hours = 20;
	 RTC_Time.RTC_Minutes = 00;
	 RTC_Time.RTC_Seconds = 00;
	 RTC_SetTime(RTC_Format_BIN, &RTC_Time);

	 //PWR_RTCAccessCmd(DISABLE);

	 RTC_TimeTypeDef alarmTime;
	 RTC_AlarmTypeDef alarm;

	 	 //PWR_RTCAccessCmd(ENABLE);
	 	//Будильник
	 	 alarmTime.RTC_H12 = RTC_H12_PM;//!!!!;
	 	 alarmTime.RTC_Hours = 20;
	 	 alarmTime.RTC_Minutes = 0;
	 	 alarmTime.RTC_Seconds = 20;

	 	 alarm.RTC_AlarmTime = alarmTime;
	 	 alarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;


	 	 RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &alarm);
	 	 RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	 	 RTC_AlarmCmd(RTC_Alarm_A,ENABLE);



*/

	 PWR_RTCAccessCmd(DISABLE);
	 return 0;
}

void RTC_Set_TimeDate(RTC_TimeTypeDef RTC_Time, RTC_DateTypeDef RTC_Date)
{
	 	 //RTC_TimeTypeDef RTC_Time;
		 //RTC_DateTypeDef RTC_Date;




		 PWR_RTCAccessCmd(ENABLE);
		 //RTC_Time.RTC_H12 = RTC_H12_PM;//!!!!;
		 //RTC_Time.RTC_Hours = 20;
		 //RTC_Time.RTC_Minutes = 00;
		 //RTC_Time.RTC_Seconds = 00;
		 RTC_SetTime(RTC_Format_BIN, &RTC_Time);
		 RTC_SetDate(RTC_Format_BIN, &RTC_Date);

		 PWR_RTCAccessCmd(DISABLE);


		 return 0;
}





int  RTC_Set_Alarm(RTC_TimeTypeDef alarmTime)
{

		 RTC_AlarmTypeDef alarm;

		 RTC_TimeTypeDef RTCTime;
		 RTC_DateTypeDef RTCDate;


/*
		 	 	 /////////////////// настройка пробуждения
		 		RCC_HSICmd(DISABLE);
		 		PWR_PVDCmd(DISABLE);
		 		PWR_UltraLowPowerCmd(ENABLE);
		 		PWR_WakeUpPinCmd(PWR_WakeUpPin_1,ENABLE);
		 		PWR_UltraLowPowerCmd(ENABLE);
*/
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



		 		PWR_RTCAccessCmd(ENABLE);//!!!!!!!!!!!!!!!!!!!!!!
		 		RTC_AlarmCmd(RTC_Alarm_A,DISABLE);


		 	 alarm.RTC_AlarmTime = alarmTime;
		 	 alarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;




		 	 RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &alarm);
		 	 RTC_OutputConfig(RTC_Output_AlarmA, RTC_OutputPolarity_High);
		 	 RTC_ITConfig(RTC_IT_ALRA,ENABLE);
		 	 RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
		 	 RTC_ClearFlag(RTC_FLAG_ALRAF);

		 	PWR_RTCAccessCmd(DISABLE);

		 	return 0;
}
