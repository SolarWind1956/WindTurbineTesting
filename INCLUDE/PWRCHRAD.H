#if !defined(__pwrchrad_h)              // Sentry, use file only if it's not already included.
#define __pwrchrad_h

/*  Project powercha
    Crimea Engineering Centre
    Авторское право © 1996. Все права защищены.

    SUBSYSTEM:    powercha.exe Application
    FILE:         pwrchrad.h
    AUTHOR:       Heckel & Davidov


    OVERVIEW
    ========
    Class definition for PowerCharAboutDlg (TDialog).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include "powercha.rh"                  // Definition of all resources.


//{{TDialog = PowerCharAboutDlg}}
class PowerCharAboutDlg : public TDialog {
public:
    PowerCharAboutDlg (TWindow *parent, TResId resId = IDD_ABOUT, TModule *module = 0);
    virtual ~PowerCharAboutDlg ();

//{{PowerCharAboutDlgVIRTUAL_BEGIN}}
public:
    void SetupWindow ();
//{{PowerCharAboutDlgVIRTUAL_END}}
};    //{{PowerCharAboutDlg}}


#endif                                      // __pwrchrad_h sentry.
