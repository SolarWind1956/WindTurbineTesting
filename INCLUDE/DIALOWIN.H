#if !defined(__DIALOWIN_H)
#define __DIALOWIN_H
/*
 * dialowin.h -  ---------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\dc.h>
#include <owl\buttonga.h>
#include <owl\controlb.h>
#include <owl\statusba.h>
#include <owl\menu.h>

/*
 *  -------------------------------------------------------------------------
 * 	Class TClientWindow
 *  -------------------------------------------------------------------------
 */
class TClientWindowForTDW : public TWindow {
  public:
	TClientWindowForTDW(TWindow* parent, TModule* module = 0) : TWindow(parent, 0){};
											// Prototips functions-methods
	void  GetWindowClass(WNDCLASS& wndClass){
	  TWindow::GetWindowClass(wndClass);
	  wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	}
	char far* GetClassName(){return "TBWindow";}
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TDialogWindow
 *  -------------------------------------------------------------------------
 */
class  TDialogWindow : public TDecoratedFrame {
	public:

	TControlBar*   			CntrBar;
	TStatusBar*    			StatBar;
	TClientWindowForTDW*   	ClntWin;
	TDialogWindow(TWindow* 	parent, const char* far Title,
					  TResID MenuName, int Width, int Height);
};

#endif  // __DIALOWIN_H
/*
 * ---------------------------------------------------------
 *	EoF(dialowin.h)
 * ----------------------------------------------------------
 */
