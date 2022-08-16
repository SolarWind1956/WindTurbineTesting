/*
 * CommClnt - Root Module --------------------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * CommClnt - главный (корневой) модуль системы коммуникации с ПК
 * инициализирующий диалог коммуникационной подсистемы.
 *         Основные функции:
 *    -  инициализирует библиотеки BWCC;
 *    -  инициализирует флаг работы подсистемы;
 *	   -  инициализирует таймеры;
 *    -  устанавливает периоды срабатывания таймеров;
 *    -  определяет массивы переменных для записи данных после
 *       технологической обработки.
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>
#include <owl\menu.h>
#include <stdio.h>
#include <ddeml.h>
#include <string.h>
#include <time.h>
#include <sys\timeb.h>

#include "messagew.h"
#include "commclnt.rh"
#include "commglob.h"
#include "monitory.h"
/*
 *		Function's prototips
 */
int  InputON	(TWindow*);
int  InputCycle();
int  InputOFF	();
void StatusWindow(TBInfoBox*);
/*
 * 	Global variables
 */
TypeBFPC BuffTransfFrom;			//	Переменная для принятия данных из контроллера
TypeBTPC BuffTransfTo;				//	Переменная для передачи данных в контроллер

char szCommandToClient[N_COMMAND_DATA];// Управляющее сообщение для клиента от сервера
char szCommandToServer[N_COMMAND_DATA];// Управляющее сообщение для сервера от клиента

BOOL bInptDataON = FALSE;			// Флаг наличия связи с контроллером
BOOL bMonitoryON = FALSE;			// Флаг наличия мониторинга
/*
 *		Подсистема пользовательских таймеров
 */
TUserTimerSystem* lpTimersList;	// Список пользовательских таймеров
TUserTimer* pMPCTimer;				// Tаймер коммуникации с контроллером
TUserTimer*	pMonTimer; 				// Таймер для отображения данных в мониторинге
											// Заголовок приложения
/*
 * 	C o n s t a n t s
 */
char  mainTitle[40]  = "Связь с Info 73 - приложение 'Клиент'";

const AdviseTimeOut	= 2000;		// Тайм-аут транзакций циклов
											// приема команд
const ExecuteTimeOut	= 2000;		// Тайм-аут транзакций
											// передачи команд
const PokeTimeOut		= 2000;		// Тайм-аут транзакций
											// передачи данных
const RequestTimeOut = 2000;		// Тайм-аут транзакций
											// приема данных
class TDMLClientWindow;


/*
 *  -------------------------------------------------------------------------
 * 	Class TDMLClientApp
 *  -------------------------------------------------------------------------
 */
class TDMLClientApp  : public TApplication {
  public:
	 TDMLClientApp () : TApplication(),CallBackProc((FARPROC)CallBack) {
									InstId = 0;
								}
	 void InitMainWindow();
	 void InitInstance();
	 int  TermInstance(int status);
	 DWORD  InstId;               // Индетификатор экземпляра приложения
	 static HDDEDATA FAR PASCAL _export CallBack(WORD, WORD, HCONV, HSZ, HSZ,
																HDDEDATA, DWORD, DWORD);
	 TProcInstance CallBackProc;
};

/*
 *  -------------------------------------------------------------------------
 * 	Class TDMLClientWindow - главное окно приложения
 *  -------------------------------------------------------------------------
 */
class TDMLClientWindow : public TFrameWindow {
  public:
	 TSetTimePeriodDialog*   pTSetTimePeriodDialog;
	 TMonitoryWindow* 		 pTMonitoryWindow;

	 TDMLClientWindow(TWindow*, const char*);
	 virtual ~TDMLClientWindow();		// Destructor

	 virtual void SetupWindow();
	 void EvInitMenu    (HMENU);
	 void CmConnect     ();				// Установление связи с сервером
	 void CmInputOnOffPC();				// Установление или разрыв связи с ПК
	 void CmHelpAbout   ();
	 void CmTimePeriod  ();				// Изменение периода опроса ПК
	 void CmMonitory    ();				// Вкл/выкл мониторинга
	 void RequestData   ();				// Запрос на данные от сервера
	 void Poke	    	  ();				// Посылка данных серверу
	 void EvTimer	     (UINT);
	 void EvClose	     ();
	 void EndSession    ();
	 void ReceivedData   (HDDEDATA);	// Принятие данных от сервера
	 void ReceivedCommand(HDDEDATA);	// Принятие команды от сервера
	 DWORD   InstId() {
		return ((TDMLClientApp *)GetApplication())->InstId;
		}

	 HCONV   HConv;                  // Хэндл диалога с Сервером
	 HSZ     Service;						// Сервис
	 HSZ     Topic;						// Тема
	 HSZ     Item[4];                // Имена элементов

  DECLARE_RESPONSE_TABLE(TDMLClientWindow);
};
/*
 *  -------------------------------------------------------------------------
 * 	Реализация класса 'TDMLClientWindow'
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TDMLClientWindow, TFrameWindow)
  EV_WM_INITMENU,
  EV_WM_CLOSE,
  EV_WM_TIMER,
  EV_COMMAND(CM_CONNECT,    CmConnect     ),	// Связь с Сервером
  EV_COMMAND(CM_CYCLE,      CmTimePeriod  ),	// Изменение периода обмена данными
  EV_COMMAND(CM_MONITORY,   CmMonitory    ),	// Триггер мониторинга
  EV_COMMAND(CM_NET,        CmInputOnOffPC),	// Связь с контроллером
  EV_COMMAND(CM_HELPABOUT,  CmHelpAbout	), // О программе
END_RESPONSE_TABLE;

static TDMLClientWindow* This = 0;

/*
 * 	C o n s t r u c t o r
 */
TDMLClientWindow::TDMLClientWindow(TWindow* parent, const char* title)
  : TFrameWindow(parent, title),
	 TWindow(parent, title)
{                                   // Мониторинг
  pTMonitoryWindow = new TMonitoryWindow(this);
  HConv = 0;                        // Хэндл диалога
}

/*
 * 	D e s t r u c t o r
 */
TDMLClientWindow::~TDMLClientWindow()
{
  if (HConv)
	 DdeDisconnect(HConv);   			// Разрываем связь с Сервером

  if (InstId()) {                   // Освобождаем ресурсы занатые под DDEML
	 DdeFreeStringHandle(InstId(), Service);
	 DdeFreeStringHandle(InstId(), Topic);
	 for (char i = 0; i < 4; i++)
		if (Item[i])
			 DdeFreeStringHandle(InstId(), Item[i]);
	 }
}
/*
 *		Нагружаем SetupWindow() собственными функциями
 */
void
TDMLClientWindow::SetupWindow()
{
  HDDEDATA hData;			  				// Хэндл данных
  DWORD 	  dwStatusFlag;            // Флаги статуса транзакции
  This = this;								// Сохраним указатель на наше окно

  TFrameWindow::SetupWindow();
  AssignMenu(TDMLClientWindow_MENU);// Инициируем меню
												// Инициализация хэндлов строк
  Service = Topic = Item[0] = Item[1] = Item[2] = Item[3] = 0;

  Service = DdeCreateStringHandle(InstId(), Service1, CP_WINANSI);
  Topic   = DdeCreateStringHandle(InstId(), Topic1, 	CP_WINANSI);

  Item[0] = DdeCreateStringHandle(InstId(), Item1, 	CP_WINANSI);
  Item[1] = DdeCreateStringHandle(InstId(), Item2, 	CP_WINANSI);
  Item[2] = DdeCreateStringHandle(InstId(), Item3,	   CP_WINANSI);
  Item[3] = DdeCreateStringHandle(InstId(), Item4,    CP_WINANSI);

  if (!Service  ||	!Topic ||
		!Item [0] ||   !Item [1] || !Item [2] || !Item [3]) {
		MessageBox("Ошибка создания хендлеров строк иерархии.", Title, MB_ICONSTOP);
		PostQuitMessage(0);				// По появлении ошибки, завершим работу
  } else
		CmConnect();						// Установка диалога с сервером
												// Запрос на передачу команд управления
												// от Сервера Клиенту
												// в "горячем" режиме цикла уведомления -
												// "Advise"
		hData = DdeClientTransaction(0, 0, HConv,
											  Item[2],// тема "CommandToClient"
											  CF_TEXT,// Формат данных TEXT
														 // транзакция XTYP_ADVSTART с ожиданием подтверждения
														 // того, что Клиент закончил обрабатывать полученную
														 // команду
											  XTYP_ADVSTART | XTYPF_ACKREQ,
											  AdviseTimeOut,// СИНХРОННАЯ транзакция
														 // Флаги статуса транзакции
											  &dwStatusFlag);
		if (hData == FALSE){		// Сообщения по транзакции
			switch (LOWORD(dwStatusFlag)){
				case DDE_FNOTPROCESSED : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_ADVISE");
												 break;
				case DDE_FBUSY			  : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_ADVISE.\n"
															 "Сервер занят");
												 break;
			};
		} else {						// Освобождаем хэндл данных
			DdeFreeDataHandle(hData);
		};
		if (DdeGetLastError(InstId()) == DMLERR_ADVACKTIMEOUT)
											 MessageW("DDEML Client",
														 "Time-out транзакции XTYP_ADVISE истек.");
										// Создаем пустой список
										// пользовательских таймеров
		lpTimersList = new TUserTimerSystem(CLIENT_START_NUMBER_UT);
										// с номерами идентификаторов,
										// начиная с значения, определенного
										// в 'commglob.h'
										// Формирование списка пользовательских таймеров
										// Таймер коммуникации с контроллером
		pMPCTimer  = lpTimersList->AddUserTimer(this);
		pMPCTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
										// Таймер для отображения данных в мониторинге
		pMonTimer  = lpTimersList->AddUserTimer(this);
		pMonTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
}
/*
 * 	Обновление меню при помощи статуса переменных
 */
void
TDMLClientWindow::EvInitMenu(HMENU menuHandle)
{
  TMenu menu(menuHandle);
  menu.EnableMenuItem (CM_CONNECT,    !HConv   ? MF_ENABLED	 : MF_GRAYED);
  menu.CheckMenuItem (CM_NET,         MF_BYCOMMAND | bInptDataON ? MF_CHECKED :
																MF_UNCHECKED);
  menu.CheckMenuItem (CM_MONITORY,    MF_BYCOMMAND | (bMonitoryON && bInptDataON)
																? MF_CHECKED :	MF_UNCHECKED);
  DrawMenuBar();						// Обновим меню
}
/*
 *		Установление связи с Сервером
 */
void
TDMLClientWindow::CmConnect()
{
	if (!HConv)
		HConv = DdeConnect(InstId(),Service, Topic, 0);
	if (HConv){                    // Обновление меню
		PostMessage(WM_INITMENU, WPARAM(GetMenu()));
	} else {
		MessageW("Связь с сервером", "Внимание! Сервер не обнаружен.");
	};

};
/*
 *		Изменение периода опроса и мониторинга
 */
void
TDMLClientWindow::CmTimePeriod(){
											// В диалоге меняем параметры цикла
  pMPCTimer->ChangeUserTimerPeriod();
  if (bInptDataON){         		// Перезапуск подсистемы опроса
		pMPCTimer->KillUserTimer();// Убиваем таймер со старыми параметрами
		pMPCTimer->SetUserTimer(This->HWindow);   // и запускаем его уже с измененными
		InputCycle();					// Цикл опроса
  };
};
/*
 * 	Функция включения и выключения системы ввода данных и разрыв связи
 *		с контроллером. Запуск таймера для коммуникации.
 */
void
TDMLClientWindow::CmInputOnOffPC(){
	HDDEDATA hData;			  		// Хэндл данных
	DWORD 	dwStatusFlag;        // Флаги статуса транзакции
	if (!bInptDataON){        	   // Если система еще не включена -
		 if (InputON(This)) {		// Инициализация связи с контроллером
			  if (InputCycle()){ 	// Принятие блока данных
											//	Устанавливаем таймер цикла
					pMPCTimer->SetUserTimer(This->HWindow);
											// Взведем флаг наличия связи с
											// контроллером
					bInptDataON = TRUE;
											// команда - сообщение "Контроллер включен"
					strcpy(szCommandToServer, PCNETOK);
			  } else {          	   // Eсли произошла ошибка при принятии данных
					InputOFF();			// Выключаем систему принятия данных
											// команда - сообщение
											// "Контроллер аварийно выключен"
					strcpy(szCommandToServer, PCNETCRASH);
			  };
		 } else {						// Eсли произошла ошибка при инициализации связи
			  InputOFF(); 				// Выключаем систему принятия данных
											// команда - сообщение
											// "Контроллер аварийно выключен"
			  strcpy(szCommandToServer, PCNETCRASH);
		 };
	} else {						      // При выключении, если система ввода
											// данных(опроса) включена,
		 if (bMonitoryON){			// Если мониторинг включен
			 CmMonitory();				// выключим его
		 };
		 bInptDataON  = FALSE;		// сбросим флаг наличия связи с контроллером
		 InputOFF(); 					// Выключаем систему принятия данных
											// Убъем таймер передачи и приема данных
		 pMPCTimer->KillUserTimer();
											// команда - сообщение "Контроллер выключен"
		 strcpy(szCommandToServer, PCNETFAIL);
	};										// Передача команды - сообщения Серверу о
											// включении или выключении контроллера
											// СИНХРОННОЙ транзакцией XTYP_EXECUTE
	hData = DdeClientTransaction((unsigned char*)szCommandToServer,
									 sizeof(szCommandToServer),// Длина буфера данных
									 HConv,						 // Хэндл диалога с сервером
									 Item[3],					 // Имя элемента данных "CommandToClient"
									 CF_TEXT,					 // Формат данных TEXT для
									 XTYP_EXECUTE,           // команды - сообщения
									 ExecuteTimeOut,         // Время time-out
									 &dwStatusFlag);         // Флаги статуса транзакции
	if (hData == FALSE){
		switch (LOWORD(dwStatusFlag)){
				case DDE_FNOTPROCESSED : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_EXECUTE");
												 break;
				case DDE_FBUSY			  : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_EXECUTE.\n"
															 "Сервер занят");
												 break;
			};
	} else {							// Освобождаем хэндл данных
		DdeFreeDataHandle(hData);
	};
	if (DdeGetLastError(InstId()) == DMLERR_EXECACKTIMEOUT)
										 MessageW("DDEML Client",
													 "Time-out транзакции XTYP_EXECUTE истек.");
											// Обновляем меню
	This->PostMessage(WM_INITMENU, WPARAM(This->GetMenu()));
};
/*
 * 	Тригер включения и выключения мониторинга
 */
void
TDMLClientWindow::CmMonitory(){
  if (bInptDataON){                          // Если связь с ПК установлена
	  if (!bMonitoryON){
			pTMonitoryWindow->Create();  			// Создаем диалог
			pTMonitoryWindow->MonitCycleONOFF();//	Устанавливаем таймер цикла мониторинга
	  } else {
			bMonitoryON  = FALSE;			  		// Сбросим флаг работы мониторинга
			pTMonitoryWindow->EvClose();			// Закроем окно мониторинга
	  };
  };
};
/*
 *		Запуск циклических программ.
 */
void
TDMLClientWindow::EvTimer(UINT IDEvent){
	if (IDEvent == pMPCTimer->IDEvTimer){// Если сработал таймер коммуникации, сравним его

		if (pMPCTimer->TestUserTimer()){ // При отработке заданного количества таймеров
			if (!InputCycle()){				// проверим, не было ли сбоя при приеме данных.
				CmInputOnOffPC();          // В случае ошибки - выключаем систему ввода данных
			} else {                      // Если ошибки принятия данных не было,
				Poke();							// Передача данных серверу
				RequestData();					// Запрос на данные от сервера
			}; // if {...} else {...}
		}; 	// if {...}
	}; 		// if {...}
};
/*
 * 			Передача данных серверу в режиме 'Poke' всех информационных массивов с данными,
 *				полученными из контроллера, запускается в цикле срабатывания
 *				таймера опроса. Транзакция СИНХРОННАЯ, с контролем по time-out
 *
 */
void
TDMLClientWindow::Poke()
{
	DWORD 	dwStatusFlag;        // Флаги статуса транзакции
	HDDEDATA hData;			  		// Хэндл данных

	hData = DdeClientTransaction((unsigned char*)BuffTransfFrom.aBuffTransf,
								sizeof(TypeFromPC),		// Длина буфера данных
								HConv,						// Хэндл диалога с сервером
								Item[0],						// Имя элемента данных "DataFromInfo73"
								CF_DIF,						// Формат данных DIF для
								XTYP_POKE,              // данных
								PokeTimeOut,            // Время time-out
								&dwStatusFlag);
	if (hData == FALSE){ 	      // Флаги статуса транзакции
			switch (LOWORD(dwStatusFlag)){
				case DDE_FNOTPROCESSED : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_POKE");
												 break;
				case DDE_FBUSY			  : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_POKE.\n"
															 "Сервер занят");
												 break;
			};
	} else {                   // Освобождаем хэндл данных
		DdeFreeDataHandle(hData);
	};
	if (DdeGetLastError(InstId()) == DMLERR_POKEACKTIMEOUT)
										 MessageW("DDEML Client",
													 "Time-out транзакции XTYP_POKE истек.");
	return;
}
/*
 * 			Запрос на получение и получение массивов управляющей информации из 'Сервера'
 *				для пересылки в Info 73. Запускается в цикле срабатывания таймеров опроса
 *				Транзакция СИНХРОННАЯ, с контролем по time-out
 */
void
TDMLClientWindow::RequestData()
{
	DWORD    dwStatusFlag;                    // Флаги статуса транзакции
	HDDEDATA hData;									// Хэндл данных
	hData = DdeClientTransaction(0,
										  0,
										  HConv,          // Хендл диалога
										  Item[1],			// Имя Элемента данных "DataToInfo73"
										  CF_DIF,			// Формат даных
										  XTYP_REQUEST,   // Тип транзакции
										  RequestTimeOut,	// Время time-out'а для СИНХРОННОЙ
																// транзакции
										  &dwStatusFlag); // Флаги статуса транзакции
	ReceivedData(hData);								// Прием запрошенных данных
															// от Сервера
	if (hData == FALSE){
			switch (LOWORD(dwStatusFlag)){
				case DDE_FNOTPROCESSED : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_REQUEST");
												 break;
				case DDE_FBUSY			  : MessageW("DDEML Client",
															 "Ошибка транзакции XTYP_REQUEST.\n"
															 "Сервер занят");
												 break;
			};
	} else {                   // Освобождаем хэндл данных
		DdeFreeDataHandle(hData);
	};
	return;
}
/*
 * 	О программе
 */
void
TDMLClientWindow::CmHelpAbout()
{
  MessageBox("Клиентное приложение на основе DDEML\n для связи с контроллером.",
				 "О программе", MB_ICONINFORMATION);
}
/*
 * 	Прием команды от Сервера. Анализ и выполнение команды.
 *		Эта функция вызывается из CALLBACK'а по приходу транзакции XTYP_ADVDATA.
 */
void
TDMLClientWindow::ReceivedCommand(HDDEDATA hData)
{
  if (hData) {
	 DdeGetData(hData, &szCommandToClient, sizeof (szCommandToClient), 0);
	 if ((strcmp(szCommandToClient, ONNETPC) == 0) && !bInptDataON)
				CmInputOnOffPC();
	 if (strcmp(szCommandToClient, OFFNETPC) == 0)
				CmInputOnOffPC();
	 Invalidate(TRUE);
  };
};
/*
 * 	Прием данных от Сервера.
 *		Эта функция вызывается из RequestData() по приходу запрашиваемых
 *		данных.
 */
void
TDMLClientWindow::ReceivedData(HDDEDATA hData)
{
  if (hData) {
		DdeGetData(hData, (unsigned char*)BuffTransfTo.aBuffTransf,
																		sizeof(TypeToPC), 0);
		Invalidate(TRUE);
  };
}
/*
 * 	Ловим сообщение о завершении приложения для анализа на разрешение завершения
 */
void
TDMLClientWindow::EvClose(){
	if (bInptDataON){					// При включеном опросе контроллера
		 MessageBox("Выключите подсистему опроса контроллера",
						"Связь с контроллером",
						MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	} else
	if (HConv){                  	// При наличии связи с Сервером
		 if (MessageBox("Внимание! Установлена связь с Сервером\n"
						"Вы действительно хотите разорвать связь?",
						"DDEML",
						MB_YESNO | MB_ICONSTOP | MB_TASKMODAL) == IDYES){
			DdeClientTransaction(0,      // Останов цикла уведомления по передачи команд управления
										0,      // из Сервера Клиенту в "горячем" режиме цикла уведомления -
										HConv,  // "Advise"
										Item[2],// Тема "CommandToClient"
										CF_TEXT,// Формат данных TEXT
										XTYP_ADVSTOP,
										1000,   // СИНХРОННАЯ транзакция
										0);
			DdeDisconnect(HConv);	// Разрыв связи с Сервером
			EndSession();      	   // При положительном ответе
		 };
	} else
			 EndSession();				// При отсутствии связи с Сервером
};
/*
 *		 Завершение работы приложения
 */
void
TDMLClientWindow::EndSession(){
  lpTimersList->Flush();      	// Удаляем сеть пользовательских таймеров
  delete lpTimersList;
  CloseWindow(IDCANCEL);   	   // Завершение приложения
};
/*
 *  -------------------------------------------------------------------------
 * 	Реализация класса 'TDMLClientApp'
 *  -------------------------------------------------------------------------
 */
/*
 * 	CALLBACK Клиента, настроенный на обработку синхронных и асинхронных
 *		транзакций ....
 *
 */
HDDEDATA FAR PASCAL _export
TDMLClientApp::CallBack(WORD type,
								WORD /*wFmt*/,
								HCONV hConv,
								HSZ,
								HSZ,
								HDDEDATA hData,
								DWORD dwData1,
								DWORD /*dwData2*/)
{
  switch (type) {
	 case XTYP_ADVDATA:					// Сервер передал команду в горячем
		if (hConv == This->HConv)     // синхронном цикле
			 This->ReceivedCommand(hData);
		return (HDDEDATA)DDE_FACK;

	 case XTYP_XACT_COMPLETE:
		break;

	 case XTYP_DISCONNECT:				// Разрав связи и завершение работы приложения
		This->PostMessage(WM_INITMENU, WPARAM(This->GetMenu()));
		This->HConv = 0;
		if (bInptDataON)					// Разрыв связи с ПК при ее наличии
			This->CmInputOnOffPC();
		This->EvClose();					// Завершение работы приложения
		break;

	 case XTYP_ERROR:
		char StMessage[100];
		if (LOWORD(dwData1) == DMLERR_LOW_MEMORY)
			sprintf(StMessage, "Обнаружена критическая ошибка DDE !\n"
									 "Недостаточное количество памати\n"
									 "Диалог %d", (unsigned long)hConv);
		else
			sprintf(StMessage, "Обнаружена критическая ошибка DDE\n"
									 "Диалог %d", (unsigned long)hConv);
		This->MessageBox(StMessage, "DDEML Client", MB_ICONSTOP | MB_TASKMODAL);
  }
  return 0;
}
/*
 * 	Инициализация главного окна приложения
 */
void
TDMLClientApp ::InitMainWindow()
{
  MainWindow = new TDMLClientWindow(0, mainTitle);
									// Икона приложения
  MainWindow->SetIcon(this, "Commclnt");
									// Атрибуты окна:
									// стиль (без кнопки максимизации);
  MainWindow->Attr.Style &= ~(WS_MAXIMIZEBOX);
									// положение (0, 0);
  MainWindow->Attr.X = 0;   MainWindow->Attr.Y = 0;
									// размер (500 х 150)
  MainWindow->Attr.W = 500; MainWindow->Attr.H = 150;
}

/*
 *			Инициализация приложения
 */
void
TDMLClientApp ::InitInstance()
{
  //
  // Инициализация DDEML - регистрируем CALLBACK функцию сервера
  // 							 -	в ячейку InstId занесется идентификатор
  //								экземпляра приложения
  //							 -	флаги филтров транзакции пока не установлены
  // 							 - установлен флаг массива команд 'ТОЛЬКО КЛИЕНТ'
  if (DdeInitialize(&InstId,
						  (PFNCALLBACK)(FARPROC)CallBackProc,
						  APPCMD_CLIENTONLY,
						  0) != DMLERR_NO_ERROR) {
		::MessageBox(0,"Инициализация DDE не прошла.", "DDEML 'Клиент'",
																	MB_ICONSTOP|MB_TASKMODAL);
		PostQuitMessage(0);           // Завершение работы
  }

  // Must come after we've initialized DDE since InitInstance will trigger
  // SetupWindow
  TApplication::InitInstance();
}

/*
 *			Деинициализация приложения и DDEML
 */
int
TDMLClientApp ::TermInstance(int status)
{
  if (InstId) {
		DdeUninitialize(InstId);
  }
  return TApplication::TermInstance(status);
}
/*
 * 		OWL - приложение
 */
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  int RetCode;								// Код завершения приложения
  TDMLClientApp* pTDMLClientApp = new TDMLClientApp();
												// Инициализация библиотек
  BWCCGetVersion();						// BWCC - support
  RetCode = pTDMLClientApp->Run();	// Запускаем приложение
  return RetCode;
}
/*
 * ---------------------------------------------------------
 *	EoF(commclnt.cpp)
 * ----------------------------------------------------------
 */

