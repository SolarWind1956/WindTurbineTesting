#if !defined(__MCONTRLS_H)
#define __MCONTRLS_H
/*
 * mcontrls.h - Handle file  ---------------------
 *
 * Author:
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files

#include <owl\edit.h>
#include <owl\combobox.h>

#include "binfobox.h"
#include "drawicbt.h"

#include "database.h"
#include "userdraw.h"
#include "testsyst.rh"
/*
 *  -------------------------------------------------------------------------
 * 	Class TMicroPCDialog
 *  -------------------------------------------------------------------------
 */
class TMContrlsDialog : public TDialog {
  public:
	TDrawICBT*	Help_ICBT;
	TDrawICBT*	Cancel_ICBT;
	TComboBox*	TypePC_CB;
	TEdit*		Firma_ED;
	TEdit*		MainProcessor_ED;
	TEdit*		NetProcessor_ED;
	TEdit*		Interface1_ED;
	TEdit*		Interface2_ED;
	TButton*		Save_BUT;
	TButton*		Delete_BUT;
	TEdit*		TypeProtocol_ED;
	TBInfoBox*	Message_IB;					// Статусная строка
	TBitmap*	   Picture;

	TMContrlsDialog(TWindow* parent);		// Class constructor

	void 	EvPaint			 	();
	void 	Paint(TDC&, BOOL, TRect&);
	void 	SetupWindow			();
	void 	ListParamsPC		();
	void 	SaveRecord 			();
	void 	DeleteRecord		();
	void 	ChangeText			();
	void 	TMContrlsHelp		(){ MessageBeep(0); };
	void 	EndSession			();
  DECLARE_RESPONSE_TABLE(TMContrlsDialog);
};
#endif  // __MCONTRLS_H
/*
 * ---------------------------------------------------------
 *	EoF(mcontrls.h)
 * ----------------------------------------------------------
 */
