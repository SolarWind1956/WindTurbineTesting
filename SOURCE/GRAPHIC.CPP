/*-------------------------------------------------------------*\
 * TGraphicsBox - Class like TBGroupBox for output
 *
 * Author: Konstantin A. Davidov
 *		Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
\*-------------------------------------------------------------*/
#include "graphic.h"
#include "testsyst.rh"
/*
 *  -------------------------------------------------------------------------
 * 	Class TGraphicsBox
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TGraphicsBox, TGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
TGraphicsBox::TGraphicsBox(TWindow* parent, int ResId,
											TModule* module)
				 : TGroupBox(parent, ResId, module) {
//-------------------- Г р а ф и к и -------------------------------------
	  GraphRect.left   = 3;
	  GraphRect.top    = 3;
	  GraphRect.right  = 400;
	  GraphRect.bottom = 330;
	  bGridCommand		 = FALSE;

	  for (int i = 0; i < COUNTGRAPHICS; i++){
			pGraphics[i] = new Graphics (
					  GraphRect,
					  0,
					  pViewTimer->uTimerPeriod / 1000 * QUANTVALUES,
					  "сек.",
					  aListParamGraphics[i].iMin,
					  aListParamGraphics[i].iMax,
					  aListParamGraphics[i].szNameVal,
					  aListParamGraphics[i].Color,
					  RGB(0, 0, 0)
												);
  }; // for (){...}
}; // End constructor

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r 														*
\*-------------------------------------------------------------------*/
TGraphicsBox::~TGraphicsBox() {
  for (int i = COUNTGRAPHICS-1; i >= 0; i--){
		delete pGraphics[i];
  }; // for (){...}
}; // End destructor

void
TGraphicsBox::EvPaint(){
	TWindow::EvPaint();
};

void
TGraphicsBox::Paint(TDC&, BOOL, TRect&){
									// Отображение окна графиков
	 ShowWindowGraphics(NumGroup);
	 ShowValuesGraphics();  // Отображение графиков
};

/*---------------------------------------------------------------------*\
 *	    Отображение окна графиков									              *
\*---------------------------------------------------------------------*/
void
TGraphicsBox::ShowWindowGraphics( // Индекс набора графиков из массива наборов
				int Group
				){
  int OldBKMode;							   // Старый режим фона
  HDC MyHDC;
  BOOL bShowLeft  = FALSE;
  BOOL bShowRight = FALSE;

  NumGroup  = Group;							// Запомним индекс набора графика из массива наборов
  MyHDC     = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // Запомним старый режим фона
  OldBKMode = dc.SetBkMode(TRANSPARENT);
													// Найдем заданную группу и отобразим
													// шкалы для этой группы графиков
  for(int i = 0; i < COUNTGRAPHICS; i++){
		if (aListParamGraphics[i].IndexGroupGraph == NumGroup){
													// Отображение окна графиков для левой	шкалы
			 if (!bShowLeft && aListParamGraphics[i].PosScale == LeftScale){
				  pGraphics[i]->DrawWindowGraphics (dc, LeftScale );
				  bShowLeft = TRUE;
			 };									// Отображение окна графиков для правой шкалы
			 if (!bShowRight && aListParamGraphics[i].PosScale == RightScale){
				  pGraphics[i]->DrawWindowGraphics (dc, RightScale);
				  bShowRight = TRUE;
			 };
			 pGraphics[i]->ShowLegenda(dc,
								  aListParamGraphics[i].szName,
								  aListParamGraphics[i].IndexElementGroup);
		};
  };
  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};

/*---------------------------------------------------------------------*\
 *	    Отображения данных на графиках							              *
\*---------------------------------------------------------------------*/
void
TGraphicsBox::ShowValuesGraphics(){
  int  OldBKMode;							   // Старый режим фона
  HDC  MyHDC;

  MyHDC     = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // Запомним старый режим фона
  OldBKMode = dc.SetBkMode(TRANSPARENT);
  if (!bGridCommand && !bGridOnOff){   // Если сетку выключили и
													// сетка еще не убрана, то
													// удалим сетку
		pGraphics[0]->DrawGridGraph(dc, bGridOnOff);
		bGridCommand = TRUE;             // Сетка убрана
  };
  if (bGridOnOff){                     // Отображение сетки
		pGraphics[0]->DrawGridGraph(dc, bGridOnOff);
		bGridCommand = FALSE;            // Сетка отображена
  };
													// Найдем заданную группу и отобразим
													// графики этой группы, в зависимости
													// от принадлежности шкал, для левой или
													// правой шкалы
  for(int i = 0; i < COUNTGRAPHICS; i++){
		if (aListParamGraphics[i].IndexGroupGraph == NumGroup){
													// Отображение графиков
			pGraphics[i]->DrawGraphic (dc, BuffValues[i], LostValues[i]);
		};
  };
  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};

void
TGraphicsBox::GetWindowClass(WNDCLASS& wndClass){
  TWindow::GetWindowClass(wndClass);
											// Фон окна
  wndClass.hbrBackground = (HBRUSH)GetStockObject(fnObject);
};
char far*
TGraphicsBox::GetClassName(){
  return SHADE_CLASS;
};
/*
 * ---------------------------------------------------------
 *	EoF(graphic.cpp)
 * ----------------------------------------------------------
 */
