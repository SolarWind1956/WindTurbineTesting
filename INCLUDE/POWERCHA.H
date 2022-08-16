#if !defined(__powercha_h)              // Sentry, use file only if it's not already included.
#define __powercha_h

/*  Project powercha
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    powercha.exe Application
	 FILE:         powercha.h
	 AUTHOR:       Heckel & Davidov


	 OVERVIEW
	 ========
	 Class definition for PowerCharApp (TApplication).
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\statusba.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\editview.h>
#include <owl\listview.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\printer.h>
#include <owl\menu.h>

#include <classlib\bags.h>

#include "powercha.rh"            // Definition of all resources.
#include "commglob.h"
#include "charisti.h"

// TFileDrop class Maintains information about a dropped file, its name, where it was dropped,
// and whether or not it was in the client area
class TFileDrop {
public:
	 operator == (const TFileDrop& other) const {return this == &other;}

	 char*   FileName;
	 TPoint  Point;
	 BOOL    InClientArea;

	 HICON   Icon;
	 BOOL    DefIcon;

	 TFileDrop (char*, TPoint&, BOOL, TModule* module);
	 ~TFileDrop ();

	 const char* WhoAmI ();
private:
	 //
	 // hidden to prevent accidental copying or assignment
	 //
	 TFileDrop (const TFileDrop&);
	 TFileDrop & operator = (const TFileDrop&);
};

typedef TIBagAsVector<TFileDrop> TFileList;
typedef TIBagAsVectorIterator<TFileDrop> TFileListIter;


//{{TApplication = PowerCharApp}}
class PowerCharApp : public TApplication {
private:
	 BOOL            HelpState;                          // Has the help engine been used.
	 BOOL            ContextHelp;                        // SHIFT-F1 state (context sensitive HELP)
	 HCURSOR         HelpCursor;                         // Context sensitive help cursor

private:
	 void SetupSpeedBar (TDecoratedMDIFrame *frame);
	 void AddFiles (TFileList* files);

public:
	 TMDIClient*    Client;
	 TButtonGadget* pSTART_STOP_BG;
	 PowerCharApp ();
	 virtual ~PowerCharApp ();

	 // Public data members used by the print menu commands and Paint routine in MDIChild.
//public :
	 TSetTimePeriodDialog* pTSetTimePeriodDialog;		  // Диалог изменения периода записи протокола
	 TPrinter        *Printer;                           // Printer support.
	 BOOL            Printing;                           // Printing in progress.

//{{PowerCharAppVIRTUAL_BEGIN}}
public:
	 virtual void InitMainWindow();
	 virtual void InitInstance();
	 virtual BOOL CanClose ();
	 virtual BOOL ProcessAppMsg (MSG& msg);
//{{PowerCharAppVIRTUAL_END}}
//{{PowerCharAppRSP_TBL_BEGIN}}
protected:
	 void EvNewView 				(TView& view);
	 void EvCloseView 			(TView& view);
	 void CmHelpAbout 			();
	 void CmHelpContents 		();
	 void CmHelpUsing 			();
	 void EvDropFiles 			(TDropInfo drop);
	 void EvWinIniChange 		(char far* section);

	 void CmPowerCharacteristic ();
	 void CmStocksCharacteristic();
	 void EvTimer					(UINT);
	 void EvInitMenu				(HMENU);
	 void CmStartStopProt		();
	 void CmCycleProt				();
//{{PowerCharAppRSP_TBL_END}}
  DECLARE_RESPONSE_TABLE(PowerCharApp);
};    //{{PowerCharApp}}


#endif   // __powercha_h.
