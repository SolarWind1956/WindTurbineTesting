
/*
 * Install - Root Module --------------------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * Install -  Программа установки пакета 'TestSyst' на твердый диск
 *         Основные функции:
 *    -  инициализирует библиотеки BVCC, VBX;
 *    -  инициализирует диалоги с оператором;
 *		-	создает рабочие каталоги на твердом диске;
 *    -  копирует файлы пакета с гибких дисков в рабочие каталоги на твердом диске;
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\inputdia.h>
#include <owl\validate.h>
#include <owl\vbxctl.h>

#include "Install.h"
/*
 * 	Global variables
 */
char SourceDisk[4] = "A";
char TargetDisk[4] = "D";

const char mainTitle[] = "АСУ испытаний - Программа установки";
//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TMainDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE2(TMainDialog, TDialog, // Таблица сообщений данного класса
								TVbxEventHandler)
  EV_WM_CLOSE,
  EV_COMMAND(IDC_BUTTON1,  InstallSystem),		// Установка системы
  EV_COMMAND(IDC_BUTTON2,  UnInstallSystem  ),	// Удаление системы с диска
//  EV_COMMAND(IDC_BUTTON3,  DataViewSystem),	// Отображение информации
//  EV_COMMAND(IDC_BUTTON4,  ),
//  EV_COMMAND(IDC_BUTTON5,  TestCtrlSystem),	// Испытания
//  EV_COMMAND(IDC_BUTTON6,  TypePCNTSystem),	// Сеть контроллеров
//  EV_COMMAND(IDC_BUTTON7,  AnalogRangeSystem),// Модули измерений
//  EV_COMMAND(IDC_BUTTON8,  TypeSensorsSystem),// Датчики
//  EV_COMMAND(IDC_BUTTON9,  ),// Каналы измерений
//  EV_COMMAND(IDC_BUTTON10,  ),// Контроль доступа
//  EV_COMMAND(IDC_BUTTON11, ),// Архивирование данных
//  EV_COMMAND(IDC_BUTTON12, ),// Сервис и настройки
//  EV_COMMAND(IDC_BUTTON14, HelpDialog),		// Помошь
  EV_COMMAND(IDCANCEL, EndSession),          // Выход
//  EV_COMMAND(IDC_BUTTON15,),
END_RESPONSE_TABLE;
//									Constructor for dialogs
TMainDialog::TMainDialog(TWindow* parent)
  : TDialog(parent, "InstallDIALOG"),	TWindow(parent)
{

  Button1  = new TDrawICBT(this, IDC_BUTTON1);
  Button2  = new TDrawICBT(this, IDC_BUTTON2);
//  Button3  = new TDrawICBT(this, IDC_BUTTON3);
//  Button4  = new TDrawICBT(this, IDC_BUTTON4);
//  Button5  = new TDrawICBT(this, IDC_BUTTON5);
//  Button6  = new TDrawICBT(this, IDC_BUTTON6);
//  Button7  = new TDrawICBT(this, IDC_BUTTON7);
//  Button8  = new TDrawICBT(this, IDC_BUTTON8);
//  Button9  = new TDrawICBT(this, IDC_BUTTON9);
//  Button10 = new TDrawICBT(this, IDC_BUTTON10);
//  Button11 = new TDrawICBT(this, IDC_BUTTON11);
//  Button12 = new TDrawICBT(this, IDC_BUTTON12);
//  Button13 = new TDrawICBT(this, IDC_BUTTON13);
//  Button14 = new TDrawICBT(this, IDC_BUTTON14);
  Button15 = new TDrawICBT(this, IDCANCEL);
}
void
TMainDialog::InstallSystem()
{

	if ((TInputDialog(this, "Установка дисковода дистрибутива",
									"Установите диск 1 в дисковод:",
									SourceDisk, sizeof(SourceDisk))).Execute() == IDOK) {
	};
};
void
TMainDialog::UnInstallSystem()
{
};
/*
void											// Окончательное создание диалога
												// отображения информации
TMainDialog::DataViewSystem()
{
	if (!ViewDialogCreated) {			// Если система отображения данных не работает

		ViewDialogCreated = TRUE;
		pTViewDialog->Create();			// Создаем диалог
		pTViewDialog->Show(SW_SHOW);
	}
	else {
		pTViewDialog->EnableWindow(TRUE);
		pTViewDialog->Show(SW_SHOW);
	};
};
void											// Окончательное создание диалога
												// запуска испытаний
TMainDialog::TestCtrlSystem()
{
	if (!TestCtrlDialogCreated) {
		TestCtrlDialogCreated = TRUE;
		pTTestCtrlDialog->Create();	// Создаем диалог
		pTTestCtrlDialog->Show(SW_SHOW);
	}
	else {
		pTTestCtrlDialog->EnableWindow(TRUE);
		pTTestCtrlDialog->Show(SW_SHOW);
	};
};                                  // Окончательное создание диалога
void                                // установки типа контроллера
TMainDialog::TypePCNTSystem()
{
		pTTypePCNTDialog->Execute();	// Создаем диалог
};
												// Окончательное создание диалога
void                                // установки типа аналогового
TMainDialog::AnalogRangeSystem()		// сигнала
{                                   // Создаем диалог
		pTAnalogRangeDialog->Execute();
};

void                                // Окончательное создание диалога
TMainDialog::TypeSensorsSystem()		// установки типов датчиков
{                                   // Создаем диалог
		pTTSensorsDialog->Execute();
};
*/
void
TMainDialog::EvClose(){
	 EndSession();
};
/*
 */
void
TMainDialog::EndSession(){
		 CloseWindow(IDCANCEL);
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TInstallWindow
 *  -------------------------------------------------------------------------
 */
												// Constructor
TInstallWindow::TInstallWindow() : TWindow(0, 0, 0)
{
	  MainDialogExecute   = FALSE; 	// Сбрасываем флаг наличия диалога
};
void
TInstallWindow::Paint(TDC&, BOOL, TRect&)
{
  if (!MainDialogExecute) {
	  Delay(this, 1000);             //  Delay  ~ 1 second
	  MainDialogExecute = TRUE; 		// Взводим флаг наличия диалога
	  TMainDialog(this).Execute();
	  CloseWindow(IDCANCEL);    		// Закрываем главное окнс
  };
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TInstallApp
 *  -------------------------------------------------------------------------
 */

class TInstallApp : public TApplication {
  public:

	TInstallApp() : TApplication() {
		nCmdShow = SW_SHOWMAXIMIZED;	// Large window
	}
	void InitMainWindow()
	{
		MainWindow = new TFrameWindow(NULL, mainTitle,
										new TInstallWindow);
		MainWindow->SetIcon(this, "Install");
		MainWindow->Attr.Style &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX |
											 WS_SYSMENU);
	}
};
/*
 *  -------------------------------------------------------------------------
 * 		Main Program
 *  -------------------------------------------------------------------------
 */
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
  TInstallApp* pTInstallApp;
												// Инициализация библиотек
  BWCCGetVersion();						// BWCC - support
  TBIVbxLibrary vbxLib;					// Init VBX - support

  pTInstallApp = new (TInstallApp);
  pTInstallApp->Run();
  return 0;
}
/*
 * ---------------------------------------------------------
 *	EoF(Install.cpp)
 * ----------------------------------------------------------
 */
