/*
 * dataview - Data view system module -------------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 * Модуль предназначен для отображения информации на экране
 * в виде графика. По мере срабатывания таймера отображения
 * осуществляется запуск циклических программ формирования
 * массива показаний, рисования шкалы и графика объекта отображения.
 * Объект отображения выбирается предварительно из списка объектов.
 * В настоящее время график рисуется для показаний
 * скорости ветра, давления, температуры воздуха, мощности ВЭУ
 * (активной и реактивной). Для отображения формируется двумерный массив
 * из 50 значений по 5 объектам отображения. Заполнение массива
 * организовано по циклическому принципу: для обозначения вновь
 * поступившего значения используются понятия "головы"  и "хвоста".
 * В зависимости от этого отображение массива значений происходит
 * по-разному. Для каждого объекта отображения(температуры, давления
 * и т.д.) рисуется своя шкала с предельными значениями.
 *
 */
#include <stdio.h>
#include "globgrph.h"
#include "dataview.h"
#include "datainpt.h"
#include "testsyst.rh"

//------------------- П е р е м е н н ы е ------------------------------------

UINT MiscPeriodTimer;               // Промежуточное значение периода для
												// контроля за его изменением
												// Массив циклических массивов
TCycleBuffer BuffValues  [COUNTGRAPHICS];
double	    LostValues  [COUNTGRAPHICS];
double	    ValuesInstruments[COUNTINSTRUMENTS];
												// Массив структур
												// для отображения определенного
												// набора графиков для левой и правой шкалы
ConfigSignals aListParamGraphics[COUNTGRAPHICS] =
	 {
		{"Температура ", 		 			 "гр.С",       -50,  50, RGB(255,   0,   0),  0,  0, LeftScale },
		{"и барометрическое давление", "мм.pт.cт.",  700, 800, RGB(  0,   0, 255),  0,  1, RightScale},

		{"Реактивная ",			 		 "кВа",	     -250, 250, RGB(255,   0,   0),  1,  0, LeftScale },
		{"и активная мощности ВЭУ ", 	 "кВт",	     -250, 250, RGB(  0,   0, 255),  1,  1, LeftScale },
		{"и скорость ветра",		 		 "м/с",          0,  25, RGB(255, 255,   0),  1,  2, RightScale},

		{"Ход штоков №1 ",		 		 "мм",	        0, 350, RGB(  0,   0, 255),  2,  0, LeftScale },
		{"и №2, ", 			 				 "мм",	        0, 350, RGB(255,   0, 255),  2,  1, LeftScale },
		{"скорость ветра ",		 		 "м/с",          0, 350, RGB(255, 255,   0),  2,  2, LeftScale },
		{"и скорость поворота ВК", 	 "об/мин",      25,  50, RGB(  0, 255, 255),  2,  3, RightScale}
	 };

												// Массив структур
												// для отображения приборов
ConfigSignals aListParamInstruments[COUNTINSTRUMENTS] =
	 {
		{"Температура", 					 	 "гр.С", 	   -50,  50, RGB(  0,   0,   0),  0,  0, LeftScale },
		{"Барометрическое давление", 	 	 "мм.cт.",	   700, 800, RGB(  0,   0,   0),  0,  0, LeftScale },
		{"Реактивная мощность ВЭУ", 		 "кВа",	     -250, 250, RGB(255,   0,   0),  0,  0, LeftScale },
		{"Активная мощность ВЭУ", 			 "кВт",		  -250, 250, RGB(  0,   0, 255),  0,  0, LeftScale },
		{"Ход штока ВК ВЭУ №1",			 	 "мм",		     0, 350, RGB(  0,   0, 255),  0,  0, LeftScale },
		{"Ход штока ВК ВЭУ №2",  			 "мм",		     0, 350, RGB(255,   0, 255),  0,  0, LeftScale },
		{"Роза ветров",						 "      ",	     0, 360, RGB(  0,   0,   0),  0,  0, LeftScale },
		{"Ориентация головки ВЭУ", 		 "      ",	     0, 360, RGB(  0,   0,   0),  0,  0, LeftScale },
		{"Скорость ветра", 					 "м/с",	 	     0,  25, RGB(255, 255,   0),  0,  0, LeftScale },
		{"Скорость ветроколеса",			 "об/м",	    	 25,  50, RGB(  0, 255, 255),  0,  0, LeftScale }
	 };


int SelIndexGraph = 0;	      			// Делаем выбранным первый элемент в списке графиков

DEFINE_RESPONSE_TABLE1(TViewDialog,TDialog)
  EV_WM_TIMER,
  EV_WM_CLOSE,
  EV_CBN_SELCHANGE (IDC_LISTGRAPH_CB, ListGraphics),
  EV_COMMAND		 (IDC_GRIDTRG_CHB,  GridOnOff   ),
  EV_COMMAND		 (IDC_VIEWHELP,  	 ViewHelp	 ),
  EV_COMMAND		 (IDC_VIEWEXIT,  	 EndSession  ),
END_RESPONSE_TABLE;

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
TViewDialog::TViewDialog(TWindow* parent)
  : TDialog(parent, "DataViewDIALOG"),	 TWindow(parent){

  ViewDialogCreated = FALSE;
  for(char j = 0; j < COUNTGRAPHICS; j++){// Инициализация массива
	  BuffValues[j].CA.Add(0.0);
	  BuffValues[j].CAIter.Restart();
  };													// Запомним первые данные,
														// чтоб их потом НЕ ПОТЕРЯТЬ
  for(j = 0; j < COUNTGRAPHICS; j++){
	  LostValues[j] = BuffValues[j].CAIter.First();
  };
  for(j = 0; j < COUNTINSTRUMENTS; j++){
	  ValuesInstruments[j] = 0.0;
  };

  GraphicsBox_SB     = new TGraphicsBox (this, IDC_GRAPHBOX_SB  );// Окно отображения графиков
																				  // Окно отображения датчиков
  InstrumentsBox_SB 	= new TInstrumentsBox (this, IDC_INSTRUMENTBOX_SB);
  GridTrigger_CHB 	= new TCheckBox(this, IDC_GRIDTRG_CHB  );// Триггер сетки
  ListGraph_CB    	= new TComboBox(this, IDC_LISTGRAPH_CB );// Список графиков
  Message_IB	   	= new TBInfoBox(this, IDC_MESSAGE_IB   );// Статусная строка
  Help_ICBT   			= new TDrawICBT(this, IDC_VIEWHELP		);// Помощь
  Exit_ICBT   			= new TDrawICBT(this, IDC_VIEWEXIT		);// Выход
};

/*---------------------------------------------------------------------*\
 *		Нагружаем функцию 'Show()' своими функциями для заполнения       *
 *		списка графиков и отображения шкал.                              *
\*---------------------------------------------------------------------*/
void
TViewDialog::SetupWindow(){
	char AddedString[150] = "";
	int igroup = 0;
	int ielem  = 0;
	int i		  = 0;

	TWindow::SetupWindow();
													// Заполним список именами графиков
	while (aListParamGraphics[i].IndexGroupGraph == igroup){
		 while (aListParamGraphics[i].IndexElementGroup == ielem){
			 strcat(AddedString, aListParamGraphics[i].szName);
			 i++;
			 ielem++;
		 };
		 ielem = 0;
		 igroup++;
		 ListGraph_CB->AddString(AddedString);
		 strcpy(AddedString, "");
	};
	ListGraph_CB->SetSelIndex(SelIndexGraph);// Делаем выбранным текущий график в ListGraph_CB'е

	MiscPeriodTimer = pMonTimer->uTimerPeriod;// Запомним старое значение интервала
														  // таймера опроса
	GraphicsBox_SB	  ->bGridOnOff = TRUE;    // Разрешим отображение сетки
														  // Маркируем CheckButton сетки
	GridTrigger_CHB->SetCheck(GraphicsBox_SB->bGridOnOff ? BF_CHECKED : BF_UNCHECKED);
	ViewDataON();									  // Включение системы отображения
};

/*---------------------------------------------------------------------*\
 *	 	Выбор графика из ListGraph_CB'а                                      *
\*---------------------------------------------------------------------*/
void
TViewDialog::ListGraphics(){
													// Получим индекс выбранного графика в ListGraph_CB'е
	SelIndexGraph = ListGraph_CB->GetSelIndex();
													// Отображение окна графиков
	GraphicsBox_SB->ShowWindowGraphics  (SelIndexGraph);
	GraphicsBox_SB->ShowValuesGraphics  ();// Отображение данных в окне графиков
};

/*---------------------------------------------------------------------*\
 * 	Сбрасывает или взводит флаг разрешения отображения сетки         *
\*---------------------------------------------------------------------*/
void
TViewDialog::GridOnOff(){
	if (GridTrigger_CHB->GetCheck() == BF_CHECKED)
		GraphicsBox_SB->bGridOnOff = TRUE; 	// Взведем флаг наличия сетки
	else 		                     	   // При выключении сетки
		GraphicsBox_SB->bGridOnOff = FALSE; // Сбросим флаг наличия сетки
	GraphicsBox_SB->ShowValuesGraphics  ();// Отображение данных
};

/*---------------------------------------------------------------------*\
 *		Отображение сообщения в статусной строке о работоспособности     *
 *		системы или о размере свободной памяти и свободном ресурсе		  *
 *		системы                     												  *
\*---------------------------------------------------------------------*/
void
TViewDialog::ShowViewMessage(char* StMessage){
												// Сообщаем
	Message_IB->StringToShadeBox(2, 2, RGB(255, 0, 0), StMessage);
};

/*---------------------------------------------------------------------*\
 *	Включение системы отображения                                       *
\*---------------------------------------------------------------------*/
void
TViewDialog::ViewDataON(){
												  // Запустим таймер с частотой опроса
												  // системы ввода данных
	pViewTimer->SetUserTimer(this->HWindow);
	bViewDataON = TRUE;	 	    		  // Взведем флаг отображения графиков
	InstrumentsBox_SB->ShowInstruments();// Отображение приборов
												  // Отображение окна графиков
	GraphicsBox_SB		 ->ShowWindowGraphics(SelIndexGraph);
};

/*---------------------------------------------------------------------*\
 * 	Выключение системы по сообщению из модуля связи с                *
 *		контроллером или по закрытию диалога отображения информации      *
\*---------------------------------------------------------------------*/
void
TViewDialog::ViewDataOFF(){
	pViewTimer->KillUserTimer();     // Убъем таймер отображения
	bViewDataON = FALSE; 	   	   // Сбросим флаг отображения графиков
};

/*---------------------------------------------------------------------*\
 *		Заполнение массивов для отображения новой порцией данных         *
\*---------------------------------------------------------------------*/
void
TViewDialog::FillArrayData(){
	double dValue = 0.0;                // Значение промежуточной переменной
													// для усреднения данных по штокам и
													// частоте вращения ВК
// -------- Начало алгоритма заполнения циклического буффера ----------
													// Сделаем копию перед тем, как
	for(char i = 0; i < COUNTGRAPHICS; i++){// занести новые данные
		LostValues[i] = BuffValues[i].CAIter.First();
	};												// Принято очередное значение
													// в хвост массива
													// Температура
	BuffValues[0].CA.Add(adwInputData1[2]);
	BuffValues[0].CAIter.Restart();
													// Давление
	BuffValues[1].CA.Add(adwInputData1[0]);
	BuffValues[1].CAIter.Restart();
													// Мощность реактивная
	BuffValues[2].CA.Add(adwInputData1[19]);
	BuffValues[2].CAIter.Restart();
													// Мощность активная
	BuffValues[3].CA.Add(adwInputData1[17]);
	BuffValues[3].CAIter.Restart();
													// Скорость ветра
	BuffValues[4].CA.Add(adwInputData1[6]);
	BuffValues[4].CAIter.Restart();
													// Вычислим среднее значение хода
	for (i = 1; i <= 20; i++)				// штока N1
		  dValue = StepAverageValue((double)i,
							  dValue,
							  adwInputData3[i-1]);
	BuffValues[5].CA.Add(dValue);			// Ход штока график 1
	dValue = 0.0;								// Сбросим значение промежуточной переменной
	BuffValues[5].CAIter.Restart();
													// Вычислим среднее значение хода
	for (i = 1; i <= 20; i++)				// штока N2
		  dValue = StepAverageValue((double)i,
							  dValue,
							  adwInputData4[i - 1]);
	BuffValues[6].CA.Add(dValue);       // Ход штока график 2
	dValue = 0.0;                       // Сбросим значение промежуточной переменной
	BuffValues[6].CAIter.Restart();
													// Скорость ветра
	BuffValues[7].CA.Add(adwInputData1[6]);
	BuffValues[7].CAIter.Restart();
													// Вычислим среднее значение частоты
	for (i = 1; i <= 20; i++)				// вращения ВК
		  dValue = StepAverageValue((double)i,
							  dValue,
							  adwInputData2[i - 1]);
	BuffValues[8].CA.Add(dValue);			// Частота вращения ВК
	dValue = 0.0;								// Сбросим значение промежуточной переменной
	BuffValues[8].CAIter.Restart();

	// Температура
	ValuesInstruments[0] = adwInputData1[2];
	// Давление
	ValuesInstruments[1] = adwInputData1[0];
	// Мощность реактивная
	ValuesInstruments[2] = adwInputData1[19];
	// Мощность активная
	ValuesInstruments[3] = adwInputData1[17];
	// Ход штока график 1
	for (i = 1; i <= 20; i++)
		ValuesInstruments[4] = StepAverageValue((double)i,	 // Номер шага усреднения
												 ValuesInstruments[4],// Предудущее Cреднее
												 adwInputData3[i - 1]);   // Текущее значение
	// Ход штока график 2
	for (i = 1; i <= 20; i++)
		ValuesInstruments[5] = StepAverageValue((double)i,	 // Номер шага усреднения
												 ValuesInstruments[5],// Предудущее Cреднее
												 adwInputData4[i - 1]);   // Текущее значение
	// Направление ветра
	ValuesInstruments[6] = adwInputData1[9];
	// Вычисление азимута головки ВЭУ
	ValuesInstruments[7] = adwInputData1[14];
	// Скорость ветра
	ValuesInstruments[8] = adwInputData1[6];
	// Скорость поворота ветроколеса 1
	for (i = 1; i <= 20; i++)
		ValuesInstruments[9] = StepAverageValue((double)i,	 // Номер шага усреднения
												 ValuesInstruments[9],// Предудущее Cреднее
												 adwInputData2[i - 1]);   // Текущее значение
};

/*---------------------------------------------------------------------*\
 *	Запуск циклических программ                                         *
\*---------------------------------------------------------------------*/
void
TViewDialog::EvTimer(UINT IDEvent){
	Message_IB->ClearShadeBox();		// Очистка окна сообщения
	if (bInptDataON){ 					// Если система ввода данных ВКЛЮЧЕНА
		if (IDEvent == pViewTimer->IDEvTimer){	// Диагностика на изменение цикла опроса
												// При изменении периода таймера
			if (MiscPeriodTimer  != pMonTimer->uTimerPeriod){
				 MiscPeriodTimer   = pMonTimer->uTimerPeriod;
				 pViewTimer->KillUserTimer();    // Уничтожаем и перезапускаем
				 pViewTimer->SetUserTimer(this->HWindow);// таймер
			};
			FillArrayData();				// Заполнение массивов довой порцией
												// данных
												// Отображение данных на приборах
			InstrumentsBox_SB->ShowValuesInstruments();
			GraphicsBox_SB->ShowValuesGraphics ();// Отображение данных в окне графиков
		};                            // Если система ввода данных ВЫКЛЮЧЕНА
	} else {                         // выдаем сообщение в статусной строке
		ShowViewMessage("ВНИМАНИЕ! Система сбора информации НЕ ВКЛЮЧЕНА!");
	}; // if {...} else {...};
};

/*---------------------------------------------------------------------*\
 * EndSession                                                          *
\*---------------------------------------------------------------------*/
void
TViewDialog::EvClose(){
	 EndSession();
};
void
TViewDialog::EndSession(){
	 ViewDataOFF();		  	 		// Выключаем систему отображения информации
	 ViewDialogCreated = FALSE;
	 CloseWindow(IDCANCEL);
};
/*
 * ---------------------------------------------------------
 *	EoF(dataview.cpp)
 * ----------------------------------------------------------
 */

