/*  Main powercha
    Crimea Engineering Centre
    ????????? ????? ? 1996. ??? ????? ????????.

    SUBSYSTEM:    powercha.exe Application
    FILE:         APXPrev.CPP
    AUTHOR:       Heckel & Davidov


    OVERVIEW
    ========
    Source file for implementation of Print Preview.      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\textgadg.h>

#include "apxprev.h"

#include "powercha.rh"


//{{PreviewWindow Implementation}}

DEFINE_RESPONSE_TABLE1(PreviewWindow, TDecoratedFrame)
    EV_COMMAND(APX_PPR_PREVIOUS, PPR_Previous),
    EV_COMMAND(APX_PPR_NEXT, PPR_Next),
    EV_COMMAND(APX_PPR_ONEUP, PPR_OneUp),
    EV_COMMAND(APX_PPR_TWOUP, PPR_TwoUp),
//{{PreviewWindowRSP_TBL_BEGIN}}
    EV_WM_NCLBUTTONDOWN,
    EV_WM_CLOSE,
//{{PreviewWindowRSP_TBL_END}}
END_RESPONSE_TABLE;


PreviewWindow::PreviewWindow (TWindow *parentWindow, TPrinter *printer, TWindow* currWindow, const char far* title, TLayoutWindow* client) :
                 TDecoratedFrame(parentWindow, title, client)
{
    CurrWindow = currWindow;
    Printer = printer;
    Client = client;
    Page1 = 0;
    Page2 = 0;

    TPrintDialog::TData& data = Printer->GetSetup();
    PrnDC = new TPrintDC(data.GetDriverName(),
                         data.GetDeviceName(),
                         data.GetOutputName(),
                         data.GetDevMode());

    PrintExtent = new TSize(PrnDC->GetDeviceCaps(HORZRES), PrnDC->GetDeviceCaps(VERTRES));
    Printout = new APXPrintOut(Printer, "Print Preview", currWindow, TRUE);

    SetBkgndColor(GetSysColor(COLOR_APPWORKSPACE));

    //
    // Create default toolbar New and associate toolbar buttons with commands.
    //
    PreviewSpeedBar = new TControlBar(this);
    PreviewSpeedBar->Insert(*new TButtonGadget(APX_PPR_PREVIOUS, APX_PPR_PREVIOUS, TButtonGadget::Command, TRUE));
    PreviewSpeedBar->Insert(*new TButtonGadget(APX_PPR_NEXT, APX_PPR_NEXT, TButtonGadget::Command, TRUE));
    PreviewSpeedBar->Insert(*new TSeparatorGadget(6));
    PreviewSpeedBar->Insert(*new TButtonGadget(APX_PPR_ONEUP, APX_PPR_ONEUP, TButtonGadget::Exclusive, TRUE, TButtonGadget::Down));
    PreviewSpeedBar->Insert(*new TButtonGadget(APX_PPR_TWOUP, APX_PPR_TWOUP, TButtonGadget::Exclusive, TRUE));
    PreviewSpeedBar->Insert(*new TSeparatorGadget(12));
    PreviewSpeedBar->Insert(*new TTextGadget(APX_PPR_CURRPAGE, TGadget::Recessed, TTextGadget::Left, 10, "Page 1"));
    Insert(*PreviewSpeedBar, TDecoratedFrame::Top);

    // We want a window that can be sized, maximized, or minimized.
    Attr.Style &= ~(WS_THICKFRAME | WS_BORDER | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
    Attr.Style |= (WS_VISIBLE | WS_POPUP | WS_CAPTION | WS_SYSMENU);

    // Don't show the border of the preview window.
    Attr.X = -1;
    Attr.Y = -1;
    Attr.W = Parent->GetClientRect().Width() + 2;
    Attr.H = Parent->GetClientRect().Height() + 2;
    parentWindow->MapWindowPoints(HWindow, (TPoint *)&(Attr.X), 1);
}


PreviewWindow::~PreviewWindow ()
{
    delete Page1;
    Page1 = 0;
    delete Page2;
    Page2 = 0;

    delete PrnDC;
    PrnDC = 0;
    delete PrintExtent;
    PrintExtent = 0;
    delete Printout;
    Printout = 0;
}


void PreviewWindow::SetupWindow ()
{
    TDecoratedFrame::SetupWindow();

    TPrintDialog::TData& data = Printer->GetSetup();
    Page1 = new TPreviewPage(Client, *Printout, *PrnDC, *PrintExtent, 1);
    Page1->SetPageNumber(1);
    data.FromPage = 1;
    data.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                , 15);
        metrics1.Height.PercentOf(Page1, int((long(PrintExtent->cy) * 100) / PrintExtent->cx), lmWidth);

        // Two up is only available for portrait mode.
        TGadget *theGadget = PreviewSpeedBar->GadgetWithId(APX_PPR_TWOUP);
        theGadget->SetEnabled(FALSE);
   } else {
        metrics1.Height.Set(lmBottom, lmAbove, lmParent, lmBottom, 15);
        metrics1.Width.PercentOf(Page1, int((long(PrintExtent->cx) * 95) / PrintExtent->cy), lmHeight);
    }

    TGadget *theGadget = PreviewSpeedBar->GadgetWithId(APX_PPR_PREVIOUS);
    theGadget->SetEnabled(FALSE);

    Page1->Create();

    Client->SetChildLayoutMetrics(*Page1, metrics1);
    Client->Layout();
}


void PreviewWindow::SpeedBarState ()
{
    TPrintDialog::TData &printerData = Printer->GetSetup();

    // Only have previous on if we're not at the first page.
    TGadget *theGadget = PreviewSpeedBar->GadgetWithId(APX_PPR_PREVIOUS);
    theGadget->SetEnabled(printerData.FromPage != 1);

    // Only have next on if we're not at the last page.
    theGadget = PreviewSpeedBar->GadgetWithId(APX_PPR_NEXT);
    theGadget->SetEnabled(printerData.ToPage != printerData.MaxPage);

    // Update the page count.
    TTextGadget *theTGadget = TYPESAFE_DOWNCAST(PreviewSpeedBar->GadgetWithId(APX_PPR_CURRPAGE), TTextGadget);
    if (theTGadget) {
        char    buffer[32];
        if (Page2 && (printerData.FromPage != printerData.ToPage))
            wsprintf(buffer, "Page %d - %d", printerData.FromPage, printerData.ToPage);
        else
            wsprintf(buffer, "Page %d", printerData.FromPage);
        theTGadget->SetText(buffer);
    }
}


void PreviewWindow::EvClose ()
{
    // Don't call the base class EvClose we do not want PreviewWindow to be destructed.
    GetApplication()->EndModal(IDCANCEL);
}


void PreviewWindow::PPR_Previous ()
{
    TPrintDialog::TData &printerData = Printer->GetSetup();

    if (printerData.FromPage > printerData.MinPage) { 
        printerData.FromPage--;
        printerData.ToPage--;

        Page1->SetPageNumber(printerData.FromPage);
        if (Page2)
            Page2->SetPageNumber(printerData.ToPage);
    }

    SpeedBarState();
}


void PreviewWindow::PPR_Next ()
{
    TPrintDialog::TData &printerData = Printer->GetSetup();

    if (printerData.ToPage < printerData.MaxPage) {
        printerData.FromPage++;
        printerData.ToPage++;

        Page1->SetPageNumber(printerData.FromPage);
        if (Page2)
            Page2->SetPageNumber(printerData.ToPage);
    }

    SpeedBarState();
}


void PreviewWindow::PPR_OneUp ()
{
    if (Page2) {
        Client->RemoveChildLayoutMetrics(*Page2);

        delete Page2;
        Page2 = 0;

        Client->Layout();

        TPrintDialog::TData &printerData = Printer->GetSetup();
        printerData.ToPage = printerData.FromPage;

        SpeedBarState();
    }
}


void PreviewWindow::PPR_TwoUp ()
{
    if (Page2 == 0) {
        Page2 = new TPreviewPage(Client, *Printout, *PrnDC, *PrintExtent, PageNumber + 1);
        Page2->Create();

        TLayoutMetrics metrics2;

        metrics2.X.Set(lmLeft, lmRightOf, Page1, lmRight, 30);
        metrics2.Y.SameAs(Page1, lmTop);

        // Assume portrait
        //
        metrics2.Width.SameAs(Page1, lmWidth);
        metrics2.Height.SameAs(Page1, lmBottom);

        Client->SetChildLayoutMetrics(*Page2, metrics2);
        Client->Layout();

        TPrintDialog::TData &printerData = Printer->GetSetup();

        // Page 2 is the next page.  If the next page is outside of our
        // range then set the first page back one and the 2nd page is
        // the current page.  If the document is only 1 page long then
        // the 2nd page is empty.
        if (printerData.FromPage == printerData.MaxPage) {
            if (printerData.FromPage > 1) {
                printerData.FromPage--;
                printerData.ToPage = printerData.FromPage + 1;
                Page1->SetPageNumber(printerData.FromPage);
                Page2->SetPageNumber(printerData.ToPage);
            } else
                Page2->SetPageNumber(0);
        } else {
            printerData.ToPage = printerData.FromPage + 1;
            Page2->SetPageNumber(printerData.ToPage);
        }

        SpeedBarState();
    }
}


// Don't allow the print preview window to be dragged around.
void PreviewWindow::EvNCLButtonDown(UINT wHitTestCode, TPoint & point)
{
    // If the LButtonDown isn't in the caption then process it normally.
    if (wHitTestCode != HTCAPTION)
        TDecoratedFrame::EvNCLButtonDown(wHitTestCode, point);
}
