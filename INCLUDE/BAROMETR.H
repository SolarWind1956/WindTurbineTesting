#if !defined(__BAROMETR_H)
#define __BAROMETR_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Barometr
	Создает модель наглядного отображения данных по барометрическому давлению
\*------------------------------------------------------------------------*/
#include "userdraw.h"

/*-----------------------------------------------------------------*\
 *		Class Barometr                                               *
\*-----------------------------------------------------------------*/
class Barometr {
public:
	 int  	 iMaxValue;
	 int  	 iMinValue;
	 int  	 iRange;

	 int      iRadiusTwo;
	 int      iRadiusOut;
	 int      iRadius;
	 int      iRadiusSun;
	 int		 iLengthCloud;
	 int		 iHeightCloud;
	 TPoint    Center;
	 int      iLengthNeedle;
	 int  	 iLengthTextBox;		   // Длина окна показания
	 char		 sDim   [25];
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта
	 BOOL 	 bOVER;  					// Флаг переполнения
	 double 	 dOldRadCorner;
	 char     StOldValue[6];
	 LOGFONT  lf;							// Structure for create user font
	 LOGBRUSH lb;							// Structure for create user brush
												// Handle user pen
	 HPEN 	 hPenGray, hPenSun, hPenOutline, hPenNeedle, hPenWhite;
	 HBRUSH   hbrWhite, hbrGray, hbrLtGray, hbrSun, hbrCloud;
	 HANDLE   hfontHelv;					// Handle user font

	 Barometr(TPoint, int, int, int, char*);
	 ~Barometr ();
	 void DrawBarometr(TDC&);
	 void DrawValueBarometr(TDC&, int);
};
#endif  // __BAROMETR_H
/* ----------------------------------------------------------
 *	EoF(barometr.cpp)
 * ----------------------------------------------------------
 */
