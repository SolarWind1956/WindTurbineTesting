/*
 * tmodulpc - List type modules PC module -----------------------------
 *
 * Author:
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 *	tmodulpc - модуль редактирования списка поддерживаемых
 * аналоговых и дискретных модулей ввода-вывода с
 * измемением, добавлением и удалением записи.
 * Вся информация находится в каталоге 'DBNET/', в формате
 *	Paradox Engine 3.0, и имеет имена файлов:
 * AnalMdXX.DB, DscrMdXX.DB, CounMdXX.DB, FCouMdXX.DB, UserMdXX.DB,
 *	где XX номер контроллера, к которому подключены модули.
 */
/*
 * 	Макросы для работы с Paradox Engine
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
 * 	Включение заголовочных файлов
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
 *		Прототипы функций
 *-------------------------------------------------------------
 */
/*
 * 		Переменные и константы
 */
struct Ranges {
	char  Boundares[10];				// Границы диапазона
	char  Dimension[4];				// Размерность диапазона
};
											// Массив диапазонов по
											// напряжению DC
const Ranges VoltRanges[4] =	{	{"-5..+5",   "V"},
											{"0..+5",    "V"},
											{"-10..+10", "V"},
											{"0..+10",   "V"}	};
											// Массив диапазонов по
											// постоянному току
const Ranges CurrRanges[4] =	{	{"0..20", "mA"},
											{"4..20", "mA"},
											{"0..10", "mA"},
											{"0..5",  "mA"} };
											// Номер выбранного элемента списка
int SelectIndex_PC  = 0;         // контроллеров
int SelectIndex_MDIO = 0;     	// Номер выбранного элемента списка
											// модулей
											// Флаг изменения в окнах ввода
static BOOL  bChangeParams = FALSE;
static BOOL  bFillList	   = FALSE;	// Флаг заполнения списка
		 BOOL  NewPC 			= FALSE;	// Флаг перехода на другой ПК
static RECORDNUMBER RecCountMDIO;   // Количество записей в файле модулей
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
  TypePC_CB	  		= new TComboBox   (this, IDC_TMicroPCList_MDIO);// тип ПК

  Analog_RB   		= new TRadioButton(this, IDC_Modul_A_MDIO);	  // Кнопка "Аналоговый тип модуля"
  Discret_RB 		= new TRadioButton(this, IDC_Modul_D_MDIO);	  // Кнопка "Дискретный тип модуля"
  Counter_RB  		= new TRadioButton(this, IDC_Modul_C_MDIO);	  // Кнопка "Счетчик модуль"
  FCounter_RB 		= new TRadioButton(this, IDC_Modul_F_MDIO);	  // Кнопка "Быстрый счетчик модуль"
  User_RB	  		= new TRadioButton(this, IDC_Modul_U_MDIO);	  // Кнопка "Нестандартный тип модуля"

  TModuleList_CB 	= new TComboBox   (this, IDC_TModuleList_MDIO); // Cписок существ. модулей
  TypeModule_ED	= new TEdit   	  	(this, IDC_TypeModule_MDIO);  // Аббревиатура имени модуля

  Current_RB  		= new TRadioButton(this, IDC_Current_MDIO);   // Кнопка "постоянный ток"
  Voltage_RB  		= new TRadioButton(this, IDC_Voltage_MDIO);   // Кнопка "напряжение DC"

  Boundares_CB		= new TComboBox   (this, IDC_Boundares_MDIO	);// Список существ. границ уравней
  Dimension_CB   	= new TComboBox   (this, IDC_Dimension_MDIO);	// Список существ. размерностей

  Help_ICBT   		= new TDrawICBT	(this, IDC_Help_MDIO);	// Помощь
  Cancel_ICBT     = new TDrawICBT	(this, IDCANCEL);		   // Выход

  Save_BUT 			= new TButton  	(this, IDC_Save_MDIO);   // Запись
  Delete_BUT  		= new TButton     (this, IDC_Delete_MDIO);	// Удаление

  Message_IB  		= new TBInfoBox   (this, IDC_Message_MDIO);// Статусная строка
  Picture  			= new TBitmap		(*GetModule(), IDC_MODULEIO);

  valid = new TFilterValidator("0-9");		// Установим фильтр ввода для цифр
  if (valid){                             // Место модуля на крейте
		new TEdit(this,IDC_NumPlace_MDIO)->SetValidator(valid);
  };
  if (valid){      								// Номер канала в модуле
		new TEdit(this,IDC_NumChannel_MDIO)->SetValidator(valid);
  };
  if (valid){      								// Разрешение (в битах)
		new TEdit(this,IDC_Resolution_MDIO)->SetValidator(valid);
  };
  validdec = new TFilterValidator("0-9.");// Установим фильтр ввода для цифр
  if (validdec){      							// Класс точности
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
	  DrawBitMap(dc, *Picture, 425, 5); // Собственно отображение картинки
};
/*
 * 	Нагружаем функцию SetupWindow() своими функциями :
 *		1. Открытие файла данных
 *		2.Заполнение списка и окон
 */
void
TModulPCDialog::SetupWindow(){      // Выбираем текущий контроллер
  RECORDNUMBER RecCount;				// Количество записей в файле
  SelectIndex_PC  = CodeController - 1;
  if (!CreateOpenMContrls()){       // Открываем файл	со списком
												// контроллеров
	  if (pT_CONT->getRecCount() == 0){ // По отсутствии информации
												// о контроллерах
		  MessageW("Модули ввода/вывода", "Нет информации по контроллерам");
		  CloseMContrls();            // Закроем файл ПК
		  TDialog::SetupWindow();		// Показываем диалог на экране
		  return;                     // !!! Но не более
	  };
  };
  TDialog::SetupWindow();				// Показываем диалог на экране
												// Добавим ПК из файла в список ПК
  if ((RecCount = pT_CONT->getRecCount()) > 0)	// Количество записей
	  for (RECORDNUMBER i = 1; i <= RecCount; i++){
												// Читаем из файла типы контроллеров
												// в динамический буфер записи
			pT_CONT->go(i);
			pT_CONT->getFieldFromCurrentRec("TypePC",
								 pT_CONT->fPoint("TypePC"));
												// и формируем ComboBox
			TypePC_CB->AddString((const char*)pT_CONT->fPoint("TypePC"));
		}; // for(;;)
  NewPC = TRUE;							// Взведем флаг перехода на другой ПК
  if (CodeController == 0){			// По умолчанию INFO 73
		SelectIndex_PC = TypePC_CB->FindString("Info73", 0);
		CodeController = SelectIndex_PC + 1;
  };
  TypePC_CB->SetSelIndex(SelectIndex_PC); // Маркируем выбранный контроллер
  Analog_RB->SetCheck(BF_CHECKED);	// Маркируем "Аналоговый тип модуля"
  TypeModule();							// и идем на анализ выбора типа
												// модуля с открытием соответствующего
												// файла и заполнения списка модулей
};
/*
 * 	Отображение списка ПК и соответствующих для него модулей
 */
void
TModulPCDialog::TMicroPCList(){  	//	Какой номер выбранного элемента в списке ПК
  SelectIndex_PC = TypePC_CB->GetSelIndex();
												// Изменим текущий контроллер
  CodeController = SelectIndex_PC + 1;
  pT_CONT->go(CodeController);		// Читаем запись из файла ПК
  pT_CONT->FromFileToDynRec();
  Analog_RB->SetCheck(BF_CHECKED);	// Маркируем "Аналоговый тип модуля"
  TypeModule();							// и идем на анализ выбора типа
												// модуля с открытием соответствующего
												// файла
};
/*
 * 	Отображение списка модулей и соответствующей для них информации
 *		по выбранному ПК
 */
void
TModulPCDialog::TModuleList(){
  char AddedString[16];  		// Инициализация строки, которая
										// в последующем будет занесена в список
										// выбранных диапазонов
										// в формате :
										// "Марка модуля[номер места на крейте](номер канала)"
										// Пример: "AI101A[10](0)"
  bFillList = TRUE;
  for (char i = 0; i < 16; i++) AddedString[i] = ' ';
  if (NewPC){								// При отображении нового списка
	  SelectIndex_MDIO = 0;				// для нового ПК
	  TModuleList_CB->ClearList();  	// Очищаем список модулей
	  TModuleList_CB->AddString("Новый");	// Добавим пункт "Новый" в список
												// для последующего выбора данного
												// пункта для создания нового
												// описания
	  for (RECORDNUMBER i = 1; i <= RecCountMDIO; i++){
			pT_MDIO->go(i);     		// Читаем информацию из файла
			pT_MDIO->FromFileToDynRec(); // и помещаем
												// ее в список диапазонов
												// Формируем строку и заносим ее
												// в список 'ListBox'
			sprintf(AddedString, "%s[%d](%d)",
					  pcCHAR("TypeModule"),
					  vINTG("NumPlace"),
					  vINTG("NumChannel"));
			TModuleList_CB->AddString(AddedString);
	  };
	  TModuleList_CB->SetSelIndex(SelectIndex_MDIO);
	  NewPC = FALSE;                 // Сбрасываем флаг перехода на другой ПК
  } else {									// Номер выбранного элемента
	  SelectIndex_MDIO = TModuleList_CB->GetSelIndex();
  };
  Boundares_CB->ClearList();	 			// Очищаем список границ
  Dimension_CB->ClearList();	 			// Очищаем список размерностей
  if (SelectIndex_MDIO != 0){           // При отображении существующей информации
	  pT_MDIO->go(SelectIndex_MDIO);
	  pT_MDIO->FromFileToDynRec();		// Читаем информацию из файла
													// Маркируем RadioButton и заполняем списки
													// границ и размерностей и т.д.
													// cоответствующие данным
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
												// Список границ
			for (int i = 0; i < sizeof(VoltRanges)/sizeof(VoltRanges[0]); i++)
				  Boundares_CB->AddString(VoltRanges[i].Boundares);
												// Размерность
			Dimension_CB->AddString(VoltRanges[0].Dimension);
	  };
												// Границы
	  Boundares_CB->SetSelIndex(Boundares_CB->FindString(pCHAR("Boundares"), 0));
												// Размерность
	  Dimension_CB->SetSelIndex(Dimension_CB->FindString(pCHAR("Dimension"), 0));
												// Марка модуля
	  SetDlgItemText(IDC_TypeModule_MDIO, pcCHAR("TypeModule"));
												// Место модуля на крейте
	  SetDlgItemInt(IDC_NumPlace_MDIO,    vUINT("NumPlace"));
												// Номер канала
	  SetDlgItemInt(IDC_NumChannel_MDIO,  vUINT("NumChannel"));
												// Разрещение АЦП / ЦАП
	  SetDlgItemInt(IDC_Resolution_MDIO,  vUINT("Resolution"));
												// Класс точности ("commfunc.h")
	  SetDlgItemDoub(this->HWindow, IDC_Tolerance_MDIO, vDOUB("Tolerance"));

	  Delete_BUT->EnableWindow(TRUE);
  } else {									// При отображении конфигурации для создания нового
												// описания по умолчанию :
	  Current_RB->SetCheck(BF_CHECKED);
	  Voltage_RB->SetCheck(BF_UNCHECKED);
	  for (int i = 0; i < sizeof(CurrRanges)/sizeof(CurrRanges[0]); i++)
			 Boundares_CB->AddString(CurrRanges[i].Boundares);

	  Dimension_CB->AddString(CurrRanges[0].Dimension);
												// Марка модуля
	  SetDlgItemText(IDC_TypeModule_MDIO,  "");
												// Место модуля на крейте
	  SetDlgItemText(IDC_NumPlace_MDIO, "0");
												// Номер канала
	  SetDlgItemText(IDC_NumChannel_MDIO, "0");
												// Разрещение АЦП / ЦАП
	  SetDlgItemText(IDC_Resolution_MDIO, "0");
												// Класс точности
	  SetDlgItemText(IDC_Tolerance_MDIO, "0");
	  Delete_BUT->EnableWindow(FALSE);
	};
  Save_BUT->EnableWindow(FALSE);
  bFillList 	 = FALSE;
  bChangeParams = FALSE;				// Сбросим флаг изменений
};
/*
 *		Отрабатывает при нажатии RadioButton'а "Напряжение"
 *		и RadioButton'а "Постоянный ток"
 */
void
TModulPCDialog::CurrentVoltage(){
	int i;
	Boundares_CB->ClearList();			// Очищаем список
	Dimension_CB->ClearList();			// Очищаем список
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
	bChangeParams = TRUE;				// Взводим флаг изменений
	Save_BUT->EnableWindow(bChangeParams);
};
/*
 *		Отрабатывает при нажатии RadioButton'ов по типу модулей:
 *		Аналоговый, Дискретный, Счетчик, Быстрый счетчик, Нестандартный
 */
void
TModulPCDialog::TypeModule(){
	CloseModuls();     					// Закроем старый файл модулей
	NewPC = TRUE;							// Эмулируем выбор другого ПК для отработки функции
												// MDIOTModuleList как для нового ПК
												// Откроем новый файл модулей
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
	};                               // Кол-во записей в файле модулей
	RecCountMDIO   =  pT_MDIO->getRecCount();
	TModuleList();						   // Отобразим список модулей
	bChangeParams = FALSE;				// Сбросим флаг измемения
};
/*
 * 	Функция взводит флаг как только было выбрано
 *		что-либо в списках границ, размерностей и т.д.
 */
void
TModulPCDialog::ChangeText(){
	if (!bFillList){
		Message_IB->ClearShadeBox();
		bChangeParams = TRUE;				// Взводим флаг изменений
		Save_BUT->EnableWindow(bChangeParams);
		Delete_BUT->EnableWindow(!bChangeParams);
	};
};
/*
 *		Записываем или изменяем данные в файле
 */
void
TModulPCDialog::SaveRecord(){
  char AddedString[16];
  for (int i = 0; i < 16; i++) AddedString[i] = ' ';
														// Получим новые данные :
														// Марка модуля
  GetDlgItemText(IDC_TypeModule_MDIO, pCHAR("TypeModule"),
												 iLEN("TypeModule"));
														// Место модуля на крейте
  vINTG("NumPlace")   = GetDlgItemInt(IDC_NumPlace_MDIO);
														// Номер канала
  vINTG("NumChannel") = GetDlgItemInt(IDC_NumChannel_MDIO);
														// Границы диапазона
  GetDlgItemText(IDC_Boundares_MDIO, pCHAR("Boundares"),
												iLEN("Boundares"));
														// Размерность диапазона
  GetDlgItemText(IDC_Dimension_MDIO, pCHAR("Dimension"),
												iLEN("Dimension"));
														// Разрешение АЦП/ЦАП
  vINTG("Resolution") = GetDlgItemInt(IDC_Resolution_MDIO);
														// Класс точности ("commfunc.h")
  vDOUB("Tolerance")  = GetDlgItemDoub(this->HWindow, IDC_Tolerance_MDIO);
														// Формируем строку для добавления
														// или замещения в списке модулей
  sprintf(AddedString, "%s[%d](%d)",
				pCHAR("TypeModule"),
				vINTG("NumPlace"),
				vINTG("NumChannel"));
														// Тип сигнала
  if (Voltage_RB->GetCheck() == BF_CHECKED){
		strcpy(pCHAR("TypeRange"), "VOLTAGE");
  } else if (Current_RB->GetCheck() == BF_CHECKED){
		strcpy(pCHAR("TypeRange"), "CURRENT");
  };
  if (SelectIndex_MDIO == 0){ 				// Если создаем новое описание
	  TModuleList_CB->AddString(AddedString);	// Добавим элемент в список
													// Номер заносимой в файл записи
	  vINTG("IndexCode") = (short)((TModuleList_CB->GetCount()) - 1);
	  pT_MDIO->clearRecordForWrite();
	  pT_MDIO->FromDynRecToFile();
	  pT_MDIO->append();						// Запись данных - добавление
	  SelectIndex_MDIO = vINTG("IndexCode");
	  pT_CONT->go(SelectIndex_PC + 1); // Читаем запись из файла ПК
	  pT_CONT->IncrRefrence();
													// Запись для БД MContrls данных - замещение
													// из-за поля кол-ва ссылок
  } else {          	                  // Заменяем старые данные
													// Номер заносимой в файл записи
	  vINTG("IndexCode")= (short)(SelectIndex_MDIO);
													// Удалим элемент из ComboBox'а
	  TModuleList_CB->DeleteString(SelectIndex_MDIO);
													// Вставим элемент в список выбранных
													// диапазонов
	  TModuleList_CB->InsertString(AddedString, SelectIndex_MDIO);
	  pT_MDIO->FromDynRecToFile();
	  pT_MDIO->update();						// Запись для БД MContrls данных - замещение
  };
													// Маркируем добавленный элемент
  TModuleList_CB->SetSelIndex(SelectIndex_MDIO);
  bChangeParams = FALSE;               // Сбросим флаг изменений
  Save_BUT->EnableWindow(bChangeParams);
  Delete_BUT->EnableWindow(!bChangeParams);
};
/*
 * 	Удаление текущей записи из файла
 */
void
TModulPCDialog::DeleteRecord(){
  if (vINTG("Refrence") == 0){
		pT_MDIO->delCurRecord();          // Удаление записи из файла
														// Удаление элемента из списка
		TModuleList_CB->DeleteString(SelectIndex_MDIO);
		SelectIndex_MDIO--;                  // Маркируем предыдущий элемент
		TModuleList_CB->SetSelIndex(SelectIndex_MDIO);
		pT_CONT->go(SelectIndex_PC + 1);// Читаем запись из файла ПК
		pT_CONT->DecrRefrence();
		TModuleList();							   // Показываем информацию о
  } else {											// предыдущем элементе
		Message_IB->ClearShadeBox();
		Message_IB->StringToShadeBox(2, 2, RGB(255, 0, 0),
											  "Данный объект нельзя удалить,"
											  "так как на него существует ссылка");
  };
};
/*
 *		Закрытие файла и выход
 */
void
TModulPCDialog::EndSession(){
  if (bChangeParams){
	  if (MessageBox("Данные не сохранены.\nСохранить перед выходом ?",
						  "Модули ввода/вывода",
						  MB_YESNO) == IDYES){
			SaveRecord();				 // Сохраним данные
	  };
  };
  CloseModuls();                // Закроем файл модулей
  CloseMContrls();                // Закроем файл ПК
  CloseWindow();
};

/*
 * ---------------------------------------------------------
 *	EoF(tmodulpc.cpp)
 * ----------------------------------------------------------
 */
