/*  Project powercha
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    powercha.exe Application
	 FILE:         charstck.cpp
	 AUTHOR:       Heckel & Davidov


	 OVERVIEW
	 ========
	 Source file for implementation of CharactStocksMDIChild (TMDIChild).
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include "powercha.h"
#include "charstck.h"

#include <stdio.h>

extern BOOL bWindowStocksChar;

DEFINE_RESPONSE_TABLE1(CharactStocksMDIChild, TMDIChild)
	EV_WM_SIZE,
	EV_WM_PAINT,
	EV_WM_TIMER,
	EV_WM_CLOSE,
END_RESPONSE_TABLE;

//////////////////////////////////////////////////////////
// CharactStocksMDIChild
// ==========
// Construction/Destruction handling.
CharactStocksMDIChild::CharactStocksMDIChild (TMDIClient 	 &parent,
															 const char far *title,
															 TWindow 		 *clientWnd,
															 BOOL 			 shrinkToClient,
															 TModule 		 *module)
	 : TMDIChild (parent, title, clientWnd, shrinkToClient, module)
{
	Attr.Style &= ~(WS_MAXIMIZEBOX); // Аттрибуты окна
	Attr.W		  	  = 500;
	Attr.H		  	  = 300;

	GraphRect.left   = 0;            // Аттрибуты графиков
	GraphRect.top    = 0;
	GraphRect.right  = Attr.W - 8;
	GraphRect.bottom = Attr.H - GetSystemMetrics(SM_CYCAPTION) - 7;
	strcpy(sDimension, "Мм.");
	iMin = 0;
	iMax = 350;
	bWindowStocksChar = TRUE;
	pGraphic = new Graphics (GraphRect, 0, 25, "м/с",
										iMin, iMax, sDimension,
										RGB(255, 0, 0), RGB(255, 255, 255));
};	// End constructor

CharactStocksMDIChild::~CharactStocksMDIChild (){
	bWindowStocksChar = FALSE;
	Destroy();
	delete pGraphic;
}; // End destructor

/*
 *		Функция изменяет размеры окна и соответственно окна графика
 */
void
CharactStocksMDIChild::SetupWindow(){
	TWindow::SetupWindow();
											// Установим таимер цикла чтения цикл.файла
	pStocksTimer->SetUserTimer(this->HWindow);
};

/*
 *		Функция изменяет размеры окна и соответственно окна графика
 */
void
CharactStocksMDIChild::EvSize(UINT /*sizeType*/, TSize& size){
	GraphRect.left   = 0;
	GraphRect.top    = 0;
	GraphRect.right  = size.cx;
	GraphRect.bottom = size.cy;
	pGraphic->ChangeParamGraphWindow(GraphRect, 0, 25, "м/с",
											iMin, iMax, sDimension);
};

void
CharactStocksMDIChild::EvPaint(){
	TWindow::EvPaint();
};
void
CharactStocksMDIChild::Paint(TDC&, BOOL, TRect&){
	ShowWindowGraphics(); // Отображение окна графиков
	ShowValuesGraphics(); // Отображение графиков
};

/*-----------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ие окна графиков для левой	шкалы
	pGraphic->DrawWindowGraphics (dc, LeftScale );
													// Отображение окна графиков для правой шкалы
	pGraphic->DrawWindowGraphics (dc, RightScale);
													// Отображение легенды графиков
	pGraphic->ShowLegenda(dc, "Место легенды", 0);
	dc.SetBkMode(OldBKMode);

	ReleaseDC(this->HWindow, MyHDC);
};

/*---------------------------------------------------------------------*\
 *	    Отображения данных на графиках							              *
\*---------------------------------------------------------------------*/
void
CharactStocksMDIChild::ShowValuesGraphics(){
	int  OldBKMode;						   // Старый режим фона
	HDC  MyHDC;

	MyHDC     = GetDC(this->HWindow);
	TDC dc(MyHDC);                      // Запомним старый режим фона
	OldBKMode = dc.SetBkMode(TRANSPARENT);

	pGraphic->DrawGridGraph(dc, TRUE);  // Отображение сетки
													// Отображение графиков
//	pGraphic->DrawGraphic (dc, ValuesX, ValuesY,
//							LostValuesX, LostValuesY, iCountValues);
	dc.SetBkMode(OldBKMode);

	ReleaseDC(this->HWindow, MyHDC);
};

/*
 *-------------------------------------------------------------
 *		Циклическое выполнение функции отображения данных
 *-------------------------------------------------------------
 */
void
CharactStocksMDIChild::EvTimer(UINT timerId){
	if (timerId == pStocksTimer->IDEvTimer){
											// Если выключили протокол,
											// то и мы закроем окно
		 if (bWriteProtON == FALSE){
			 EvClose();
			 return;
		 };
		 if (pStocksTimer->TestUserTimer()){
			  ShowValuesGraphics(); // Отображение графика
		 };
	};
};

/*---------------------------------------------------------------------*\
 *	    Закрываем окно, но прежде убъем таймер					           *
\*---------------------------------------------------------------------*/
void
CharactStocksMDIChild::EvClose(){
	pStocksTimer->KillUserTimer();	// Убъем таймер
	TWindow::EvClose();
};

void
CharactStocksMDIChild::GetWindowClass(WNDCLASS& wndClass){
	TWindow::GetWindowClass(wndClass);
											// Фон окна
	wndClass.hbrBackground = (HBRUSH)GetStockObject(fnObject);
};
char far*
CharactStocksMDIChild::GetClassName(){
	return SHADE_CLASS;
};
/*
 * ---------------------------------------------------------
 *	EoF(charstck.cpp)
 * ----------------------------------------------------------
 */