#if !defined(__ROSEWIND_H)
#define __ROSEWIND_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class RoseWind
	Создает модель наглядного отображения данных по направлению ветра в
	виде графика розы ветров
\*------------------------------------------------------------------------*/
#include "userdraw.h"

/*-----------------------------------------------------------------*\
 *		Class RoseWind                                               *
\*-----------------------------------------------------------------*/
class RoseWind {
public:
	 int      iRadiusAll, iRadiusRoseWind;
	 TPoint   CenterRoseWind;
	 int      iLengthAxis;
	 int  	 iLengthValueBox;       // Длина окна показания
	 int		 iWidthValueBox;			// Ширина окна показания
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта
	 BOOL 	 bOVER;  					// Флаг переполнения
	 TRect	 RectValue;					// Окно отображения
	 char		 StOldValue[17];			// Строка старого показания

	 TPoint DirectPoints   [COUNTRUMBS + 1];
	 TPoint OldDirectPoints[COUNTRUMBS + 1];

	 double dCountDirect    [COUNTRUMBS];
	 double dCountShowDirect[COUNTRUMBS];

	 LOGFONT  lf;							// Structure for create user font
	 LOGBRUSH lb;							// Structure for create user brush
												// Handle user pen
	 HPEN 	 hPenGray, hPenOutline, hPenGraph, hPenAxis, hPenWhite;
	 HBRUSH   hbrRose,  hbrBlack,    hbrLtGray;
	 HANDLE   hfontHelv;					// Handle user font

	 RoseWind(TPoint, int);
	 ~RoseWind ();
	 void DrawRoseWind(TDC&);
	 void DrawValueRoseWind(TDC&, int);
};
#endif  // __ROSEWIND_H
/* ----------------------------------------------------------
 *	EoF(rosewind.cpp)
 * ----------------------------------------------------------
 */

