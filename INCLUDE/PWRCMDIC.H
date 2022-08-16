#if !defined(__pwrcmdic_h)              // Sentry, use file only if it's not already included.
#define __pwrcmdic_h

/*  Project powercha
    Crimea Engineering Centre
    Авторское право © 1996. Все права защищены.

    SUBSYSTEM:    powercha.exe Application
    FILE:         pwrcmdic.h
    AUTHOR:       Heckel & Davidov


    OVERVIEW
    ========
    Class definition for PowerCharMDIClient (TMDIClient).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\opensave.h>

#include "powercha.rh"            // Definition of all resources.


//{{TMDIClient = PowerCharMDIClient}}
class PowerCharMDIClient : public TMDIClient {
public:
    int                     ChildCount;                 // Number of child window created. 

    PowerCharMDIClient ();
    virtual ~PowerCharMDIClient ();

    void OpenFile (const char *fileName = 0);

private:
    void LoadTextFile ();

//{{PowerCharMDIClientVIRTUAL_BEGIN}}
protected:
    virtual void SetupWindow ();
//{{PowerCharMDIClientVIRTUAL_END}}

//{{PowerCharMDIClientRSP_TBL_BEGIN}}
protected:
    void CmFilePrint ();
    void CmFilePrintSetup ();
    void CmFilePrintPreview ();
    void CmPrintEnable (TCommandEnabler &tce);
    void EvDropFiles (TDropInfo);
//{{PowerCharMDIClientRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(PowerCharMDIClient);
};    //{{PowerCharMDIClient}}


#endif                                      // __pwrcmdic_h sentry.
