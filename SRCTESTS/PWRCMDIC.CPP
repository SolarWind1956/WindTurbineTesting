                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                

//{{PowerCharMDIClient Implementation}}


//
// Build a response table for all messages/commands handled
// by PowerCharMDIClient derived from TMDIClient.
//
DEFINE_RESPONSE_TABLE1(PowerCharMDIClient, TMDIClient)
//{{PowerCharMDIClientRSP_TBL_BEGIN}}
    EV_COMMAND(CM_FILEPRINT, CmFilePrint),
    EV_COMMAND(CM_FILEPRINTERSETUP, CmFilePrintSetup),
    EV_COMMAND(CM_FILEPRINTPREVIEW, CmFilePrintPreview),
    EV_COMMAND_ENABLE(CM_FILEPRINT, CmPrintEnable),
    EV_COMMAND_ENABLE(CM_FILEPRINTERSETUP, CmPrintEnable),
    EV_COMMAND_ENABLE(CM_FILEPRINTPREVIEW, CmPrintEnable),
    EV_WM_DROPFILES,
//{{PowerCharMDIClientRSP_TBL_END}}
END_RESPONSE_TABLE;


//////////////////////////////////////////////////////////
// PowerCharMDIClient
// ===========
// Construction/Destruction handling.
 PowerCharMDIClient::PowerCharMDIClient ()
 : TMDIClient ()
{
    ChildCount = 0;

    // INSERT>> Your constructor code here.

}


 PowerCharMDIClient::~PowerCharMDIClient ()
{
    Destroy();

    // INSERT>> Your destructor code here.

}


//////////////////////////////////////////////////////////
// PowerCharMDIClient
// ===========
// MDIClient site initialization.
void PowerCharMDIClient::SetupWindow ()
{
    // Default SetUpWindow processing.
    TMDIClient::SetupWindow ();

    // Accept files via drag/drop in the client window.
    DragAcceptFiles(TRUE);
}


//////////////////////////////////////////////////////////
// PowerCharMDIClient
// ==========
// Menu File Print command
void PowerCharMDIClient::CmFilePrint ()
{
    //
    // Create Printer object if not already created.
    // 
    PowerCharApp *theApp = TYPESAFE_DOWNCAST(GetApplication(), PowerCharApp);
    if (theApp) {
        if (!theApp->Printer)
            theApp->Printer = new TPrinter;

        //
        // Create Printout window and set characteristics.
        //
        APXPrintOut printout(theApp->Printer, Title, GetActiveMDIChild(), TRUE);

        theApp->Printing = TRUE;

        //
        // Bring up the Print dialog and print the document.
        //
        theApp->Printer->Print(GetActiveMDIChild()->GetClientWindow(), printout, TRUE);

        theApp->Printing = FALSE;
    }
}


//////////////////////////////////////////////////////////
// PowerCharMDIClient
// ==========
// Menu File Print Setup command
void PowerCharMDIClient::CmFilePrintSetup ()
{
    PowerCharApp *theApp = TYPESAFE_DOWNCAST(GetApplication(), PowerCharApp);
    if (theApp) {
        if (!theApp->Printer)
            theApp->Printer = new TPrinter;

        //
        // Bring up the Print Setup dialog.
        //
        theApp->Printer->Setup(this);
    }
}


//////////////////////////////////////////////////////////
// PowerCharMDIClient
// ==========
// Menu File Print Preview command
void PowerCharMDIClient::CmFilePrintPreview ()
{
    PowerCharApp *theApp = TYPESAFE_DOWNCAST(GetApplication(), PowerCharApp);
    if (theApp) {
        if (!theApp->Printer)
            theApp->Printer = new TPrinter;

        theApp->Printing = TRUE;

        PreviewWindow *prevW = new PreviewWindow(Parent, theApp->Printer, GetActiveMDIChild(), "Print Preview", new TLayoutWindow(0));
        prevW->Create();

        GetApplication()->BeginModal(GetApplication()->MainWindow);

        // We must destroy the preview window explicitly.  Otherwise, the window will not be destroyed until
        // it's parent the MainWindow is destroyed.
        prevW->Destroy();
        delete prevW;

        theApp->Printing = FALSE;
    }
}


//////////////////////////////////////////////////////////
// PowerCharMDIClient
// ==========
// Menu enabler used by Print, Print Setup and Print Preview.
void PowerCharMDIClient::CmPrintEnable (TCommandEnabler &tce)
{
    if (GetActiveMDIChild()) {
        PowerCharApp *theApp = TYPESAFE_DOWNCAST(GetApplication(), PowerCharApp);
        if (theApp) {
            // If we have a Printer already created just test if all is okay.
            // Otherwise create a Printer object and make sure the printer
            // really exists and then delete the Printer object.
            if (!theApp->Printer) {
                theApp->Printer = new TPrinter;
                
                tce.Enable(theApp->Printer->GetSetup().Error == 0);
            } else
                tce.Enable(theApp->Printer->GetSetup().Error == 0);
        }
    } else
        tce.Enable(FALSE);
}


void PowerCharMDIClient::EvDropFiles (TDropInfo)
{
    Parent->ForwardMessage();
}
