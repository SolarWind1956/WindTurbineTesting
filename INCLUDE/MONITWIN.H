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
const char PANEL_NUM 		= 8;				// Количество панелей в боксе
const char NAME_STR_LENTH 	= 25; 			// Длина текста наименования панели в символах
const char VAL_STR_LENTH	= 50;				// Количество символов числа, включая десятичную точку
const char NVG_STR_LENTH	= 1 + 2 + 1;	// Количество символов строки номера числа в группе,
														// с учетов левой и правой круглых скобок

const char displUp 			= 2;				// Смещение вывода строки от верхней части панели
const char displPANEL  		= 23;				// Смещение межды двумя панелями
const char displSTR    		= 2;  			// Смещение слева для вывода строки
const char displVAL			= 125;			// Смещение слева для вывода числа

const char displVAL_NUM1	= 107;			// для однозначного номера
const char displVAL_NUM2   = 102;         // для двузначного номера


struct TBoxForInteger{
		 BOOL    		isEmpty[PANEL_NUM]; 								// Признаки отсутствия чисел
		 char  			StrText[PANEL_NUM][NAME_STR_LENTH];			// Надписи на панелях
		 int 				aiValue[PANEL_NUM];        					// Числа
		 int				NumberInGroup[PANEL_NUM];						// Номера чисел в группе
};
struct TBoxForDouble {
		 BOOL    		isEmpty[PANEL_NUM]; 								// Признаки отсутствия чисел
		 char  			StrText[PANEL_NUM][NAME_STR_LENTH];		 	// Надписи на панелях
		 double  		adValue[PANEL_NUM];        					// Числа
		 int				NumberInGroup[PANEL_NUM];						// Номера чисел в группе
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
		char  displVAL_NUM;              // Смещения для вывода номера числа в группе
		BOOL 	FlagInfoToShadeBox;			// Флаг разрешения вывода комплексной
													// информации в окно
		int   fnObject;
		void  (*pOutFunction)(TInfoBox*);
													// Характеристики выводимого элемента

		BOOL  isEmpty[PANEL_NUM];							// Признаки отсутствия чисел
		char  StrText[PANEL_NUM][NAME_STR_LENTH];		// Надписи на панелях
		char  StrValue[PANEL_NUM][VAL_STR_LENTH];   	// Строки значений чисел
		int	NumberInGroup[PANEL_NUM];					// Строки номеров чисел в группе (в скобках)

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

// 	С о б с т в е н н о     к л а с с
class TMonitoringWindow : public TDialog {
  public:
												  // Характеристики выводимого элемента
												  // Окна мониторинга
	TInfoBox*    pTMonitorBox1;        // Окно 1
	TInfoBox*    pTMonitorBox2;        // Окно 2
	TInfoBox*    pTMonitorBox3;        // Окно 3
	TInfoBox*    pTMonitorBox4;        // Окно 4
	TInfoBox*    pTMonitorBox5;        // Окно 5
	TInfoBox*    pTMonitorBox6;        // Окно 6
	TInfoBox*    pTMonitorBox7;        // Окно 7
	TInfoBox*    pTMonitorBox8;        // Окно 8

	TBInfoBox*   pTStatusWindow;			// Статусная строка

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
