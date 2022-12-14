/*
 * clntmoni - Data input system module -------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * Cl[i]n[e]tMoni[tory] - ??????, ??????????????? ??? ??????????? ??????????,
 * ?????????? ? ??????????? Info 73.
 * ?????????? ?????????  ? ?????????? ????? ??????????? ?
 * ??????????????, ???????????? ???????? ???????????.
 */
#include "commglob.h"
#include "monitory.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TMonitoryWindow
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE2(TMonitoryWindow, TMonitoringWindow, TWindow)
  EV_WM_TIMER,
  EV_WM_CLOSE,
END_RESPONSE_TABLE;

TMonitoryWindow::TMonitoryWindow(TWindow* parent)
	: TMonitoringWindow(parent), TWindow(parent)
{																			// ???? ???????????
//  pTStatusWindow= new TBInfoBox   (this, IDW_STATUSWND); // ????????? ??????
};

/*
 *		??????? - ??????? ????????? ???????????
 */
void
TMonitoryWindow::MonitCycleONOFF() {
	if (!bMonitoryON){                           // ???? ???????? ??? ?? ??????? ??????????
		pMonTimer->SetUserTimer(this->HWindow);	//	????????????? ?????? ????? ???????????
		bMonitoryON = TRUE;								// ??????? ???? ????????? ???????????
		MonitONECycle();									// ? ???? ??? ?????????? ??????????
	};
};
/*
 * 	??????? ?????? (? ???????) ????? ???????????
 */
void
TMonitoryWindow::MonitONECycle(){
															// ??? ???? 1
	ShowIntegersInMonitBox(1,
							"????????. ????????", 		BuffTransfFrom.BuffFromPC.awInputData1[0],  0,
							"??????????? ???????", 		BuffTransfFrom.BuffFromPC.awInputData1[2],  0,
							"??????????? ?????", 		BuffTransfFrom.BuffFromPC.awInputData1[9],  0,
							"?????? ??????? ???", 		BuffTransfFrom.BuffFromPC.awInputData1[12], 1,
							"?????? ??????? ???",	 	BuffTransfFrom.BuffFromPC.awInputData1[13], 2,
							"???? ??????", 				BuffTransfFrom.BuffFromPC.awInputData1[25], 0);
															// ??? ???? 2
	ShowIntegersInMonitBox(2,
							"???????? ?????", BuffTransfFrom.BuffFromPC.awInputData1[6],  0);
															// ??? ???? 3
	ShowIntegersInMonitBox(3,
							"???????? ????????", 	BuffTransfFrom.BuffFromPC.awInputData1[17],  0,
							"?????????? ????????", 	BuffTransfFrom.BuffFromPC.awInputData1[19],  0,
							"??? ????????? ?????", 	BuffTransfFrom.BuffFromPC.awInputData1[21],  0);
															// ??? ???? 4
	ShowIntegersInMonitBox(4,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[0],  1,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[1],  2,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[2],  3,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[3],  4,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[16], 17,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[17], 18,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[18], 19,
							"???????? ?/??????", BuffTransfFrom.BuffFromPC.awInputData2[19], 20);
															// ??? ???? 5
	ShowIntegersInMonitBox(5,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[0],  1,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[1],  2,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[2],  3,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[3],  4,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[4],  5,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[5],  6,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[6],  7,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[7],  8);
															// ??? ???? 6
	ShowIntegersInMonitBox(6,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[12], 13,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[13], 14,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[14], 15,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[15], 16,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[16], 17,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[17], 18,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[18], 19,
							"??? ????? 1", BuffTransfFrom.BuffFromPC.awInputData3[19], 20);
															// ??? ???? 7
	ShowIntegersInMonitBox(7,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[0],  1,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[1],  2,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[2],  3,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[3],  4,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[4],  5,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[5],  6,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[6],  7,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[7],  8);
															// ??? ???? 8
	ShowIntegersInMonitBox(8,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[12], 13,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[13], 14,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[14], 15,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[15], 16,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[16], 17,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[17], 18,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[18], 19,
							"??? ????? 2", BuffTransfFrom.BuffFromPC.awInputData4[19], 20);
};
/*
 *		?????????? ?????? ??????????? ?? ??????? ??????
 *		??? ?? ???? ? ???????, ?? ?? ?? ??????? 'Alt+F4'
 */
void
TMonitoryWindow::EvClose(){
	if (!bMonitoryON){						// ???? ?????????? ????????
		pMonTimer->KillUserTimer();	  	// ??????????? ???????
		MonitONECycle();      			  	// ????????? ????????? ????? ???????????
		CloseWindow(IDCANCEL);
	};
};
/*
 *		????????? ??????? ?????? ? ???????????
 */
void
TMonitoryWindow::MonitCycleService(){
	pMonTimer->ChangeUserTimerPeriod();	// ? ??????? ?????? ????????? ?????
	if (bMonitoryON){         				// ?????????? ??????????? ???????????
		 pMonTimer->KillUserTimer();  	// ??????? ?????? ?? ??????? ???????????
		 pMonTimer->SetUserTimer(this->HWindow);   // ? ????????? ??? ??? ? ???????????
		 MonitONECycle();	 	 		 		// ???? ???????????
	};
};
/*
 *		??????????? ?????????? ???????????
 */
void
TMonitoryWindow::EvTimer(UINT IDEvent)
{
	if (IDEvent == pMonTimer->IDEvTimer) {			// ???? ???????? ?????? ???????????
		 if (!bMonitoryON){                       // ????????, ????? ??? ?????? ????????? ??????????
			  EvClose();									// ????? ????????? ??? ??????
		 } else if (pMonTimer->TestUserTimer())	// ???? ?????????.?????? ?????
			  MonitONECycle(); 				 			// ?????????????? ???? ???????????
	};
};
/*
 * ---------------------------------------------------------
 *	EoF(clntmoni.cpp)
 * ---------------------------------------------------------
 */