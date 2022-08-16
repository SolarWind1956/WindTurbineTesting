#if !defined(__DATAVIEW_H)
#define __DATAVIEW_H
/*
 * dataview.h - Handle file DataInpt Module ---------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files

#include <owl\dialog.h>
#include <owl\dc.h>
#include <owl\combobox.h>
#include <owl\checkbox.h>

#include "graphic.h"
#include "instrum.h"
#include "binfobox.h"
#include "drawicbt.h"
#include "commfunc.h"

/*---------------------------------------------------------------------*\
 *  Class TViewDialog		                                            *
\*---------------------------------------------------------------------*/
class TViewDialog : public TDialog {
  public:
	 BOOL       		ViewDialogCreated;
	 TGraphicsBox*  	GraphicsBox_SB;
	 TInstrumentsBox* InstrumentsBox_SB;
	 TComboBox* 		ListGraph_CB;
	 TCheckBox*			GridTrigger_CHB;
	 TBInfoBox* 		Message_IB;
	 TDrawICBT*			Help_ICBT;
	 TDrawICBT*			Exit_ICBT;

	 TViewDialog(TWindow* parent);

	 void ViewDataON		();
	 void ViewDataOFF		();
	 void GridOnOff		();
	 void SetupWindow		();
	 void ShowViewMessage(char*);
	 void ListGraphics	();
	 void FillArrayData	();
	 void EvTimer			(UINT);
	 void EvClose			();
	 void EndSession		();
	 void ViewHelp       (){};

  DECLARE_RESPONSE_TABLE(TViewDialog);
};
#endif  // __DATAVIEW_H
/*
 * ---------------------------------------------------------
 *	EoF(dataview.h)
 * ----------------------------------------------------------
 */
