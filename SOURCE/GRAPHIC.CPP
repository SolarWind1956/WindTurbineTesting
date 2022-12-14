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
//-------------------- ? ? ? ? ? ? ? -------------------------------------
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
					  "???.",
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
									// ??????????? ???? ????????
	 ShowWindowGraphics(NumGroup);
	 ShowValuesGraphics();  // ??????????? ????????
};

/*---------------------------------------------------------------------*\
 *	    ??????????? ???? ????????									              *
\*---------------------------------------------------------------------*/
void
TGraphicsBox::ShowWindowGraphics( // ?????? ?????? ???????? ?? ??????? ???????
				int Group
				){
  int OldBKMode;							   // ?????? ????? ????
  HDC MyHDC;
  BOOL bShowLeft  = FALSE;
  BOOL bShowRight = FALSE;

  NumGroup  = Group;							// ???????? ?????? ?????? ??????? ?? ??????? ???????
  MyHDC     = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // ???????? ?????? ????? ????
  OldBKMode = dc.SetBkMode(TRANSPARENT);
													// ?????? ???????? ?????? ? ?????????
													// ????? ??? ???? ?????? ????????
  for(int i = 0; i < COUNTGRAPHICS; i++){
		if (aListParamGraphics[i].IndexGroupGraph == NumGroup){
													// ??????????? ???? ???????? ??? ?????	?????
			 if (!bShowLeft && aListParamGraphics[i].PosScale == LeftScale){
				  pGraphics[i]->DrawWindowGraphics (dc, LeftScale );
				  bShowLeft = TRUE;
			 };									// ??????????? ???? ???????? ??? ?????? ?????
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
 *	    ??????????? ?????? ?? ????????							              *
\*---------------------------------------------------------------------*/
void
TGraphicsBox::ShowValuesGraphics(){
  int  OldBKMode;							   // ?????? ????? ????
  HDC  MyHDC;

  MyHDC     = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // ???????? ?????? ????? ????
  OldBKMode = dc.SetBkMode(TRANSPARENT);
  if (!bGridCommand && !bGridOnOff){   // ???? ????? ????????? ?
													// ????? ??? ?? ??????, ??
													// ?????? ?????
		pGraphics[0]->DrawGridGraph(dc, bGridOnOff);
		bGridCommand = TRUE;             // ????? ??????
  };
  if (bGridOnOff){                     // ??????????? ?????
		pGraphics[0]->DrawGridGraph(dc, bGridOnOff);
		bGridCommand = FALSE;            // ????? ??????????
  };
													// ?????? ???????? ?????? ? ?????????
													// ??????? ???? ??????, ? ???????????
													// ?? ?????????????? ????, ??? ????? ???
													// ?????? ?????
  for(int i = 0; i < COUNTGRAPHICS; i++){
		if (aListParamGraphics[i].IndexGroupGraph == NumGroup){
													// ??????????? ????????
			pGraphics[i]->DrawGraphic (dc, BuffValues[i], LostValues[i]);
		};
  };
  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};

void
TGraphicsBox::GetWindowClass(WNDCLASS& wndClass){
  TWindow::GetWindowClass(wndClass);
											// ??? ????
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
