#if !defined(__COPYDISK_H)
#define __COPYDISK_H
/*
 * copydisk.h - Handle file CopyDisk Module ---------------------
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

#include "testsyst.rh"
/*
 *		Function's prototips
 */
//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TCopyDisksDialog
 *  -------------------------------------------------------------------------
 */
class TCopyDisksDialog : public TDialog {
  public:
	TDrawICBT*    Button1;
	TDrawICBT*    Button2;
	TDrawICBT*    Button3;
	TDrawICBT*    CheckBox1;
	TDrawICBT*    CheckBox2;

	TCopyDisksDialog(TWindow* parent);		// Class constructor

	void CopyToDisk();
	void CopyDisksHelp()     {MessageBeep(0);}
	void EndSession();
  DECLARE_RESPONSE_TABLE(TCopyDisksDialog);
};
#endif  // __COPYDISK_H
/*
 * ---------------------------------------------------------
 *	EoF(copydisk.h)
 * ----------------------------------------------------------
 */
