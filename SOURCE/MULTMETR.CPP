/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Multimetr
	Создает модель наглядного отображения данных на мультиметре

	Member functions:

		Multimetr(TRect MultimetrRect,	размер и положение модели
					 int   iMin,         	минимальное показание
					 int   iMax,         	максимальное показание
					 char* sDimension)   	текст размерности
			(Consrtuctor)

		~Multimetr()
			(Destructor)

		DrawMultimetr(TDC& dc)        	DC окна
			(Рисует корпус, окно показания, и т.д)

		DrawValueMultimetr(TDC& dc,		DC окна
								 int  iIndex)	индекс данного в циклическом массиве данных
			(Отображает одно значение датчика)
\*------------------------------------------------------------------------*/
#include <stdio.h>

#include "multmetr.h"

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
Multimetr::Multimetr(TRect MultimetrRect, // размер и положение модели
							int   iMin,          // минимальное показание
							int   iMax,          // максимальное показание
							char* sDimension,    // текст размерности
							COLORREF Color){

	 strcpy(sDim, sDimension);			// Запомним размерность
	 iMinValue = iMin;  					// Минимальное допустимое значение мультиметра
	 iMaxValue = iMax;					// Максимальное допустимое значение мультиметра
	 iRange    = iMax - iMin; 			// Диапазон мультиметра

//	------------------ Размеры фигур ------------------------
	 AllRect 	=  MultimetrRect;
	 iHeight  	=  MultimetrRect.Height();	// Высота мультиметра
	 iWidth   	=  MultimetrRect.Width();	// Длина мультиметра

	 iLengthValueBox 	= 35;            		// Длина окна показания
	 iWidthValueBox 	= 17;            		// Ширина окна показания
	 FontHeight     	= 13;						// Высота и
	 FontWidth		 	= 4;						// ширина шрифта
	 iRadiusOut		 	= iHeight / 10;		// Радиус крепления стрелки

//	------------------ Координаты фигур ---------------------
												// Размеры окна мультиметра
	 RectWind.left      = AllRect.left   + DELTAFROMMULT;
	 RectWind.top       = AllRect.top    + DELTAFROMMULT;
	 RectWind.right     = AllRect.right  - DELTAFROMMULT;
	 RectWind.bottom    = AllRect.bottom - DELTAFROMMULT - (int)((double)iWidth / 6.0);
												// Длина стрелки либо Height / 2 либо Width
	 if ((double)RectWind.Width() / 2.0 >= (double)RectWind.Height())
		 iLengthNeedle    = RectWind.Height() - 2;
	 else
		 iLengthNeedle    = (int)((double)RectWind.Width() / 2.0 - 2.0);
												// Размеры нижнего корпуса мультиметра
	 RectMultim.left    = AllRect.left   + DELTAFROMMULT;
	 RectMultim.top     = AllRect.bottom - DELTAFROMMULT - (int)((double)iWidth / 6.0);
	 RectMultim.right   = AllRect.right  - DELTAFROMMULT;
	 RectMultim.bottom  = AllRect.bottom - DELTAFROMMULT;
												// Размеры окна показания
	 RectValue.left   = RectMultim.right  - iLengthValueBox - DELTAFROMMULT;
	 RectValue.top    = RectMultim.bottom - iWidthValueBox  - DELTAFROMMULT;
	 RectValue.right  = RectMultim.right  - DELTAFROMMULT;
	 RectValue.bottom = RectMultim.bottom - DELTAFROMMULT;

//	---------- Создание перьев, шрифтов, кистей -------------
												// Перо "ТЕНИ"
	 hPenGray    = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо графика
	 hPenGraph   = CreatePen(PS_SOLID, 1, Color);
												// Перо контура
	 hPenOutline  = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
												// Перо "ОСВЕЩЕНИЯ"
	 hPenWhite   = (HPEN)GetStockObject(WHITE_PEN);
												// Кисть для мультиметра - белого цвета
	 hbrWhite    = (HBRUSH)GetStockObject(WHITE_BRUSH);
												// Кисть для фона - серого цвета
	 hbrGray     = (HBRUSH)GetStockObject(GRAY_BRUSH);
												// Кисть для окна показания - светло-серого цвета
	 hbrLtGray   = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
												// Кисть для нижней части корпуса - черного цвета
	 hbrBlack    = (HBRUSH)GetStockObject(BLACK_BRUSH);
												// Создаем шрифт для показания и шкалы
	 memset(&lf, 0, sizeof(LOGFONT));
	 lf.lfHeight = FontHeight;			// Высота и
	 lf.lfWidth  = FontWidth;        // ширина шрифта
	 lstrcpy((LPSTR)&lf.lfFaceName[0], (LPSTR)"Helvetica");
	 hfontHelv = CreateFontIndirect(&lf);

	 dOldRadCorner = 0.0;
	 strcpy(StOldValue, "");
};

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r															*
\*-------------------------------------------------------------------*/
Multimetr::~Multimetr()
{
	 DeleteObject(hPenGray);      	// Erase block memory for user pen
	 DeleteObject(hPenGraph);     	// Erase block memory for user pen
	 DeleteObject(hPenOutline);   	   // Erase block memory for user pen
	 DeleteObject(hPenWhite);   	   // Erase block memory for user pen
	 DeleteObject((HBRUSH)hbrWhite); // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrGray);  // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrLtGray);// Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrBlack); // Erase block memory for user brush
	 DeleteObject((HFONT)hfontHelv); // Erase block memory for user font
};

/*-------------------------------------------------------------------*\
 *		Отображение мультиметра														*
\*-------------------------------------------------------------------*/
void Multimetr::DrawMultimetr(TDC& dc){   // DC окна

// --------- Рисуем мультиметр ------------------------------------
//	 dc.SelectObject((HPEN)hPenGray);    	// Выбор пера и заливки для корпуса
//	 dc.SelectObject((HBRUSH)hbrLtGray);
//	 dc.Rectangle(AllRect);						// Окно для всего мультиметра
														//	Рамка
	 DrawLightBorder(dc, AllRect, hPenWhite, hPenGray, RAISED);

	 dc.SelectObject((HPEN)hPenOutline);    // Рисуем прямоугольник окна мультиметра
	 dc.SelectObject((HBRUSH)hbrWhite);
	 dc.Rectangle(RectWind);

	 dc.SelectObject((HPEN)hPenOutline);    // Рисуем низний корпус мультиметра
	 dc.SelectObject((HBRUSH)hbrBlack);
	 dc.Rectangle(RectMultim);

	 dc.SelectObject((HBRUSH)hbrBlack);    // Рисуем крепление стрелки барометра
	 TPoint Center;
	 dc.Ellipse(RectWind.left + RectWind.Width() / 2 - iRadiusOut,
					RectWind.bottom 							 - iRadiusOut,
					RectWind.left + RectWind.Width() / 2 + iRadiusOut,
					RectWind.bottom 							 + iRadiusOut);
	 Center.x = RectWind.left + RectWind.Width() / 2;
	 Center.y = RectWind.bottom;           // Световая рамка
	 DrawLightCircle(dc, Center, iRadiusOut, hPenWhite, hPenGray, RAISED);
	 dc.SelectObject((HPEN)hPenGray);      // Прорезь крепления
	 dc.MoveTo(Center.x, Center.y - iRadiusOut);
	 dc.LineTo(Center.x, Center.y + iRadiusOut);

	 dc.SelectObject((HPEN)hPenGray);      // Рисуем окно показания
	 dc.SelectObject((HBRUSH)hbrLtGray);
	 dc.Rectangle(RectValue);
	 DrawLightBorder(dc, RectValue, hPenWhite, hPenGray, EMBOSSED);

// ------------------ Ш к а л а ------------------------------------
	 dc.SelectObject((HPEN)hPenOutline);
	 dc.SetTextColor(TColor::Black);
	 dc.SelectObject((HFONT)hfontHelv);
	 DrawCircleScale(dc,
				  RectWind.left + ((double)RectWind.Width () / 2.0),
				  RectWind.bottom - 1,
				  iLengthNeedle, sDim, iMinValue, iRange);
};


/*-------------------------------------------------------------------*\
 *		Отображение одного значения мультиметра								*
\*-------------------------------------------------------------------*/
void
Multimetr::DrawValueMultimetr(TDC& dc,      // DC окна
										int  iIndex){ // индекс данного в циклическом массиве данных
	 double dValue;
	 char   StValue[6];			 				// Строка отображения показания
	 TPoint  Center;
	 double dRadCorner;

	 dValue = ValuesInstruments[iIndex];
// ------------------ П о к а з а н и я ----------------------------
	 dc.SelectObject((HFONT)hfontHelv);
	 dc.SetTextColor(TColor::LtGray); 		// Устанавливаем цвет текста
														// Зарисуем старый текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StOldValue);
														// Сравнение для определения переполнения
	 if (dValue <= (double)iMaxValue && dValue >= (double)iMinValue){
		 bOVER = FALSE;                     // Сбросим флаг переполнения
		 dc.SetTextColor(TColor::Black);		// Устанавливаем цвет текста
		 sprintf(StValue, "%3.1f", dValue);	// Формируем текст показания
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 dc.SetTextColor(TColor::LtRed);		// Устанавливаем цвет текста
		 if (dValue > (double)iMaxValue)
			 dValue = (double)iMaxValue;
		 if (dValue < (double)iMinValue)
			 dValue = (double)iMinValue;
		 sprintf(StValue, "OVER");
	 };												// Отображаем текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StValue);

// ------------------ Г р а ф и к ---------------------------------
	 Center.x = RectWind.left + (int)((double)RectWind.Width () / 2.0);
	 Center.y = RectWind.bottom - 2;

	 double k    = 180.0 / (double)iRange;
	 double corn;
	 if (fabs(iMaxValue) - fabs(iMinValue) == 0) corn =   0.0;
	 if (fabs(iMaxValue) - fabs(iMinValue) >  0) corn = -90.0;
	 if (fabs(iMaxValue) - fabs(iMinValue) <  0) corn =  90.0;
	 dRadCorner = GRAD_TO_RAD(dValue * k) + GRAD_TO_RAD(corn);

// ---------- С т а р ы й    Г р а ф и к ---------------------------
	 dc.SelectObject((HPEN)hPenWhite);    // Выбор пера для удаления старой стрелки
	 MoveToCorner(dc, Center.x, Center.y, iRadiusOut + 1.0, iRadiusOut + 1.0, dOldRadCorner);
	 LineToCorner(dc, Center.x, Center.y, iLengthNeedle, 	iLengthNeedle, 	dOldRadCorner);

// ------------------ Ш к а л а ------------------------------------
	 dc.SelectObject((HPEN)hPenOutline);
	 dc.SetTextColor(TColor::Black);
	 dc.SelectObject((HFONT)hfontHelv);
	 DrawCircleScale(dc, Center.x, Center.y + 1,
						iLengthNeedle, sDim, iMinValue, iRange);

// ------------------ Г р а ф и к ---------------------------------
	 dc.SelectObject((HPEN)hPenGraph);     // Выбор пера для стрелки
	 MoveToCorner(dc, Center.x, Center.y, iRadiusOut + 1.0, iRadiusOut + 1.0, dRadCorner);
	 LineToCorner(dc, Center.x, Center.y, iLengthNeedle, 	iLengthNeedle, 	dRadCorner);

	 dOldRadCorner = dRadCorner;           // Запомним предыдущее значение
	 strcpy(StOldValue, StValue);
};
/* ----------------------------------------------------------
 *	EoF(multmetr.cpp)
 * ----------------------------------------------------------
 */
