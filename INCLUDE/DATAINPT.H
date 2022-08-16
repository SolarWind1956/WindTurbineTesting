#if !defined(__DATAINPT_H)
#define __DATAINPT_H
/*
 * datainpt.h - Handle file DataInpt Module ---------------------
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

#include <owl\buttonga.h>
#include <owl\controlb.h>
#include <owl\statusba.h>

#include <stdio.h>
#include <time.h>
#include <sys\timeb.h>

#include "commglob.h"
#include "monitory.h"
#include "dialowin.h"
#include "database.h"

//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TDInptWindow
 *  -------------------------------------------------------------------------
 */
class TDInptWindow : public TDialogWindow {
  public:
	BOOL							DInptWindowCreated;
	TSetTimePeriodDialog*   pTSetTimePeriodDialog;
	TMonitoryWindow* 			pTMonitoryWindow;
																// ”казатели на кнопки, дублирующие меню
	TButtonGadget*          ButtonG1;
	TButtonGadget*          ButtonG2;
	TButtonGadget*          ButtonG3;
	TButtonGadget*          ButtonG4;
	TButtonGadget*          ButtonG5;
	TButtonGadget*          ButtonG6;
	TButtonGadget*          ButtonG7;
	TButtonGadget*          ButtonG8;
	TButtonGadget*          ButtonG9;
	TButtonGadget*          ButtonG10;
	// »ндикаторы
	TTextGadget* 				pNet;						// "NET ON" "NET OFF"
	TTextGadget* 				pInstantVal;			// "MET ON" "MET OFF"
	TTextGadget* 				pCycle;					// "CYC ON" "CYC OFF"
	// Class constructor
	TDInptWindow(TWindow* parent, const char* far Title, TResID MenuName,
					 int Width, int Height);
			//		P r o t o t y p e s   f u n c t i o n s - m e m b e r s
	void EvInitMenu			(HMENU);
	void EvTimer				(UINT);

	void InfoInptON			();
	void InfoInptOFF			();
	void InputCycleService	();

	void MonitoryONOFF		();
	void MonitCycleService	();

	void InstantValInptON	();
	void InstantValInptOFF	();
	void InstantValCycleService	();

	void CycleFInptON			();
	void CycleFInptOFF		();
	void CycleFCycleService	();

	void ShowMessage			(char*);
	void DataMonitHelp		(){MessageBeep(0);}
	void EvClose				();
	void EndSession			();
  DECLARE_RESPONSE_TABLE(TDInptWindow);
};
#endif  // __DATAINPT_H
/*
 * ---------------------------------------------------------
 *	EoF(datainpt.h)
 * ----------------------------------------------------------
 */
