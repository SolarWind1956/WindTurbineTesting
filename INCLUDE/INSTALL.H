#if !defined(__INSTALL_H)
#define __INSTALL_H
/*
 * install.h - Handle file of Root Module ---------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\button.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\dc.h>
#include <owl\vbxctl.h>
#include <stdio.h>

#include "install\install.rh"
#include "drawicbt.h"
//----------------------------------------------------------------------------
BOOL Delay(TWindow*, WORD);
/*
 *  -------------------------------------------------------------------------
 * 	Class TMainDialog
 *  -------------------------------------------------------------------------
 */
class TMainDialog : public TDialog,
					public TVbxEventHandler {
  public:
	TDrawICBT*   Button1;
	TDrawICBT*   Button2;
//	TDrawICBT*   Button3;
//	TDrawICBT*   Button4;
//	TDrawICBT*   Button5;
//	TDrawICBT*   Button6;
//	TDrawICBT*   Button7;
//	TDrawICBT*   Button8;
//	TDrawICBT*   Button9;
// DrawICBT*   Button10;
//	TDrawICBT*   Button11;
//	TDrawICBT*   Button12;
//	TDrawICBT*   Button13;
//	TDrawICBT*   Button14;
	TDrawICBT*   Button15;

	TMainDialog(TWindow* parent);
//	void HelpDialog       () {MessageBeep(0);}  //  Fict. access control
	void InstallSystem	 ();
	void UnInstallSystem	 ();
	void EndSession       ();
	void EvClose			 ();
  DECLARE_RESPONSE_TABLE(TMainDialog);
};

/*
 *  -------------------------------------------------------------------------
 * 	Class TInstallWindow
 *  -------------------------------------------------------------------------
 */
class TInstallWindow : public TWindow {
  public:
		TMainDialog* pTMainDialog;
		BOOL  MainDialogExecute;	// Execute flags

		TInstallWindow(); // Constructor
		void 	Paint(TDC&, BOOL erase, TRect&);
};
/*
 * ---------------------------------------------------------
 *	EoF(Install.h)
 * ----------------------------------------------------------
 */
#endif  // __INSTALL_H
