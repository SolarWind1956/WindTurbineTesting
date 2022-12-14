#if !defined(__apxprev_h)               // Sentry, use file only if it's not already included.
#define __apxprev_h

/*  Main powercha
    Crimea Engineering Centre
    ????????? ????? ? 1996. ??? ????? ????????.

    SUBSYSTEM:    powercha.exe Application
    FILE:         APXPrev.H
    AUTHOR:       Heckel & Davidov


    OVERVIEW
    ========
    Class definition for PreviewWindow (Print Preview).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\controlb.h>
#include <owl\printdia.h>
#include <owl\preview.h>

#include "apxprint.h"
#include "powercha.rh"


//{{TDecoratedFrame = PreviewWindow}}
class PreviewWindow : public TDecoratedFrame {
public:
    PreviewWindow (TWindow *parentWindow, TPrinter *printer, TWindow* currWindow, const char far* title, TLayoutWindow* client);
    ~PreviewWindow ();

    int             PageNumber;

    TWindow         *CurrWindow;
    TControlBar     *PreviewSpeedBar;
    TPreviewPage    *Page1;
    TPreviewPage    *Page2;
    TPrinter        *Printer;

    TPrintDC        *PrnDC;
    TSize           *PrintExtent;
    APXPrintOut     *Printout;

private:
    TLayoutWindow   *Client;

    void SpeedBarState ();

//{{PreviewWindowVIRTUAL_BEGIN}}
protected:
    virtual void SetupWindow ();
//{{PreviewWindowVIRTUAL_END}}

//{{PreviewWindowRSP_TBL_BEGIN}}
protected:
    void PPR_Previous ();
    void PPR_Next ();
    void PPR_OneUp ();
    void PPR_TwoUp ();
    void EvNCLButtonDown (UINT wHitTestCode, TPoint & point);
    void EvClose ();
//{{PreviewWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(PreviewWindow);
};    //{{PreviewWindow}}


#endif      // __apxprev_h sentry.
