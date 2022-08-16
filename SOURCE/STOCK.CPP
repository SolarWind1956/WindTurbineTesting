/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Stock
	Создает модель наглядного отображения данных по датчику хода штока

	Member functions:
---------------------------------------------------------------------------
		Stock(TRect StockRect,			размер и положение модели
				int 	iMin,					минимальное показание
				int 	iMax,            	максимальное показание
				char* sDimension)    	текст размерности
			(Consrtuctor)
---------------------------------------------------------------------------
		~Stock()
			(Destructor)
---------------------------------------------------------------------------
		DrawStock(TDC& dc)          	DC окна
			(Рисует корпус штока, окно показания, и т.д)
---------------------------------------------------------------------------
		DrawValueStock(TDC& dc,			DC окна
							int  iIndex)	индекс данного в циклическом массиве данных
			(Отображает одно значение датчика хода штока)
\*------------------------------------------------------------------------*/
#include <stdio.h>

#include "stock.h"

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
Stock::Stock(TRect StockRect,			// размер и положение модели
				 int   iMin,            // минимальное показание
				 int   iMax,            // максимальное показание
				 char* sDimension,      // текст размерности
				 COLORREF Color){       // Цвет столбика
	 strcpy(sDim, sDimension);
	 iMinValue = iMin;  					// Минимальное допустимое значение штока
	 iMaxValue = iMax;		 			// Максимальное допустимое значение штока
	 iRange    = iMax - iMin;			// Диапазон штока
												// Шаг шкалы
	 iStepScale = iRange / QUANTMARKRECT;

//	------------------ Размеры фигур ------------------------
	 AllRect = StockRect;
	 iLength = StockRect.Height();	// Длинна штока
	 iWidth  = StockRect.Width(); 	// Ширина штока

	 iLengthMark  		= iWidth / 3;	// Длинна засечки шкалы
	 iLengthValueBox 	= 35;          // Длина окна показания
	 iWidthValueBox  	= 17;          // Ширина окна показания
	 FontHeight     	= 13;				// Высота и
	 FontWidth		 	= 4;				// ширина шрифта

	 iHeightDown	 	= (int)((double)iLength / 7.0);
	 iWidthDown		 	= (int)((double)iWidth  / 3.0);
	 iWidthLR		 	= (int)((double)iWidthDown  / 7.0);

//	------------------ Координаты фигур ---------------------
												// Размеры окна показания
	 RectValue.left   = AllRect.right  - iLengthValueBox - DELTAFROMSTOCK;
	 RectValue.top    = AllRect.bottom - iWidthValueBox  - DELTAFROMSTOCK;
	 RectValue.right  = AllRect.right  - DELTAFROMSTOCK;
	 RectValue.bottom = AllRect.bottom - DELTAFROMSTOCK;
												// Размеры низа корпуса
	 DownRectStock.left    = AllRect.left   + DELTAFROMSTOCK;
	 DownRectStock.top     = AllRect.bottom - DELTAFROMSTOCK - iHeightDown;
	 DownRectStock.right   = AllRect.left   + DELTAFROMSTOCK + iWidthDown;
	 DownRectStock.bottom  = AllRect.bottom - DELTAFROMSTOCK;
												// Размеры левой и правой части корпуса
	 LeftRectStock.left    = DownRectStock.left + DELTAFROMSTOCK;
	 LeftRectStock.top     = AllRect.top  + DELTAFROMSTOCK;
	 LeftRectStock.right   = LeftRectStock.left + iWidthLR;
	 LeftRectStock.bottom  = DownRectStock.top;

	 RightRectStock.left   = DownRectStock.right - iWidthLR - DELTAFROMSTOCK;
	 RightRectStock.top    = LeftRectStock.top;
	 RightRectStock.right  = RightRectStock.left + iWidthLR;
	 RightRectStock.bottom = LeftRectStock.bottom;

	 RectScale.left       = DownRectStock.right;
	 RectScale.top        = AllRect.top   + DELTAFROMSTOCK;
	 RectScale.right      = AllRect.right - DELTAFROMSTOCK;
	 RectScale.bottom     = DownRectStock.top;
												// Расстояние между засечками шкалы
	 dDeltaMark   = (double)((RectScale.bottom) - (RectScale.top)) /
									(double)QUANTMARKRECT;
												// Коэффициент отображения графика
	 dRatioValue  = (double)iRange / (dDeltaMark * (double)QUANTMARKRECT);

//	---------- Создание перьев, шрифтов, кистей -------------
												// Перо для удаления
												// старого показания графика
	 hPenLtGray  = CreatePen(PS_SOLID, 2, RGB(192, 192, 192));
												// Перо "ТЕНИ"
	 hPenGray    = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо графика
	 hPenGraph   = (HPEN)GetStockObject(BLACK_PEN);
												// Перо контура
	 hPenOutline = (HPEN)GetStockObject(BLACK_PEN);
												// Перо "ОСВЕЩЕНИЯ"
	 hPenWhite   = (HPEN)GetStockObject(WHITE_PEN);
	 lb.lbStyle = BS_SOLID;          // Кисть для штока - черного цвета
	 lb.lbColor = Color;
	 hbrColor = CreateBrushIndirect(&lb);
												// Кисть для шкалы - белого цвета
	 hbrWhite    = (HBRUSH)GetStockObject(WHITE_BRUSH);
												// Кисть для фона - серого цвета
	 hbrGray     = (HBRUSH)GetStockObject(GRAY_BRUSH);
												// Кисть для окна показания - светло-серого цвета
	 hbrLtGray   = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

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
Stock::~Stock()
{
	 DeleteObject(hPenLtGray);      	// Erase block memory for user pen
	 DeleteObject(hPenGray);      	// Erase block memory for user pen
	 DeleteObject(hPenGraph);     	// Erase block memory for user pen
	 DeleteObject(hPenOutline);      // Erase block memory for user pen
	 DeleteObject(hPenWhite);   	   // Erase block memory for user pen
	 DeleteObject((HBRUSH)hbrColor); // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrWhite); // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrGray);  // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrLtGray);// Erase block memory for user brush
	 DeleteObject((HFONT)hfontHelv); // Erase block memory for user font
};

/*-------------------------------------------------------------------*\
 *		Отображение штока																*
\*-------------------------------------------------------------------*/
void Stock::DrawStock(TDC& dc){           // DC окна
	 char  MiscStr[17]; 				        	// Строка отображения значений шкалы
	 div_t t;
	 double dx, dy;
	 TSize size;

// --------- Рисуем корпус датчика хода штока ---------------------
														//	Рамка
	 DrawLightBorder(dc, AllRect, hPenWhite, hPenGray, RAISED);

	 dc.SelectObject((HPEN)hPenOutline);  	// Выбор пера и заливки для корпуса
	 dc.SelectObject((HBRUSH)hbrGray);
	 dc.Rectangle(DownRectStock);				// Низ корпуса

	 dc.Rectangle(LeftRectStock );         // Левая и правая часть корпуса
	 dc.Rectangle(RightRectStock);

	 dc.SelectObject((HPEN)hPenGray);      // Рисуем окно показания
	 dc.SelectObject((HBRUSH)hbrLtGray);
	 dc.Rectangle(RectValue);
	 DrawLightBorder(dc, RectValue, hPenWhite, hPenGray, EMBOSSED);

	 dc.SelectObject((HPEN)hPenOutline);    // Рисуем окно шкалы
	 dc.SelectObject((HBRUSH)hbrWhite);
	 dc.Rectangle	 (RectScale);

// ---------------------- Ш к а л а -------------------------------
	 dc.SetTextColor(TColor::Black);			// Устанавливаем цвет текста
	 dc.SelectObject((HFONT)hfontHelv);
														// Ось шкалы
	 dc.MoveTo(RectScale.left + AXISDELTA, RectScale.bottom - 1);
	 dc.LineTo(RectScale.left + AXISDELTA, RectScale.top);
	 for (char i = 0; i <= QUANTMARKRECT * 2; i++){
		t  = div(i, 2);                     // ЧЕТ или НЕЧЕТ = ГЛАВНЫЕ или ВТОРИЧНЫЕ засечки
														// Координаты отображения засечек
														// и текста шкалы
		dx = (double)(RectScale.left + AXISDELTA);
		dy = (double)RectScale.bottom - (double)i * dDeltaMark / 2.0;

		if (t.rem == 0){							// Прорисовка ГЛАВНЫХ засечек
			if (i != 0){
				dc.MoveTo ((int)dx, (int)dy);
				dc.LineTo ((int)dx + iLengthMark / 2, (int)dy);
														// Формирование и прорисовка текста
				itoa(i * iStepScale / 2 + iMinValue, MiscStr, 10);
				dc.TextOut(RectScale.left + AXISDELTA + DELTAFROMSTOCK + iLengthMark / 2,
													(int)dy - 1, MiscStr);
			} else {     							// Рисуем текст размерности
				dc.GetTextExtent(sDim, strlen(sDim), size);
				dc.TextOut(RectScale.right - DELTAFROMSTOCK - size.cx,
													(int)dy - size.cy, sDim);
			};
		} else {										// Прорисовка ВТОРИЧНЫХ засечек
			dc.MoveTo ((int)dx, (int)dy);
			dc.LineTo ((int)dx + iLengthMark / 4, (int)dy);
		};
	 };
};

/*-------------------------------------------------------------------*\
 *		Отображение одного значения штока										*
\*-------------------------------------------------------------------*/
void Stock::DrawValueStock(TDC& dc,			// DC окна
									int  iIndex){  // индекс данного в циклическом массиве данных
	 double dValue = 0.0;
	 char   StValue[6];			 				// Строка отображения показания

	 dValue = ValuesInstruments[iIndex];

// ------------------ П о к а з а н и я ----------------------------
	 dc.SelectObject((HFONT)hfontHelv);
	 dc.SetTextColor(TColor::LtGray); 		// Устанавливаем цвет текста
														// Зарисуем старый текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StOldValue);

														// Сравнение для определения переполнения
	 if (dValue <= (double)(iMaxValue) && dValue >= (double)iMinValue){
		 bOVER = FALSE;                     // Сбросим флаг переполнения
		 dc.SetTextColor(TColor::Black);		// Устанавливаем цвет текста
														// Формируем текст показания
		 sprintf(StValue, "%3.1f", dValue);
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 if (dValue > (double)iMaxValue)
			 dValue = (double)iMaxValue;
		 if (dValue < (double)iMinValue)
			 dValue = (double)iMinValue;
		 dc.SetTextColor(TColor::LtRed);		// Устанавливаем цвет сообщения
		 sprintf(StValue, "OVER");
	 };												// Отображаем текст
	 dc.TextOut(RectValue.left + 2, RectValue.top + 2, StValue);
// ------------------ Г р а ф и к ---------------------------------
														// Либо дорисовываем кусочек,
	 DrawEraseStock(dc, dValue);				// либо вытираем

	 dOldValue = dValue;                   // Запомним предыдущее значение
	 strcpy(StOldValue, StValue);				// Строка числа предыдущего значения
};

void
Stock::DrawEraseStock(TDC& dc, double dValue){
	int Top, Bottom;
	double dOld;

	dValue = dValue    - (double)iMinValue;
	if (dValue < 0.0) dValue = 0.0;
	dOld   = dOldValue - (double)iMinValue;
	if (dOld   < 0.0) dOld  = 0.0;
	Top    = DownRectStock.top - (int)(max(dValue, dOld) / dRatioValue);
	Bottom = DownRectStock.top - (int)(min(dValue, dOld) / dRatioValue);
	if (Top > DownRectStock.top) return;

	if (dOldValue > dValue){
		dc.SelectObject((HPEN)hPenLtGray);
		dc.SelectObject((HBRUSH)hbrLtGray);
		dc.Rectangle(LeftRectStock.right + DELTAFROMSTOCK, Top,
						 RightRectStock.left - DELTAFROMSTOCK, Bottom);
	};
	dc.SelectObject((HPEN)hPenGraph);
	dc.SelectObject((HBRUSH)hbrColor);
	dc.Rectangle(LeftRectStock.right + DELTAFROMSTOCK,
					 DownRectStock.top - (int)(dValue / dRatioValue),
					 RightRectStock.left - DELTAFROMSTOCK,
					 DownRectStock.top);
};
/* ----------------------------------------------------------
 *	EoF(stock.cpp)
 * ----------------------------------------------------------
 */
