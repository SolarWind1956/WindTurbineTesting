/*
 * copydisk - Data input system module -------------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\radiobut.h>
#include <owl\button.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <stdio.h>
#include <bios.h>

#include "drawicbt.h"
#include "config.h"
#include "testsyst.h"
							// ????????? ???????????? ??? ??????? biosequip()
							// ???? 6-7 ?????????? ???-?? ????.
							// ??. ???????? ??????? biosequip()
int  FloppyDrive;
long FloppySize;
const unsigned TWO_FLOPPY   = 64;	// 01
const unsigned THREE_FLOPPY = 256;	// 10

//----------------------------------------------------------------------------

/*
 *  -------------------------------------------------------------------------
 * 	Class TConfigurationDialog
 *  -------------------------------------------------------------------------
 */
/*
DEFINE_RESPONSE_TABLE1(TConfigurationDialog, TDialog)
  EV_COMMAND(IDC_SBUTTON1,  SaveConfig),	  // ?????? ????????????
  EV_COMMAND(IDC_SBUTTON2,  ConfigHelp),	  // ??????
  EV_COMMAND(IDC_SBUTTON3,  EndSession),	  // ?????

  EV_COMMAND(IDC_RSBUTTON1, SizeCopyRest144),  // ???????? ?? 1.44
  EV_COMMAND(IDC_RSBUTTON2, SizeCopyRest121),  // ???????? ?? 1.21
  EV_COMMAND(IDC_RSBUTTON3, SizeCopyRest720),  // ???????? ?? 720
  EV_COMMAND(IDC_RSBUTTON4, SizeCopyRest360),  // ???????? ?? 360
  EV_COMMAND(IDC_RDBUTTON1, DiskCopyRestA), // ???????? ?:
  EV_COMMAND(IDC_RDBUTTON2, DiskCopyRestB), // ???????? ?:
END_RESPONSE_TABLE;
*/
//								C o n s t r u c t o r
/*TConfigurationDialog::TConfigurationDialog(TWindow* parent)
  : TDialog(parent, "ConfigDIALOG"),
	TWindow(parent)
{
  Button1   		= new TDrawICBT(this, IDC_SBUTTON1);
  Button2   		= new TDrawICBT(this, IDC_SBUTTON2);
  Button3   		= new TDrawICBT(this, IDC_SBUTTON3);
  RadioButton1   	= new TRadioButton(this, IDC_RSBUTTON1);
  RadioButton2   	= new TRadioButton(this, IDC_RSBUTTON2);
  RadioButton3   	= new TRadioButton(this, IDC_RSBUTTON3);
  RadioButton4   	= new TRadioButton(this, IDC_RSBUTTON4);
  RadioButton5   	= new TRadioButton(this, IDC_RDBUTTON1);
  RadioButton6   	= new TRadioButton(this, IDC_RDBUTTON2);
};
void
TConfigurationDialog::SaveConfig(){
		 MessageBox("???????????? ?????? ???????????? ?? ????", "????????????", MB_OK);
};

void
TConfigurationDialog::SizeCopyRest144(){
		RadioButton3->SetCheck(BF_UNCHECKED);  // 720 Kb  - ?????????
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - ?????????
	if (FloppyDrive == 1){
		FloppySize = 1210;
		RadioButton1->SetCheck(BF_UNCHECKED);	// 1.44 Mb - ?????????
		RadioButton2->SetCheck(BF_CHECKED);    // 1.21 Mb - ???????
	}
	else {
		FloppySize = 1440;
		RadioButton1->SetCheck(BF_CHECKED);		// 1.44 Mb - ???????
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - ?????????
	};
};
void
TConfigurationDialog::SizeCopyRest121(){
		RadioButton3->SetCheck(BF_UNCHECKED);  // 720 Kb  - ?????????
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - ?????????
	if (FloppyDrive == 1){
		FloppySize = 1210;
		RadioButton1->SetCheck(BF_UNCHECKED);	// 1.44 Mb - ?????????
		RadioButton2->SetCheck(BF_CHECKED);    // 1.21 Mb - ???????
	}
	else {
		FloppySize = 1440;
		RadioButton1->SetCheck(BF_CHECKED);		// 1.44 Mb - ???????
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - ?????????
	};
};
void
TConfigurationDialog::SizeCopyRest720(){
		FloppySize = 720;
		RadioButton1->SetCheck(BF_UNCHECKED);	// 1.44 Mb - ?????????
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - ?????????
		RadioButton3->SetCheck(BF_CHECKED);  	// 720 Kb  - ???????
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - ?????????
};
void
TConfigurationDialog::SizeCopyRest360(){
		RadioButton1->SetCheck(BF_UNCHECKED);  // 1.44 Mb - ?????????
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - ?????????
		RadioButton3->SetCheck(BF_UNCHECKED);	// 720 Kb  - ?????????
	if (FloppyDrive == 1){
		FloppySize = 360;
		RadioButton4->SetCheck(BF_CHECKED);    // 360 Kb  - ???????
	}
	else {
		FloppySize = 1210;
		RadioButton1->SetCheck(BF_CHECKED);		// 1.44 Mb - ???????
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - ?????????
	};
};
void
TConfigurationDialog::DiskCopyRestA(){
										// ??? ????????? A:
		FloppyDrive = 1;
		RadioButton1->SetCheck(BF_UNCHECKED | BF_GRAYED);  // 1.44 Mb - ?????????
		RadioButton1->Attr.Style |= WS_DISABLED;
		RadioButton2->SetCheck(BF_CHECKED);  	// 1.21 Mb - ???????
		RadioButton3->SetCheck(BF_UNCHECKED);	// 720 Kb  - ?????????
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - ?????????
};
void
TConfigurationDialog::DiskCopyRestB(){
										// ??? ????????? B:
		FloppyDrive = 2;
		RadioButton1->SetCheck(BF_CHECKED);  	// 1.44 Mb - ???????
		RadioButton2->SetCheck(BF_UNCHECKED | BF_GRAYED);  // 1.21 Mb - ?????????
		RadioButton3->SetCheck(BF_UNCHECKED);	// 720 Kb  - ?????????
		RadioButton4->SetCheck(BF_UNCHECKED | BF_GRAYED);  // 360 Kb  - ?????????
};
void
TConfigurationDialog::EndSession(){
		 EnableWindow(FALSE); // ????????? ?????? ? ???? ???????
		 Show(SW_HIDE);		 // ????? ???? ???????
		 ::SendMessage(GetParent(), WM_PAINT, 0, 0);
							 // ???? ??? ?????????, ??????? ??????
		 TYPESAFE_DOWNCAST(Parent, TMainDialog)->
											ConfigurationDialogCreated = FALSE;
		 CloseWindow(IDCANCEL);
};
*/
/*
 * ---------------------------------------------------------
 *	EoF(copydisk.cpp)
 * ----------------------------------------------------------
 */

