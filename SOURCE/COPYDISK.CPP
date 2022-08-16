/*
 * copydisk - Data input system module -------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\button.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\inputdia.h>
#include <owl\validate.h>
#include <owl\dc.h>
#include <stdio.h>

#include "drawicbt.h"
#include "copydisk.h"
#include "testsyst.h"

//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TCopyDisksDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TCopyDisksDialog, TDialog)
  EV_WM_TIMER,
  EV_COMMAND(IDC_CBUTTON1,  CopyToDisk),
  EV_COMMAND(IDC_CBUTTON2,  CopyDisksHelp),
  EV_COMMAND(IDC_CBUTTON3,  EndSession),
END_RESPONSE_TABLE;

//								C o n s t r u c t o r
TCopyDisksDialog::TCopyDisksDialog(TWindow* parent)
  : TDialog(parent, "CopyDiskDIALOG"),
	TWindow(parent)
{
  Button1   = new TDrawICBT(this, IDC_CBUTTON1);
  Button2   = new TDrawICBT(this, IDC_CBUTTON2);
  Button3   = new TDrawICBT(this, IDC_CBUTTON3);
  CheckBox1 = new TDrawICBT(this, IDC_TCHECKBOX1);
  CheckBox2 = new TDrawICBT(this, IDC_TCHECKBOX2);
};
void
TCopyDisksDialog::CopyToDisk(){
	{
		 MessageBox("Производится копирование на дискету", "Копирование", MB_OK);
	}
};
void
TCopyDisksDialog::EndSession(){
	{
		 EnableWindow(FALSE); // Запрещаем доступ к окну диалога
		 Show(SW_HIDE);		 // Гасим окно диалога
		 ::SendMessage(GetParent(), WM_PAINT, 0, 0);
							 // Если все выключено, убиваем диалог
		 TYPESAFE_DOWNCAST(Parent, TMainDialog)->
											CopyDisksDialogCreated = FALSE;
		 CloseWindow(IDCANCEL);
	}
};
/*
 *  -------------------------------------------------------------------------
 * 	Main Procedure
 *  -------------------------------------------------------------------------
 */
void
CopyDisks() {
};
/*
 * ---------------------------------------------------------
 *	EoF(copydisk.cpp)
 * ----------------------------------------------------------
 */

