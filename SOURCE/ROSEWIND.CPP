/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class RoseWind
	Создает модель наглядного отображения данных по направлению ветра в
	виде графика розы ветров

	Member functions:
---------------------------------------------------------------------------
		RoseWind(TPoint Center,     	 	центр розы ветров
					int 	iRadius)			 	радиус розы ветров
			(Consrtuctor)
---------------------------------------------------------------------------
		~RoseWind()
			(Destructor)
---------------------------------------------------------------------------
		DrawRoseWind(TDC& dc)          	DC окна
			(Рисует корпус, окно показания, и т.д)
---------------------------------------------------------------------------
		DrawValueRoseWind(TDC& dc,			DC окна
								int  iIndex)	индекс данного в циклическом массиве данных
			(Отображает одно значение датчика)
\*------------------------------------------------------------------------*/
#include <stdio.h>
#include "rosewind.h"
/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
RoseWind::RoseWind(TPoint Center,      // центр розы ветров
						 int   iRadius){     // радиус розы ветров
//	------------------ Размеры фигур ------------------------
	 CenterRoseWind  = Center;			   // Центр розы ветров

	 iRadiusAll      = iRadius; 			// Радиус корпуса
													// Радиус окна розы ветров
	 iRadiusRoseWind = (int)((double)iRadius / 1.1);
													// Радиус розы ветров
	 iLengthValueBox = 40;            	// Длина окна показания
	 iWidthValueBox  = 15;            	// Ширина окна показания
	 FontHeight      = 13;				  	// Высота и
	 FontWidth	     = 4;		 			// ширина шрифта
	 bOVER		     = FALSE;			  	// Флаг переполнения
													// Размеры и положение окна показания
	 RectValue.left   = CenterRoseWind.x - iLengthValueBox;
	 RectValue.top    = CenterRoseWind.y + iRadiusAll;
	 RectValue.right  = CenterRoseWind.x + iLengthValueBox;
	 RectValue.bottom = CenterRoseWind.y + iRadiusAll + iWidthValueBox;

//	---------- Создание перьев, шрифтов, кистей ---------------------
												// Перо "ТЕНИ" - серого цвета
	 hPenGray    = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо контура - черного цвета
	 hPenOutline = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
												// Перо "ОСВЕЩЕНИЯ" - белого цвета
	 hPenWhite   = (HPEN)GetStockObject(WHITE_PEN);
												// Перо оси
	 hPenAxis    = CreatePen(PS_SOLID, 0, RGB(0, 128, 0));
												// Перо графика
	 hPenGraph   = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
												// Кисть для розы - белого цвета
	 hbrRose 	 = (HBRUSH)GetStockObject(WHITE_BRUSH);
												// Кисть для фона - черного цвета
	 hbrBlack    = (HBRUSH)GetStockObject(BLACK_BRUSH);
												// Кисть для окна показания - светло-серого цвета
	 hbrLtGray   = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
												// Создаем шрифт для показания и шкалы
	 memset(&lf, 0, sizeof(LOGFONT));
	 lf.lfHeight = FontHeight;			// Высота и
	 lf.lfWidth  = FontWidth;        // ширина шрифта
	 lstrcpy((LPSTR)&lf.lfFaceName[0], (LPSTR)"Helvetica");
	 hfontHelv = CreateFontIndirect(&lf);

												// Данные для подсчета кол-ва
	 for (char i = 0; i < COUNTRUMBS; i++){
		dCountDirect    [i] = 0.0;		// повторений по направлению ветра
		dCountShowDirect[i] = 0.0;
	 };										// Данные для удаления старой розы
	 for (i = 0; i < COUNTRUMBS + 1; i++){
		OldDirectPoints[i].x = 0;
		OldDirectPoints[i].y = 0;
	 };
	 strcpy(StOldValue, "");
};

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r															*
\*-------------------------------------------------------------------*/
RoseWind::~RoseWind()
{
	 DeleteObject(hPenGray);      	// Erase block memory for user pen
	 DeleteObject(hPenOutline);  	   // Erase block memory for user pen
	 DeleteObject(hPenWhite);     	// Erase block memory for user pen
	 DeleteObject(hPenAxis);  	   	// Erase block memory for user pen
	 DeleteObject(hPenGraph);     	// Erase block memory for user pen
	 DeleteObject((HBRUSH)hbrRose);	// Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrBlack); // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrLtGray);// Erase block memory for user brush
	 DeleteObject((HFONT)hfontHelv); // Erase block memory for user font
};

/*-----------------------------------------------------------------*\
 * 	Отображение розы ветров                                      *
\*-----------------------------------------------------------------*/
void RoseWind::DrawRoseWind(TDC& dc){  // DC окна
	 TSize size;
// ---------------------- Рисуем розу ветров --------------------------
	 dc.SelectObject((HBRUSH)hbrBlack);    // Рисуем окно розы ветров
	 DrawLightCircle(dc, CenterRoseWind, iRadiusAll, hPenWhite, hPenGray, RAISED);
	 dc.Ellipse(CenterRoseWind.x - iRadiusRoseWind, CenterRoseWind.y - iRadiusRoseWind,
					CenterRoseWind.x + iRadiusRoseWind, CenterRoseWind.y + iRadiusRoseWind);
	 DrawLightCircle(dc, CenterRoseWind, iRadiusRoseWind, hPenWhite, hPenGray, EMBOSSED);
/*
// --------------------- Рисуем окно показания ------------------------
	 dc.SelectObject((HBRUSH)hbrLtGray);
	 dc.Rectangle(RectValue);
	 DrawLightBorder(dc, RectValue, hPenWhite, hPenGray, EMBOSSED);
*/
};

/*-----------------------------------------------------------------*\
 * 	Отображение одного значения розы ветров                      *
\*-----------------------------------------------------------------*/
void RoseWind::DrawValueRoseWind(TDC& dc,		 // DC окна
											int  iIndex){// индекс данного в циклическом
															 // массиве данных
	double dMaxRays = 0.0;			 	// Самый длинный луч
	TSize  size;
	double dRatio = 1.0;             // Коэфициент масштабирования
	BOOL   bFlagRatio = FALSE;			// Флаг необходимости масштабирования
	char   StValue[10] = "";			// Строка показания
	double dValue;
	dValue = ValuesInstruments[iIndex];
/*
// ------------ У д а л е н и е   п о к а з а н и я -------------------
	 dc.SelectObject((HFONT)hfontHelv);
	 dc.SetTextColor(TColor::LtGray); 		// Устанавливаем цвет текста
														// Зарисуем старый текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StOldValue);
*/
// ------------ У д а л е н и е   с т а р о й   р о з ы ---------------
												// Загрузим перо и кисть
	dc.SelectObject((HPEN)hPenOutline);
	dc.SelectObject((HBRUSH)hbrBlack);
												// Удалим старую розу ветров
	dc.Polygon(OldDirectPoints, COUNTRUMBS);
// ------------------ О с и  ------------------------------------------
	dc.SelectObject((HPEN)hPenAxis);
	for (int ij = 0; ij < 360; ij += 45){
		 MoveToCorner(dc, CenterRoseWind.x, CenterRoseWind.y, 0, 0, GRAD_TO_RAD(0.0));
		 LineToCorner(dc, CenterRoseWind.x, CenterRoseWind.y, iRadiusRoseWind - 1, iRadiusRoseWind - 1, GRAD_TO_RAD((double)ij));
	};

// ------------В ы ч и с л е н и е   к о л и ч е с т в а --------------
// ------------------------ н а п р а в л е н и й ---------------------
		  if (dValue <   11.2)     	  		  	  dCountDirect[0]++; //	С
	else if (dValue >=  11.3 && dValue <  34.2) dCountDirect[1]++; //	ССВ
	else if (dValue >=  34.3 && dValue <  56.2) dCountDirect[2]++; //	СВ
	else if (dValue >=  56.3 && dValue <  79.2) dCountDirect[3]++; //	СВВ
	else if (dValue >=  79.3 && dValue < 101.2) dCountDirect[4]++; //	В
	else if (dValue >= 101.3 && dValue < 124.2) dCountDirect[5]++; //	ЮВВ
	else if (dValue >= 124.3 && dValue < 146.2) dCountDirect[6]++; //	ЮВ
	else if (dValue >= 146.3 && dValue < 169.2) dCountDirect[7]++; //	ЮЮВ
	else if (dValue >= 169.3 && dValue < 191.2) dCountDirect[8]++; //	Ю
	else if (dValue >= 191.3 && dValue < 214.2) dCountDirect[9]++; //	ЮЮЗ
	else if (dValue >= 214.3 && dValue < 236.2) dCountDirect[10]++;//	ЮЗ
	else if (dValue >= 236.3 && dValue < 259.2) dCountDirect[11]++;//	ЮЗЗ
	else if (dValue >= 259.3 && dValue < 281.2) dCountDirect[12]++;//	З
	else if (dValue >= 281.3 && dValue < 304.2) dCountDirect[13]++;//	СЗЗ
	else if (dValue >= 304.3 && dValue < 326.2) dCountDirect[14]++;//	СЗ
	else if (dValue >= 326.3 && dValue < 349.2) dCountDirect[15]++;//	ССЗ
	else if (dValue >= 349.3)     	  			  dCountDirect[0]++; //	С
													// Найдем луч наибольший луч
	for (char i = 0; i < COUNTRUMBS; i++){
	  if (dCountDirect[i] >= dMaxRays){
		  dMaxRays    = dCountDirect[i];// Если наибольший луч больше, чем
													// радиус розы ветров, то
		  if (dMaxRays > (double)(iRadiusRoseWind - 1))
			  bFlagRatio = TRUE;          // взведем флаг масштабирования
	  };
	};
	if (bFlagRatio){
		dRatio = dMaxRays / (double)(iRadiusRoseWind - 1);
	};
												  // Если необходимо масштабировать -
	for (i = 0; i < COUNTRUMBS; i++){  // масштабируем
		dCountShowDirect[i] = dCountDirect[i] / dRatio;
	};

//------------------- C (0) -------------------
	DirectPoints[0].x = CenterRoseWind.x;
	DirectPoints[0].y = CenterRoseWind.y - (int)dCountShowDirect[0];
//------------------- ССВ (22,5) --------------
	DirectPoints[1].x = CenterRoseWind.x + (int)(dCountShowDirect[1] * sin(RADCORNER22_5));
	DirectPoints[1].y = CenterRoseWind.y - (int)(dCountShowDirect[1] * cos(RADCORNER22_5));
//------------------- СВ (45) -----------------
	DirectPoints[2].x = CenterRoseWind.x + (int)(dCountShowDirect[2] * sin(RADCORNER45));
	DirectPoints[2].y = CenterRoseWind.y - (int)(dCountShowDirect[2] * cos(RADCORNER45));
//------------------- СВВ (67,5) --------------
	DirectPoints[3].x = CenterRoseWind.x + (int)(dCountShowDirect[3] * sin(RADCORNER67_5));
	DirectPoints[3].y = CenterRoseWind.y - (int)(dCountShowDirect[3] * cos(RADCORNER67_5));
//------------------- В (90) ------------------
	DirectPoints[4].x = CenterRoseWind.x + (int)dCountShowDirect[4];
	DirectPoints[4].y = CenterRoseWind.y;
//------------------- ЮВВ (22,5) --------------
	DirectPoints[5].x = CenterRoseWind.x + (int)(dCountShowDirect[5] * cos(RADCORNER22_5));
	DirectPoints[5].y = CenterRoseWind.y + (int)(dCountShowDirect[5] * sin(RADCORNER22_5));
//------------------- ЮВ (45) -----------------
	DirectPoints[6].x = CenterRoseWind.x + (int)(dCountShowDirect[6] * cos(RADCORNER45));
	DirectPoints[6].y = CenterRoseWind.y + (int)(dCountShowDirect[6] * sin(RADCORNER45));
//------------------- ЮЮВ (67,5) --------------
	DirectPoints[7].x = CenterRoseWind.x + (int)(dCountShowDirect[7] * cos(RADCORNER67_5));
	DirectPoints[7].y = CenterRoseWind.y + (int)(dCountShowDirect[7] * sin(RADCORNER67_5));
//------------------- Ю (90) ------------------
	DirectPoints[8].x = CenterRoseWind.x;
	DirectPoints[8].y = CenterRoseWind.y + (int)dCountShowDirect[8];
//------------------- ЮЮЗ(22,5) ---------------
	DirectPoints[9].x = CenterRoseWind.x - (int)(dCountShowDirect[9] * sin(RADCORNER22_5));
	DirectPoints[9].y = CenterRoseWind.y + (int)(dCountShowDirect[9] * cos(RADCORNER22_5));
//------------------- ЮЗ(45) ------------------
	DirectPoints[10].x = CenterRoseWind.x - (int)(dCountShowDirect[10] * sin(RADCORNER45));
	DirectPoints[10].y = CenterRoseWind.y + (int)(dCountShowDirect[10] * cos(RADCORNER45));
//------------------- ЮЗЗ(67,5) ---------------
	DirectPoints[11].x = CenterRoseWind.x - (int)(dCountShowDirect[11] * sin(RADCORNER67_5));
	DirectPoints[11].y = CenterRoseWind.y + (int)(dCountShowDirect[11] * cos(RADCORNER67_5));
//------------------- З(90) -------------------
	DirectPoints[12].x = CenterRoseWind.x - (int)dCountShowDirect[12];
	DirectPoints[12].y = CenterRoseWind.y;
//------------------- СЗЗ(22,5) ---------------
	DirectPoints[13].x = CenterRoseWind.x - (int)(dCountShowDirect[13] * cos(RADCORNER22_5));
	DirectPoints[13].y = CenterRoseWind.y - (int)(dCountShowDirect[13] * sin(RADCORNER22_5));
//------------------- СЗ(45) ------------------
	DirectPoints[14].x = CenterRoseWind.x - (int)(dCountShowDirect[14] * cos(RADCORNER45));
	DirectPoints[14].y = CenterRoseWind.y - (int)(dCountShowDirect[14] * sin(RADCORNER45));
//------------------- ССЗ(67,5) ---------------
	DirectPoints[15].x = CenterRoseWind.x - (int)(dCountShowDirect[15] * cos(RADCORNER67_5));
	DirectPoints[15].y = CenterRoseWind.y - (int)(dCountShowDirect[15] * sin(RADCORNER67_5));
//------------------- С(90) -------------------
	DirectPoints[16].x = CenterRoseWind.x;
	DirectPoints[16].y = CenterRoseWind.y - (int)dCountShowDirect[0];
/*
// ------------------ П о к а з а н и я ----------------------------
	 if (dValue <= 360.0 && dValue >= 0.0){// Сравнение для определения переполнения
		 bOVER = FALSE;                     // Сбросим флаг переполнения
		 dc.SetTextColor(TColor::Black);		// Устанавливаем цвет текста
														// Формируем текст показания из
														// максимума повторения и коэфициента
														// масштабирования
		 sprintf(StValue, "%3.0f:%3.1f", dMaxRays, dRatio);
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 dc.SetTextColor(TColor::LtRed);		// Устанавливаем цвет сообщения
		 if (dValue > 360.0)
			 dValue = 360.0;
		 if (dValue < 0.0)
			 dValue = 0.0;
		 sprintf(StValue, "OVER");
	 };							 			// Отображаем текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StValue);
*/
// ------------ П р о р и с о в к а   н о в о й   р о з ы -------------
	dc.SelectObject((HPEN)hPenGraph);// Загрузим перо и кисть
	dc.SelectObject((HBRUSH)hbrRose);
												// Отобразим новую розу ветров
	dc.Polygon(DirectPoints, COUNTRUMBS);

												// Запомним данные удаления старой розы
	for (i = 0; i < COUNTRUMBS + 1; i++){
		OldDirectPoints[i].x = DirectPoints[i].x;
		OldDirectPoints[i].y = DirectPoints[i].y;
	};
	strcpy(StOldValue, StValue);
};
/* ----------------------------------------------------------
 *	EoF(rosewind.cpp)
 * ----------------------------------------------------------
 */
