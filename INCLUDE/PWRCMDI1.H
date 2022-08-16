#if !defined(__pwrcmdi1_h)              // Sentry, use file only if it's not already included.
#define __pwrcmdi1_h

/*  Project powercha
    Crimea Engineering Centre
    Авторское право © 1996. Все права защищены.

    SUBSYSTEM:    powercha.exe Application
    FILE:         pwrcmdi1.h
    AUTHOR:       Heckel & Davidov


    OVERVIEW
    ========
    Class definition for PowerCharMDIChild (TMDIChild).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\editfile.h>
#include <owl\listbox.h>

#include "powercha.rh"            // Definition of all resources.


//{{TMDIChild = PowerCharMDIChild}}
class PowerCharMDIChild : public TMDIChild {
public:
    PowerCharMDIChild (TMDIClient &parent, const char far *title, TWindow *clientWnd, BOOL shrinkToClient = FALSE, TModule* module = 0);
    virtual ~PowerCharMDIChild ();

//{{PowerCharMDIChildVIRTUAL_BEGIN}}
public:
    virtual void Paint (TDC& dc, BOOL erase, TRect& rect);
//{{PowerCharMDIChildVIRTUAL_END}}
//{{PowerCharMDIChildRSP_TBL_BEGIN}}
protected:
    void EvGetMinMaxInfo (MINMAXINFO far& minmaxinfo);
//{{PowerCharMDIChildRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(PowerCharMDIChild);
};    //{{PowerCharMDIChild}}


#endif                                      // __pwrcmdi1_h sentry.
