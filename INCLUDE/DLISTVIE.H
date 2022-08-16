#if !defined(__DLISTVIE_H)
#define __DLISTVIE_H
/*
 * dlistvie.h - Handle file TBInfoBox Class ---------------------
 *
 *					Класс предназначен для
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

#include <owl\listbox.h>

#include "testsyst.rh"

/*
 *  -------------------------------------------------------------------------
 * 	Class TDrawListView
 *  -------------------------------------------------------------------------
 */
class TDrawListView : public TListBox {
  public:
	 TDrawListView(TWindow* parent, int resId, TModule* module = 0);
//	~TDrawListView();

	 // Overridden virtuals from TWindow
	 //
//	 BOOL CanClose()   {return TWindow::CanClose();}
	 BOOL CreateListBox(void (*)(TListBox*));

	 // Указатель на функцию, формирующую список ListBox"а
	 //
	 void (*pCreateFunction)(TListBox*);
};
#endif  // __DLISTVIE_H
/*
 * ---------------------------------------------------------
 *	EoF(dlistvie.h)
 * ----------------------------------------------------------
 */
