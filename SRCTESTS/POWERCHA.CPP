/*  Project powercha
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    powercha.exe Application
	 FILE:         powercha.cpp
	 AUTHOR:       Heckel & Davidov


	 OVERVIEW
	 ========
	 Source file for implementation of PowerCharApp (TApplication).
*/

#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\menu.h>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>

#include "powercha.h"
#include "pwrcmdic.h"
#include "pwrcmdi1.h"
#include "charpowr.h"
#include "charstck.h"
#include "pwrchrad.h"                     // Definition of about dialog.

#include "database.h"
#include "messagew.h"
/*
 *-------------------------------------------------------------
 *		Прототипы функций
 *-------------------------------------------------------------
 */
BOOL 	ReadAndAvergProcess(double NumCFRead);	// Чтение и пошаговое усреднение
int  	AppendAvergToTP(void);               // функция записи протокола
void 	PXErrMesg(int errCode);					// Функция отображения ошибки .

/*
 *-------------------------------------------------------------
 *		Инициализация внешних переменных
 *-------------------------------------------------------------
 */
BOOL bInptDataON   = FALSE;// Флаг работы коммуникации
BOOL bMonitoryON   = FALSE;// Флаг работы мониторинга
BOOL bMeteoInptON  = FALSE;// Флаг записи протоколов метеоданных
BOOL bCycleFInptON = FALSE;// Флаг записи протоколов циклических данных
BOOL bViewDataON	 = FALSE;// Флаг работы подсистемы отображения данных
BOOL bTestDataON	 = FALSE;// Флаг работы испытания
BOOL bWriteProtON  = FALSE;// Флаг записи протокола
									// Флаги наличия окон с графиками по разным характеристикам
BOOL bWindowPowerChar  = FALSE;
BOOL bWindowStocksChar = FALSE;

/*
 *		Подсистема пользовательских таймеров
 */
TUserTimerSystem* lpTimersList;				// Список пользовательских таймеров
TUserTimer*       pCFTimer;  					//	Таймер для доступа к циклическому файлу
TUserTimer*			pTPWTimer;				   // Таймер для записи протокола испытания
TUserTimer*       pPowerTimer;  				//	Таймер для доступа к циклическому файлу
TUserTimer*			pStocksTimer; 			   // Таймер для записи протокола испытания

int CurNumCFRead, MaxNumCFRead;				// Текущий и максимально возможный номер
														// чтения циклического файла в ОДНОМ цикле
														// записи протокола испытания
double adwInputData1  [N_INPUT_DATA];		// Первый    массив преобразованных данных
double adwInputData2  [N_INPUT_DATA];		// Вторй     массив преобразованных данных
double adwInputData3  [N_INPUT_DATA];		// Третий    массив преобразованных данных
double adwInputData4  [N_INPUT_DATA];		// Четвертый массив преобразованных данных

double adwAvergData1  [N_INPUT_DATA];		// Первый     массив усредненных данных
double adwAvergData2  [N_INPUT_DATA];		// Вторй      массив усредненных данных
double adwAvergData3  [N_INPUT_DATA];		// Третий     массив усредненных данных
double adwAvergData4  [N_INPUT_DATA];		// Четвертый  массив усредненных данных
double adwAddAvergData[N_INPUT_DATA];		// Добавочный массив усредненных данных

int aiControlData1	 [N_CONTROL_DATA];
int aiControlData2	 [N_CONTROL_DATA];

unsigned int iTailCycleFile;

char     mainTitle [70];

/*
 * Generated help file.
 */
const char HelpFileName[] = "powercha.hlp";
char	DBFileName[8];
/*
 * TFileDrop
 * =====
 * Drag / Drop support:
 */
TFileDrop::TFileDrop (char* fileName, TPoint& p, BOOL inClient, TModule* module)
{
	 char    exePath[MAXPATH];

	 exePath[0] = 0;
	 FileName = strcpy(new char[strlen(fileName) + 1], fileName);
	 Point = p;
	 InClientArea = inClient;

	 Icon = (WORD)FindExecutable(FileName, ".\\", exePath) <= 32 ? 0 : ::ExtractIcon(*module, exePath, 0);
	 // Use a question mark if couldn't get the icon from the executable.
	 if ((WORD)Icon <= 1) {  // 0=no icons in exe,  1=not an exe
		  Icon = LoadIcon(0, (WORD)Icon == 1 ? IDI_APPLICATION : IDI_QUESTION);
		  DefIcon = TRUE;
	 } else
		  DefIcon = FALSE;
}
TFileDrop::~TFileDrop ()
{
	 delete FileName;
	 if (!DefIcon)
		  FreeResource(Icon);
}
const char *TFileDrop::WhoAmI ()
{
  return FileName;
}
/*
 * PowerCharApp
 * =====
 * PowerCharApp Implementation
 */
DEFINE_DOC_TEMPLATE_CLASS(TFileDocument, TEditView, DocType1);
// DOC_MANAGER
DocType1 __dvt1("All Files (*.*)", "*.*", 0, "TXT", dtAutoDelete | dtUpdateDir);
/*
 * Build a response table for all messages/commands handled
 * by the application.
 */
DEFINE_RESPONSE_TABLE1(PowerCharApp, TApplication)
	 EV_WM_TIMER,
	 EV_WM_INITMENU,
	 EV_COMMAND(CM_START_STOP, 		CmStartStopProt),
//	 EV_COMMAND(CM_CYCLEPROT, 			CmCycleProt),
	 EV_COMMAND(CM_POWERCHAR, 			CmPowerCharacteristic),
	 EV_COMMAND(CM_STOCKSCHAR,   		CmStocksCharacteristic),
	 EV_OWLVIEW(dnCreate, 				EvNewView),   // Открыть предварительный просмотр
	 EV_OWLVIEW(dnClose,  				EvCloseView), // Закрыть предварительный просмотр
	 EV_COMMAND(CM_HELPABOUT, 			CmHelpAbout),
	 EV_COMMAND(CM_HELPCONTENTS, 		CmHelpContents),
	 EV_COMMAND(CM_HELPUSING, 			CmHelpUsing),
	 EV_WM_DROPFILES,
	 EV_WM_WININICHANGE,
END_RESPONSE_TABLE;

PowerCharApp::PowerCharApp () : TApplication(mainTitle)	{
	 HelpState = FALSE;
	 ContextHelp = FALSE;
	 HelpCursor = 0;

	 Printer = 0;
	 Printing = FALSE;
	 DocManager = new TDocManager(dmMDI | dmMenu);
	 // INSERT>> Your constructor code here.
}
PowerCharApp::~PowerCharApp ()	{
	 if (Printer)
		  delete Printer;
	 // INSERT>> Your destructor code here.
}
BOOL PowerCharApp::CanClose()    {
	 BOOL result = TApplication::CanClose();
	 if (GetKeyState(VK_F4) < 0 && GetKeyState(VK_MENU) < 0){
		 ShowWindow(GetMainWindow()->HWindow, SW_MINIMIZE);
		 return 0;
	 } else {
		if (bWriteProtON){
			MessageBox(GetMainWindow()->HWindow,
						  "Выключите запись протокола", mainTitle,
						  MB_ICONEXCLAMATION);
			return 0;
		} else {
				 // Close the help engine if we used it.
			if (result && HelpState)
				MainWindow->WinHelp(HelpFileName, HELP_QUIT, 0L);
			return result;
		};
	 };
}
void PowerCharApp::SetupSpeedBar (TDecoratedMDIFrame *frame){
	 // Create default toolbar New and associate toolbar buttons with commands.
	 TControlBar* cb = new TControlBar(frame);
											// Кнопка особенная - с фиксацией положения
	 pSTART_STOP_BG  = new TButtonGadget(CM_START_STOP, CM_START_STOP,
											 TButtonGadget::Exclusive, FALSE,
											 TButtonGadget::Up/*Indeterminate*/);
	 cb->Insert(*pSTART_STOP_BG);
											// Остальные - пока простые
	 cb->Insert(*new TButtonGadget(CM_CYCLEPROT,       CM_CYCLEPROT	 ));
	 cb->Insert(*new TSeparatorGadget(6));
	 cb->Insert(*new TButtonGadget(CM_AREAGRAPHIC,     CM_AREAGRAPHIC  ));
	 cb->Insert(*new TButtonGadget(CM_AREA3DGRAPHIC,   CM_AREA3DGRAPHIC));
	 cb->Insert(*new TButtonGadget(CM_COLUMNGRAPHIC,   CM_COLUMNGRAPHIC));
	 cb->Insert(*new TButtonGadget(CM_BAR3DGRAPHIC,    CM_BAR3DGRAPHIC ));
	 cb->Insert(*new TButtonGadget(CM_COLUMNGRAPHIC,   CM_COLUMNGRAPHIC));
	 cb->Insert(*new TButtonGadget(CM_COLUMN3DGRAPHIC, CM_COLUMN3DGRAPHIC));
	 cb->Insert(*new TButtonGadget(CM_LINEGRAPHIC,     CM_LINEGRAPHIC  ));
	 cb->Insert(*new TButtonGadget(CM_LINE3DGRAPHIC,   CM_LINE3DGRAPHIC));
	 cb->Insert(*new TButtonGadget(CM_PIEGRAPHIC,      CM_PIEGRAPHIC   ));
	 cb->Insert(*new TButtonGadget(CM_PIE3DGRAPHIC,    CM_PIE3DGRAPHIC ));
	 cb->Insert(*new TSeparatorGadget(6));
	 cb->Insert(*new TButtonGadget(CM_HELPCONTENTS,    CM_HELPCONTENTS));
	 // Add fly-over help hints.
	 cb->SetHintMode(TGadgetWindow::EnterHints);
	 frame->Insert(*cb, TDecoratedFrame::Top);
}
/*
 * PowerCharApp
 * =====
 * Application intialization.
 */
void PowerCharApp::InitMainWindow () {
	 Client = new PowerCharMDIClient;

	 TDecoratedMDIFrame* frame = new TDecoratedMDIFrame(Name, MDI_MENU, *Client, TRUE);
	 // Override the default window style for the main window.
	 frame->Attr.Style |= WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME | WS_VISIBLE;
	 frame->Attr.Style &= ~(WS_CHILD);
	 nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWMINIMIZED : nCmdShow;
	 // Assign ICON w/ this application.
	 frame->SetIcon(this, IDI_MDIAPPLICATION);
	 // Menu associated with window and accelerator table associated with table.
	 frame->AssignMenu(MDI_MENU);
	 // Associate with the accelerator table.
	 frame->Attr.AccelTable = MDI_MENU;
	 SetupSpeedBar(frame);
	 TStatusBar *sb = new TStatusBar(frame, TGadget::Recessed,
												TStatusBar::CapsLock        |
												TStatusBar::NumLock         |
												TStatusBar::ScrollLock      |
												TStatusBar::Overtype);
	 frame->Insert(*sb, TDecoratedFrame::Bottom);
	 MainWindow = frame;
	 // Borland Windows custom controls.
	 EnableBWCC();
														// Создаем пустой список
														// пользовательских таймеров
	 lpTimersList 	= new TUserTimerSystem(500); // с номерами идентификаторов,
														// начиная с 500.
	 // Формирование списка пользовательских таймеров
										//	Таймер для доступа к циклическому файлу
	 pCFTimer   = lpTimersList->AddUserTimer(GetMainWindow());
	 pCFTimer->InitUserTimer(SCALE_SEC, 2, -100); // и его инициализация
										//	Таймер для записи протокола на диск
	 pTPWTimer  = lpTimersList->AddUserTimer(GetMainWindow());
	 pTPWTimer->InitUserTimer(SCALE_MIN, 1, -100); // и его инициализация
										//	Таймер для отображения МХ
	 pPowerTimer  = lpTimersList->AddUserTimer(GetMainWindow());
	 pPowerTimer->InitUserTimer(SCALE_SEC, 5, -100); // и его инициализация
										//	Таймер для отображения характеристики штоков
	 pStocksTimer  = lpTimersList->AddUserTimer(GetMainWindow());
	 pStocksTimer->InitUserTimer(SCALE_SEC, 5, -100); // и его инициализация
										// Инициализация счетчиков циклов
	 MaxNumCFRead     = lpTimersList->MultipleUTPeriod(pTPWTimer, 	 pCFTimer);
	 CurNumCFRead = 1;
}
/*
 * PowerCharApp
 * =====
 * Response Table handlers:
 */
void PowerCharApp::EvNewView (TView& view){
	 TMDIClient *mdiClient = TYPESAFE_DOWNCAST(MainWindow->GetClientWindow(), TMDIClient);
	 if (mdiClient) {
		  PowerCharMDIChild* child = new PowerCharMDIChild(*mdiClient, 0, view.GetWindow());
		  // Associate ICON w/ this child window.
		  child->SetIcon(this, IDI_DOC);
		  child->Create();
	 }
};

void PowerCharApp::EvCloseView (TView&){
}
/*
 * PowerCharApp
 * =====
 * Menu Help Contents command
 */
void PowerCharApp::CmHelpContents (){
	 // Show the help table of contents.
	 HelpState = MainWindow->WinHelp(HelpFileName, HELP_CONTENTS, 0L);
}
/*
 * PowerCharApp
 * =====
 * Menu Help Using Help command
 */
void PowerCharApp::CmHelpUsing ()   {
	 // Display the contents of the Windows help file.
	 HelpState = MainWindow->WinHelp(HelpFileName, HELP_HELPONHELP, 0L);
}
/*
 * PowerCharApp
 * ===========
 * Menu Help About powercha.exe command
 */
void PowerCharApp::CmHelpAbout (){
	 // Show the modal dialog.
	 PowerCharAboutDlg(MainWindow).Execute();
}
void PowerCharApp::InitInstance (){
	 int   PosSepar;
	 TApplication::InitInstance();
	 if (hPrevInstance == 0)
//		 InitParadoxEngine();	         // Init Paradox Engine 3.0
	 // Accept files via drag/drop in the frame window.
	 MainWindow->DragAcceptFiles(TRUE);
	 TMenu menu(GetSystemMenu(GetMainWindow()->HWindow, FALSE));
	 // Get count items system menu
	 PosSepar = menu.GetMenuItemCount();
	 // Find position for item 'Close'
	 for (char i = 0; i < PosSepar; i++)
		 if (menu.GetMenuItemID(i) == SC_CLOSE){
			 PosSepar = i;
			 break;
		 };
	 // Delete Item 'Close' and 'Separate' from System menu
//	 menu.DeleteMenu(PosSepar - 1, MF_BYPOSITION);
//	 menu.DeleteMenu(SC_CLOSE, MF_BYCOMMAND);
}
void PowerCharApp::EvDropFiles (TDropInfo drop)	{
	 // Number of files dropped.
	 int totalNumberOfFiles = drop.DragQueryFileCount();
	 TFileList* files = new TFileList;
	 for (int i = 0; i < totalNumberOfFiles; i++) {
		  // Tell DragQueryFile the file interested in (i) and the length of your buffer.
		  int     fileLength = drop.DragQueryFileNameLen(i) + 1;
		  char    *fileName = new char[fileLength];
		  drop.DragQueryFile(i, fileName, fileLength);
		  // Getting the file dropped. The location is relative to your client coordinates,
		  // and will have negative values if dropped in the non client parts of the window.
		  // DragQueryPoint copies that point where the file was dropped and returns whether
		  // or not the point is in the client area.  Regardless of whether or not the file
		  // is dropped in the client or non-client area of the window, you will still receive
		  // the file name.
		  TPoint  point;
		  BOOL    inClientArea = drop.DragQueryPoint(point);
		  files->Add(new TFileDrop(fileName, point, inClientArea, this));
	 }
	 // Open the files that were dropped.
	 AddFiles(files);
	 // Release the memory allocated for this handle with DragFinish.
	 drop.DragFinish();
}
void PowerCharApp::AddFiles (TFileList* files) {
	 // Open all files dragged in.
	 TFileListIter fileIter(*files);
	 while (fileIter) {
		  TDocTemplate* tpl = GetDocManager()->MatchTemplate(fileIter.Current()->WhoAmI());
		  if (tpl)
				tpl->CreateDoc(fileIter.Current()->WhoAmI());
		  fileIter++;
	 }
}
BOOL PowerCharApp::ProcessAppMsg (MSG& msg) {
	 if (msg.message == WM_COMMAND) {
		  if (ContextHelp || (GetKeyState(VK_F1) < 0)) {
				ContextHelp = FALSE;
				MainWindow->WinHelp(HelpFileName, HELP_CONTEXT, msg.wParam);
				return TRUE;
		  }
	 } else
		  switch (msg.message) {
		  case WM_KEYDOWN:
				if (msg.wParam == VK_F1) {
					 // If the Shift/F1 then set the help cursor and turn on the modal help state.
					 if (::GetKeyState(VK_SHIFT) < 0) {
						  ContextHelp = TRUE;
						  HelpCursor = ::LoadCursor(MainWindow->GetModule()->GetInstance(), MAKEINTRESOURCE(IDC_HELPCURSOR));
						  ::SetCursor(HelpCursor);
						  return TRUE;        // Gobble up the message.
					 } else {
						  // If F1 w/o the Shift key then bring up help's main index.
						  MainWindow->WinHelp(HelpFileName, HELP_INDEX, 0L);
						  return TRUE;        // Gobble up the message.
					 }
				} else {
					 if (ContextHelp && (msg.wParam == VK_ESCAPE)) {
						  if (HelpCursor)
								::DestroyCursor(HelpCursor);
						  ContextHelp = FALSE;
						  HelpCursor = 0;
						  MainWindow->SetCursor(0, IDC_ARROW);
						  return TRUE;    // Gobble up the message.
					 }
				}
				break;
		  case WM_MOUSEMOVE:
		  case WM_NCMOUSEMOVE:
				if (ContextHelp) {
					 ::SetCursor(HelpCursor);
					 return TRUE;        // Gobble up the message.
				}
				break;
		  case WM_INITMENU:
				if (ContextHelp) {
					 ::SetCursor(HelpCursor);
					 return TRUE;        // Gobble up the message.
				}
				break;
		  case WM_ENTERIDLE:
				if (msg.wParam == MSGF_MENU)
					 if (GetKeyState(VK_F1) < 0) {
					 ContextHelp = TRUE;
					 MainWindow->PostMessage(WM_KEYDOWN, VK_RETURN, 0L);
					 return TRUE;       // Gobble up the message.
				}
				break;
		  default:
				;
		  };  // End of switch

	 // Continue normal processing.
	 return TApplication::ProcessAppMsg(msg);
}
void PowerCharApp::EvWinIniChange (char far* section){
	 if (lstrcmp(section, "windows") == 0) {
		  // If the device changed in the WIN.INI file then the printer
		  // might have changed.  If we have a TPrinter (Printer) then
		  // check and make sure it's identical to the current device
		  // entry in WIN.INI.
		  if (Printer) {
				char printDBuffer[255];
				LPSTR printDevice = printDBuffer;
				LPSTR devName = 0;
				LPSTR driverName = 0;
				LPSTR outputName = 0;
				if (::GetProfileString("windows", "device", "", printDevice, sizeof(printDevice))) {
					 // The string which should come back is something like:
					 //      HP LaserJet III,hppcl5a,LPT1:
					 // Where the format is:
					 //      devName,driverName,outputName
					 devName = printDevice;
					 while (*printDevice) {
						  if (*printDevice == ',') {
								*printDevice++ = 0;
								if (!driverName)
									 driverName = printDevice;
								else
									 outputName = printDevice;
						  } else
								printDevice = AnsiNext(printDevice);
					 }
					 if ((Printer->GetSetup().Error != 0)                                ||
						  (lstrcmp(devName, Printer->GetSetup().GetDeviceName()) != 0)    ||
						  (lstrcmp(driverName, Printer->GetSetup().GetDriverName()) != 0) ||
						  (lstrcmp(outputName, Printer->GetSetup().GetOutputName()) != 0)) {
						  // New printer installed so get the new printer device now.
						  delete Printer;
						  Printer = new TPrinter;
					 }
				} else {
					 // No printer installed (GetProfileString failed).
					 delete Printer;
					 Printer = new TPrinter;
				}
		  }
	 }
};
/*
 *-------------------------------------------------------------
 *
 * 			Н а ш и    ф у н к ц и и
 *
 *-------------------------------------------------------------
 */
/*
 *-------------------------------------------------------------
 *		Циклическое выполнение функций записи и чтения данных
 *-------------------------------------------------------------
 */
void PowerCharApp::EvTimer(UINT timerId){

	if (timerId == pCFTimer->IDEvTimer){
		 if (pCFTimer->TestUserTimer()){
											// Запуск процедуры обработки и усреднения
											// При чтении новой записи
			  if (ReadAndAvergProcess((double)CurNumCFRead) == TRUE){
				  Characteristic(); 	// Очередной шаг формирования МХ
				  CurNumCFRead++;		// Наращиваем счетчик усреднеий
			  };
			  if (CurNumCFRead > MaxNumCFRead){ // Если счетчик усреднений достиг предела
					AppendAvergToTP();// пишем усредненную информацию в
					CurNumCFRead = 1; // файл-протокол испытаний
			  };
		 };
	};
};
/*
 *-------------------------------------------------------------
 *		Инициализация меню
 *-------------------------------------------------------------
 */
void PowerCharApp::EvInitMenu(HMENU menuHandle)	{
  // This technique is used to automatically update the status of the various
  // menu choices just before the menu is displayed.
  TMenu menu(menuHandle);
  menu.CheckMenuItem (CM_POWERCHAR,
						MF_BYCOMMAND | bWindowPowerChar ? MF_CHECKED : MF_UNCHECKED);
  menu.CheckMenuItem (CM_STOCKSCHAR,
						MF_BYCOMMAND | bWindowStocksChar ? MF_CHECKED : MF_UNCHECKED);
  menu.CheckMenuItem (CM_START_STOP,
						MF_BYCOMMAND | bWriteProtON ? MF_CHECKED : MF_UNCHECKED);
  pSTART_STOP_BG->SetButtonState(
				bWriteProtON ? TButtonGadget::Down : TButtonGadget::Up);
  DrawMenuBar(this->GetMainWindow()->HWindow);
}
/*
 *-------------------------------------------------------------
 *		Запуск функции изменения периода записи протокола МХ
 *-------------------------------------------------------------
 */
void PowerCharApp::CmCycleProt(){
	  pTPWTimer->ChangeUserTimerPeriod();
	  MaxNumCFRead = lpTimersList->MultipleUTPeriod(pTPWTimer, pCFTimer);
	  CurNumCFRead = 1;
};
/*
 *-------------------------------------------------------------
 *		Функция пуска / останова записи файла
 *-------------------------------------------------------------
 */
void PowerCharApp::CmStartStopProt(){
	if (!bWriteProtON){					// Если запись не включена
		  bWriteProtON = TRUE;      	// Установим флаг работы с протоколами,
												// Установим таимер цикла чтения цикл.файла
		  pCFTimer->SetUserTimer(GetMainWindow()->HWindow);
	}  else {            				// Если запись в МХ, и чтение из ЦФ успешно
		  bWriteProtON = FALSE;      	// Сбросим флаг работы с протоколами,
		  pCFTimer->KillUserTimer();	// Убъем таймер

// 'Закроем все открытые MDI окна'
// пока тут этой функции нет, так как не найдена подходящая

	};                               // Обновим меню
	PostMessage(this->GetMainWindow()->HWindow, WM_INITMENU,
					WPARAM(GetMenu(this->GetMainWindow()->HWindow)), 0);
};
/*
 *-------------------------------------------------------------
 *		Функция создания нового окна графиков по МХ
 *-------------------------------------------------------------
 */
void PowerCharApp::CmPowerCharacteristic(){
												// Создадим окно толко в том случае, если
												// 1. Включен протокол
												// 2. Окно еще не создано
	if (bWindowPowerChar == FALSE && bWriteProtON){
												// Создание указатель на новое дочернее
												// окно MDI
		CharactPowerMDIChild* child = new CharactPowerMDIChild(
								*Client, "Мощностная характеристика ВЭУ",	NULL);
												// Свяжем икону с окном
		child->SetIcon(this, IDI_CHARACT);
		child->Create();					// Создаем окно
	};
};

/*
 *-------------------------------------------------------------
 *		Функция создания нового окна графиков по характеристике штоков
 *-------------------------------------------------------------
 */
void PowerCharApp::CmStocksCharacteristic(){
												// Создадим окно толко в том случае, если
												// 1. Включен протокол
												// 2. Окно еще не создано
	if (bWindowStocksChar == FALSE && bWriteProtON){
												// Создание указатель на новое дочернее
												// окно MDI
		CharactStocksMDIChild* child = new CharactStocksMDIChild(
								*Client, "Характеристика хода штоков ВЭУ", NULL);
												// Свяжем икону с окном
		child->SetIcon(this, IDI_CHARACT);
		child->Create();					// Создаем окно
	};
};

/*
 *-------------------------------------------------------------
 *		Главная функция программы
 *-------------------------------------------------------------
 */
int OwlMain(int ParamCount, char* argv[])	{
  int result;										// Код завершения приложения
  if (ParamCount > 1){
		strcpy(mainTitle,   argv[1]);			// 'Caption' берем из 1-го пареметра
		strcpy(DBFileName,  argv[2]); 		// Имя БД берем из 2-го пареметра
  } else {
		strcpy(mainTitle, "Нет параметров");
  };
  BWCCGetVersion();								// BWCC - support
  if (!InitPXEngine("PowerChar"))			// Создаем механизм Paradox Engine
		return -1;
  // Создадим класс приложения
  PowerCharApp    App;
  // Откроем в начале сеанса и циклический файл и файл протокола испытаний.
  CreatePowerCharTables();						// Создаем таблицы файлов базы данных
  if (!CreateOpenCycleFile() || !CreateOpenTestProtocol()){
														// Если есть проблемы хотя бы с одним
		return -1;									// из них приложение не может работать
  };
  // Толкаем приложение в работу
  result 			= App.Run();

  CloseCycleFile();	CloseTestProtocol(); // Закрываем файлы базы данных
  DeletePowerCharTables();						// И уничтожаем их таблицы
  delete lpTimersList;							// Уничтожаем пользовательские таймеры
														// А также всю кухню
  OffPXEngine();									// ParadoX Engine & DB FW
  return result;
}
/*
 *	--------------------------------------------------------------
 * 	End of file powercha.cpp
 *--------------------------------------------------------------
 */
