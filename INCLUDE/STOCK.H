#if !defined(__STOCK_H)
#define __STOCK_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Stock
	Создает модель наглядного отображения данных по датчику хода штока
\*------------------------------------------------------------------------*/
#include "userdraw.h"

class Stock {
public:
	 int iMinValue;  						// Минимальное допустимое значение шток
	 int iMaxValue;						// Максимальное допустимое значение шток
	 int iRange;     						// Диапазон шток
	 int iStepScale;                 // Шаг шкалы
	 char sDim[25];				      // Размерность
												// Прямоугольники: общий,
												// окна показания, корпуса
	 TRect    AllRect,
				 RectValue,
				 DownRectStock,
				 LeftRectStock,
				 RightRectStock,
				 RectScale;
	 int   	 iLength, iWidth;       // Его длина и ширина
	 int 	 	 iHeightDown, iWidthDown; // Высота и ширина низа корпуса
	 int 		 iLengthValueBox;   		// Длина окна показания
	 int 		 iWidthValueBox;			// Ширина окна показания
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта
	 int      iWidthLR;					// Ширина левой и правой части корпуса

	 LOGFONT  lf;							// Structure for create user font
	 HANDLE   hfontHelv;					// Handle user font
	 HPEN 	 hPenGray,              // Handle user pen
				 hPenLtGray,
				 hPenGraph,
				 hPenOutline,
				 hPenWhite;
	 LOGBRUSH lb;
	 HBRUSH   hbrWhite,              // Handle user brush
				 hbrColor,
				 hbrGray,
				 hbrLtGray;
	 double	 dDeltaMark;             // Расстояние между засечками
	 int 		 iLengthMark;   			// Длинна засечки
	 double 	 dRatioValue;				// Коэффициент отображения графика
	 BOOL 	 bOVER;   					// Флаг переполнения

	 double dOldValue;
	 char   StOldValue[6];

	 Stock(TRect, int, int, char*, COLORREF);
	 ~Stock ();

	 void DrawStock(TDC&);
	 void DrawValueStock(TDC&, int);
	 void DrawEraseStock(TDC& dc, double dValue);
};
#endif  // __STOCK_H
/* ----------------------------------------------------------
 *	EoF(stock.h)
 * ----------------------------------------------------------
 */
