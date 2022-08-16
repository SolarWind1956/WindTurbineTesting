#if !defined(__MULTMETR_H)
#define __MULTMETR_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Multimetr
	Создает модель наглядного отображения данных на мультиметре
\*------------------------------------------------------------------------*/
#include "userdraw.h"

class Multimetr {
public:
	 int iMinValue;  						// Минимальное допустимое значение мультиметра
	 int iMaxValue;						// Максимальное допустимое значение мультиметра
	 int iRange;     						// Диапазон мультиметра
												// Прямоугольники: общий,
												// окна показания, корпуса мультиметра,
												// окна мультиметра
	 TRect    AllRect, RectValue, RectMultim, RectWind;
	 int   	 iLengthNeedle;         // Длина стрелки
	 int   	 iHeight, iWidth;       // Его длина и ширина
	 int 		 iLengthValueBox;   		// Длина окна показания
	 int 		 iWidthValueBox;			// Ширина окна показания
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта
	 char		 sDim[25];              // Размерность
	 int		 iRadiusOut;			  	// Радиус крепления стрелки

	 double   dOldRadCorner;  			// Значение для удаления старого графика
	 char		 StOldValue[6];			// Строка для удаления старого значения

	 LOGFONT  lf;							// Structure for create user font
	 HANDLE   hfontHelv;					// Handle user font
	 HPEN 	 hPenGray,              // Handle user pen
				 hPenGraph,
				 hPenOutline,
				 hPenWhite;
	 LOGBRUSH lb;
	 HBRUSH   hbrWhite,              // Handle user brush
				 hbrGray,
				 hbrLtGray,
				 hbrBlack;
	 BOOL 	 bOVER;   					// Флаг переполнения

	 Multimetr(TRect, int, int, char*, COLORREF);
	 ~Multimetr ();

	 void DrawMultimetr(TDC&);
	 void DrawValueMultimetr(TDC&, int);
};
#endif  // __MULTMETR_H
/* ----------------------------------------------------------
 *	EoF(multimetr.h)
 * ----------------------------------------------------------
 */
