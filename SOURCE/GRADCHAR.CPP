/*-------------------------------------------------------------*\
 * TGraduirCharact - Class like TBGroupBox for output          *
 *                                                             *
 * Author: Konstantin A. Davidov                               *
 *		Crimea Engineering Centre                                *
 *	   Phone: (06557) 68-2-39                                   *
 *                                                             *
 *	Description :                                               *
 *			Отображение градуировочной характеристики             *
 *			в виде графика                                        *
\*-------------------------------------------------------------*/
#include "gradchar.h"
#include "testsyst.rh"
/*
 * -------------------------------------------------------------------------
 * 	Class TGraduirCharact
 * -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TGraduirCharact, TGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

TGraduirCharact::TGraduirCharact(TWindow* parent, int ResId,
											BOOL bLegenda,
											TModule* module)
				 : TGroupBox(parent, ResId, module) {

     bShowCharact 	 = FALSE;
	  GraphRect.left   = 1;
	  GraphRect.top    = 1;
	  GraphRect.right  = 309;
	  GraphRect.bottom = 285;

	  pGradCharact   = new Graphics(GraphRect, 0, 10, " ",
															 0, 10, " ",
											  RGB(255, 0, 0), RGB(192, 192, 192),
											  PS_SOLID, bLegenda);
	  pApproxCharact = new Graphics (GraphRect, 0, 10, " ",
															  0, 10, " ",
											  RGB(0, 255, 0), RGB(192, 192, 192),
											  PS_SOLID, bLegenda);
}; // End constructor

void
TGraduirCharact::ChangeParamGraphic(double dMinX, double dMaxX, char* sDimX,
												double dMinY, double dMaxY, char* sDimY){
										// Изменение параметров графиков
	pGradCharact->  ChangeParamGraphWindow(GraphRect, dMinX,	dMaxX, sDimX,
																	  dMinY, dMaxY, sDimY);
	pApproxCharact->ChangeParamGraphWindow(GraphRect, dMinX,	dMaxX, sDimX,
																	  dMinY, dMaxY, sDimY);
}; // End of Function

void
TGraduirCharact::EvPaint(){
	TWindow::EvPaint();
};
void
TGraduirCharact::Paint(TDC&, BOOL, TRect&){
	if (bShowCharact){	      // Отображение графиков
		ShowWindowGraphics();   // Отображение окна графиков
		ShowValuesGraphics();
	};
};
/*---------------------------------------------------------------------*\
 *	    Отображение окна графиков									              *
\*---------------------------------------------------------------------*/
void
TGraduirCharact::ShowWindowGraphics(){
  int  OldBKMode;							   // Старый режим фона
  HDC  MyHDC;
  BOOL bShowLeft  = FALSE;
  MyHDC     = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // Запомним старый режим фона
  OldBKMode = dc.SetBkMode(TRANSPARENT);
													// Отобразим шкалы для этого графика
  if (!bShowLeft){                     // Отображение окна графиков для левой	шкалы
	  pGradCharact->DrawWindowGraphics (dc, LeftScale);
	  bShowLeft = TRUE;
  };
  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};
/*---------------------------------------------------------------------*\
 *	    Отображения данных на графиках							              *
\*---------------------------------------------------------------------*/
void
TGraduirCharact::ShowValuesGraphics(){
  int  OldBKMode;							   // Старый режим фона
  HDC  MyHDC;

  MyHDC      = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // Запомним старый режим фона
  OldBKMode  = dc.SetBkMode(TRANSPARENT);
													// ЛИНЕЙНЫЙ или КУСОЧНОЛИНЕЙНЫЙ
  if (TypeApprox < 2){
	  pGradCharact->  DrawGraphicChar(dc, CharBufferX, // Данные по X
													  CharBufferY, // Данные по Y
													  LastValuesX, // Прошлые данные по X
													  LastValuesY, // Прошлые данные по Y
													  QUANTVALUES);// Кол-во данных в массиве
  } else											// Остальные с аппроксимацией
	  pApproxCharact->DrawGraphicChar(dc, CharBufferX, // Данные по X
													  CharBufferY, // Данные по Y
													  LastValuesX, // Прошлые данные по X
													  LastValuesY, // Прошлые данные по Y
													  QUANTVALUES);// Кол-во данных в массиве
  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};

void
TGraduirCharact::ClearGraphicWindow(){
	HDC  MyHDC;
	MyHDC     = GetDC(this->HWindow);
	TDC dc(MyHDC);
	dc.SelectObject((HBRUSH)GetStockObject(LTGRAY_BRUSH));
	dc.PatBlt(GraphRect, PATCOPY);
	ReleaseDC(this->HWindow, MyHDC);
};

void
TGraduirCharact::GetWindowClass(WNDCLASS& wndClass){
  TWindow::GetWindowClass(wndClass);
											// Фон окна
  wndClass.hbrBackground = (HBRUSH)GetStockObject(fnObject);
};
char far*
TGraduirCharact::GetClassName(){
  return SHADE_CLASS;
};
/*
 * ---------------------------------------------------------
 *	EoF(gradchar.cpp)
 * ----------------------------------------------------------
 */
