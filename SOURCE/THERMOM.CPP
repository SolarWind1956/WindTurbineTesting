/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Termom
	Создает модель наглядного отображения данных по температуре

	Member functions:
---------------------------------------------------------------------------
		Termom(TRect ThermoRect, 		размер и положение модели
				 int 	 iMin,  				минимальное показание
				 int 	 iMax,           	максимальное показание
				 char* sDimension)    	текст размерности
			(Consrtuctor)
---------------------------------------------------------------------------
		~Termom()
			(Destructor)
---------------------------------------------------------------------------
		DrawTermom(TDC& dc)          	DC окна
			(Рисует корпус, окно показания, и т.д)
---------------------------------------------------------------------------
		DrawValueTermom(TDC& dc,		DC окна
							int  iIndex)	индекс данного в циклическом массиве данных
			(Отображает одно значение датчика)
\*------------------------------------------------------------------------*/
#include <stdio.h>
#include "thermom.h"
/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
Thermom::Thermom(TRect ThermoRect,  // размер и положение модели
					  int   iMin,        // минимальное показание
					  int   iMax,        // максимальное показание
					  char* sDimension) {// текст размерности
	 strcpy(sDim, sDimension);
	 iMinValue = iMin;  					// Минимальное допустимое значение термометра
	 iMaxValue = iMax;					// Максимальное допустимое значение термометра
	 iRange    = iMax - iMin; 			// Диапазон термометра
												// Шаг шкалы
	 iStepScale = iRange / QUANTMARKRECT;

//	------------------ Размеры фигур ------------------------
	 AllRect =  ThermoRect;
	 Length  =  ThermoRect.Height();	// Длинна термометра
	 Width   =  ThermoRect.Width();	// Ширина термометра

	 iLengthMark  		= Width / 3;	// Длинна засечки шкалы
	 iLengthValueBox 	= 35;          // Длина окна показания
	 iWidthValueBox  	= 17;          // Ширина окна показания
	 FontHeight     	= 13;				// Высота и
	 FontWidth		 	= 4;				// ширина шрифта
												// Длинна носика термометра 1/10 длинны термометра
	 iHeadLenght = (int)((double)Length / 10.0);
												// Ширина носика термометра 1/7 ширины термометра
	 iHeadWidth  = (int)((double)Width  / 7.0);

//	------------------ Координаты фигур ---------------------
												// Размеры окна показания
	 RectValue.left   = AllRect.right  - iLengthValueBox - DELTAFROMTERM * 2;
	 RectValue.top    = AllRect.bottom - iWidthValueBox  - DELTAFROMTERM * 2;
	 RectValue.right  = AllRect.right  - DELTAFROMTERM * 2;
	 RectValue.bottom = AllRect.bottom - DELTAFROMTERM * 2;
												// Размеры термометра
	 RectTerm.left    = AllRect.left   + DELTAFROMTERM;
	 RectTerm.top     = AllRect.top    + DELTAFROMTERM;
	 RectTerm.right   = AllRect.right  - DELTAFROMTERM * 2;
	 RectTerm.bottom  = AllRect.bottom - 5 - iHeadLenght - DELTAFROMTERM;
												// Размеры носика термометра
	 RectHead.left		= RectTerm.left  + iLengthMark / 2 + AXISDELTA - iHeadWidth / 2;
	 RectHead.top     = RectTerm.bottom + AXISDELTA;
	 RectHead.right   = RectHead.left   + iHeadWidth;
	 RectHead.bottom  = RectHead.top    + iHeadLenght;
												// Вычислим углы скругления носика
	 RoundCorner.x = 8; RoundCorner.y = 8;
												// Расстояние между засечками шкалы
	 dDeltaMark   	= (double)((RectTerm.bottom - AXISDELTA) - (RectTerm.top + AXISDELTA)) /
									(double)QUANTMARKRECT;
												// Коэффициент отображения графика
	 dRatioValue  	= (double)iRange / (dDeltaMark * (double)QUANTMARKRECT);

//	---------- Создание перьев, шрифтов, кистей -------------
												// Перо для удаления
												// старого показания графика
	 hPenDelete  = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
												// Перо для общей оси
	 hPenLtGray  = CreatePen(PS_SOLID, 2, RGB(192, 192, 192));
												// Перо "ТЕНИ"
	 hPenGray    = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо графика
	 hPenGraph   = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
												// Перо контура
	 hPenOutline = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
												// Перо "ОСВЕЩЕНИЯ"
	 hPenWhite   = (HPEN)GetStockObject(WHITE_PEN);
												// Кисть для термометра - белого цвета
	 hbrWhite    = (HBRUSH)GetStockObject(WHITE_BRUSH);
												// Кисть для фона - серого цвета
	 hbrGray     = (HBRUSH)GetStockObject(GRAY_BRUSH);
												// Кисть для окна показания - светло-серого цвета
	 hbrLtGray   = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	 lb.lbStyle = BS_SOLID;          // Создаем кисть для носика - красного цвета
	 lb.lbColor = RGB(255, 0, 0);
	 hbrRed     = CreateBrushIndirect(&lb);
												// Создаем шрифт для показания и шкалы
	 memset(&lf, 0, sizeof(LOGFONT));
	 lf.lfHeight = FontHeight;			// Высота и
	 lf.lfWidth  = FontWidth;        // ширина шрифта
	 lstrcpy((LPSTR)&lf.lfFaceName[0], (LPSTR)"Helvetica");
	 hfontHelv = CreateFontIndirect(&lf);

	 dOldValue = 0.0;
	 strcpy(StOldValue, "");
};

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r															*
\*-------------------------------------------------------------------*/
Thermom::~Thermom(){
	 DeleteObject(hPenDelete);      	// Erase block memory for user pen
	 DeleteObject(hPenLtGray);      	// Erase block memory for user pen
	 DeleteObject(hPenGray);      	// Erase block memory for user pen
	 DeleteObject(hPenGraph);     	// Erase block memory for user pen
	 DeleteObject(hPenOutline);  	   // Erase block memory for user pen
	 DeleteObject(hPenWhite);   	   // Erase block memory for user pen
	 DeleteObject((HBRUSH)hbrWhite); // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrGray);  // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrLtGray);// Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrRed);	// Erase block memory for user brush
	 DeleteObject((HFONT)hfontHelv); // Erase block memory for user font
};

/*-------------------------------------------------------------------*\
 *		Отображение термометра														*
\*-------------------------------------------------------------------*/
void Thermom::DrawThermom(TDC& dc){ // DC окна
	 char  MiscStr[17]; 				   // Строка отображения значений шкалы
	 div_t t;
	 int   ix, iy;
	 TSize size;

// --------- Рисуем термометр ------------------------------------
//	 dc.SelectObject((HPEN)hPenGray);    	// Выбор пера и заливки для корпуса
//	 dc.SelectObject((HBRUSH)hbrLtGray);
//	 dc.Rectangle(AllRect);						// Окно для всего термометра
														//	Рамка
	 DrawLightBorder(dc, AllRect, hPenWhite, hPenGray, RAISED/*EMBOSSED*/);

	 dc.SelectObject((HPEN)hPenOutline);    // Рисуем прямоугольник термометра
	 dc.SelectObject((HBRUSH)hbrWhite);
	 dc.Rectangle(RectTerm);

	 dc.SetTextColor(TColor::Black);       // Рисуем текст размерности
	 dc.SelectObject((HFONT)hfontHelv);
	 dc.GetTextExtent(sDim, strlen(sDim), size);
	 dc.TextOut(RectTerm.right  - size.cx - DELTAFROMTERM,
					RectTerm.bottom - size.cy, sDim);

	 dc.SelectObject((HPEN)hPenGray);      // Рисуем окно показания
	 dc.SelectObject((HBRUSH)hbrLtGray);
	 dc.Rectangle(RectValue);
	 DrawLightBorder(dc, RectValue, hPenWhite, hPenGray, EMBOSSED);

	 dc.SelectObject((HPEN)hPenLtGray);    // Ось термометра
	 dc.MoveTo(RectTerm.left + iLengthMark / 2 + AXISDELTA, RectTerm.top    + AXISDELTA);
	 dc.LineTo(RectTerm.left + iLengthMark / 2 + AXISDELTA, RectTerm.bottom - AXISDELTA);

	 dc.SelectObject((HPEN)hPenOutline);     // Рисуем носик термометра красным цветом
//	 dc.SelectObject((HBRUSH)hbrRed);
//	 dc.RoundRect(RectHead, RoundCorner);

// ---------------------- Ш к а л а -------------------------------
	 dc.SetTextColor(TColor::Black);			// Устанавливаем цвет текста
	 dc.SelectObject((HFONT)hfontHelv);

	 for (char i = 0; i <= QUANTMARKRECT * 2; i++){
		t  = div(i, 2);                     // ЧЕТ или НЕЧЕТ = ГЛАВНЫЕ или ВТОРИЧНЫЕ засечки
														// Координаты отображения засечек
		ix = RectTerm.left + AXISDELTA + iLengthMark / 2;// и текста шкалы
		iy = RectTerm.top  + AXISDELTA + i * (int)(dDeltaMark / 2.0);

		if (t.rem == 0){							// Прорисовка ГЛАВНЫХ засечек
			dc.MoveTo (ix - iLengthMark / 4, iy);
			dc.LineTo (ix + iLengthMark / 4, iy);
		} else {										// Прорисовка ВТОРИЧНЫХ засечек
			dc.MoveTo (ix - iLengthMark / 2, iy);
			dc.LineTo (ix + iLengthMark / 2, iy);
														// Формирование и прорисовка текста
			itoa(iMaxValue - i * iStepScale / 2, MiscStr, 10);
			dc.TextOut(RectTerm.left + AXISDELTA * 2 + iLengthMark, iy - lf.lfHeight / 2, MiscStr);
		};
	 };
};

/*-------------------------------------------------------------------*\
 *		Отображение одного значения термометра									*
\*-------------------------------------------------------------------*/
void Thermom::DrawValueThermom(TDC& dc,   // DC окна
														// Индекс данного в циклическом массиве данных
										 int  iIndex){
	 double dValue;
	 char   StValue[6];			 				// Строка отображения показания
	 dValue = ValuesInstruments[iIndex];
// ------------------ П о к а з а н и я ----------------------------
														// Удалим старый график
	 if (dOldValue > ((dValue + 50.0) / dRatioValue)){
		 dc.SelectObject((HPEN)hPenDelete); // путем закрашивания разности
														// пером оси графика
		 dc.MoveTo(RectTerm.left + AXISDELTA + (double)iLengthMark / 2.0,
					  RectTerm.top  + AXISDELTA +
					  (int)(((double)iRange / dRatioValue) - fabs(dOldValue)));

		 dc.LineTo(RectTerm.left + AXISDELTA + (double)iLengthMark / 2.0,
					  RectTerm.top  + AXISDELTA +
					  (int)(((double)iRange / dRatioValue) - fabs(dValue)));
	 };
	 dc.SelectObject((HFONT)hfontHelv);
	 dc.SetTextColor(TColor::LtGray); 		// Устанавливаем цвет текста
														// Зарисуем старый текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StOldValue);
														// Сравнение для определения переполнения
	 if (dValue <= (double)iMaxValue && dValue >= (double)iMinValue){
		 bOVER = FALSE;                     // Сбросим флаг переполнения
		 dc.SetTextColor(TColor::Black);		// Устанавливаем цвет текста
		 sprintf(StValue, "%+2.1f", dValue);// Формируем текст показания
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 if (dValue > (double)iMaxValue){ 	// Устанавливаем цвет сообщения
			 dc.SetTextColor(TColor::LtRed);	// в зависимости от того слишком высокая или
			 dValue = (double)iMaxValue;
		 };
		 if (dValue < (double)iMinValue){	// слишком низкая температура
			 dc.SetTextColor(TColor::LtBlue);
			 dValue = (double)iMinValue;
		 };
		 sprintf(StValue, "OVER");
	 };												// Отображаем текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StValue);

// ------------------ Г р а ф и к ---------------------------------
	 dc.SelectObject((HPEN)hPenGraph);
	 dValue = (dValue + 50.0) / dRatioValue;

	 dc.MoveTo(RectTerm.left + iLengthMark / 2 + AXISDELTA,
				  RectTerm.bottom - AXISDELTA - (int)fabs(dValue));
	 dc.LineTo(RectTerm.left + iLengthMark / 2 + AXISDELTA,
				  RectTerm.bottom);

	 dOldValue = dValue;                   // Запомним предыдущее значение
	 strcpy(StOldValue, StValue);
};
/* ----------------------------------------------------------
 *	EoF(thermom.cpp)
 * ----------------------------------------------------------
 */
