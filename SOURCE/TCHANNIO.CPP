/*
 * tchannio - List type sensors module ------------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 *	tchannio - модуль редактирования списка канала измерений
 * с измемением, добавлением и удалением записи
 * Вся информация находиться в файле 'DBNET/CHANIOXX.DB' где ХХ - номер
 * контроллера к которому подсоеденен данный канал измерений,
 * поддерживаемом пакетом PXEngine 3.0
 */
/*
 * 	Макросы для работы с Paradox Engine
 */
									// Каналы ввода / вывода
#define 	pVOID_CHNL(X)	  pT_CHNL->fPoint(X)

#define	vDOUB_CHNL(X)	 *((double*)(pVOID_CHNL(X)))
#define	pDOUB_CHNL(X)	  (double*)(pVOID_CHNL(X))
#define	vUINT_CHNL(X)	 *((UINT*)(pVOID_CHNL(X)))
#define	vINTG_CHNL(X)	 *((INT16*)(pVOID_CHNL(X)))
#define	pINTG_CHNL(X)	  (INT16*)(pVOID_CHNL(X))
#define  pCHAR_CHNL(X)   (char*)(pVOID_CHNL(X))
#define  pcCHAR_CHNL(X)  (const char*)(pVOID_CHNL(X))
#define  iLEN_CHNL(X)	 (int)(pT_CHNL->fLen(X))

									// Модули ввода / вывода
#define 	pVOID_MDIO(X)	  pT_MDIO->fPoint(X)
#define	vDOUB_MDIO(X)	 *((double*)(pVOID_MDIO(X)))
#define	pDOUB_MDIO(X)	  (double*)(pVOID_MDIO(X))
#define	vUINT_MDIO(X)	 *((UINT*)(pVOID_MDIO(X)))
#define	vINTG_MDIO(X)	 *((INT16*)(pVOID_MDIO(X)))
#define	pINTG_MDIO(X)	  (INT16*)(pVOID_MDIO(X))
#define  pCHAR_MDIO(X)    (char*)(pVOID_MDIO(X))
#define  pcCHAR_MDIO(X)   (const char*)(pVOID_MDIO(X))
#define  iLEN_MDIO(X)	  (int)(pT_MDIO->fLen(X))
									// Контроллеры
#define 	pVOID_CONT(X)	  pT_CONT->fPoint(X)
#define	vDOUB_CONT(X)	 *((double*)(pVOID_CONT(X)))
#define	pDOUB_CONT(X)	  (double*)(pVOID_CONT(X))
#define	vUINT_CONT(X)	 *((UINT*)(pVOID_CONT(X)))
#define	vINTG_CONT(X)	 *((INT16*)(pVOID_CONT(X)))
#define	pINTG_CONT(X)	  (INT16*)(pVOID_CONT(X))
#define  pCHAR_CONT(X)    (char*)(pVOID_CONT(X))
#define  pcCHAR_CONT(X)   (const char*)(pVOID_CONT(X))
#define  iLEN_CONT(X)	  (int)(pT_CONT->fLen(X))
									// Преобразователи
#define 	pVOID_TRNS(X)	  pT_TRNS->fPoint(X)
#define	vDOUB_TRNS(X)	 *((double*)(pVOID_TRNS(X)))
#define	pDOUB_TRNS(X)	  (double*)(pVOID_TRNS(X))
#define	vUINT_TRNS(X)	 *((UINT*)(pVOID_TRNS(X)))
#define	vINTG_TRNS(X)	 *((INT16*)(pVOID_TRNS(X)))
#define	pINTG_TRNS(X)	  (INT16*)(pVOID_TRNS(X))
#define  pCHAR_TRNS(X)    (char*)(pVOID_TRNS(X))
#define  pcCHAR_TRNS(X)   (const char*)(pVOID_TRNS(X))
#define  iLEN_TRNS(X)	  (int)(pT_TRNSC->fLen(X))
									// Датчики
#define 	pVOID_SENS(X)	  pT_SENS->fPoint(X)
#define	vDOUB_SENS(X)	 *((double*)(pVOID_SENS(X)))
#define	pDOUB_SENS(X)	  (double*)(pVOID_SENS(X))
#define	vUINT_SENS(X)	 *((UINT*)(pVOID_SENS(X)))
#define	vINTG_SENS(X)	 *((INT16*)(pVOID_SENS(X)))
#define	pINTG_SENS(X)	  (INT16*)(pVOID_SENS(X))
#define  pCHAR_SENS(X)    (char*)(pVOID_SENS(X))
#define  pcCHAR_SENS(X)   (const char*)(pVOID_SENS(X))
#define  iLEN_SENS(X)	  (int)(pT_SENS->fLen(X))
/*
 * 	Включение заголовочных файлов
 */
#include <stdio.h>
#include "commfunc.h"
#include "commglob.h"
#include "database.h"
#include "tchannio.h"

static int SelectIndex_CHNL = 0;	// Номер выбранного элемента списка
											// Флаг изменения в окнах ввода
static BOOL  bChangeParams = FALSE;
static BOOL  bFillList = FALSE;	// Флаг заполнения списка
int    IndexTypeChannel;

int    OldCodePC,					// Измененные значения для удаления ссылки из поля
		 OldModIO,					// Refrence в файлах БД
		 OldTransduc,
		 OldSensor;

RECORDNUMBER RecCountCHNL,	  		// Количество записей в файле каналов измерений
				 RecCountCONT,		  	// Количество записей в файле контроллеров
				 RecCountMDIO,      	// Количество записей в файле модулей ввода/вывода
				 RecCountTRNS,       // Количество записей в файле преобразователей
				 RecCountSENS;			// Количество записей в файле датчиков

/*
 *  -------------------------------------------------------------------------
 * 	Class TChannelIODialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TChannelIODialog, TDialog)
  EV_COMMAND		 (IDC_Help_CHNL,   			TChannelIOHelp),
  EV_COMMAND		 (IDCANCEL,  		 		 	EndSession),
  EV_COMMAND		 (IDOK,		  		  			EndSession),
  EV_CBN_SELCHANGE (IDC_NameChanIO_CHNL,		ListChannelIOName),
  EV_CBN_EDITCHANGE(IDC_NameChanIO_CHNL,		ChangeSelect),
  EV_CBN_SELCHANGE (IDC_MicroPC_CHNL, 	   	ListMicroPC),
  EV_COMMAND		 (IDC_ChanIO_A_CHNL,    	ChannelIOType),
  EV_COMMAND		 (IDC_ChanIO_D_CHNL,    	ChannelIOType),
  EV_COMMAND		 (IDC_ChanIO_C_CHNL,   	   ChannelIOType),
  EV_COMMAND		 (IDC_ChanIO_F_CHNL,    	ChannelIOType),
  EV_COMMAND		 (IDC_ChanIO_N_CHNL,    	ChannelIOType),

  EV_EN_CHANGE		 (IDC_CodeChanIO_CHNL, 	   ChangeSelect),
  EV_EN_CHANGE 	 (IDC_NumLogChan_CHNL, 		ChangeSelect),
  EV_EN_CHANGE 	 (IDC_ArrayIndex_CHNL, 		ChangeSelect),
  EV_CBN_SELCHANGE (IDC_TypeChanIO_CHNL, 		ChangeSelect),
  EV_CBN_SELCHANGE (IDC_MicroPC_CHNL, 			ChangeSelect),
  EV_CBN_SELCHANGE (IDC_ModuleIC_CHNL, 	 	ChangeSelect),
  EV_CBN_SELCHANGE (IDC_TransducIC_CHNL, 		ChangeSelect),
  EV_CBN_SELCHANGE (IDC_SensorIC_CHNL, 	 	ChangeSelect),

  EV_CBN_SELCHANGE (IDC_MicroPC_CHNL, 			ListChannelIOName),

  EV_COMMAND		 (IDC_Save_CHNL, 				SaveRecord),
  EV_COMMAND		 (IDC_Delete_CHNL,     		DeleteRecord),
  EV_COMMAND		 (IDCANCEL,	  		 	 	 	EndSession),
  EV_COMMAND		 (IDOK,			  		 	 	EndSession),
END_RESPONSE_TABLE;

//								C o n s t r u c t o r
TChannelIODialog::TChannelIODialog(TWindow* parent)
  : TDialog(parent, "ChannelIODIALOG"),	TWindow(parent)
{
  NameChanIO_CB	 = new TComboBox(this, IDC_NameChanIO_CHNL );// Имя преобразователя
  CodeChanIO_ED  	 = new TEdit	 (this, IDC_CodeChanIO_CHNL );// Марка преобразователя
																	// Аналоговый тип канала измерений
  ChanIO_A_RB		 = new TRadioButton(this, IDC_ChanIO_A_CHNL);
																	// Дискретный тип канала измерений
  ChanIO_D_RB		 = new TRadioButton(this, IDC_ChanIO_D_CHNL);
																	// Тип канала измерений - счетчик
  ChanIO_C_RB		 = new TRadioButton(this, IDC_ChanIO_C_CHNL);
																// Тип канала измерений - быстрый счетчик
  ChanIO_F_RB		 = new TRadioButton(this, IDC_ChanIO_F_CHNL);
																	// Нестандартный тип канала измерений
  ChanIO_N_RB		 = new TRadioButton(this, IDC_ChanIO_N_CHNL);

  TypeChanIO_CB  	 = new TComboBox(this, IDC_TypeChanIO_CHNL );// Тип канала измерений

  MicroPC_CB		 = new TComboBox(this, IDC_MicroPC_CHNL    );// Список контроллеров
  ModuleIC_CB		 = new TComboBox(this, IDC_ModuleIC_CHNL   );// Список модулей
  TransducIC_CB  	 = new TComboBox(this, IDC_TransducIC_CHNL );// Список преобразователей
  SensorIC_CB		 = new TComboBox(this, IDC_SensorIC_CHNL   );// Список датчиков

  Help_ICBT  		 = new TDrawICBT(this, IDC_Help_CHNL  	 	 );// Помощь
  Cancel_ICBT  	 = new TDrawICBT(this, IDCANCEL       	    );// Выход
  Save_BUT 			 = new TButton  (this, IDC_Save_CHNL	 	 );// Запись
  Delete_BUT  		 = new TButton  (this, IDC_Delete_CHNL	 	 );// Удаление

  Message_IB	    = new TBInfoBox(this, IDC_Message_CHNL  	 );// Статусная строка

  valid = new TFilterValidator("0-9");		  					// Установим фильтр ввода для цифр
	 if (valid){                                          // Номер логического канала передачи данных
		new TEdit(this, IDC_NumLogChan_CHNL)->SetValidator(valid);
	 };
  valid = new TFilterValidator("0-9");		  					// Установим фильтр ввода для цифр
	 if (valid){                                          // Индекс в массиве общей области данных
		new TEdit(this, IDC_ArrayIndex_CHNL)->SetValidator(valid);
	 };
};
/*
 * 	Нагружаем функцию SetupWindow() своими функциями :
 *		1. Открытие файлов данных
 *		2. Заполнение списка и окон
 */
void
TChannelIODialog::SetupWindow(){
  char MiscString[25];
  bFillList = TRUE;
  for (char i = 0; i < 25; i++) MiscString[i] = ' ';
  if (CodeController == 0)
		CodeController = 4;
  IndexTypeChannel = 1;

  TDialog::SetupWindow(); 				// Показываем диалог на экране
												// Открываем файл каналов
  CreateOpenChanIO(CodeController); // измерений для текущего ПК
  RecCountCHNL  = pT_CHNL->getRecCount();
  CreateOpenMContrls();             // Открываем файл контроллеров
  RecCountCONT  = pT_CONT->getRecCount();
												// Открываем файл модулей ввода/вывода для аналоговых модулей
  CreateOpenModuls(CodeController, "ANALMD");
  RecCountMDIO  = pT_MDIO->getRecCount();
  CreateOpenTransduc();
  RecCountTRNS  = pT_TRNS->getRecCount(); 	// Открываем файл преобразователей
  CreateOpenTSensors();
  RecCountSENS  = pT_SENS->getRecCount();  	// Открываем файл датчиков

  NameChanIO_CB->AddString("Новый");	  		// Добавим пункт "Новый" в список каналов изм.
												// для последующего выбора данного
												// пункта для создания нового
												// канала изм.

  for (RECORDNUMBER nNbr = 1; nNbr <= RecCountCHNL; nNbr++){
		pT_CHNL->go(nNbr);				// Читаем информацию из файла
		pT_CHNL->FromFileToDynRec();
		NameChanIO_CB->AddString(pcCHAR_CHNL("NameChanIO"));
  };
												// Маркируем RadioButton для аналоговых каналов изм.
  ChanIO_A_RB->SetCheck(BF_CHECKED);
  for (nNbr = 1; nNbr <= RecCountCONT; nNbr++){
		pT_CONT->go(nNbr);				// Читаем информацию из файла
		pT_CONT->FromFileToDynRec();
		MicroPC_CB->AddString(pcCHAR_CONT("TypePC"));
  };
  for (nNbr = 1; nNbr <= RecCountMDIO; nNbr++){
		pT_MDIO->go(nNbr);				// Читаем информацию из файла
		pT_MDIO->FromFileToDynRec();
		sprintf(MiscString, "%s[%d](%d)", pcCHAR_MDIO("TypeModule"),
													 vINTG_MDIO("NumPlace"),
													 vINTG_MDIO("NumChannel"));
		ModuleIC_CB->AddString(MiscString);
  };
  TransducIC_CB->AddString("Отсутствует");
  for (nNbr = 1; nNbr <= RecCountTRNS; nNbr++){
		pT_TRNS->go(nNbr);				// Читаем информацию из файла
		pT_TRNS->FromFileToDynRec();
		TransducIC_CB->AddString(pcCHAR_TRNS("CodeTransduc"));
  };
  for (nNbr = 1; nNbr <= RecCountSENS; nNbr++){
		pT_SENS->go(nNbr);				// Читаем информацию из файла датчиков
		pT_SENS->FromFileToDynRec();
		SensorIC_CB->AddString(pcCHAR_SENS("CodeSensor"));
  };
  TypeChanIO_CB->AddString("ввод");
  TypeChanIO_CB->AddString("вывод");
  TypeChanIO_CB->SetSelIndex(0);
  NameChanIO_CB->SetSelIndex(0);
  ListChannelIOName();
  bFillList = FALSE;
  bChangeParams = FALSE;	 			// Сбросим флаг измемения
};												// содержимого окон и списка
/*
 * 	Отображение списка ПК и соответствующих для него модулей
 */
void
TChannelIODialog::ListMicroPC(){
  char MiscString[25];
  for (char i = 0; i < 25; i++) MiscString[i] = ' ';

  bFillList 		= TRUE;
  OldCodePC		  	= CodeController;
  CodeController 	= MicroPC_CB->GetSelIndex() + 1;
													// Изменим текущий контроллер
  pT_CONT->go(CodeController);			// Читаем запись из файла ПК
  pT_CONT->FromFileToDynRec();
  CloseChanIO();                    	// Закроем старый и откроем новый
													// файл каналов ввода / вывода
  CreateOpenChanIO(CodeController); 	// измерений для текущего ПК
  RecCountCHNL  = pT_CHNL->getRecCount();
  NameChanIO_CB->ClearList();    	   // Очистим и отобразим список каналов изм.
  NameChanIO_CB->AddString("Новый");
  for (i = 1; i <= RecCountCHNL; i++){
		pT_CHNL->go(i);						// Читаем запись из файла ПК
		pT_CHNL->FromFileToDynRec();
		NameChanIO_CB->AddString(pcCHAR_CHNL("NameChanIO"));
  };
  CreateOpenModuleFile();					// Выбираем нужный тип модулей
													// и открываем соответствующий файл
  ModuleIC_CB->ClearList(); 	         // Очистим и отобразим список модулей
  for (i = 1; i <= RecCountMDIO; i++){
		pT_MDIO->go(i);				// Читаем информацию из файла
		pT_MDIO->FromFileToDynRec();
		sprintf(MiscString, "%s[%d](%d)", pcCHAR_MDIO("TypeModule"),
													 vINTG_MDIO("NumPlace"),
													 vINTG_MDIO("NumChannel"));
		ModuleIC_CB->AddString(MiscString);
  };
  ModuleIC_CB->SetSelIndex(0);			// Выбираем строку "Ввода" в типах кан. изм.

  NameChanIO_CB->SetSelIndex(0);				// Выбираем строку "Новый" в именах кан. изм.
  bFillList = FALSE;
};
/*
 *		Отрабатывает при нажатии RadioButton'ов по типу модулей:
 *		Аналоговый, Дискретный, Счетчик, Быстрый счетчик, Нестандартный
 */
void
TChannelIODialog::CreateOpenModuleFile(){
	CloseModuls();     				// Закроем старый файл модулей
												// Откроем новый файл модулей
	if (ChanIO_A_RB->GetCheck() == BF_CHECKED) {
			CreateOpenModuls(CodeController, "ANALMD");
	} else if (ChanIO_D_RB->GetCheck() == BF_CHECKED) {
			CreateOpenModuls(CodeController, "DSCRMD");
	} else if (ChanIO_C_RB->GetCheck () == BF_CHECKED) {
			CreateOpenModuls(CodeController, "COUNMD");
	} else if (ChanIO_F_RB->GetCheck() == BF_CHECKED) {
			CreateOpenModuls(CodeController, "FCOUMD");
	} else if (ChanIO_N_RB->GetCheck    () == BF_CHECKED) {
			CreateOpenModuls(CodeController, "USERMD");
	};                               // Кол-во записей в файле модулей
	RecCountMDIO   =  pT_MDIO->getRecCount();
//	TModuleList();						   // Отобразим список модулей
	bChangeParams = FALSE;				// Сбросим флаг измемения
};
/*
 *		Отрабатывает при нажатии RadioButton'ов по типу каналов измерений:
 *		Аналоговый, Дискретный, Счетчик, Быстрый счетчик, Нестандартный
 */
void
TChannelIODialog::ChannelIOType(){
  char MiscString[25];
  for (char i = 0; i < 25; i++) MiscString[i] = ' ';
  if (ChanIO_A_RB->GetCheck  () == BF_CHECKED) {
		  IndexTypeChannel = 1;
  } else if (ChanIO_D_RB->GetCheck () == BF_CHECKED) {
		  IndexTypeChannel = 2;
  } else if (ChanIO_C_RB->GetCheck () == BF_CHECKED) {
		  IndexTypeChannel = 3;
  } else if (ChanIO_F_RB->GetCheck() == BF_CHECKED) {
		  IndexTypeChannel = 4;
  } else if (ChanIO_N_RB->GetCheck    () == BF_CHECKED) {
		  IndexTypeChannel = 5;
  };
  CreateOpenModuleFile();			  	// Закроем старый файл модулей
												// Откроем новый файл модулей
  ModuleIC_CB->ClearList();         // Очистим и отобразим список модулей
  for (i = 1; i <= RecCountMDIO; i++){
		pT_MDIO->go(i);				// Читаем информацию из файла
		pT_MDIO->FromFileToDynRec();
		sprintf(MiscString, "%s[%d](%d)", pcCHAR_MDIO("TypeModule"),
													 vINTG_MDIO("NumPlace"),
													 vINTG_MDIO("NumChannel"));
		ModuleIC_CB->AddString(MiscString);
  };
  ModuleIC_CB->SetSelIndex(0);
  bChangeParams = TRUE;				 // Взводим флаг изменений
  if (!bFillList)
		Save_BUT->EnableWindow(bChangeParams);
};
/*
 * 	Отображение списка
 */
void
TChannelIODialog::ListChannelIOName(){
  bFillList = TRUE;
  SelectIndex_CHNL = NameChanIO_CB->GetSelIndex();
  if (SelectIndex_CHNL != 0){         	// Читаем запись из файла
		pT_CHNL->go(SelectIndex_CHNL);	// Читаем информацию из файла
		pT_CHNL->FromFileToDynRec();
		FromDynRecToDia();					// и далее - в диалог
  } else {                          	// По выбору пункта "Новый" данная конфигурация полей
	  SetDlgItemText(IDC_CodeChanIO_CHNL, 		"");
	  SetDlgItemText(IDC_NumLogChan_CHNL,     "0");
	  SetDlgItemText(IDC_ArrayIndex_CHNL, 	   "0");
	  ChanIO_D_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_C_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_F_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_N_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_A_RB-> SetCheck(BF_CHECKED);	// Выбран аналоговый тип сигнала
	  ChannelIOType();                  	//	?????
	  TypeChanIO_CB->SetSelIndex(0);
	  MicroPC_CB   ->SetSelIndex(CodeController - 1);
	  OldCodePC	   = 0;
	  ModuleIC_CB  ->SetSelIndex(-1);
	  TransducIC_CB->SetSelIndex(0);
	  SensorIC_CB  ->SetSelIndex(-1);
	  Delete_BUT	->EnableWindow(FALSE);
  };
  OldModIO		= ModuleIC_CB->GetSelIndex() + 1;
  OldTransduc	= TransducIC_CB->GetSelIndex();
  OldSensor		= SensorIC_CB->GetSelIndex() + 1;
  Save_BUT->EnableWindow(FALSE);
  bFillList		 = FALSE;
  bChangeParams = FALSE;				// Сбросим флаг изменений
};
/*
 * 	Функция взводит флаг как только было выбрано
 *		что-либо в списках диапазонов и размерностей
 */
void
TChannelIODialog::ChangeSelect(){
  if (!bFillList){
	  bChangeParams = TRUE;				 // Взводим флаг изменений
	  Save_BUT->EnableWindow(bChangeParams);
  };
};
/*
 *		Записываем или изменяем данные в файле
 */
void
TChannelIODialog::SaveRecord(){
  if (bChangeParams){                     // Получим новые данные :
	  GetDlgItemText(IDC_NameChanIO_CHNL, pCHAR_CHNL("NameChanIO"), 30);
	  switch(IndexTypeChannel) {
			case 1 :	strcpy(pCHAR_CHNL("TypeSensor"), "ANALOG"      	); break;
			case 2 : strcpy(pCHAR_CHNL("TypeSensor"), "DISKRETE"		); break;
			case 3 : strcpy(pCHAR_CHNL("TypeSensor"), "COUNTER"		);	break;
			case 4 : strcpy(pCHAR_CHNL("TypeSensor"), "FAST_COUNTER"	); break;
			case 5 : strcpy(pCHAR_CHNL("TypeSensor"), "CUSTOM"			); break;
	  };
	  switch(TypeChanIO_CB->GetSelIndex()) {
			case 0 : strcpy(pCHAR_CHNL("TypeChanIO"), "INPUT" ); break;
			case 1 : strcpy(pCHAR_CHNL("TypeChanIO"), "OUTPUT"); break;
	  };
	  GetDlgItemText(IDC_CodeChanIO_CHNL, pCHAR_CHNL("CodeChanIO"), 20);
	  vINTG_CHNL("NumLogChan") = GetDlgItemInt(IDC_NumLogChan_CHNL);
	  vINTG_CHNL("ArrayIndex") = GetDlgItemInt(IDC_ArrayIndex_CHNL);
	  vINTG_CHNL("ModuleIC")   = ModuleIC_CB->GetSelIndex() + 1;

	  vINTG_CHNL("TransducIC") = TransducIC_CB->GetSelIndex();
	  vINTG_CHNL("SensorIC")	= SensorIC_CB->GetSelIndex() + 1;

	  if (SelectIndex_CHNL == 0){ 			// Если создаем новое описание
														// Добавим элемент в список
		  NameChanIO_CB->AddString(pcCHAR_CHNL("NameChanIO"));
														// Номер заносимой в файл записи
		  vINTG_CHNL("IndexCode") = (short)((NameChanIO_CB->GetCount()) - 1);
														// Добавляем новый канал
		  pT_CHNL->clearRecordForWrite();	// ввода / вывода
		  pT_CHNL->FromDynRecToFile();
		  pT_CHNL->append();
		  SelectIndex_CHNL = vINTG_CHNL("IndexCode");
	  } else {                             // Заменяем старые данные
														// Номер заносимой в файл записи
		  vINTG_CHNL("IndexCode") = (short)(SelectIndex_CHNL);
														// Удалим элемент из ComboBox'а
		  NameChanIO_CB->DeleteString(SelectIndex_CHNL);
														// Вставим элемент в список выбранных
														// диапазонов
		  NameChanIO_CB->InsertString(pcCHAR_CHNL("NameChanIO"), SelectIndex_CHNL);
		  pT_CHNL->FromDynRecToFile();     	// Запишем данные в файл
		  pT_CHNL->update();
														// Читаем запись из файла и добавляем
		  if (OldCodePC != (MicroPC_CB->GetSelIndex() + 1)){
				pT_CONT->go(OldCodePC);		  	// Читаем информацию из файла
				pT_CONT->DecrRefrence();      // и уменьшаем кол-во
														// ссылок на старую запись
		  };
		  if (OldModIO !=  (ModuleIC_CB->GetSelIndex() + 1)){
				pT_MDIO->go(OldModIO);		  	// Читаем информацию из файла
				pT_MDIO->DecrRefrence();      // модулей ввода/вывода и уменьшаем
														// кол-во ссылок на старую запись
		  };
		  if ((OldTransduc != 0) && (OldTransduc != TransducIC_CB->GetSelIndex())){
				pT_TRNS->go(OldTransduc);	  // Читаем информацию из файла
				pT_TRNS->DecrRefrence();     // преобразователей и уменьшаем
													  // кол-во ссылок на старую запись
		  };
		  if (OldSensor !=  (SensorIC_CB->GetSelIndex() + 1)){
				pT_SENS->go(OldSensor);		  // Читаем информацию из файла
				pT_SENS->DecrRefrence();     // датчиков и уменьшаем
													  // кол-во ссылок на старую запись
		  };
	};
/*
 * 	Работа с полями Refrence в файлах БД
 */
//------------------ Контроллеры ------------------------------
	  if (OldCodePC != (MicroPC_CB->GetSelIndex() + 1)){
			pT_CONT->go(MicroPC_CB->GetSelIndex() + 1);	 // Читаем информацию из файла
			pT_CONT->IncrRefrence();
	  };
//------------------ Модули -----------------------------------
														// Читаем запись из файла и увеличиваем кол-во
	  if (OldModIO !=  (ModuleIC_CB->GetSelIndex() + 1)){
				pT_MDIO->go(ModuleIC_CB->GetSelIndex() + 1);
				pT_MDIO->IncrRefrence();
	  };
//------------------ Преобразователи --------------------------
														// Читаем запись из файла и увеличиваем кол-во
	  if ((TransducIC_CB->GetSelIndex() != 0) && OldTransduc != (TransducIC_CB->GetSelIndex())){
				pT_TRNS->go(TransducIC_CB->GetSelIndex());
				pT_TRNS->IncrRefrence();
	  };
//------------------ Датчики ----------------------------------
														// Читаем запись из файла
	  if (OldSensor !=  (SensorIC_CB->GetSelIndex() + 1)){
				pT_SENS->go(SensorIC_CB->GetSelIndex() + 1);
				pT_SENS->IncrRefrence();
	  };
//-------------------------------------------------------------
	  NameChanIO_CB->SetSelIndex(SelectIndex_CHNL); // Маркируем добавленный элемент
	  bChangeParams = FALSE;               // Сбросим флаг изменений
	  Save_BUT->EnableWindow(bChangeParams);
	  Delete_BUT->EnableWindow(!bChangeParams);
	  OldCodePC	   = MicroPC_CB   ->GetSelIndex() + 1;
	  OldModIO		= ModuleIC_CB  ->GetSelIndex() + 1;
	  OldTransduc	= TransducIC_CB->GetSelIndex();
	  OldSensor		= SensorIC_CB  ->GetSelIndex() + 1;
  };
};
/*
 * 	Удаление текущей записи из файла
 */
void
TChannelIODialog::DeleteRecord(){
  if (SelectIndex_CHNL != 0) {
		pT_CHNL->delCurRecord();
		NameChanIO_CB->DeleteString(SelectIndex_CHNL);// Удаление элемента из списка
		SelectIndex_CHNL--;
		NameChanIO_CB->SetSelIndex(SelectIndex_CHNL); // Маркируем предыдущий элемент
/*
 * 	Работа с полями Refrence в файлах БД
 */
//------------------ Контроллеры ------------------------------
		pT_CONT->go(MicroPC_CB->GetSelIndex() + 1); // Читаем информацию из файла
		pT_CONT->DecrRefrence();

//------------------ Модули -----------------------------------
													 // Читаем запись из файла и уменьшаем кол-во
	  if (ModuleIC_CB->GetSelIndex() != -1){
			pT_MDIO->go(ModuleIC_CB->GetSelIndex() + 1);
			pT_MDIO->DecrRefrence();
	  };
//------------------ Преобразователи --------------------------
	  if (TransducIC_CB->GetSelIndex() != 0){
													 // Читаем запись из файла
			pT_TRNS->go(TransducIC_CB->GetSelIndex());
			pT_TRNS->DecrRefrence();
	  };
//------------------ Датчики ----------------------------------

	  if (SensorIC_CB->GetSelIndex() != -1){
			pT_SENS->go(SensorIC_CB->GetSelIndex() + 1);
													 // Читаем информацию из файла
			pT_SENS->DecrRefrence();
	  };
//-------------------------------------------------------------
	  ListChannelIOName();						// Показываем информацию о
  };													// предыдущем элементе
};
/*
 *		Закрытие файла и выход
 */
void
TChannelIODialog::EndSession(){
  if (bChangeParams){
	  if (MessageBox("Данные не сохранены.\nСохранить перед выходом ?",
						  "Модули ввода/вывода",
						  MB_YESNO) == IDYES){
			SaveRecord();				 // Сохраним данные
	  };
  };
  CloseModuls();                // Закроем файл модулей
  CloseMContrls();                // Закроем файл ПК
  CloseTransduc();               	 // Закроем файл преобразователей
  CloseTSensors();          	    // Закроем файл датчиков
  CloseChanIO();               	 // Закроем файл канала измерений
  CloseWindow();
};

// 					Информация из динамической записи в диалог
void
TChannelIODialog::FromDynRecToDia   (){
	  SetDlgItemText(IDC_CodeChanIO_CHNL, pcCHAR_CHNL("CodeChanIO"));
	  SetDlgItemInt(IDC_NumLogChan_CHNL,	 vUINT_CHNL("NumLogChan" ));
	  SetDlgItemInt(IDC_ArrayIndex_CHNL,	 vUINT_CHNL("ArrayIndex" ));

												// Убираем маркеры с RadioButton'ов
	  ChanIO_A_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_D_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_C_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_F_RB-> SetCheck(BF_UNCHECKED);
	  ChanIO_N_RB-> SetCheck(BF_UNCHECKED);
												// Маркируем соответствующий RadioButton
				if (strcmp(pcCHAR_CHNL("TypeSensor"), "ANALOG"   ) == 0){
			ChanIO_A_RB->  SetCheck(BF_CHECKED);
	  } else if (strcmp(pcCHAR_CHNL("TypeSensor"), "DISKRETE"    ) == 0){
			ChanIO_D_RB->  SetCheck(BF_CHECKED);
	  } else if (strcmp(pcCHAR_CHNL("TypeSensor"), "COUNTER"     ) == 0){
			ChanIO_C_RB->  SetCheck(BF_CHECKED);
	  } else if (strcmp(pcCHAR_CHNL("TypeSensor"), "FAST_COUNTER") == 0){
			ChanIO_F_RB->  SetCheck(BF_CHECKED);
	  } else if (strcmp(pcCHAR_CHNL("TypeSensor"), "CUSTOM"      ) == 0){
			ChanIO_N_RB->  SetCheck(BF_CHECKED);
	  };
	  ChannelIOType();
												// Выбираем соответствующий тип
	  if (strcmp(pcCHAR_CHNL("TypeChanIO"), "INPUT") == 0){
		  TypeChanIO_CB->SetSelIndex(0);
	  } else {
		  TypeChanIO_CB->SetSelIndex(1);
	  };
	  MicroPC_CB	->SetSelIndex(CodeController - 1);
	  OldCodePC	   = MicroPC_CB->GetSelIndex() + 1;
	  ModuleIC_CB	->SetSelIndex(vUINT_CHNL("ModuleIC") - 1);
	  TransducIC_CB->SetSelIndex(vUINT_CHNL("TransducIC"));
	  SensorIC_CB  ->SetSelIndex(vUINT_CHNL("SensorIC") - 1);
	  Delete_BUT	->EnableWindow(TRUE);
};
/*
 * ---------------------------------------------------------
 *	EoF(tchannio.cpp)
 * ----------------------------------------------------------
 */

