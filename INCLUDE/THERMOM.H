#if !defined(__THERMOM_H)
#define __THERMOM_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Termom
	Создает модель наглядного отображения данных по температуре
\*------------------------------------------------------------------------*/
#include "userdraw.h"

class Thermom {
public:
	 int iMinValue;  						// Минимальное допустимое значение термометра
	 int iMaxValue;						// Максимальное допустимое значение термометра
	 int iRange;     						// Диапазон термометра
	 int iStepScale;                 // Шаг шкалы
	 char sDim[25];				      // Размерность
												// Прямоугольники: общий,
												// окна показания, термометра, носика
	 TRect    AllRect, RectValue, RectTerm, RectHead;
	 TPoint   RoundCorner;
	 int   	 Length, Width;         // Его длина и ширина
	 int   	 iHeadLenght;           // Длина носика термометра
	 int   	 iHeadWidth;            // Ширина носика
	 int 		 iLengthValueBox;   		// Длина окна показания
	 int 		 iWidthValueBox;			// Ширина окна показания
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта

	 LOGFONT  lf;							// Structure for create user font
	 HANDLE   hfontHelv;					// Handle user font
	 HPEN 	 hPenGray,              // Handle user pen
				 hPenDelete,
				 hPenLtGray,
				 hPenGraph,
				 hPenOutline,
				 hPenWhite;
	 LOGBRUSH lb;
	 HBRUSH   hbrWhite,              // Handle user brush
				 hbrRed,
				 hbrGray,
				 hbrLtGray;
	 double	 dDeltaMark;            // Расстояние между засечками
	 int 		 iLengthMark;   			// Длинна засечки
	 double 	 dRatioValue;				// Коэффициент отображения графика
	 BOOL 	 bOVER;   					// Флаг переполнения

	 double dOldValue;
	 char   StOldValue[6];

	 Thermom(TRect, int, int, char*);
	 ~Thermom ();

	 void DrawThermom(TDC&);
	 void DrawValueThermom(TDC&, int);
};
#endif  // __THERMOM_H
/* ----------------------------------------------------------
 *	EoF(thermom.h)
 * ----------------------------------------------------------
 */

