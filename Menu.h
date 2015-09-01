//int i,j;

void ShowCurentItemMenu(int t_curent, int t_globa_menu);
void Key_Down();
void Key_Up();
void Key_OK();

//макс размер меню, сдвиг по меню
const unsigned char size_menu = 5;
//Текст меню
/*																										Настнойка 													Программы 		*/
const char menu[20][16] = {"1_Выйти","2_Настнойка","Программы","",""		,"","","","",""	,		"Назад","Подсветка","Инверсия Lcd","Заряд","Время/дата",		"Назад","Дамп БТ","АЦП","Температура","СМС"};
//команды к каждому пункту
const unsigned char menu_cmd[20] = {11,0,0,0,0								,0,0,0,0,0				,0,21,22,23,24												,0,31,32,33,34};
//сколько выводить пунктов в подменю
const char cout_global_menu[4]={	3, 													0, 						5, 												5};

int curent=0;
unsigned char globa_menu=0; ////глобальное меню 0...и дальше
unsigned char  f_aktive_menu=0;
unsigned int f_work_comand=0;


void ShowCurentItemMenu(int t_curent, int t_globa_menu)
{
	//unsigned char i,j,k,n,c;
	unsigned int i,j,k,n,c;


	LcdClear();
	j=0;
	i=0;
	k = size_menu*globa_menu;
	n = size_menu*globa_menu+cout_global_menu[t_globa_menu];
	c = cout_global_menu[t_globa_menu];

	for(i = k; i < n; i++)
	{
		if (j> c) j=0;

		if(i==t_curent)
		{
			//ClrLcdString(2,3+i);
			//sprintf (lcd_buf, "%s %i", menu[j+5*t_globa_menu],i);
			sprintf (lcd_buf, "%s", menu[i]);
			LcdStringInv(2,3+j);
		}
		else
		{
			//ClrLcdString(2,3+i);
			//sprintf (lcd_buf, "%s %i", menu[j+5*t_globa_menu],i);
			sprintf (lcd_buf, "%s", menu[i]);
			LcdString(2,3+j);
		}

		j++;


	}

	sprintf (lcd_buf, "c:%i g:%i l:%i", t_curent, t_globa_menu,  cout_global_menu[t_globa_menu]);
	LcdString(1,8);
	LcdUpdate();
}


void Key_Down()
{
if(f_work_comand>0)
{
	Key_OK();
}else
if (f_aktive_menu!=0)
{
		if (curent < size_menu*globa_menu+cout_global_menu[globa_menu]-1)
	{
		curent++;
		//ShowCurentItemMenu(curent-1,globa_menu);
		ShowCurentItemMenu(curent,globa_menu);
	}/*
	else
	{
		curent=5*globa_menu;
		ShowCurentItemMenu(curent,globa_menu);
		//curent++;
	}*/

}
else
{	LcdLiht(1);
	f_aktive_menu=1;
	LcdClear();
}
}

void Key_Up()
{

	if(f_work_comand>0)
	{
		Key_OK();
	}else
	if (curent > size_menu*globa_menu)
	{
		curent--;
		ShowCurentItemMenu(curent,globa_menu);
		//curent--;
		   		   //
	}/*
	else
	{
	curent=cout_global_menu[globa_menu];
	ShowCurentItemMenu(curent-1,globa_menu);
	curent--;
	}*/
}
/*
void Key_OK()
{
unsigned char	cmd;
if (f_aktive_menu==1)
{
	if (globa_menu==0)
	{
		if (curent==0)
		{
			f_aktive_menu = 0;
			curent=0;
			globa_menu=0;
			LcdClear();

		}
		globa_menu = curent+1;
		curent = size_menu*globa_menu;
		ShowCurentItemMenu(curent,globa_menu);

	   	  /*sprintf (lcd_buf, "%i", globa_menu);
	   	  LcdString(1,7);
	   	  LcdUpdate();
	   	  */
	   	//curent++;

/*
	 }
	 else
	 {
	 if (curent == size_menu*globa_menu)
		 //if (curent == size_menu*globa_menu)
	 {
		 globa_menu=0;
		 curent=0;
		 ShowCurentItemMenu(curent,globa_menu);
		//curent++;
		 cmd = menu_cmd[curent];

		 sprintf (lcd_buf, "%i", cmd);
		 	   	  LcdString(1,7);
		 	   	  LcdUpdate();

		 switch(cmd)
		 {
		 	case 0: break;
		 	case 31:
		 			LCD_Liht_On;
		 			break;
		 	case 32:
		 			LCD_Liht_Off;
		 			break;
		 }


	 }

	   }


}
else
{
	LCD_Liht_Xor;
}
}
*/

void Key_OK()
{
unsigned char	cmd;
if (f_aktive_menu==1)
{
	cmd = menu_cmd[curent];
	//sprintf (lcd_buf, "%i", cmd);
	//LcdString(1,7);
	//LcdUpdate();


	switch(cmd)
			 {
			 	case 0:
			 			if (globa_menu!=0)
			 				{
			 				if (curent == size_menu*globa_menu)
			 						 //if (curent == size_menu*globa_menu)
			 				{
			 						 globa_menu=0;
			 						 curent=0;
			 						 ShowCurentItemMenu(curent,globa_menu);
			 				}
			 				}
			 			else
			 				{
			 						globa_menu = curent+1;
			 						curent = size_menu*globa_menu;
			 						ShowCurentItemMenu(curent,globa_menu);
			 				}
			 			f_work_comand=0;
			 			break;

			 	case 11:
			 			f_aktive_menu = 0;
			 			curent=0;
			 			globa_menu=0;
			 			LcdClear();
			 			//f_work_comand=1;
			 			break;
			 	case 21:
			 			//LCD_Liht_Xor;
			 			LcdLiht(2);
			 			//f_work_comand=1;
			 			break;
				case 22:
						 //LCD_Liht_Xor;
						LcdInv();
						break;


				case 23:


						break;




				case 24: //настройка времени
						//LCD_Liht_Xor;
						//LcdInv();
						SetupTime();
					//while(1){}

						//f_work_comand=1;
						break;

			 	case 32:
			 			//LCD_Liht_Off;
			 			f_work_comand=1;
			 			break;
			 }



}
else
{
	LCD_Liht_Xor;
}
}
