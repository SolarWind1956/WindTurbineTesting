#if !defined(__WINDHEAD_H)
#define __WINDHEAD_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class DirWindHead
	Создает модель наглядного отображения данных по направлению ветра
	и головки ВЭУ
\*------------------------------------------------------------------------*/
#include "userdraw.h"
/*-----------------------------------------------------------------*\
 *		Class DirWindHead                                            *
\*-----------------------------------------------------------------*/
class DirWindHead {
public:
	 TypeDirectObject DirectObject;  // Тип объекта направления
	 int      iRadiusAll, iRadiusDirWindHead;
	 TPoint    CenterDirWindHead;
	 int      iLengthNeedle;		 	// Длина стрелки направления ветра
	 int  	 iLengthValueBox;       // Длина окна показания
	 int		 iWidthValueBox;			// Ширина окна показания
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта
	 BOOL 	 bOVER;  					// Флаг переполнения
	 TRect	 RectWindValue;  			// Окно отображения направления
	 TRect	 RectHeadValue;  			// Окно отображения ориентации
	 char		 StOldWindValue[6];		// Строка старого показания
	 char		 StOldHeadValue[6];		// Строка старого показания

	 double	 dOldWindRadCorner;	 	// Старое данное
	 double	 dOldHeadRadCorner; 	 	// Старое данное
	 TPoint   OldTip1, OldTip2, OldTip3, OldCenter;

	 LOGFONT  lf;							// Structure for create user font
	 LOGBRUSH lb;							// Structure for create user brush
												// Handle user pen
	 HPEN     hPenErase,
				 hPenGray,
				 hPenWhite,
				 hPenAxis,
				 hPenHead,
				 hPenNeedle;
	 HBRUSH   hbrHead,
				 hbrNeedle,
				 hbrLtGray;
	 HANDLE   hfontHelv;					// Handle user font

	 DirWindHead(TPoint, int, COLORREF);
	 ~DirWindHead ();
	 void DrawDirWindHead(TDC&);
	 void DrawValueDirHead(TDC&, int);
	 void DrawValueDirWind(TDC&, int);
	 void DrawShapeDirWind(TDC&, double);
	 void DrawShapeDirHead(TDC&, double);
};	 
#endif  // __WINDHEAD_H
/* ----------------------------------------------------------
 *	EoF(windhead.h)
 * ----------------------------------------------------------
 */
