/*
 * mcontrls - List type  -----------------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 *	mcontrls - модуль редактирования списка поддерживаемых
 * контроллеров с измемением, добавлением и удалением записи
 * Вся информация о ПК находиться в файле 'DBNET/MContrls.DB'
 * поддерживаемом пакетом PXEngine 3.0
 */
/*
 * 	Макросы для работы с Paradox Engine
 */
#define 	pVOID(X)	pT_CONT->fPoint(X)

#define	vDOUB(X)	*((double*)(pVOID(X)))
#define	pDOUB(X)	  (double*)(pVOID(X))
#define	vINTG(X)	*((INT16*)(pVOID(X)))
#define	pINTG(X)	  (INT16*)(pVOID(X))
#define  pCHAR(X)   (char*)(pVOID(X))
#define  pcCHAR(X)  (const char*)(pVOID(X))
#define  iLEN(X)	  (int)(pT_CONT->fLen(X))
/*
 * 	Включение заголовочных файлов
 */
#include <owl\edit.h>
#include "mcontrls.h"
#include "commglob.h"					// Global variable
/*
 * 		Переменные и константы
 */
int 		SelectIndex      = 0;		// Номер выбранного элемента списка
BOOL  	bChangeParams    = FALSE;	// Флаг изменения в окнах ввода
static 	BOOL  bFillList  = FALSE;	// Флаг заполнения списка
/*
 *  -------------------------------------------------------------------------
 * 	Class TMContrlsDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TMContrlsDialog, TDialog)
  EV_WM_PAINT,

  EV_CBN_SELCHANGE (IDC_TypePC_CONT, 		     	ListParamsPC),
  EV_CBN_EDITCHANGE(IDC_TypePC_CONT,          	ChangeText	),
  
  EV_EN_CHANGE		 (IDC_Firma_CONT,           	ChangeText	),
  EV_EN_CHANGE		 (IDC_MainProcessor_CONT,   	ChangeText	),
  EV_EN_CHANGE		 (IDC_NetProcessor_CONT,    	ChangeText	),
  EV_EN_CHANGE		 (IDC_Interface1_CONT, 			ChangeText	),
  EV_EN_CHANGE		 (IDC_Interface2_CONT, 			ChangeText	),
  EV_EN_CHANGE		 (IDC_TypeProtocol_CONT,   	ChangeText	),

  EV_COMMAND		 (IDC_Help_CONT,       			TMContrlsHelp),
  EV_COMMAND		 (IDCANCEL,    	     			EndSession	),
  EV_COMMAND		 (IDOK,			  		 	 		EndSession),
  EV_COMMAND		 (IDC_Save_CONT,  	  			SaveRecord	),
  EV_COMMAND		 (IDC_Delete_CONT,  	  	  		DeleteRecord),
END_RESPONSE_TABLE;

//								C o n s t r u c t o r
TMContrlsDialog::TMContrlsDialog(TWindow* parent)
  : TDialog(parent, "MContrlsDIALOG"),	TWindow(parent)	{

  TypePC_CB      	= new TComboBox(this, IDC_TypePC_CONT			);
  Firma_ED	 	 	= new TEdit	 	(this, IDC_Firma_CONT       	);
  MainProcessor_ED= new TEdit	 	(this, IDC_MainProcessor_CONT	);
  NetProcessor_ED	= new TEdit	 	(this, IDC_NetProcessor_CONT 	);
  Interface1_ED  	= new TEdit	 	(this, IDC_Interface1_CONT		);
  Interface2_ED  	= new TEdit	 	(this, IDC_Interface2_CONT		);
  TypeProtocol_ED	= new TEdit	 	(this, IDC_TypeProtocol_CONT	);

  Help_ICBT 		= new TDrawICBT(this, IDC_Help_CONT			);// Помощь
  Cancel_ICBT 		= new TDrawICBT(this, IDCANCEL				);// Выход
  Save_BUT			= new TButton  (this, IDC_Save_CONT			);// Запись

  Delete_BUT 		= new TButton  (this, IDC_Delete_CONT		);// Удаление
  Message_IB		= new TBInfoBox(this, IDC_Message_CONT		);// Статусная строка

  Picture  			= new TBitmap	(*GetModule(), IDC_MICROPC	);
};
void
TMContrlsDialog::EvPaint(){
	TWindow::EvPaint();
};
void
TMContrlsDialog::Paint(TDC& dc, BOOL, TRect&)
{
	  DrawBitMap(dc, *Picture, 300, 5); // Собственно отображение картинки
};
/*
 * 	Нагружаем функцию SetupWindow() своими функциями :
 *		1. Открытие файла данных
 *		2. Заполнение списка и окон
 */
void
TMContrlsDialog::SetupWindow(){
  RECORDNUMBER RecCount;				// Количество записей в файле
  CreateOpenMContrls();   				// Открываем файл (или создаем)
  TDialog::SetupWindow();				// Показываем диалог на экране
  TypePC_CB->AddString("Новый");		// Добавим пункт "Новый" в список
												// для последующего выбора данного
												// пункта для создания нового
												// описания РС
												// Добавим остальные РС из файла
												// если они там, конечно, есть
  if ((RecCount = pT_CONT->getRecCount()) > 0)	// Количество записей в нем
		for (int i = 1; i <= RecCount; i++){
												// Читаем из файла типы контроллеров
			pT_CONT->go(i);
			pT_CONT->getFieldFromCurrentRec("TypePC", pCHAR("TypePC"));
			TypePC_CB->AddString(pcCHAR("TypePC"));
		};
  TypePC_CB->SetSelIndex(SelectIndex);	// Установим указатель на выбранный
												// элемент или в начало списка
  ListParamsPC();							// Считаем всю запись выбранного
												// элемента
};
/*
 * 	Отображение информации по выбранному РС
 */
void
TMContrlsDialog::ListParamsPC(){
  SelectIndex = TypePC_CB->GetSelIndex();	// Определяем номер выбранного
														// элемента списка
  bFillList = TRUE;
  if (SelectIndex != 0){
	  pT_CONT->go(SelectIndex);
	  pT_CONT->FromFileToDynRec();	 // Читаем данные из файла
												 // и запишем их в окна
	  SetDlgItemText	(IDC_Firma_CONT, 				pcCHAR("Firma"));
	  SetDlgItemText	(IDC_MainProcessor_CONT, 	pcCHAR("MainProcessor"));
	  SetDlgItemText	(IDC_NetProcessor_CONT, 	pcCHAR("NetProcessor"));
	  SetDlgItemText	(IDC_Interface1_CONT, 		pcCHAR("Interface1"));
	  SetDlgItemText	(IDC_Interface2_CONT,		pcCHAR("Interface2"));
	  SetDlgItemText	(IDC_TypeProtocol_CONT,   pcCHAR("TypeProtocol"));
	  Delete_BUT->EnableWindow(TRUE); // Разрешим удалить запись
  }
  else {									 	// При выборе пункта 'Новый'
	  SetDlgItemText(IDC_Firma_CONT,	     	  "");
	  SetDlgItemText(IDC_MainProcessor_CONT, "");
	  SetDlgItemText(IDC_NetProcessor_CONT,  "");
	  SetDlgItemText(IDC_Interface1_CONT, 	  "");
	  SetDlgItemText(IDC_Interface2_CONT, 	  "");
	  SetDlgItemText(IDC_TypeProtocol_CONT,  "");
	  Delete_BUT->EnableWindow(FALSE);  // Пустую запись не удаляем
  };
  Save_BUT->EnableWindow(FALSE);			// Как вошли в бокс - можно запоминать
  CodeController = SelectIndex;        // Запомним индекс выбранного PC
  bFillList      = FALSE;         		// Сбросим флаг изменения
  bChangeParams  = FALSE;         		// содержимого окон и списка
};
/*
 * 	Следим за изменением содержимого окон и списка
 */
void
TMContrlsDialog::ChangeText(){
	if (!bFillList){
		Message_IB->ClearShadeBox();
		bChangeParams = TRUE;				// Взводим флаг изменений
													// Записывать можно, но нельзя удалять
		Save_BUT->EnableWindow(bChangeParams);
		Delete_BUT->EnableWindow(!bChangeParams);
	};
};
/*
 *		Записываем или изменяем данные в файле
 */
void
TMContrlsDialog::SaveRecord(){
  if (bChangeParams){                  // Получим новые данные из окна
													// диалога в динамический буфер
													// записи файла
	  GetDlgItemText	(IDC_TypePC_CONT,			 pCHAR("TypePC"),
							 iLEN("TypePC"));
	  GetDlgItemText	(IDC_Firma_CONT,  		 pCHAR("Firma"),
							 iLEN("Firma"));
	  GetDlgItemText	(IDC_MainProcessor_CONT, pCHAR("MainProcessor"),
							 iLEN("MainProcessor"));
	  GetDlgItemText	(IDC_NetProcessor_CONT,  pCHAR("NetProcessor"),
							 iLEN("NetProcessor"));
	  GetDlgItemText	(IDC_Interface1_CONT,    pCHAR("Interface1"),
							 iLEN("Interface1"));
	  GetDlgItemText	(IDC_Interface1_CONT,    pCHAR("Interface2"),
							 iLEN("Interface2"));
	  GetDlgItemText	(IDC_TypeProtocol_CONT,  pCHAR("TypeProtocol"),
							 iLEN("TypeProtocol"));

	  if (SelectIndex == 0){ 					// Если создаем новое описание
			TypePC_CB->AddString(pcCHAR("TypePC"));	// Добавим элемент в список
														// Номер заносимой в файл записи
			vINTG("IndexCode") = (INT16)((TypePC_CB->GetCount()) - 1);
			pT_CONT->clearRecordForWrite();
			pT_CONT->FromDynRecToFile();
			pT_CONT->append();               // Запись данных - добавление
			SelectIndex = TypePC_CB->GetCount() - 1;
	  } else {                             // Заменяем старые данные
														// Номер заносимой в файл записи
			vINTG("IndexCode") = (INT16)(SelectIndex);
														// Удалим элемент из ComboBox'а
			TypePC_CB->DeleteString(SelectIndex);
														// Вставим элемент в список
			TypePC_CB->InsertString(pcCHAR("TypePC"), SelectIndex);
			pT_CONT->FromDynRecToFile();     // Вставим данные в файл - замещение
			pT_CONT->update();
	  };
	  TypePC_CB->SetSelIndex(SelectIndex); // Маркируем добавленный элемент
	  bChangeParams = FALSE;               // Сбросим флаг изменений
	  Save_BUT->EnableWindow(bChangeParams);
	  Delete_BUT->EnableWindow(!bChangeParams);
  };
};
/*
 * 	Удаление текущей записи из файла
 */
void
TMContrlsDialog::DeleteRecord(){
  if (SelectIndex != 0 && vINTG("Refrence") == 0){
		pT_CONT->delCurRecord();	         // Удаление записи из файла
		TypePC_CB->DeleteString(SelectIndex); 	// Удаление элемента из списка
		TypePC_CB->SetSelIndex(--SelectIndex); // Маркируем предыдущий элемент
		ListParamsPC();							// Показываем информацию о
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
TMContrlsDialog::EndSession(){
  if (bChangeParams){  				// Если даные изменены
	  if (MessageBox("Данные не сохранены.\nСохранить перед выходом ?",
						  "Программируемые контроллеры",
						  MB_YESNO) == IDYES){
			SaveRecord();				// Сохраним данные
	  };
  };
  CloseMContrls();
  CodeController = TypePC_CB->GetSelIndex();
  CloseWindow(IDCANCEL);
};

/*
 *    Обновление записи с изменениями
 */
BOOL UpdateTMContrls(){
	pT_CONT->FromDynRecToFile();
	pT_CONT->update();
	return TRUE;
};
/*
 * ---------------------------------------------------------
 *	EoF(mcontrls.cpp)
 * ----------------------------------------------------------
 */

