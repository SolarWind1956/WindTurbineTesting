/*
 * tmodulpc - List type modules PC module -----------------------------
 *
 * Author:
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 *	tmodulpc - ?????? ?????????????? ?????? ??????????????
 * ?????????? ? ?????????? ??????? ?????-?????? ?
 * ??????????, ??????????? ? ????????? ??????.
 * ??? ?????????? ????????? ? ???????? 'DBNET/', ? ???????
 *	Paradox Engine 3.0, ? ????? ????? ??????:
 * AnalMdXX.DB, DscrMdXX.DB, CounMdXX.DB, FCouMdXX.DB, UserMdXX.DB,
 *	??? XX ????? ???????????, ? ???????? ?????????? ??????.
 */
/*
 * 	??????? ??? ?????? ? Paradox Engine
 */
#define 	pVOID(X)	pT_MDIO->fPoint(X)

#define	vDOUB(X)	*((double*)(pVOID(X)))
#define	pDOUB(X)	  (double*)(pVOID(X))
#define	vUINT(X)	*((UINT*)(pVOID(X)))
#define	vINTG(X)	*((INT16*)(pVOID(X)))
#define	pINTG(X)	  (INT16*)(pVOID(X))
#define  pCHAR(X)   (char*)(pVOID(X))
#define  pcCHAR(X)  (const char*)(pVOID(X))
#define  iLEN(X)	  (int)(pT_MDIO->fLen(X))
/*
 * 	????????? ???????????? ??????
 */
#include <owl\edit.h>
#include <stdio.h>
#include "commfunc.h"
#include "commglob.h"
#include "messagew.h"
#include "database.h"
#include "tmodulpc.h"
//----------------------------------------------------------------------------
//				Global variable
//----------------------------------------------------------------------------

/*
 *-------------------------------------------------------------
 *		????????? ???????
 *-------------------------------------------------------------
 */
/*
 * 		?????????? ? ?????????
 */
struct Ranges {
	char  Boundares[10];				// ??????? ?????????
	char  Dimension[4];				// ??????????? ?????????
};
											// ?????? ?????????? ??
											// ?????????? DC
const Ranges VoltRanges[4] =	{	{"-5..+5",   "V"},
											{"0..+5",    "V"},
											{"-10..+10", "V"},
											{"0..+10",   "V"}	};
											// ?????? ?????????? ??
											// ??????????? ????
const Ranges CurrRanges[4] =	{	{"0..20", "mA"},
											{"4..20", "mA"},
											{"0..10", "mA"},
											{"0..5",  "mA"} };
											// ????? ?????????? ???????? ??????
int SelectIndex_PC  = 0;         // ????????????
int SelectIndex_MDIO = 0;     	// ????? ?????????? ???????? ??????
											// ???????
											// ???? ????????? ? ????? ?????
static BOOL  bChangeParams = FALSE;
static BOOL  bFillList	   = FALSE;	// ???? ?????????? ??????
		 BOOL  NewPC 			= FALSE;	// ???? ???????? ?? ?????? ??
static RECORDNUMBER RecCountMDIO;   // ?????????? ??????? ? ????? ???????
/*
 *  -------------------------------------------------------------------------
 * 	Class TModulPCDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TModulPCDialog, TDialog)
  EV_WM_PAINT,

  EV_CBN_SELCHANGE (IDC_TMicroPCList_MDIO, TMicroPCList),

  EV_EN_CHANGE		 (IDC_TypeModule_MDIO,	 ChangeText),
  EV_EN_CHANGE		 (IDC_NumPlace_MDIO,		 ChangeText),
  EV_EN_CHANGE		 (IDC_NumChannel_MDIO,	 ChangeText),
  EV_EN_CHANGE		 (IDC_Resolution_MDIO,	 ChangeText),
  EV_EN_CHANGE		 (IDC_Tolerance_MDIO, 	 ChangeText),

  EV_COMMAND		 (IDC_Modul_A_MDIO,      TypeModule),
  EV_COMMAND		 (IDC_Modul_D_MDIO,      TypeModule),
  EV_COMMAND		 (IDC_Modul_C_MDIO,   	 TypeModule),
  EV_COMMAND		 (IDC_Modul_F_MDIO,      TypeModule),
  EV_COMMAND		 (IDC_Modul_U_MDIO,      TypeModule),

  EV_CBN_SELCHANGE (IDC_TModuleList_MDIO,	 TModuleList),
  EV_CBN_SELCHANGE (IDC_Boundares_MDIO,	 ChangeText),
  EV_CBN_SELCHANGE (IDC_Dimension_MDIO,	 ChangeText),

  EV_COMMAND		 (IDC_Current_MDIO,  	 CurrentVoltage),
  EV_COMMAND		 (IDC_Voltage_MDIO,  	 CurrentVoltage),

  EV_COMMAND		 (IDC_Save_MDIO,     	 SaveRecord),
  EV_COMMAND		 (IDC_Delete_MDIO,   	 DeleteRecord),

  EV_COMMAND		 (IDC_Help_MDIO, 		    Help			 ),
  EV_COMMAND		 (IDCANCEL,  		  		 EndSession	 ),
  EV_COMMAND		 (IDOK,		  		  		 EndSession	 ),
END_RESPONSE_TABLE;
//								C o n s t r u c t o r
TModulPCDialog::TModulPCDialog(TWindow* parent)
  : TDialog(parent, "ModulPCDIALOG"),	TWindow(parent)	{
  TypePC_CB	  		= new TComboBox   (this, IDC_TMicroPCList_MDIO);// ??? ??

  Analog_RB   		= new TRadioButton(this, IDC_Modul_A_MDIO);	  // ?????? "?????????? ??? ??????"
  Discret_RB 		= new TRadioButton(this, IDC_Modul_D_MDIO);	  // ?????? "?????????? ??? ??????"
  Counter_RB  		= new TRadioButton(this, IDC_Modul_C_MDIO);	  // ?????? "??????? ??????"
  FCounter_RB 		= new TRadioButton(this, IDC_Modul_F_MDIO);	  // ?????? "??????? ??????? ??????"
  User_RB	  		= new TRadioButton(this, IDC_Modul_U_MDIO);	  // ?????? "????????????? ??? ??????"

  TModuleList_CB 	= new TComboBox   (this, IDC_TModuleList_MDIO); // C????? ???????. ???????
  TypeModule_ED	= new TEdit   	  	(this, IDC_TypeModule_MDIO);  // ???????????? ????? ??????

  Current_RB  		= new TRadioButton(this, IDC_Current_MDIO);   // ?????? "?????????? ???"
  Voltage_RB  		= new TRadioButton(this, IDC_Voltage_MDIO);   // ?????? "?????????? DC"

  Boundares_CB		= new TComboBox   (this, IDC_Boundares_MDIO	);// ?????? ???????. ?????? ???????
  Dimension_CB   	= new TComboBox   (this, IDC_Dimension_MDIO);	// ?????? ???????. ????????????

  Help_ICBT   		= new TDrawICBT	(this, IDC_Help_MDIO);	// ??????
  Cancel_ICBT     = new TDrawICBT	(this, IDCANCEL);		   // ?????

  Save_BUT 			= new TButton  	(this, IDC_Save_MDIO);   // ??????
  Delete_BUT  		= new TButton     (this, IDC_Delete_MDIO);	// ????????

  Message_IB  		= new TBInfoBox   (this, IDC_Message_MDIO);// ????????? ??????
  Picture  			= new TBitmap		(*GetModule(), IDC_MODULEIO);

  valid = new TFilterValidator("0-9");		// ????????? ?????? ????? ??? ????
  if (valid){                             // ????? ?????? ?? ??????
		new TEdit(this,IDC_NumPlace_MDIO)->SetValidator(valid);
  };
  if (valid){      								// ????? ?????? ? ??????
		new TEdit(this,IDC_NumChannel_MDIO)->SetValidator(valid);
  };
  if (valid){      								// ?????????? (? ?????)
		new TEdit(this,IDC_Resolution_MDIO)->SetValidator(valid);
  };
  validdec = new TFilterValidator("0-9.");// ????????? ?????? ????? ??? ????
  if (validdec){      							// ????? ????????
		new TEdit(this,IDC_Tolerance_MDIO)->SetValidator(validdec);
  };
};
void
TModulPCDialog::EvPaint(){
	TWindow::EvPaint();
};
void
TModulPCDialog::Paint(TDC& dc, BOOL, TRect&)
{
	  DrawBitMap(dc, *Picture, 425, 5); // ?????????? ??????????? ????????
};
/*
 * 	????????? ??????? SetupWindow() ?????? ????????? :
 *		1. ???????? ????? ??????
 *		2.?????????? ?????? ? ????
 */
void
TModulPCDialog::SetupWindow(){      // ???????? ??????? ??????????
  RECORDNUMBER RecCount;				// ?????????? ??????? ? ?????
  SelectIndex_PC  = CodeController - 1;
  if (!CreateOpenMContrls()){       // ????????? ????	?? ???????
												// ????????????
	  if (pT_CONT->getRecCount() == 0){ // ?? ?????????? ??????????
												// ? ????????????
		  MessageW("?????? ?????/??????", "??? ?????????? ?? ????????????");
		  CloseMContrls();            // ??????? ???? ??
		  TDialog::SetupWindow();		// ?????????? ?????? ?? ??????
		  return;                     // !!! ?? ?? ?????
	  };
  };
  TDialog::SetupWindow();				// ?????????? ?????? ?? ??????
												// ??????? ?? ?? ????? ? ?????? ??
  if ((RecCount = pT_CONT->getRecCount()) > 0)	// ?????????? ???????
	  for (RECORDNUMBER i = 1; i <= RecCount; i++){
												// ?????? ?? ????? ???? ????????????
												// ? ???????????? ????? ??????
			pT_CONT->go(i);
			pT_CONT->getFieldFromCurrentRec("TypePC",
								 pT_CONT->fPoint("TypePC"));
												// ? ????????? ComboBox
			TypePC_CB->AddString((const char*)pT_CONT->fPoint("TypePC"));
		}; // for(;;)
  NewPC = TRUE;							// ??????? ???? ???????? ?? ?????? ??
  if (CodeController == 0){			// ?? ????????? INFO 73
		SelectIndex_PC = TypePC_CB->FindString("Info73", 0);
		CodeController = SelectIndex_PC + 1;
  };
  TypePC_CB->SetSelIndex(SelectIndex_PC); // ????????? ????????? ??????????
  Analog_RB->SetCheck(BF_CHECKED);	// ????????? "?????????? ??? ??????"
  TypeModule();							// ? ???? ?? ?????? ?????? ????
												// ?????? ? ????????? ????????????????
												// ????? ? ?????????? ?????? ???????
};
/*
 * 	??????????? ?????? ?? ? ??????????????? ??? ???? ???????
 */
void
TModulPCDialog::TMicroPCList(){  	//	????? ????? ?????????? ???????? ? ?????? ??
  SelectIndex_PC = TypePC_CB->GetSelIndex();
												// ??????? ??????? ??????????
  CodeController = SelectIndex_PC + 1;
  pT_CONT->go(CodeController);		// ?????? ?????? ?? ????? ??
  pT_CONT->FromFileToDynRec();
  Analog_RB->SetCheck(BF_CHECKED);	// ????????? "?????????? ??? ??????"
  TypeModule();							// ? ???? ?? ?????? ?????? ????
												// ?????? ? ????????? ????????????????
												// ?????
};
/*
 * 	??????????? ?????? ??????? ? ??????????????? ??? ??? ??????????
 *		?? ?????????? ??
 */
void
TModulPCDialog::TModuleList(){
  char AddedString[16];  		// ????????????? ??????, ???????
										// ? ??????????? ????? ???????? ? ??????
										// ????????? ??????????
										// ? ??????? :
										// "????? ??????[????? ????? ?? ??????](????? ??????)"
										// ??????: "AI101A[10](0)"
  bFillList = TRUE;
  for (char i = 0; i < 16; i++) AddedString[i] = ' ';
  if (NewPC){								// ??? ??????????? ?????? ??????
	  SelectIndex_MDIO = 0;				// ??? ?????? ??
	  TModuleList_CB->ClearList();  	// ??????? ?????? ???????
	  TModuleList_CB->AddString("?????");	// ??????? ????? "?????" ? ??????
												// ??? ???????????? ?????? ???????
												// ?????? ??? ???????? ??????
												// ????????
	  for (RECORDNUMBER i = 1; i <= RecCountMDIO; i++){
			pT_MDIO->go(i);     		// ?????? ?????????? ?? ?????
			pT_MDIO->FromFileToDynRec(); // ? ????????
												// ?? ? ?????? ??????????
												// ????????? ?????? ? ??????? ??
												// ? ?????? 'ListBox'
			sprintf(AddedString, "%s[%d](%d)",
					  pcCHAR("TypeModule"),
					  vINTG("NumPlace"),
					  vINTG("NumChannel"));
			TModuleList_CB->AddString(AddedString);
	  };
	  TModuleList_CB->SetSelIndex(SelectIndex_MDIO);
	  NewPC = FALSE;                 // ?????????? ???? ???????? ?? ?????? ??
  } else {									// ????? ?????????? ????????
	  SelectIndex_MDIO = TModuleList_CB->GetSelIndex();
  };
  Boundares_CB->ClearList();	 			// ??????? ?????? ??????
  Dimension_CB->ClearList();	 			// ??????? ?????? ????????????
  if (SelectIndex_MDIO != 0){           // ??? ??????????? ???????????? ??????????
	  pT_MDIO->go(SelectIndex_MDIO);
	  pT_MDIO->FromFileToDynRec();		// ?????? ?????????? ?? ?????
													// ????????? RadioButton ? ????????? ??????
													// ?????? ? ???????????? ? ?.?.
													// c?????????????? ??????
	  if (strcmp(pCHAR("TypeRange"), "CURRENT") == 0)
	  {
			Current_RB->SetCheck(BF_CHECKED);
			Voltage_RB->SetCheck(BF_UNCHECKED);
			for (int i = 0; i < sizeof(CurrRanges)/sizeof(CurrRanges[0]); i++)
				  Boundares_CB->AddString(CurrRanges[i].Boundares);
			Dimension_CB->AddString(CurrRanges[0].Dimension);
	  } else if (strcmp(pCHAR("TypeRange"), "VOLTAGE") == 0){
			Current_RB->SetCheck(BF_UNCHECKED);
			Voltage_RB->SetCheck(BF_CHECKED);
												// ?????? ??????
			for (int i = 0; i < sizeof(VoltRanges)/sizeof(VoltRanges[0]); i++)
				  Boundares_CB->AddString(VoltRanges[i].Boundares);
												// ???????????
			Dimension_CB->AddString(VoltRanges[0].Dimension);
	  };
												// ???????
	  Boundares_CB->SetSelIndex(Boundares_CB->FindString(pCHAR("Boundares"), 0));
												// ???????????
	  Dimension_CB->SetSelIndex(Dimension_CB->FindString(pCHAR("Dimension"), 0));
												// ????? ??????
	  SetDlgItemText(IDC_TypeModule_MDIO, pcCHAR("TypeModule"));
												// ????? ?????? ?? ??????
	  SetDlgItemInt(IDC_NumPlace_MDIO,    vUINT("NumPlace"));
												// ????? ??????
	  SetDlgItemInt(IDC_NumChannel_MDIO,  vUINT("NumChannel"));
												// ?????????? ??? / ???
	  SetDlgItemInt(IDC_Resolution_MDIO,  vUINT("Resolution"));
												// ????? ???????? ("commfunc.h")
	  SetDlgItemDoub(this->HWindow, IDC_Tolerance_MDIO, vDOUB("Tolerance"));

	  Delete_BUT->EnableWindow(TRUE);
  } else {									// ??? ??????????? ???????????? ??? ???????? ??????
												// ???????? ?? ????????? :
	  Current_RB->SetCheck(BF_CHECKED);
	  Voltage_RB->SetCheck(BF_UNCHECKED);
	  for (int i = 0; i < sizeof(CurrRanges)/sizeof(CurrRanges[0]); i++)
			 Boundares_CB->AddString(CurrRanges[i].Boundares);

	  Dimension_CB->AddString(CurrRanges[0].Dimension);
												// ????? ??????
	  SetDlgItemText(IDC_TypeModule_MDIO,  "");
												// ????? ?????? ?? ??????
	  SetDlgItemText(IDC_NumPlace_MDIO, "0");
												// ????? ??????
	  SetDlgItemText(IDC_NumChannel_MDIO, "0");
												// ?????????? ??? / ???
	  SetDlgItemText(IDC_Resolution_MDIO, "0");
												// ????? ????????
	  SetDlgItemText(IDC_Tolerance_MDIO, "0");
	  Delete_BUT->EnableWindow(FALSE);
	};
  Save_BUT->EnableWindow(FALSE);
  bFillList 	 = FALSE;
  bChangeParams = FALSE;				// ??????? ???? ?????????
};
/*
 *		???????????? ??? ??????? RadioButton'? "??????????"
 *		? RadioButton'? "?????????? ???"
 */
void
TModulPCDialog::CurrentVoltage(){
	int i;
	Boundares_CB->ClearList();			// ??????? ??????
	Dimension_CB->ClearList();			// ??????? ??????
	switch (Voltage_RB->GetCheck()){
		case BF_CHECKED   :
				for (i = 0; i < sizeof(VoltRanges)/sizeof(VoltRanges[0]); i++)
					Boundares_CB->AddString(VoltRanges[i].Boundares);
				Dimension_CB->AddString(VoltRanges[0].Dimension);
				break;
		case BF_UNCHECKED :
				for (i = 0; i < sizeof(CurrRanges)/sizeof(CurrRanges[0]); i++)
					  Boundares_CB->AddString(CurrRanges[i].Boundares);
				Dimension_CB->AddString(CurrRanges[0].Dimension);
				break;
	};
	bChangeParams = TRUE;				// ??????? ???? ?????????
	Save_BUT->EnableWindow(bChangeParams);
};
/*
 *		???????????? ??? ??????? RadioButton'?? ?? ???? ???????:
 *		??????????, ??????????, ???????, ??????? ???????, ?????????????
 */
void
TModulPCDialog::TypeModule(){
	CloseModuls();     					// ??????? ?????? ???? ???????
	NewPC = TRUE;							// ????????? ????? ??????? ?? ??? ????????? ???????
												// MDIOTModuleList ??? ??? ?????? ??
												// ??????? ????? ???? ???????
	if (Analog_RB->GetCheck() == BF_CHECKED) {
			CreateOpenModuls(SelectIndex_PC + 1, "ANALMD");
	} else if (Discret_RB->GetCheck() == BF_CHECKED) {
			CreateOpenModuls(SelectIndex_PC + 1, "DSCRMD");
	} else if (Counter_RB->GetCheck () == BF_CHECKED) {
			CreateOpenModuls(SelectIndex_PC + 1, "COUNMD");
	} else if (FCounter_RB->GetCheck() == BF_CHECKED) {
			CreateOpenModuls(SelectIndex_PC + 1, "FCOUMD");
	} else if (User_RB->GetCheck    () == BF_CHECKED) {
			CreateOpenModuls(SelectIndex_PC + 1, "USERMD");
	};                               // ???-?? ??????? ? ????? ???????
	RecCountMDIO   =  pT_MDIO->getRecCount();
	TModuleList();						   // ????????? ?????? ???????
	bChangeParams = FALSE;				// ??????? ???? ?????????
};
/*
 * 	??????? ??????? ???? ??? ?????? ???? ???????
 *		???-???? ? ??????? ??????, ???????????? ? ?.?.
 */
void
TModulPCDialog::ChangeText(){
	if (!bFillList){
		Message_IB->ClearShadeBox();
		bChangeParams = TRUE;				// ??????? ???? ?????????
		Save_BUT->EnableWindow(bChangeParams);
		Delete_BUT->EnableWindow(!bChangeParams);
	};
};
/*
 *		?????????? ??? ???????? ?????? ? ?????
 */
void
TModulPCDialog::SaveRecord(){
  char AddedString[16];
  for (int i = 0; i < 16; i++) AddedString[i] = ' ';
														// ??????? ????? ?????? :
														// ????? ??????
  GetDlgItemText(IDC_TypeModule_MDIO, pCHAR("TypeModule"),
												 iLEN("TypeModule"));
														// ????? ?????? ?? ??????
  vINTG("NumPlace")   = GetDlgItemInt(IDC_NumPlace_MDIO);
														// ????? ??????
  vINTG("NumChannel") = GetDlgItemInt(IDC_NumChannel_MDIO);
														// ??????? ?????????
  GetDlgItemText(IDC_Boundares_MDIO, pCHAR("Boundares"),
												iLEN("Boundares"));
														// ??????????? ?????????
  GetDlgItemText(IDC_Dimension_MDIO, pCHAR("Dimension"),
												iLEN("Dimension"));
														// ?????????? ???/???
  vINTG("Resolution") = GetDlgItemInt(IDC_Resolution_MDIO);
														// ????? ???????? ("commfunc.h")
  vDOUB("Tolerance")  = GetDlgItemDoub(this->HWindow, IDC_Tolerance_MDIO);
														// ????????? ?????? ??? ??????????
														// ??? ????????? ? ?????? ???????
  sprintf(AddedString, "%s[%d](%d)",
				pCHAR("TypeModule"),
				vINTG("NumPlace"),
				vINTG("NumChannel"));
														// ??? ???????
  if (Voltage_RB->GetCheck() == BF_CHECKED){
		strcpy(pCHAR("TypeRange"), "VOLTAGE");
  } else if (Current_RB->GetCheck() == BF_CHECKED){
		strcpy(pCHAR("TypeRange"), "CURRENT");
  };
  if (SelectIndex_MDIO == 0){ 				// ???? ??????? ????? ????????
	  TModuleList_CB->AddString(AddedString);	// ??????? ??????? ? ??????
													// ????? ????????? ? ???? ??????
	  vINTG("IndexCode") = (short)((TModuleList_CB->GetCount()) - 1);
	  pT_MDIO->clearRecordForWrite();
	  pT_MDIO->FromDynRecToFile();
	  pT_MDIO->append();						// ?????? ?????? - ??????????
	  SelectIndex_MDIO = vINTG("IndexCode");
	  pT_CONT->go(SelectIndex_PC + 1); // ?????? ?????? ?? ????? ??
	  pT_CONT->IncrRefrence();
													// ?????? ??? ?? MContrls ?????? - ?????????
													// ??-?? ???? ???-?? ??????
  } else {          	                  // ???????? ?????? ??????
													// ????? ????????? ? ???? ??????
	  vINTG("IndexCode")= (short)(SelectIndex_MDIO);
													// ?????? ??????? ?? ComboBox'?
	  TModuleList_CB->DeleteString(SelectIndex_MDIO);
													// ??????? ??????? ? ?????? ?????????
													// ??????????
	  TModuleList_CB->InsertString(AddedString, SelectIndex_MDIO);
	  pT_MDIO->FromDynRecToFile();
	  pT_MDIO->update();						// ?????? ??? ?? MContrls ?????? - ?????????
  };
													// ????????? ??????????? ???????
  TModuleList_CB->SetSelIndex(SelectIndex_MDIO);
  bChangeParams = FALSE;               // ??????? ???? ?????????
  Save_BUT->EnableWindow(bChangeParams);
  Delete_BUT->EnableWindow(!bChangeParams);
};
/*
 * 	???????? ??????? ?????? ?? ?????
 */
void
TModulPCDialog::DeleteRecord(){
  if (vINTG("Refrence") == 0){
		pT_MDIO->delCurRecord();          // ???????? ?????? ?? ?????
														// ???????? ???????? ?? ??????
		TModuleList_CB->DeleteString(SelectIndex_MDIO);
		SelectIndex_MDIO--;                  // ????????? ?????????? ???????
		TModuleList_CB->SetSelIndex(SelectIndex_MDIO);
		pT_CONT->go(SelectIndex_PC + 1);// ?????? ?????? ?? ????? ??
		pT_CONT->DecrRefrence();
		TModuleList();							   // ?????????? ?????????? ?
  } else {											// ?????????? ????????
		Message_IB->ClearShadeBox();
		Message_IB->StringToShadeBox(2, 2, RGB(255, 0, 0),
											  "?????? ?????? ?????? ???????,"
											  "??? ??? ?? ???? ?????????? ??????");
  };
};
/*
 *		???????? ????? ? ?????
 */
void
TModulPCDialog::EndSession(){
  if (bChangeParams){
	  if (MessageBox("?????? ?? ?????????.\n????????? ????? ??????? ?",
						  "?????? ?????/??????",
						  MB_YESNO) == IDYES){
			SaveRecord();				 // ???????? ??????
	  };
  };
  CloseModuls();                // ??????? ???? ???????
  CloseMContrls();                // ??????? ???? ??
  CloseWindow();
};

/*
 * ---------------------------------------------------------
 *	EoF(tmodulpc.cpp)
 * ----------------------------------------------------------
 */
