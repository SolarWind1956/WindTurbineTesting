#if !defined(__TESTSYST_H)
#define __TESTSYST_H
/*
 * testsyst.h - Handle file of Root Module ---------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <stdio.h>
#include <ddeml.h>

#include "maindial.h"
#include "commglob.h"
//----------------------------------------------------------------------------
BOOL Delay(TWindow*, WORD);
/*
 *  -------------------------------------------------------------------------
 * 	Class TDMLServerApp
 *  -------------------------------------------------------------------------
 */
class TDMLServerApp : public TApplication {
  public:
	 TDMLServerApp();
	 void 	InitMainWindow();			   // Инициализация главного окна
	 void 	InitInstance();            // Инициализация экземпляра приложения
	 int  	TermInstance(int status);  // Завершение работы экземпляра приложения
	 DWORD   InstId;							// Идентификатор экземпляра приложения
													// CALLBACK "Сервера"
	 static HDDEDATA FAR PASCAL _export CallBack(WORD, WORD, HCONV, HSZ, HSZ,
																HDDEDATA, DWORD, DWORD);
	 TProcInstance CallBackProc;			// Класс
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TDMLServerWindow - главное окно приложения
 *  -------------------------------------------------------------------------
 */
class TDMLServerWindow : public TFrameWindow {
  public:
	 TMainDialog* MainDialog;
	 BOOL  	MainDialogExecute;			// Execute flags

	 TDMLServerWindow(TWindow*, const char*);
	 ~TDMLServerWindow();

	 void 	SetupWindow ();
	 void 	Paint(TDC&, BOOL erase, TRect&);

	 virtual BOOL 		MatchTopicAndService(HSZ, HSZ);
	 virtual BOOL 		MatchTopicAndItem(HSZ, HSZ);
	 virtual HDDEDATA DataRequested();
	 virtual HDDEDATA CommandRequested();
	 virtual void 		UpdateData();
												// функция получения идентификатора
	 DWORD   InstId() {              // экземпляра приложения
					return ((TDMLServerApp*)GetApplication())->InstId;
				}

	 HCONV   HConv;   					// Хендлер Диалога с "Клиентом"
	 BOOL    Loop;
	 HSZ     Service;
	 HSZ     Topic;						// Название темы
	 HSZ     Item [4];	  				// Имя Элемента Данных для передачи "Клиенту"
  DECLARE_CASTABLE;
};
extern TDMLServerWindow* This;		// Указатель еще всем будет нужен
/*
 * ---------------------------------------------------------
 *	EoF(testsyst.h)
 * ----------------------------------------------------------
 */
#endif  // __TESTSYST_H
