/*-------------------------------------------------------------*\
 * TInstrumentsBox - Class like TBGroupBox for output
 *
 * Author: Konstantin A. Davidov
 *		Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
\*-------------------------------------------------------------*/
#include "instrum.h"
#include "testsyst.rh"
/*
 *  -------------------------------------------------------------------------
 * 	Class TInstrumentsBox
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TInstrumentsBox, TGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
TInstrumentsBox::TInstrumentsBox(TWindow* parent, int ResId,
											TModule* module)
				 : TGroupBox(parent, ResId, module) {
/*-------------------- М у л ь т и м е т р -------------------------------*\
 *						А к т и в н а я   м о щ н о с т ь							  *
\*------------------------------------------------------------------------*/
	  ActivePowerRect.left   = DELTAFROMSENS * 8;
	  ActivePowerRect.top    = DELTAFROMSENS * 2;
	  ActivePowerRect.right  = ActivePowerRect.left + 130;
	  ActivePowerRect.bottom = ActivePowerRect.top  + 90;
/*-------------------- М у л ь т и м е т р -------------------------------*\
 *						Р е а к т и в н а я   м о щ н о с т ь						  *
\*------------------------------------------------------------------------*/
	  ReactivePowerRect.left   = ActivePowerRect.right + DELTAFROMSENS * 2;
	  ReactivePowerRect.top    = ActivePowerRect.top;
	  ReactivePowerRect.right  = ReactivePowerRect.left + 130;
	  ReactivePowerRect.bottom = ReactivePowerRect.top  + 90;
/*--------------------------- Б а р о м е т р ----------------------------*\
\*------------------------------------------------------------------------*/
	  RadiusBarom   = 58;
	  CenterBarom.x = DELTAFROMSENS + RadiusBarom;
	  CenterBarom.y = ReactivePowerRect.bottom + DELTAFROMSENS * 5 + RadiusBarom;
/*------------------- Р о з а    в е т р о в -----------------------------*\
\*------------------------------------------------------------------------*/
	  RadiusRoseWind   = RadiusBarom;
	  CenterRoseWind.x = CenterBarom.x + RadiusBarom +
								DELTAFROMSENS / 2 + RadiusRoseWind;
	  CenterRoseWind.y = CenterBarom.y;
/*------------ Н а п р а в л е н и е   и   о р и е н т а ц и я -----------*\
\*------------------------------------------------------------------------*/
	  RadiusDirWindHead   = RadiusBarom;
	  CenterDirWindHead.x = CenterRoseWind.x + RadiusRoseWind +
									DELTAFROMSENS / 2 + RadiusDirWindHead;
	  CenterDirWindHead.y = CenterRoseWind.y;
/*--------------------------- Т е р м о м е т р --------------------------*\
\*------------------------------------------------------------------------*/
	  ThermoRect.left   = DELTAFROMSENS * 2;
	  ThermoRect.top    = DELTAFROMSENS * 13 + CenterBarom.y + RadiusBarom;
	  ThermoRect.right  = ThermoRect.left + 60;
	  ThermoRect.bottom = ThermoRect.top  + 150;
/*----------------------- Скорость ветра ---------------------------------*\
 *						С к о р о с т ь     в е т р а         					  	  *
\*------------------------------------------------------------------------*/
	  SpeedWindRect.left   = ThermoRect.right + DELTAFROMSENS * 2;
	  SpeedWindRect.top    = ThermoRect.top;
	  SpeedWindRect.right  = SpeedWindRect.left   + 60;
	  SpeedWindRect.bottom = SpeedWindRect.top    + 150;
/*---------------------------- Ш т о к -----------------------------------*\
 *						Х о д    ш т о к а   N1               					  	  *
\*------------------------------------------------------------------------*/
	  StockRect1.left   = SpeedWindRect.right + DELTAFROMSENS * 2;
	  StockRect1.top    = SpeedWindRect.top;
	  StockRect1.right  = StockRect1.left   + 60;
	  StockRect1.bottom = StockRect1.top    + 150;
/*---------------------------- Ш т о к -----------------------------------*\
 *						Х о д    ш т о к а   N2               					  	  *
\*------------------------------------------------------------------------*/
	  StockRect2.left   = StockRect1.right + DELTAFROMSENS * 2;
	  StockRect2.top    = StockRect1.top;
	  StockRect2.right  = StockRect2.left + 60;
	  StockRect2.bottom = StockRect2.top  + 150;
/*----------------------- Скорость ветра ---------------------------------*\
 *						С к о р о с т ь     в е т р о к о л е с а				  	  *
\*------------------------------------------------------------------------*/
	  SpeedWndRtrRect.left   = StockRect2.right + DELTAFROMSENS * 2;
	  SpeedWndRtrRect.top    = StockRect1.top;
	  SpeedWndRtrRect.right  = SpeedWndRtrRect.left   + 60;
	  SpeedWndRtrRect.bottom = SpeedWndRtrRect.top    + 150;

						// Создание термометра
	  pThermom      = new Thermom  (ThermoRect,	aListParamInstruments[0].iMin,
									aListParamInstruments[0].iMax, aListParamInstruments[0].szNameVal);
						// Создание барометра
	  pBarometr     = new Barometr (CenterBarom, RadiusBarom,
									aListParamInstruments[1].iMin, aListParamInstruments[1].iMax,
									aListParamInstruments[1].szNameVal);
						// Создание мультиметра для реактивной мощности
	  pReactivePower = new Multimetr(ReactivePowerRect,
									aListParamInstruments[2].iMin, aListParamInstruments[2].iMax,
									aListParamInstruments[2].szNameVal,
									aListParamInstruments[2].Color);
						// Создание мультиметра для активной мощности
	  pActivePower   = new Multimetr(ActivePowerRect,
									aListParamInstruments[3].iMin, aListParamInstruments[3].iMax,
									aListParamInstruments[3].szNameVal,
									aListParamInstruments[3].Color);
						// Создание прибора хода штока N1
	  pStock1		  = new Stock(StockRect1, aListParamInstruments[4].iMin,
									aListParamInstruments[4].iMax, aListParamInstruments[4].szNameVal,
									aListParamInstruments[4].Color);
						// Создание прибора хода штока N2
	  pStock2		  = new Stock(StockRect2, aListParamInstruments[5].iMin,
									aListParamInstruments[5].iMax, aListParamInstruments[5].szNameVal,
									aListParamInstruments[5].Color);
						// Создание розы ветров
	  pRoseWind      = new RoseWind (CenterRoseWind, RadiusRoseWind);
						// Создание направления
	  pDirWindHead   = new DirWindHead(CenterDirWindHead,
									RadiusDirWindHead, aListParamInstruments[7].Color);
						// Создание прибора скорости ветра
	  pSpeedWind    = new Stock(SpeedWindRect, aListParamInstruments[8].iMin,
									aListParamInstruments[8].iMax, aListParamInstruments[8].szNameVal,
									aListParamInstruments[8].Color);
						// Создание прибора скорости ветра
	  pSpeedWndRtr  = new Stock(SpeedWndRtrRect, aListParamInstruments[9].iMin,
									aListParamInstruments[9].iMax, aListParamInstruments[9].szNameVal,
									aListParamInstruments[9].Color);
}; // End constructor

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r 														*
\*-------------------------------------------------------------------*/
TInstrumentsBox::~TInstrumentsBox(){
	  delete pSpeedWndRtr;
	  delete pSpeedWind;
	  delete pDirWindHead;
	  delete pRoseWind;
	  delete pStock2;
	  delete pStock1;
	  delete pActivePower;
	  delete pReactivePower;
	  delete pBarometr;
	  delete pThermom;
}; // End destructor

void
TInstrumentsBox::EvPaint(){
	TWindow::EvPaint();
};

void
TInstrumentsBox::Paint(TDC&, BOOL, TRect&){
	 ShowInstruments	 		();  // Отображение приборов
	 ShowValuesInstruments  ();  // Отображение данных на приборах
};

/*---------------------------------------------------------------------*\
 *	    Отображение приборов 										              *
\*---------------------------------------------------------------------*/
void
TInstrumentsBox::ShowInstruments(){
  int OldBKMode;							   // Old Backgrownd mode
  HDC MyHDC;

  MyHDC     = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // Запомним старый режим фона
  OldBKMode = dc.SetBkMode(TRANSPARENT);

  pThermom    ->DrawThermom	 (dc);   // Отображение термометра
  pBarometr   ->DrawBarometr	 (dc); 	// Отображение барометра
  pRoseWind   ->DrawRoseWind	 (dc);	// Отображение розы ветров
  pDirWindHead->DrawDirWindHead(dc);	// Отображение окна направления ветра
													// и ориентации головки ВЭУ
  pActivePower  ->DrawMultimetr(dc);	// Отображение мультиметра
  pReactivePower->DrawMultimetr(dc);	// Отображение мультиметра
  pStock1		 ->DrawStock	 (dc);	// Отображение прибора хода штока N1
  pStock2		 ->DrawStock	 (dc);	// Отображение прибора хода штока N2
  pSpeedWind	 ->DrawStock	 (dc);	// Отображение прибора скорости ветра
  pSpeedWndRtr	 ->DrawStock	 (dc);	// Отображение прибора скорости ветроколеса
  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};


/*---------------------------------------------------------------------*\
 *	    Отображения данных на приборах							              *
\*---------------------------------------------------------------------*/
void
TInstrumentsBox::ShowValuesInstruments(){
  int OldBKMode;							   // Старый режим фона
  HDC MyHDC;

  MyHDC = GetDC(this->HWindow);
  TDC dc(MyHDC);                       // Запомним старый режим фона
  OldBKMode = dc.SetBkMode(TRANSPARENT);

  pThermom ->DrawValueThermom  (dc, 0);// Отображение значения термометра
  pBarometr->DrawValueBarometr (dc, 1);// Отображение значения барометра
  pRoseWind->DrawValueRoseWind (dc, 6);// Отображение значения розы ветров
													// Отображение значения мощностей
  pReactivePower->DrawValueMultimetr(dc, 2);
  pActivePower  ->DrawValueMultimetr(dc, 3);

  pStock1->DrawValueStock		(dc, 4); // Отображение значения прибора хода штока N1
  pStock2->DrawValueStock		(dc, 5); // Отображение значения прибора хода штока N2
													// Отображение окна направления ветра
  pDirWindHead->DrawValueDirWind(dc, 6);
													// Отображение окна ориентации головки ВЭУ
  pDirWindHead->DrawValueDirHead(dc, 7);
													// Отображение прибора скорости ветра
  pSpeedWind  ->DrawValueStock  (dc, 8);
													// Отображение прибора скорости ветра
  pSpeedWndRtr->DrawValueStock  (dc, 9);

  dc.SetBkMode(OldBKMode);

  ReleaseDC(this->HWindow, MyHDC);
};

void
TInstrumentsBox::GetWindowClass(WNDCLASS& wndClass){
  TWindow::GetWindowClass(wndClass);
											// Фон окна
  wndClass.hbrBackground = (HBRUSH)GetStockObject(fnObject);
};
char far*
TInstrumentsBox::GetClassName(){
  return SHADE_CLASS;
};
/*
 * ---------------------------------------------------------
 *	EoF(instrum.cpp)
 * ----------------------------------------------------------
 */
