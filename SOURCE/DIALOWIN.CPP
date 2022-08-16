/*
 * TDialogWindow = [T] DIALO [g] WIN [dow]----------------
 *
 * Authors: Sergey E. Heckel & Ljudmila M. Baybus
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
#include "dialowin.h"
/*
 *  -------------------------------------------------------------------------
 * 	Class TDialogWindow
 *  -------------------------------------------------------------------------
 */
TDialogWindow::TDialogWindow(TWindow* parent, const char* far Title,
									TResID MenuName,
									int Width, int Height) :
		TDecoratedFrame(parent, Title, ClntWin = new TClientWindowForTDW(parent, NULL), TRUE){

		Attr.Style = !WS_CHILD | /* Требуется автономное окно сообщения, не ДОЧЕРНЕЕ ! */
						 WS_VISIBLE | WS_BORDER;
		Attr.X    = 0;	Attr.Y    = 20;      // Координаты левого верхнего угла диалога

		Attr.W    = Width;    	 		      // Максимальная ширина окна
		Attr.H    = Height;     	         // Высота окна
		Attr.Menu = MenuName;          		// Имя меню в файле ресурсов

		CntrBar = new TControlBar(this);		// Создание TControlBar
														// Отобразим его в верху окна
		Insert(*CntrBar, TDecoratedFrame::Top);
														// Создание TStatusBar
		StatBar = new TStatusBar(this, 	TGadget::Recessed);
														// Отобразим его внизу окна
		Insert(*StatBar, TDecoratedFrame::Bottom);
};
/*
 * ---------------------------------------------------------
 *	EoF(dialowin.cpp)
 * ----------------------------------------------------------
 */