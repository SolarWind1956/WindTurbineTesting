#if !defined(__TESTCTRL_H)
#define __TESTCTRL_H
/*
 * testctrl.h -
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\listbox.h>

#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <shellapi.h>
#include <mem.h>

#include "drawicbt.h"

#include "dlistvie.h"

#include "binfobox.h"
#include "commglob.h"
#include "testsyst.rh"
#include "messagew.h"
//----------------------------------------------------------------------------
/*
 *		Function's prototips
 */
void CreateListBox1(TListBox*);
void CreateListBox2(TListBox*);
void CreateListBox3(TListBox*);
void GetRealTime(TBInfoBox*);        // Отображение текущего времени


/*
 *  -------------------------------------------------------------------------
 * 	Class TTestCrtlDialog
 *  -------------------------------------------------------------------------
 */
class TTestCtrlDialog : public TDialog{
 public:
	BOOL 				  TestCtrlDialogCreated;
	TDrawICBT*    	  Button1;
	TDrawICBT*    	  Button2;
	TDrawICBT*    	  Button3;
	TDrawICBT*    	  Button4;
	TDrawICBT*    	  Button5;
	TDrawListView*   ListBox1;
	TDrawListView*   ListBox2;
	TDrawListView*   ListBox3;

	TTestCtrlDialog(TWindow* parent);	// Class constructor

	void MaximizeTestApp    ();
	void FormatString			(int);
	void SetupWindow 			();
	void TestCtrlStart		();	  		// Запуск испытания
	void TestCtrlStop			();    		// Остановка испытания
	void ListTypeTests		();  			// Листинг типов испытаний
	void ListTypeWindTurbine(); 			// Листинг типов ВЭУ
	void TestCtrlHelp			()     {MessageBeep(0);}
	void EvClose				();
	void EndSession			();
  DECLARE_RESPONSE_TABLE(TTestCtrlDialog);
};
#endif  // __TESTCTRL_H
/*
 * ---------------------------------------------------------
 *	EoF(testctrl.h)
 * ----------------------------------------------------------
 */
