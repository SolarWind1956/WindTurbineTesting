/*
 * testctrl - ?????? ??????? ?????????
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 * TestCtrl - ????????? ?????????? ??????? ?????????.
 * ??????????? ??? ListBox'?:
 *			- ?????? ?????????? ?????????;
 *			- ?????? ????? ?????????;
 *			- ?????? ????? ???.
 * ?????? ?????????? ????????? ?????????? ????? ?????? ???? ?????????
 * ? ??? ?? ??????????????? ??????? ? ????????? ?? ?????????????.
 * ??? ?????????? ????????? ????? ???? ???????? ?????? ?????? ????????
 * ? ???????? ???? ? ??????? ?????????????. ? ?????? ??? ??????? ?????????
 * ??????? ??????? ? ?????? ????????? ????????? ? ??????? ?????? ??????.
 * ??? ???????? ListBox'?? ???????????? ??????? ????????? ? 'dlistvie.h'
 */
 /*
 * 	??????? ??? ?????? ? Paradox Engine
 */
#define 	pVOID(X)	pT_TJNL->fPoint(X)

#define	vDOUB(X)	*((double*)(pVOID(X)))
#define	pDOUB(X)	  (double*)(pVOID(X))
#define	vUINT(X)	*((UINT*)(pVOID(X)))
#define	vINTG(X)	*((INT16*)(pVOID(X)))
#define	pINTG(X)	  (INT16*)(pVOID(X))
#define  pCHAR(X)   (char*)(pVOID(X))
#define  pcCHAR(X)  (const char*)(pVOID(X))
#define  iLEN(X)	  (int)(pT_TJNL->fLen(X))
/*
 * 	????????? ???????????? ??????
 */
#include "testctrl.h"
#include "database.h"
char 		TypesTests[6][40] =				// ???? ?????????
			{"??????????? ?????????? ??????????????",
			 "??????????? ????? ? ??? ??????? ???",
			 "???????? ???????? ??????????????",
			 "??????????? ?????? ???",
			 "????????? ?????? ?? ?????????",
			 "???????? ?????? ?????? ???"};
char AbbrevTypesTests[6][5] =	{  	// ???????????? ????? ????????? ??? ????? ????? ?????????
										 "PwCh", "WnAz", "    ",
										 "    ", "    ", "    "
										};
char TypesWindTurbines[4][12] =     // ?????????? ???
		{"???-250 N5",
		 "???-250 N7",
		 "???-250 N10",
		 "???-250 N11"};
typedef struct {                    // ????????? ?????????
	unsigned int 	Index;				// ????? ?????????
	char 				TimeTest  [9];	  	// ????? ???????
	char 				TypeTest  [40];   // ???
	char 				NumWStTest[12];   // ???
	char 				StTest    [80];	// ?????? ? ??????
	HWND 				hChldWnd;		 	// ???. ????? ???? ?????????
	HINSTANCE		hInstTestApp;
} StructTest;
StructTest*	WorkTests;					// ?????????? ?????????
StructTest*	MiscTest;               // ????????????? ?????? ?????????? ?????????

int	iTypeTest   	= 0;
int 	iNumWStTest 	= 0;
												// ??????? ??????????? ? ??????
int 	Tab_Stops[4] 	= {23, 60, 247, 297};
												// ?????????? ?????????
int  	iQuantTests 	= 0;    	 		// ???-?? ?????????? ?????????
int  	iQuantRec   	= 0;				// ???-?? ??????? ? ??????? ?????????
int  	iNumTests   	= 0;				// ???-?? ????????? ?????????
/*
 *  -------------------------------------------------------------------------
 * 	Class TTestCtrlDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TTestCtrlDialog, TDialog)
  EV_WM_CLOSE,
  EV_LBN_SELCHANGE(IDC_TLISTBOX2, ListTypeTests),// ?????? ????? ?????????
																 // ?????? ????? ???
  EV_LBN_SELCHANGE(IDC_TLISTBOX3, ListTypeWindTurbine),
  EV_LBN_DBLCLK	(IDC_TLISTBOX1, MaximizeTestApp	  ),
  EV_COMMAND		(CM_TESTHELP,   TestCtrlHelp),  // ??????
  EV_COMMAND		(CM_TESTEXIT,   EndSession),	  // ?????
  EV_COMMAND		(CM_TESTSTART,  TestCtrlStart), // ?????? ?????????
  EV_COMMAND		(CM_TESTSTOP,   TestCtrlStop),  // ????????? ?????????
END_RESPONSE_TABLE;
//								C o n s t r u c t o r
TTestCtrlDialog::TTestCtrlDialog(TWindow* parent)
  : TDialog(parent, "TestCtrlDIALOG"),	TWindow(parent) {
  TestCtrlDialogCreated = FALSE;
  Button1   = new TDrawICBT    (this, CM_TESTHELP);   // ??????
  Button2   = new TDrawICBT    (this, CM_TESTEXIT);   // ?????
  Button3   = new TDrawICBT    (this, CM_TESTSTART);  // ?????? ?????????
  Button4   = new TDrawICBT    (this, CM_TESTSTOP);   // ????????? ?????????
  ListBox1  = new TDrawListView(this, IDC_TLISTBOX1); // ?????? ?????????? ?????????
  ListBox2  = new TDrawListView(this, IDC_TLISTBOX2); // ?????? ????? ?????????
  ListBox3  = new TDrawListView(this, IDC_TLISTBOX3); // ?????? ????? ???
};
void
TTestCtrlDialog::MaximizeTestApp(){
	::ShowWindow(WorkTests[ListBox1->GetSelIndex()].hChldWnd, SW_RESTORE);
};
void
TTestCtrlDialog::ListTypeTests(){
												// ????????? ??? ?????????
	iTypeTest = ListBox2->GetSelIndex();
};
void
TTestCtrlDialog::ListTypeWindTurbine(){
												// ????????? ???
	iNumWStTest = ListBox3->GetSelIndex();
};
/*
 * 	?????????? ??????? ListBox
 */
									// ?????? ?????????? ?????????
void
CreateListBox1(TListBox* ListBox1){
  for (char i = 0; i < iQuantTests; i++){
		ListBox1->AddString(WorkTests[i].StTest);
  };
  if (ListBox1->GetCount() > 0) { // ???? ?????? ?? ??????
		ListBox1->SetSelIndex(0);   // ???????? ?????? ??????
  };
};
									// ?????? ????? ?????????
void
CreateListBox2(TListBox* ListBox2){
  for (char i = 0; i < 6; i++)
		ListBox2->AddString(TypesTests[i]);
  ListBox2->SetSelIndex(0);		 // ???????? ?????? ??????
											 // ??????? ? ??????
											 // ????????? ???
};
									// ?????? ???
void
CreateListBox3(TListBox* ListBox3){
  for (char i = 0; i < 4; i++)
		ListBox3->AddString(TypesWindTurbines[i]);
  ListBox3->SetSelIndex(0);         // ???????? ?????? ??????
												// ??????? ? ?????? ????????? ???
};
												// ????????? 'SetupWindow'
void											// ??????????????? ?????????
TTestCtrlDialog::SetupWindow(){
	CreateOpenTestJrnlFile();  		// ??????? ????-?????? ?????????
	TDialog::SetupWindow();				// ??????? ?????? ... ? ????? ...
												// ????????????? ??????? ??????????
	ListBox1->SetTabStops(4, Tab_Stops);
												// ?????????? ?????? ????? ?????????
	void (*pCreateListBox2)(TListBox*);
	pCreateListBox2 = CreateListBox2;
	ListBox2->CreateListBox(pCreateListBox2);
												// ?????????? ?????? ???
	void (*pCreateListBox3)(TListBox*);
	pCreateListBox3 = CreateListBox3;
	ListBox3->CreateListBox(pCreateListBox3);
};
/*
 * ???????????? ?????? ??? ?????? ? ListBox ?????????? ?????????
 */
void
TTestCtrlDialog::FormatString(int NumItem){

	strcpy(WorkTests[NumItem].StTest,     " ");
	strcpy(WorkTests[NumItem].TypeTest,   TypesTests[iTypeTest]);
	strcpy(WorkTests[NumItem].NumWStTest, TypesWindTurbines[iNumWStTest]);

	sprintf (WorkTests[NumItem].StTest,
				" %d\x09%s\x09%s\x09%s\x09%s",
				iNumTests,
				WorkTests[NumItem].TimeTest,
				WorkTests[NumItem].TypeTest,
				WorkTests[NumItem].NumWStTest);
						// ??????? ?????? ? ?????????? ????
	vINTG("IndexCode") = iNumTests; // ????? ?? ???????
												// ????? ????????? ?????????
	strcpy(pCHAR("EndTime"),	WorkTests[NumItem].TimeTest);
												// ???????????? ?????????
	strcpy(pCHAR("TestName"),  WorkTests[NumItem].TypeTest);
	vINTG("TestMark") = 0;				// ?????????? ?????????
	vINTG("TestNumber") = 0;        	// ????? ????? ?????????
	vINTG("SeriesNumber") = 0;       // ????? ????????? ? ?????
	sprintf(pCHAR("PerdCyclTime"), "00:00:00");
	strcpy(pCHAR("WindTurbine"), WorkTests[NumItem].NumWStTest);
	vINTG("CodeTypeWT") = 0;        // ??? ???
};

void InitItemStruct(int NumItem){
	WorkTests[NumItem].Index 				= 	iNumTests;

	strcpy(WorkTests[NumItem].TimeTest,   	" ");
	strcpy(WorkTests[NumItem].TypeTest,   	" ");
	strcpy(WorkTests[NumItem].NumWStTest, 	" ");
	strcpy(WorkTests[NumItem].StTest, 		" ");

	WorkTests[NumItem].hChldWnd  			=  NULL;
};
void               						// ?????? ?????????
TTestCtrlDialog::TestCtrlStart(){
	const char ProgName [14] = "powercha.exe";
	char ProgParam[70];
	strcpy(ProgParam, " ");
	if (iQuantTests == 0){				// ??? ??????? ?????????
		WorkTests = new StructTest[iQuantTests + 1];
		WorkTests[iQuantTests].Index = 0;
		iNumTests++;
		InitItemStruct(iQuantTests);
	} else {									// ???????? ????????????? ?????. ?????? ?
												// ???????? iQuantTests - 1
		MiscTest = new StructTest[iQuantTests];
												// ????????? ?????? ?????? ?? ?????????? ??
												// ?????????? ??????
		memmove(MiscTest, WorkTests, size_t(sizeof(StructTest) * (iQuantTests)));
		delete[] WorkTests;           // ?????? ?????? ?????????? ?????????
												// ???????? ?????. ?????? ?????????? ????????? ?
												// ???????? iQuantTests
		WorkTests = new StructTest[iQuantTests + 1];
		iNumTests++;
		InitItemStruct(iQuantTests);
												// ????????? ?????? ?????? ?? ?????????? ??
												// ??????????? ??????? ? ?????????? ??????
		memmove(WorkTests, MiscTest, size_t(sizeof(StructTest) * (iQuantTests)));
		delete[] MiscTest;            // ?????? ????????? ?????? ?????????? ?????????
	};
	iQuantTests++; 						// ???????? ?????????? ?????????
	_strdate(pCHAR("BeginDate"));    // ???????? ???? ?????? ????????? ????????? ????
												// ???????? ???? ????????? ????????? ????????? ????
												// ??? ???? ????? ? ???? ??????? ????????? ?? ??????
												// ?????? ???????? ???? ????????? ?????????
												// ?? ?????????? ????????? ??? ???????? ??????? ??
												// ???????? ???? ????????? ?????????
	_strdate(pCHAR("EndDate"));		// ????? ?????? ?????????
	strcpy(WorkTests[iQuantTests - 1].TimeTest, _strtime(pCHAR("BeginTime")));
	FormatString(iQuantTests - 1);	// ?????????????? ?????? ??? ????????? ?? ? ??????
												// ??????? ????????? ?????????
												// ? ?????? ?????????? ?????????
	ListBox1->AddString(WorkTests[iQuantTests - 1].StTest);
												// ??????? ? ?????? ?????????? ?????????
												// ????????? ?????????
	ListBox1->SetSelIndex(iQuantTests - 1);
/*
 *			-----------------------------------------------------
 *			?????? ????????? ?????????? ??? ?????????? ?????????
 *			-----------------------------------------------------
 */
												//	????????? ??? ??????????? ??????????? ??? :
												//	"??? ????????? + ????? ???"
												//	?22 - ?????? " ??? ?????? ?????????
	sprintf(ProgParam, "\x22%d %s %s\x22 \x22%s\x22",
							 iNumTests,
							 WorkTests[iQuantTests - 1].TypeTest,
							 WorkTests[iQuantTests - 1].NumWStTest,
							 AbbrevTypesTests[0/*iTypeTest*/]);
	WorkTests[iQuantTests - 1].hInstTestApp =
				ShellExecute(HWindow,	  	  		// Handle ?????? ????
								 NULL,			 	 	// ???? ??? ??????? ??????????
								 ProgName,		  		// ??? ??????????
								 ProgParam,			  	// ?????????
								 NULL,			  		// ??????? ???????
								 SW_SHOWNORMAL);		// ?????? ???? ??????????
															// ??????? handle ???? ??????????? ??????????
	WorkTests[iQuantTests - 1].hChldWnd = GetActiveWindow();
//	---------------------------------------------------------
	SetActiveWindow();					// ???????????? ???? ????
	pT_TJNL->FromDynRecToFile();     // ??????? ?????? ? ?????? ????????? ? ?????? ?????????
	pT_TJNL->append();               // ? ?????????? ???? ?????????
};
void                                // ????????? ?????????
TTestCtrlDialog::TestCtrlStop(){
	int iSelectTest;
	if (iQuantTests != 0) {
		iQuantTests--;						// ???????? ????? ?????????? ?????????
												// ????????? ?????? ?? ???????????????? ????????? ??
												// ??????? ?????????
		iSelectTest = ListBox1->GetSelIndex();
												// ?????? ?????????? ?? ???????????
												// ?????
		pT_TJNL->go(iQuantRec + WorkTests[iSelectTest].Index);
		pT_TJNL->FromFileToDynRec();
		_strdate(pCHAR("EndDate"));   // ???? ????????? ?????????
		_strtime(pCHAR("EndTime"));   // ????? ????????? ?????????
		pT_TJNL->FromDynRecToFile();
		pT_TJNL->update();            // ??????? ?????? ?? ????????? ????????? ? ?????? ?????????
/*
 *			??????? ????????? ?????????? ??? ?????????????? ?????????
 */
												// ?????? ????????? ????????? ??????????
												// ???????????????? ??????? ????????? ???
												// ???????? ?????????? ??????? ?????????
		::SendMessage(WorkTests[iSelectTest].hChldWnd, WM_CLOSE, 0, 0);

		ListBox1->DeleteString(iSelectTest);

		if (iSelectTest == 0) {       // ??????? ?????????? ????????? ????? ????????? ??
			ListBox1->SetSelIndex(0);  // ?????? ?????????
		} else {
			ListBox1->SetSelIndex(iSelectTest - 1);
		};                            // ????????? ????????? ??????? ??
												// ?????. ??????? ?????????? ?????????
		for (char i = iSelectTest; i < iQuantTests; i++){
			WorkTests[i] = WorkTests[i + 1];
		};    								// ???????? ????????????? ?????. ?????? ?
												// ???????? iQuantTests
		if (iQuantTests != 0){
			MiscTest = new StructTest[iQuantTests];
												// ????????? ?????? ?????? ?? ??????????
												// ?? ?????????? ??????
			memmove(MiscTest, WorkTests, size_t(sizeof(StructTest) * iQuantTests));
			delete[] WorkTests;        // ?????? ?????? ?????????? ?????????
												// ???????? ?????. ?????? ?????????? ????????? ?
												// ???????? iQuantTests
			WorkTests = new StructTest[iQuantTests];
			if (WorkTests == NULL) MessageBox("Error WorkTests", "Error", MB_OK);
												// ????????? ?????? ?????? ?? ?????????? ??
												// ??????????? ??????? ? ?????????? ??????
			memmove(WorkTests, MiscTest, size_t(sizeof(StructTest) * iQuantTests));
			delete[] MiscTest;         // ?????? ????????? ?????? ?????????? ?????????
		};
	};
};

void
TTestCtrlDialog::EvClose(){
	 EndSession();
};
void
TTestCtrlDialog::EndSession(){
	if (iQuantTests != 0){
		 EnableWindow(FALSE); 						// ????????? ?????? ? ???? ???????
		 Show(SW_HIDE);					 			// ????? ???? ???????
		 ::SendMessage(GetParent(), WM_PAINT, 0, 0);
	 } else {											// ???? ??? ?????????, ??????? ??????
		 CloseTestJrnlFile();							// ??????? ????-?????? ?????????
		 TestCtrlDialogCreated = FALSE;
		 CloseWindow(IDCANCEL);
	 };
};
/*
 * ---------------------------------------------------------
 *	EoF(testctrl.cpp)
 * ----------------------------------------------------------
 */

