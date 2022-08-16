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
							// Константа используется для функции biosequip()
							// биты 6-7 определяют кол-во НГМД.
							// см. Описание функции biosequip()
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
  EV_COMMAND(IDC_SBUTTON1,  SaveConfig),	  // Запись конфигурации
  EV_COMMAND(IDC_SBUTTON2,  ConfigHelp),	  // Помощь
  EV_COMMAND(IDC_SBUTTON3,  EndSession),	  // Выход

  EV_COMMAND(IDC_RSBUTTON1, SizeCopyRest144),  // Дисковод на 1.44
  EV_COMMAND(IDC_RSBUTTON2, SizeCopyRest121),  // Дисковод на 1.21
  EV_COMMAND(IDC_RSBUTTON3, SizeCopyRest720),  // Дисковод на 720
  EV_COMMAND(IDC_RSBUTTON4, SizeCopyRest360),  // Дисковод на 360
  EV_COMMAND(IDC_RDBUTTON1, DiskCopyRestA), // Дисковод А:
  EV_COMMAND(IDC_RDBUTTON2, DiskCopyRestB), // Дисковод В:
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
		 MessageBox("Производится запись конфигурации на диск", "Конфигурация", MB_OK);
};

void
TConfigurationDialog::SizeCopyRest144(){
		RadioButton3->SetCheck(BF_UNCHECKED);  // 720 Kb  - Неактивен
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - Неактивен
	if (FloppyDrive == 1){
		FloppySize = 1210;
		RadioButton1->SetCheck(BF_UNCHECKED);	// 1.44 Mb - Неактивен
		RadioButton2->SetCheck(BF_CHECKED);    // 1.21 Mb - Активен
	}
	else {
		FloppySize = 1440;
		RadioButton1->SetCheck(BF_CHECKED);		// 1.44 Mb - Активен
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - Неактивен
	};
};
void
TConfigurationDialog::SizeCopyRest121(){
		RadioButton3->SetCheck(BF_UNCHECKED);  // 720 Kb  - Неактивен
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - Неактивен
	if (FloppyDrive == 1){
		FloppySize = 1210;
		RadioButton1->SetCheck(BF_UNCHECKED);	// 1.44 Mb - Неактивен
		RadioButton2->SetCheck(BF_CHECKED);    // 1.21 Mb - Активен
	}
	else {
		FloppySize = 1440;
		RadioButton1->SetCheck(BF_CHECKED);		// 1.44 Mb - Активен
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - Неактивен
	};
};
void
TConfigurationDialog::SizeCopyRest720(){
		FloppySize = 720;
		RadioButton1->SetCheck(BF_UNCHECKED);	// 1.44 Mb - Неактивен
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - Неактивен
		RadioButton3->SetCheck(BF_CHECKED);  	// 720 Kb  - Активен
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - Неактивен
};
void
TConfigurationDialog::SizeCopyRest360(){
		RadioButton1->SetCheck(BF_UNCHECKED);  // 1.44 Mb - Неактивен
		RadioButton2->SetCheck(BF_UNCHECKED);  // 1.21 Mb - Неактивен
		RadioButton3->SetCheck(BF_UNCHECKED);	// 720 Kb  - Неактивен
	if (FloppyDrive == 1){
		FloppySize = 360;
		RadioButton4->SetCheck(BF_CHECKED);    // 360 Kb  - Активен
	}
	else {
		FloppySize = 1210;
		RadioButton1->SetCheck(BF_CHECKED);		// 1.44 Mb - Активен
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - Неактивен
	};
};
void
TConfigurationDialog::DiskCopyRestA(){
										// Для дисковода A:
		FloppyDrive = 1;
		RadioButton1->SetCheck(BF_UNCHECKED | BF_GRAYED);  // 1.44 Mb - Неактивен
		RadioButton1->Attr.Style |= WS_DISABLED;
		RadioButton2->SetCheck(BF_CHECKED);  	// 1.21 Mb - Активен
		RadioButton3->SetCheck(BF_UNCHECKED);	// 720 Kb  - Неактивен
		RadioButton4->SetCheck(BF_UNCHECKED);  // 360 Kb  - Неактивен
};
void
TConfigurationDialog::DiskCopyRestB(){
										// Для дисковода B:
		FloppyDrive = 2;
		RadioButton1->SetCheck(BF_CHECKED);  	// 1.44 Mb - Активен
		RadioButton2->SetCheck(BF_UNCHECKED | BF_GRAYED);  // 1.21 Mb - Неактивен
		RadioButton3->SetCheck(BF_UNCHECKED);	// 720 Kb  - Неактивен
		RadioButton4->SetCheck(BF_UNCHECKED | BF_GRAYED);  // 360 Kb  - Неактивен
};
void
TConfigurationDialog::EndSession(){
		 EnableWindow(FALSE); // Запрещаем доступ к окну диалога
		 Show(SW_HIDE);		 // Гасим окно диалога
		 ::SendMessage(GetParent(), WM_PAINT, 0, 0);
							 // Если все выключено, убиваем диалог
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

