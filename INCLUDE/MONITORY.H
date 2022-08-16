#if !defined(__MONITORY_H)
#define __MONITORY_H
/*
 * datainpt.h - Handle file DataInpt Module ---------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <bwcc.h>
#include "binfobox.h"
#include "monitwin.h"

class TMonitoryWindow : public TMonitoringWindow {
  public:
//	TBInfoBox*    pTStatusWindow;

	TMonitoryWindow(TWindow* parent);		// Class constructor

	void EvClose				();
	void EvTimer				(UINT);
	void MonitCycleONOFF	 	();
	void MonitONECycle	 	();
	void MonitCycleService  ();
	DECLARE_RESPONSE_TABLE(TMonitoryWindow);
};

#endif  // __MONITORY_H
/*
 * ---------------------------------------------------------
 *	EoF(monitory.h)
 * ----------------------------------------------------------
 */