#if !defined(__MAINDIAL_H)
#define __MAINDIAL_H
/*
 * MainDial.h - Handle
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */

#include <owl\button.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\dc.h>
										// Д и а л о г и
#include "datainpt.h"
#include "dataview.h"
#include "testctrl.h"
#include "mcontrls.h"
#include "tmodulpc.h"
#include "tsensors.h"
#include "transduc.h"
#include "tchannio.h"
#include "drawicbt.h"
//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TMainDialog
 *  -------------------------------------------------------------------------
 */
class TMainDialog : public TDialog{
 public:							// Диалоги основных подсистем
	TDInptWindow*   		pTDInptWindow;
	TViewDialog*   		pTViewDialog;
	TTestCtrlDialog* 		pTTestCtrlDialog;

	TMContrlsDialog* 		pTMContrlsDialog;
	TModulPCDialog* 		pTModulPCDialog;
	TSensorsDialog*  		pTSensorsDialog;
	TTransducDialog* 	   pTTransducDialog;
	TChannelIODialog* 	pTChannelIODialog;

	TDrawICBT*   Button1;
	TDrawICBT*   Button2;
	TDrawICBT*   Button3;
	TDrawICBT*   Button4;
	TDrawICBT*   Button5;
	TDrawICBT*   Button6;
	TDrawICBT*   Button7;
	TDrawICBT*   Button8;
	TDrawICBT*   Button9;
	TDrawICBT*   Button10;
	TDrawICBT*   Button11;
	TDrawICBT*   Button12;
	TDrawICBT*   Button13;
	TDrawICBT*   Button14;
	TDrawICBT*   Button15;

	TBitmap*		 Picture;
									// C o n s t r u c t o r
	TMainDialog					(TWindow* parent);

	TMainDialog* getPointer	() {return this;};
	void EvPaint			 	();
	void Paint					(TDC&, BOOL, TRect&);

	void InputSystem			();		   //  Execute Input system
													//  dialog
	void DataViewSystem   	();
	void TestCtrlSystem   	();
	void TMContrlsSystem  	();
	void TModulPCSystem	 	();
	void TSensorsSystem	 	();
	void TTransducSystem  	();
	void TypeChannelIOSystem();
	void CopyDisksSystem  	(){MessageBeep(0);};  //  Fict. access control
	void ConfigSystem     	(){MessageBeep(0);};  //  Fict. access control
	void HelpDialog       	(){MessageBeep(0);};  //  Fict. access control
	void EvClose			 	();
  DECLARE_RESPONSE_TABLE(TMainDialog);
};
/*
 * ---------------------------------------------------------
 *	EoF(maindial.h)
 * ----------------------------------------------------------
 */
#endif  // __MAINDIAL_H
