#if !defined(__USERGRPH_H)
#define __USERGRPH_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Graphics
	Создает модель наглядного отображения данных в виде графиков
\*------------------------------------------------------------------------*/
#include "userdraw.h"

/*---------------------------------------------------------------------*\
 *  Class Graphics                                                     *
\*---------------------------------------------------------------------*/
class Graphics {
public:              					// Позиция шкалы
	 double dMinValueX; 					// Минимальное допустимое значение
	 double dMaxValueX; 					// Максимальное допустимое значение
	 double dRangeX;    					// Диапазон
	 char   sDimX   [25];  		      // Размерность

	 double dMinValueY; 					// Минимальное допустимое значение
	 double dMaxValueY; 					// Максимальное допустимое значение
	 double dRangeY;    					// Диапазон
	 char   sDimY   [25];  		      // Размерность
												// Прямоугольники: общий,
												// окна графика
	 TRect    AllRect, GraphicRect;
	 int 		 FontHeight, FontWidth; // Высота и ширина шрифта
    int 		 DeltaForLegenda;			// Отступ графика от границы окна для легенды
	 LOGFONT  lf;							// Structure for create user font
	 LOGBRUSH lb;							// Structure for create user brush
	 HANDLE   hfontHelv;					// Handle user font

	 HPEN 	 hPenGray;              // Handles user pens
	 HPEN 	 hPenLtGray;
	 HPEN		 hPenScale;
	 HPEN		 hPenGraph;
	 HPEN		 hPenGrid;
	 HPEN		 hPenWhite;
	 HPEN		 hPenErase;

	 HBRUSH   hbrBlack;              // Handles user brushes
	 HBRUSH   hbrLtGray;
	 double 	 dRatioValX, dRatioValY;// Коэффициент отображения графика
	 BOOL 	 bOVER;   					// Флаг переполнения

	 Graphics(TRect GraphRect,
				 double dMinX, double dMaxX, char* sDimensionX,
				 double dMinY, double dMaxY, char* sDimensionY,
				 COLORREF Color, COLORREF BackGround, int fnPenStyle = PS_SOLID,
				 BOOL   bLegenda = TRUE);
	 ~Graphics ();

	 void ChangeParamGraphWindow(
			TRect GraphRect,					// размер и положение окна графиков
			double dMinX,			  		 	// минимальное показание по X
			double dMaxX,                 // максимальное показание по X
			char*  sDimensionX,           // текст размерности по X
			double dMinY, 						// минимальное показание по Y
			double dMaxY,  		  			// максимальное показание по Y
			char*  sDimensionY);          // текст размерности по Y
	 void DrawWindowGraphics(TDC& dc, TypePosScale TypePos);
	 void DrawGraphic 		(TDC& dc, TCycleBuffer, double);
	 void DrawGraphicChar	(TDC&   dc,      	   // DC окна
									 double CharBufferX[],// Данные по X
									 double CharBufferY[],// Данные по Y
									 double dLastValuesX[],
									 double dLastValuesY[],
									 const int iCount);	// Кол-во данных в массиве
	 void DrawGridGraph		(TDC& dc, BOOL bGridOnOff);
	 void ShowLegenda			(TDC& dc, char* szName, int iIndex);
};
#endif  // __USERGRPH_H
/* ----------------------------------------------------------
 *	EoF(usergrph.h)
 * ----------------------------------------------------------
 */
