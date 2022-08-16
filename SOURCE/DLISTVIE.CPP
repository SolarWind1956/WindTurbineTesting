/*
 * dlistvie - Class like TListBox for fill listbox's ----------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 */

#include <owl\listbox.h>
#include "dlistvie.h"
//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TDrawListView
 *  -------------------------------------------------------------------------
 */
TDrawListView::TDrawListView(TWindow* parent, int resId,
									  TModule* module)
	 : TListBox(parent, resId, module){};
											// Создание ListBox'а при помощи
											// пользовательской функции, указатель
											// на которую передается в качестве
											// параметра
BOOL TDrawListView::CreateListBox(void(*CreateFunction)(TListBox*))
{
  TListBox::Create();
  pCreateFunction = CreateFunction;
  (*pCreateFunction)(this);
  return TRUE;
};
/*
 * ---------------------------------------------------------
 *	EoF(dlistvie.cpp)
 * ----------------------------------------------------------
 */

