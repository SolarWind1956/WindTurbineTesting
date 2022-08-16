/*
 * Testsyst - Root Module --------------------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * Testsyst -  главный (корневой) модуль системы испытаний, диспетчер
 * инициализирующий диалоги основных подсистем.
 *         Основные функции:
 *    -  инициализирует библиотеки BWCC, Paradox Engine 3.0;
 *    -  инициализирует флаги работы подсистем;
 *	   -  инициализирует таймеры;
 *    -  устанавливает периоды срабатывания таймеров;
 *    -  определяет массивы переменных для записи данных после
 *       технологической обработки.
 */
#include "testsyst.h"
#include "testsyst.rh"
#include "database.h"
/*
 *-------------------------------------------------------------
 *		Прототипы функций
 *-------------------------------------------------------------
 */
void  	TechnolProcess();	 	 // Предварительная технологическая обработка
/*
 * 	Инициализация глобальных переменных по "Верхнему" уровню
 */
BOOL 		bInptDataON   = FALSE;// Флаг работы коммуникации
BOOL 		bMonitoryON   = FALSE;// Флаг работы мониторинга
BOOL 		bInstantValInptON  = FALSE;// Флаг записи протоколов метеоданных
BOOL 		bCycleFInptON = FALSE;// Флаг записи протоколов циклических данных
BOOL 		bViewDataON	  = FALSE;// Флаг работы подсистемы отображения данных
BOOL 		bTestDataON	  = FALSE;// Флаг работы испытания

char 		szCommandToClient[N_COMMAND_DATA];
char 		szCommandToServer[N_COMMAND_DATA];

TypeBFPC BuffTransfFrom;
TypeBTPC BuffTransfTo;

int CodeController;			 		// Текущий работающий контроллер

int aiControlData1	 [N_CONTROL_DATA]; 	// Первый массив управляющей информации
int aiControlData2	 [N_CONTROL_DATA]; 	// Второй массив управляющей информации
/*
 *		Подсистема пользовательских таймеров
 */
TUserTimerSystem* lpTimersList;	// Список пользовательских таймеров
TUserTimer*	pMPCTimer;				//	Tаймер для опроса контроллера
TUserTimer*	pMonTimer; 				// Таймер для отображения данных в мониторинге
TUserTimer*	pCFTimer;      		// Таймер для доступа к циклическому файлу
TUserTimer*	pInstTimer;				// Таймер для записи мгновенных значений
TUserTimer*	pTimeTimer;				// Таймер для отображения текущего времени
TUserTimer*	pViewTimer;				// Таймер для отображения графиков
TUserTimer*	pTPWTimer;				// Таймер для записи протокола испытания
											// Заголовок
const char mainTitle[] = "Автоматизированная система испытаний";
//----------------------------------------------------------------------------

IMPLEMENT_CASTABLE1(TDMLServerWindow, TFrameWindow);

TDMLServerWindow* This = 0;
/*
 *  -------------------------------------------------------------------------
 * 	Реализация класса 'TDMLServerWindow'
 *  -------------------------------------------------------------------------
 */
/*
 *		C o n s t r u c t o r
 */
TDMLServerWindow::TDMLServerWindow(TWindow* parent, const char* title)
  : TFrameWindow(parent, title)
{
  HConv = 0;
  Loop = FALSE;							// Флаг наличия "горячего" цикла для послания команды Клиенту
  bInptDataON  = FALSE;
  InitPXEngine("TestSyst");			// Создаем механизм Paradox Engine
  CreateTestSystTables();				// Создаем таблицы файлов базы данных
  CreateOpenCycleFile();				// Создаем стержневой файл - циклический
												// причем без него работать нельзя
												// Создаем пустой список
												// пользовательских таймеров
  lpTimersList = new TUserTimerSystem(SERVER_START_NUMBER_UT);
												// с номерами идентификаторов,
												// начиная с значения, определенного
												// в 'commglob.h'
}
TDMLServerWindow::~TDMLServerWindow(){
  /*
	* This clean up is required for those resources that were allocated during
	* the DDE conversation.
	*/
  if (HConv)
		DdeDisconnect(HConv);   // Let the other party know we are leaving

  if (InstId()) {
		DdeNameService(InstId(), Service, 0, DNS_UNREGISTER);	// Отменяем регистрацию сервера
		if (Service){
			 DdeFreeStringHandle(InstId(), Service);
		};
		if (Topic){
				 DdeFreeStringHandle(InstId(), Topic);
		};
		for (char i = 0; i < 4; i++){
			if (Item[i]){
				 DdeFreeStringHandle(InstId(), Item[i]);
			};
		};
  };
  delete lpTimersList;					// удаляем систему пользовательских
												// таймеров
  CloseCycleFile();						// Закрываем циклический файл
  DeleteTestSystTables();				// И уничтожаем их таблицы
												// А также всю кухню
  OffPXEngine();							// ParadoX Engine & DB FW
};

void
TDMLServerWindow::SetupWindow()
{
  HINSTANCE hInstClientApp;
  This = this;
  MainDialogExecute	= FALSE;		// Execute flags

  TFrameWindow::SetupWindow();

  Service = Topic = Item[0] = Item[1] = Item[2] = Item[3] = 0;
  /*
	* The strings below are the 'Service', 'Topic' and 'Item' identifiers
	* that this application makes available, through DDE, to other
	* applications.
	*/
  Service = DdeCreateStringHandle(InstId(), Service1, CP_WINANSI);
  Topic   = DdeCreateStringHandle(InstId(), Topic1, 	CP_WINANSI);

  Item[0] = DdeCreateStringHandle(InstId(), Item1, 	CP_WINANSI);
  Item[1] = DdeCreateStringHandle(InstId(), Item2, 	CP_WINANSI);
  Item[2] = DdeCreateStringHandle(InstId(), Item3,		CP_WINANSI);
  Item[3] = DdeCreateStringHandle(InstId(), Item4,		CP_WINANSI);

  if (Service && Topic && Item[0] && Item[1] && Item[2] && Item[3]) {
	  if (DdeNameService(InstId(), Service, 0, DNS_REGISTER) == 0){
		  MessageBox("Registration failed.", Title, MB_ICONSTOP);
		  PostQuitMessage(0);
	  } else {
		  hInstClientApp = ShellExecute(
							HWindow,  	   	// Handle our windows
							NULL,		 	 		// Default open programm
							"CommClnt.exe",	// Name programm
							NULL,			  		// Parametrs
							NULL,				  	// Default directory
							SW_SHOWMINIMIZED);// Status window programm
													// File not found
		  if (hInstClientApp == (HINSTANCE)ERR_FILE_NOT_FOUND){
			  if (MessageBox("Клиентное приложение не найдено!\n"
								  "Продолжить без него?",
								  Title, MB_YESNO) == IDNO){
				  PostQuitMessage(WM_QUIT);
			  };
		  };
	  };
  } else {
		 MessageBox("String creation failed.", Title, MB_ICONSTOP);
		 PostQuitMessage(WM_QUIT);
  };
};
/*
 * This function is used to compare incoming Topic and Service requests.
 * This example DDE Server only makes one Service and one Topic available
 * so the logic is simple for this case but could be more complex for
 * Servers that offer multiple Services or Topics.
 */
BOOL
TDMLServerWindow::MatchTopicAndService(HSZ topic, HSZ service)
{
  if (DdeCmpStringHandles(Topic, topic) == 0)
		if (DdeCmpStringHandles(Service, service) == 0)
			 return TRUE;
  return FALSE;
}
/*
 * This function is used to compare incoming Topic and Item pair requests.
 * This Server only makes one Topic with one Item available so the logic is
 * simple for this case but could be more complex if the Server offered
 * multiple Items for multiple Topics.
 */
BOOL
TDMLServerWindow::MatchTopicAndItem(HSZ hsz1, HSZ hsz2)
{
  if (DdeCmpStringHandles(Topic, hsz1) == 0)
	  for (char i = 0; i < 4; i++)
		  if (DdeCmpStringHandles(Item[i], hsz2) == 0)
				return TRUE;
  return FALSE;
};
/*
 *		Функция создания handle'ов данных для передачи "Клиенту" инициировавшему
 *		запрос на данные транзакции XTYP_REQUEST.
 */
HDDEDATA
TDMLServerWindow::DataRequested()
{
  HDDEDATA RetCode;

  RetCode = DdeCreateDataHandle( // создаем handle данных для формата данных типа DIF,
								InstId(),// запрашиваемых "Клиентом" транзакцией XTYP_REQUEST
											// Указатель на массив данных
								(unsigned char *)BuffTransfTo.aBuffTransf,
								sizeof(TypeToPC),
								0,       // Смещение - игнорируем
								Item[1], // Имя Элемента данных
								CF_DIF,  // Формат даных
								0);      // Флаги транзакции - игнорируем
											// Сразу по получении данных
  TechnolProcess();					// Предварительная технологическая обработка
  return RetCode;
}
/*
 *		Функция создания handle'ов данных для передачи "Клиенту" команды
 *		в ГОРЯЧЕМ асинхронном цикле цикле
 */
HDDEDATA
TDMLServerWindow::CommandRequested()
{
  HDDEDATA RetCode;					// создаем handle данных для формата данных типа TEXT,
											// ожидаемых "Клиентом" в "горячем" цикле уведомления
  RetCode = DdeCreateDataHandle(InstId(),
								&szCommandToClient,
								sizeof(szCommandToClient),
								0,
								Item[2],
								CF_TEXT,
								0);
  return RetCode;
}

/*
 * This is triggered by the IdleAction() loop above whenever the user
 * enters an advise loop.
 */
void
TDMLServerWindow::UpdateData()
{
	if(!DdePostAdvise(InstId(), Topic, Item[2])){
		MessageBox("Передача Клиенту команды не прошла.", "DDEML Сервер", MB_ICONINFORMATION);
		PostQuitMessage(0);
	};
}
void
TDMLServerWindow::Paint(TDC&, BOOL, TRect&)
{
  if (!MainDialogExecute) {
	  Delay(this, 1000);             // Delay  ~ 1 second
	  MainDialogExecute = TRUE; 		// Взводим флаг наличия диалога
	  // Формирование списка пользовательских таймеров
											// Таймер для отображения данных в мониторинге
	  pMonTimer  = lpTimersList->AddUserTimer(this);
	  pMonTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
											//	Tаймер для опроса контроллера
	  pMPCTimer  = lpTimersList->AddUserTimer(this);
	  pMPCTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
											// Таймер для доступа к циклическому файлу
	  pCFTimer   = lpTimersList->AddUserTimer(this);
	  pCFTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
											// Таймер для отображения текущего времени
	  pTimeTimer = lpTimersList->AddUserTimer(this);
											// Таймер для отображения графиков
	  pViewTimer = lpTimersList->AddUserTimer(this);
	  pViewTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
											// Таймер для записи протокола мгновенных значений
	  pInstTimer  = lpTimersList->AddUserTimer(this);
	  pInstTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
/*
 *		---------------------         Такая конструкция повидимому необходима
 *												так как иначе не удается создать механизм
 *												создания и уничтожения класса главного
 *												диалога
 */
	  TMainDialog TMainDialogClass = TMainDialog(this);
	  MainDialog = TMainDialogClass.getPointer();
	  MainDialog->Execute();   		// Толкаем главный диалог - Also los !
//		---------------------
												// При выходе из главного диалога -
	  lpTimersList->Flush();         // Удаляем сеть пользовательских таймеров
	  SendMessage(WM_CLOSE, 0, 0);   // Закрываем главное окнс
  };
};
/*		-------------------------------------------------------------------------
 * 				Реализация класса 'TDLMServerApp'
 *		-------------------------------------------------------------------------
 */
// ----------- C o n s t r u c t o r ----------------------
TDMLServerApp::TDMLServerApp() :
	TApplication(), CallBackProc((FARPROC)CallBack) {

	  nCmdShow = SW_SHOWNORMAL;			// Normal window
	  InstId=0;									// Сбрасываем перед регистрацией идентификатор
													// экземпляра приложения
													// Создаем главное окно СЕРВЕРА
	  MainWindow = new TDMLServerWindow(0, mainTitle);
};
/*
 *		CALLBACK Сервера
 *
 *
 *
 */
HDDEDATA FAR PASCAL
TDMLServerApp::CallBack(WORD wType,
								WORD /*wFmt*/,
								HCONV hConv,
								HSZ hsz1,
								HSZ hsz2,
								HDDEDATA hData,
								DWORD /*dwData1*/,
								DWORD /*dwData2*/)
{
  switch (wType) {
	 case XTYP_ADVREQ:				// Эту транзакцию Сервер передает в этот
											// самый СВОЙ CALLBACK сам - при помощи
											// DdePostAdvice().
		if (This->MatchTopicAndItem(hsz1, hsz2))
											// Здесь возвращается
			 return This->CommandRequested();
		return 0;


	 case XTYP_ADVSTART:
		if (!This->Loop && This->MatchTopicAndItem(hsz1, hsz2)) {
			 This->Loop = TRUE;
			 return (HDDEDATA)1;
		}
		return 0;

	 case XTYP_ADVSTOP:
		if (This->Loop && This->MatchTopicAndItem(hsz1, hsz2));
			 This->Loop = FALSE;
		break;

	 case XTYP_CONNECT:
		if (!This->HConv)
			 if (This->MatchTopicAndService(hsz1, hsz2))
				  return (HDDEDATA)1;
		return 0;

	 case XTYP_CONNECT_CONFIRM:
		This->HConv = hConv;				// Получаем хэндл диалога
		break;
	 case XTYP_DISCONNECT:				// "Клиент" выполнил процедуру DdeDisconect();
		if (hConv == This->HConv) {
			 This->MessageBox("Приложение - 'Клиент' прервало связь DDE.", This->Title, MB_ICONINFORMATION);
			 This->HConv = 0;
			 This->Loop  = FALSE;
			 bInptDataON = FALSE;      // Сбросим флаг приняти данных от "Клиента" -
		}										// т.е от контроллера
		break;
	 case XTYP_ERROR:
		This->MessageBox("Произошла критическая ошибка DDEML.", This->Title, MB_ICONINFORMATION);
		break;
	 case XTYP_EXECUTE:{					// Прием команды - сообщения от "Клиента",
												// передача которых инициирована
												// XTYP_EXECUTE транзакцией
			DdeGetData(hData, (unsigned char *)&szCommandToServer,
						  sizeof(szCommandToServer), 4);
			if (strcmp(szCommandToServer, PCNETOK) == 0){
				// тут ничего не делаем. O'k так O'k
			} else if (strcmp(szCommandToServer, PCNETFAIL) == 0){
				bInptDataON = FALSE;    // Выключим систему сбора информации
				if (This->MainDialogExecute)
					 This->MainDialog->pTDInptWindow->InfoInptOFF();
			} else if (strcmp(szCommandToServer, PCNETCRASH) == 0){
				bInptDataON = FALSE; 	// Выключим систему сбора информации
				if (This->MainDialogExecute)
					 This->MainDialog->pTDInptWindow->InfoInptOFF();
			};
			return (HDDEDATA)DDE_FACK;	// Сообщаем системе DDE о завершении
												// обработки транзакции
	  }
	 case XTYP_POKE: {					// Прием данных от "Клиента",
												// передача которых инициирована
												// XTYP_POKE транзакцией
			DdeGetData(hData, (unsigned char *)&BuffTransfFrom.aBuffTransf,
						  sizeof(TypeFromPC), 0);
			return (HDDEDATA)DDE_FACK;	// Сообщаем системе DDE о завершении
												// обработки транзакции передачи данных
	  }
	 case XTYP_REQUEST:
		if (This->MatchTopicAndItem(hsz1, hsz2))
		  return This->DataRequested();
		return 0;
  }
  return 0;
}
// Инициализация главного окна
void
TDMLServerApp::InitMainWindow()
{
  MainWindow->SetIcon(this, "TestSyst");
  MainWindow->Attr.Style &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX |
										 WS_SYSMENU);

};
/*
 * 			Инициализация экземпляра приложения
 */
void
TDMLServerApp::InitInstance()
{
  // Инициализация DDEML - регистрируем CALLBACK функцию сервера
  // 							 -	в ячейку InstId занесется идентификатор
  //								экземпляра приложения
  //							 -	флаги филтров транзакции пока не установлены
  if (DdeInitialize(&InstId, (PFNCALLBACK)(FARPROC)CallBackProc, 0, 0) != DMLERR_NO_ERROR) {
		::MessageBox(0,"Инициализация сервера не прошла.", "DDEML Сервер", MB_ICONSTOP | MB_TASKMODAL);
		PostQuitMessage(0);	// Делаем запрос в Windows на завершение работы приложения
  }

  // Must come after we've initialized DDE since InitInstance will trigger
  // SetupWindow
  TApplication::InitInstance();
}
/*
 *  		Завершение работы экземпляра приложения
 */
int
TDMLServerApp::TermInstance(int status)
{
  if (InstId) {				// Если идентификатор экземпляра приложения
									// зарегистрирован, вызываем программу для
									// Отказа от услуг DDEML
		DdeUninitialize(InstId);
  }
  return TApplication::TermInstance(status);
};
//			---------------	OWL Main ------------------------
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  int RetCode;								// Код завершения приложения
  TDMLServerApp*	lpTDMLServerApp;
												// Инициализация библиотек
  BWCCGetVersion();						// BWCC - support
  BWCCIntlTerm();
  BWCCIntlInit(866);
  lpTDMLServerApp = new TDMLServerApp();
  RetCode = lpTDMLServerApp->Run(); // Толкаем приложение на прогон
  return RetCode;
};
/*
 * ---------------------------------------------------------
 *	EoF(testsyst.cpp)
 * ----------------------------------------------------------
 */
