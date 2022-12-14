#if !defined(__MONITWIN_H)
#define __MONITWIN_H
/*
 * monitwin.h - Handle file 'MonitWin' Module ---------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\owlpch.h>
#include <bwcc.h>
#include <owl\groupbox.h>
#include "binfobox.h"
/*
 *		Function's prototips
 */
//----------------------------------------------------------------------------
const char PANEL_NUM 		= 8;				// ?????????? ??????? ? ?????
const char NAME_STR_LENTH 	= 25; 			// ????? ?????? ???????????? ?????? ? ????????
const char VAL_STR_LENTH	= 50;				// ?????????? ???????? ?????, ??????? ?????????? ?????
const char NVG_STR_LENTH	= 1 + 2 + 1;	// ?????????? ???????? ?????? ?????? ????? ? ??????,
														// ? ?????? ????? ? ?????? ??????? ??????

const char displUp 			= 2;				// ???????? ?????? ?????? ?? ??????? ????? ??????
const char displPANEL  		= 23;				// ???????? ????? ????? ????????
const char displSTR    		= 2;  			// ???????? ????? ??? ?????? ??????
const char displVAL			= 125;			// ???????? ????? ??? ?????? ?????

const char displVAL_NUM1	= 107;			// ??? ???????????? ??????
const char displVAL_NUM2   = 102;         // ??? ??????????? ??????


struct TBoxForInteger{
		 BOOL    		isEmpty[PANEL_NUM]; 								// ???????? ?????????? ?????
		 char  			StrText[PANEL_NUM][NAME_STR_LENTH];			// ??????? ?? ???????
		 int 				aiValue[PANEL_NUM];        					// ?????
		 int				NumberInGroup[PANEL_NUM];						// ?????? ????? ? ??????
};
struct TBoxForDouble {
		 BOOL    		isEmpty[PANEL_NUM]; 								// ???????? ?????????? ?????
		 char  			StrText[PANEL_NUM][NAME_STR_LENTH];		 	// ??????? ?? ???????
		 double  		adValue[PANEL_NUM];        					// ?????
		 int				NumberInGroup[PANEL_NUM];						// ?????? ????? ? ??????
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TInfoBox
 *  -------------------------------------------------------------------------
 */
class TMonitoringWindow;
class TInfoBox : public TGroupBox {
public:
		HFONT		 hButnFont;
		LOGFONT   ButnFontRec;
		typedef   LOGFONT FAR * LPLOGFONT;
		COLORREF FGColor;
		char  displVAL_NUM;              // ???????? ??? ?????? ?????? ????? ? ??????
		BOOL 	FlagInfoToShadeBox;			// ???? ?????????? ?????? ???????????
													// ?????????? ? ????
		int   fnObject;
		void  (*pOutFunction)(TInfoBox*);
													// ?????????????? ?????????? ????????

		BOOL  isEmpty[PANEL_NUM];							// ???????? ?????????? ?????
		char  StrText[PANEL_NUM][NAME_STR_LENTH];		// ??????? ?? ???????
		char  StrValue[PANEL_NUM][VAL_STR_LENTH];   	// ?????? ???????? ?????
		int	NumberInGroup[PANEL_NUM];					// ?????? ??????? ????? ? ?????? (? ???????)

		TInfoBox(TWindow* parent, int resourceId, TModule* module = 0);
		void 	InfoToShadeBox		(void(*)(TInfoBox*));
//		void TInfoBox::StringToShadeBox(int X, int Y, char* TextString);
		void 	Paint					(TDC&, BOOL, TRect&);
		void 	EvPaint				();
		void	GetWindowClass		(WNDCLASS&);
		char 	far* GetClassName	();
		DECLARE_RESPONSE_TABLE(TInfoBox);
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TMonitoringWindow
 *  -------------------------------------------------------------------------
 */

// 	? ? ? ? ? ? ? ? ? ?     ? ? ? ? ?
class TMonitoringWindow : public TDialog {
  public:
												  // ?????????????? ?????????? ????????
												  // ???? ???????????
	TInfoBox*    pTMonitorBox1;        // ???? 1
	TInfoBox*    pTMonitorBox2;        // ???? 2
	TInfoBox*    pTMonitorBox3;        // ???? 3
	TInfoBox*    pTMonitorBox4;        // ???? 4
	TInfoBox*    pTMonitorBox5;        // ???? 5
	TInfoBox*    pTMonitorBox6;        // ???? 6
	TInfoBox*    pTMonitorBox7;        // ???? 7
	TInfoBox*    pTMonitorBox8;        // ???? 8

	TBInfoBox*   pTStatusWindow;			// ????????? ??????

	TMonitoringWindow(TWindow* parent);	// Class constructor


	void ShowDoublesInMonitBox(int BoxNumber,
		char* StrText1 = "", double dValue1 = 0.0, int NumInGrp1 = 0,
		char* StrText2 = "", double dValue2 = 0.0, int NumInGrp2 = 0,
		char* StrText3 = "", double dValue3 = 0.0, int NumInGrp3 = 0,
		char* StrText4 = "", double dValue4 = 0.0, int NumInGrp4 = 0,
		char* StrText5 = "", double dValue5 = 0.0, int NumInGrp5 = 0,
		char* StrText6 = "", double dValue6 = 0.0, int NumInGrp6 = 0,
		char* StrText7 = "", double dValue7 = 0.0, int NumInGrp7 = 0,
		char* StrText8 = "", double dValue8 = 0.0, int NumInGrp8 = 0);
	void ShowIntegersInMonitBox(int BoxNumber,
		char* StrText1 = "", int iValue1 = 0, int NumInGrp1 = 0,
		char* StrText2 = "", int iValue2 = 0, int NumInGrp2 = 0,
		char* StrText3 = "", int iValue3 = 0, int NumInGrp3 = 0,
		char* StrText4 = "", int iValue4 = 0, int NumInGrp4 = 0,
		char* StrText5 = "", int iValue5 = 0, int NumInGrp5 = 0,
		char* StrText6 = "", int iValue6 = 0, int NumInGrp6 = 0,
		char* StrText7 = "", int iValue7 = 0, int NumInGrp7 = 0,
		char* StrText8 = "", int iValue8 = 0, int NumInGrp8 = 0);
	DECLARE_RESPONSE_TABLE(TMonitoringWindow);

protected:

	void ShowBoxData(char ShadeBoxNum, TBoxForDouble&  TBoxD);
	void ShowBoxData(char ShadeBoxNum, TBoxForInteger& TBoxI);

};

#endif  // __MONITWIN_H
/*
 * ---------------------------------------------------------
 *	EoF(monitwin.h)
 * ----------------------------------------------------------
 */
