                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ld)
	EV_WM_SIZE,
	EV_WM_PAINT,
	EV_WM_TIMER,
	EV_WM_CLOSE,
END_RESPONSE_TABLE;

//////////////////////////////////////////////////////////
// CharactPowerMDIChild
// ==========
// Construction/Destruction handling.
CharactPowerMDIChild::CharactPowerMDIChild (TMDIClient 	 &parent,
														  const char far *title,
														  TWindow 		 *clientWnd,
														  BOOL 			 shrinkToClient,
														  TModule 		 *module)
	 : TMDIChild (parent, title, clientWnd, shrinkToClient, module)
{
	Attr.Style &= ~(WS_MAXIMIZEBOX); // ????????? ????
	Attr.W		  	  = 500;
	Attr.H		  	  = 300;

	GraphRect.left   = 0;            // ????????? ????????
	GraphRect.top    = 0;
	GraphRect.right  = Attr.W - 8;
	GraphRect.bottom = Attr.H - GetSystemMetrics(SM_CYCAPTION) - 7;
	strcpy(sDimension, "???.");
	iMin = 0;
	iMax = 250;
	bWindowPowerChar = TRUE;
	pGraphicNorm = new Graphics (GraphRect, 0, 25, "?/?",
										iMin, iMax, sDimension,
										RGB(0, 0, 0), RGB(255, 255, 255), PS_DOT);
	pGraphicMin = new Graphics (GraphRect, 0, 25, "?/?",
										iMin, iMax, sDimension,
										RGB(0, 0, 255), RGB(255, 255, 255), PS_DASH);
	pGraphicAvr = new Graphics (GraphRect, 0, 25, "?/?",
										iMin, iMax, sDimension,
										RGB(255, 0, 0), RGB(255, 255, 255));
	pGraphicMax = new Graphics (GraphRect, 0, 25, "?/?",
										iMin, iMax, sDimension,
										RGB(125, 125, 0), RGB(255, 255, 255), PS_DASHDOT);
	for (char i = 0; i < BINQuant; i++){
		 LastValuesWind[i] = ValuesWind[i] = 0.0;
		 LastValuesMin [i] = ValuesMin [i] = 0.0;
		 LastValuesAvr [i] = ValuesAvr [i] = 0.0;
		 LastValuesMax [i] = ValuesMax [i] = 0.0;
		 LastValuesNorm[i] = ValuesNorm[i] = 0.0;
	};
};	// End constructor

CharactPowerMDIChild::~CharactPowerMDIChild (){
	bWindowPowerChar  = FALSE;
	Destroy();
	delete pGraphicMax;
	delete pGraphicAvr;
	delete pGraphicMin;
	delete pGraphicNorm;
}; // End destructor

/*
 *		??????? ???????? ??????? ???? ? ?????????????? ???? ???????
 */
void
CharactPowerMDIChild::SetupWindow(){
	TWindow::SetupWindow();
											// ????????? ?????? ????? ?????? ????.?????
	pPowerTimer->SetUserTimer(this->HWindow);
};

/*
 *		??????? ???????? ??????? ???? ? ?????????????? ???? ???????
 */
void
CharactPowerMDIChild::EvSize(UINT /*sizeType*/, TSize& size){
	GraphRect.left   = 0;
	GraphRect.top    = 0;
	GraphRect.right  = size.cx;
	GraphRect.bottom = size.cy;
	pGraphicNorm->ChangeParamGraphWindow(GraphRect, 0, 25, "?/?",
											iMin, iMax, sDimension);
	pGraphicMin->ChangeParamGraphWindow(GraphRect, 0, 25, "?/?",
											iMin, iMax, sDimension);
	pGraphicAvr->ChangeParamGraphWindow(GraphRect, 0, 25, "?/?",
											iMin, iMax, sDimension);
	pGraphicMax->ChangeParamGraphWindow(GraphRect, 0, 25, "?/?",
											iMin, iMax, sDimension);
};

void
CharactPowerMDIChild::EvPaint(){
	TWindow::EvPaint();
};
void
CharactPowerMDIChild::Paint(TDC&, BOOL, TRect&){
	ShowWindowGraphics(); // ??????????? ???? ????????
	ShowValuesGraphics(); // ??????????? ????????
};

/*---------------------------------------------------------------------*\
 *	    ??????????? ???? ????????									              *
\*---------------------------------------------------------------------*/
void
CharactPowerMDIChild::ShowWindowGraphics(){
	int OldBKMode;							   // ?????? ????? ????
	HDC MyHDC;

	MyHDC     = GetDC(this->HWindow);
	TDC dc(MyHDC);                      // ???????? ?????? ????? ????
	OldBKMode = dc.SetBkMode(TRANSPARENT);
													// ????????? ????? ??? ???????
													// ??????????? ???? ???????? ??? ????
	pGraphicAvr->DrawWindowGraphics (dc, LeftScale );
	pGraphicAvr->DrawWindowGraphics (dc, RightScale );
													// ??????????? ??????? ????????
	pGraphicMin->ShowLegenda (dc, "??????????? ???????? ???????? ? ????",  0);
	pGraphicAvr->ShowLegenda (dc, "??????? ???????? ???????? ? ????",  	  1);
	pGraphicMax->ShowLegenda (dc, "???????????? ???????? ???????? ? ????", 2);
	pGraphicNorm->ShowLegenda(dc, "????????? ?????????????? ????????",     3);
	dc.SetBkMode(OldBKMode);

	ReleaseDC(this->HWindow, MyHDC);
};

/*---------------------------------------------------------------------*\
 *	    ??????????? ?????? ?? ????????							              *
\*---------------------------------------------------------------------*/
void
CharactPowerMDIChild::ShowValuesGraphics(){
	int  OldBKMode;						   // ?????? ????? ????
	HDC  MyHDC;

	MyHDC     = GetDC(this->HWindow);
	TDC dc(MyHDC);                      // ???????? ?????? ????? ????
	OldBKMode = dc.SetBkMode(TRANSPARENT);
													// ??????????? ?????
	pGraphicAvr->DrawGridGraph(dc, TRUE);
													// ??????????? ????????
	pGraphicNorm->DrawGraphicChar(dc, ValuesWind, ValuesNorm, LastValuesWind, LastValuesNorm,
							BINQuant);
	pGraphicMax->DrawGraphicChar (dc, ValuesWind, ValuesMax, LastValuesWind,  LastValuesMax,
							BINQuant);
	pGraphicMin->DrawGraphicChar (dc, ValuesWind, ValuesMin, LastValuesWind,  LastValuesMin,
							BINQuant);
	pGraphicAvr->DrawGraphicChar (dc, ValuesWind, ValuesAvr, LastValuesWind,  LastValuesAvr,
							BINQuant);
	dc.SetBkMode(OldBKMode);

	ReleaseDC(this->HWindow, MyHDC);
};

/*
 *-------------------------------------------------------------
 *		??????????? ?????????? ??????? ??????????? ??????
 *-------------------------------------------------------------
 */
void
CharactPowerMDIChild::EvTimer(UINT timerId){
	if (timerId == pPowerTimer->IDEvTimer){
											// ???? ????????? ????????,
											// ?? ? ?? ??????? ????
		 if (bWriteProtON == FALSE){
			 EvClose();
			 return;
		 };
		 if (pPowerTimer->TestUserTimer()){
											// ??????? ????? ?????? ? ??????
			  for (char i = 0; i < BINQuant; i++){
					 ValuesWind[i] = TB[i].AvrWindSpeed;
					 ValuesMin [i] = TB[i].MinNetPower;
					 ValuesAvr [i] = TB[i].AvrNetPower;
					 ValuesMax [i] = TB[i].MaxNetPower;
					 ValuesNorm[i] = TB[i].NormNetPower;
			  };
			  ShowValuesGraphics(); // ??????????? ???????
											// ???????? ?????? ?????? ??? ???????? ???????
			  for (i = 0; i < BINQuant; i++){
					 LastValuesWind[i] = ValuesWind[i];
					 LastValuesMin [i] = ValuesMin [i];
					 LastValuesAvr [i] = ValuesAvr [i];
					 LastValuesMax [i] = ValuesMax [i];
					 LastValuesNorm[i] = ValuesNorm[i];
			  };
		 };
	};
};

/*---------------------------------------------------------------------*\
 *	    ????????? ????, ?? ?????? ????? ??????					           *
\*---------------------------------------------------------------------*/
void
CharactPowerMDIChild::EvClose(){
	pPowerTimer->KillUserTimer();	// ????? ??????
	TWindow::EvClose();
};

void
CharactPowerMDIChild::GetWindowClass(WNDCLASS& wndClass){
	TWindow::GetWindowClass(wndClass);
											// ??? ????
	wndClass.hbrBackground = (HBRUSH)GetStockObject(fnObject);
};
char far*
CharactPowerMDIChild::GetClassName(){
	return SHADE_CLASS;
};
/*
 * ---------------------------------------------------------
 *	EoF(charpowr.cpp)
 * ----------------------------------------------------------
 */
