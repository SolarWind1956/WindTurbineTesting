/*
 * transduc -  ------------------------
 *
 * Author: Konstantin A. Davidov & N. Megenina
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 *	transduc - модуль редактирования списка поддерживаемых
 * преобразователей с измемением, добавлением и удалением записи
 * Вся информация находиться в файле 'DBNET/TTRANSDC.DB'
 * поддерживаемом пакетом PXEngine 3.0
 */
/*
 * 	Макросы для работы с Paradox Engine
 */
#define 	pVOID(X)	pT_TRNS->fPoint(X)

#define	vDOUB(X)	*((double*)(pVOID(X)))
#define	pDOUB(X)	  (double*)(pVOID(X))
#define	vUINT(X)	*((UINT*)(pVOID(X)))
#define	vINTG(X)	*((INT16*)(pVOID(X)))
#define	pINTG(X)	  (INT16*)(pVOID(X))
#define  pCHAR(X)   (char*)(pVOID(X))
#define  pcCHAR(X)  (const char*)(pVOID(X))
#define  iLEN(X)	  (int)(pT_TRNS->fLen(X))

/*
 * 	Включение заголовочных файлов
 */
#include <owl\validate.h>
#include <math.h>
#include <stdio.h>

#include "testsyst.rh"
#include "commfunc.h"
#include "database.h"
#include "transduc.h"
//----------------------------------------------------------------------------

/*
 * 		Глобальные переменные - поля файла DB
 */
											// Массив размерностей выходного сигнала
static const char aOutDimens [][4] =	{"V", "mA"};
static const int  iCountOutDimens = sizeof(aOutDimens) / sizeof(aOutDimens[0]);
											// Массив типов аппроксимации
static const char aTypesApprox[][30] = {
										"Линейная",
										"Кусочно-линейная",
										"Форма Лагранжа",
										"Форма Ньютона",
										"Метод Гаусса",
										"Метод наименьших квадратов",
										"Приближения Чебышева"
										  };
static const int  iCountTypesApprox = sizeof(aTypesApprox) / sizeof(aTypesApprox[0]);


int 	 SelectIndex_TT 	 	= 0;       // Номер выбранного элемента списка
BOOL   bChangeParamsTrans 	= FALSE;   // Флаг изменения в окнах ввода
static BOOL  bFillList 	 	= FALSE;
/*
 *  -------------------------------------------------------------------------
 * 	Class TTransducDialog
 *  -------------------------------------------------------------------------
  */
DEFINE_RESPONSE_TABLE1(TTransducDialog, TDialog)
  EV_WM_CLOSE,
  EV_CBN_SELCHANGE (IDC_NameTransduc_TRNS, 	TRDCNameList),
  EV_CBN_EDITCHANGE(IDC_NameTransduc_TRNS, 	ChangeSelectText),
  EV_EN_CHANGE		 (IDC_CodeTransduc_TRNS, 	ChangeSelectText),
  EV_EN_CHANGE 	 (IDC_LoBoundIn_TRNS, 	 	ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_HiBoundIn_TRNS, 	 	ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_LoBoundOut_TRNS, 		ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_HiBoundOut_TRNS, 		ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_DimensIn_TRNS,	 		ChangeSelectValues),
  EV_CBN_SELCHANGE (IDC_DimensOut_TRNS,	 	ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_Tolerance_TRNS, 		ChangeSelectText),
  EV_EN_CHANGE		 (IDC_GradPointX1_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointX2_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointX3_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointX4_TRNS, 	ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointX5_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointX6_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointY1_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointY2_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointY3_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointY4_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointY5_TRNS,		ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointY6_TRNS,		ChangeSelectValues),

  EV_EN_KILLFOCUS	 (IDC_DimensIn_TRNS,  		ChangeDimension),
  EV_CBN_SELENDOK  (IDC_DimensOut_TRNS, 		ChangeDimension),
  EV_CBN_SELCHANGE (IDC_TypeApprox_TRNS,	   ChangeTypeApprox),
  EV_COMMAND		 (IDC_Save_TRNS, 	 			SaveRecord),
  EV_COMMAND		 (IDC_Delete_TRNS,  	   	DeleteRecord),
  EV_COMMAND		 (IDC_Help_TRNS, 	 			Help),
  EV_COMMAND		 (IDCANCEL,  		 	 		EndSession),
  EV_COMMAND		 (IDOK,		  		 	 	 	EndSession),
END_RESPONSE_TABLE;

//								C o n s t r u c t o r
TTransducDialog::TTransducDialog(TWindow* parent)
  : TDialog(parent, "TransducDIALOG"),	TWindow(parent)  {
  NameTRDC_CB 			= new TComboBox   (this, IDC_NameTransduc_TRNS);// Имя преобразователя
  CodeTRDC_ED 			= new TEdit	      (this, IDC_CodeTransduc_TRNS);// Марка преобразователя
  DimensOut_CB 		= new TComboBox   (this, IDC_DimensOut_TRNS);	// Размерности выходного сигнала
  TypeApprox_CB		= new TComboBox   (this, IDC_TypeApprox_TRNS);	// Тип аппроксимации

  GradPointX_ED[0] 	= new TEdit	 		(this, IDC_GradPointX1_TRNS);
  GradPointX_ED[1] 	= new TEdit	 		(this, IDC_GradPointX2_TRNS);
  GradPointX_ED[2] 	= new TEdit	 		(this, IDC_GradPointX3_TRNS);
  GradPointX_ED[3] 	= new TEdit	 		(this, IDC_GradPointX4_TRNS);
  GradPointX_ED[4] 	= new TEdit	 		(this, IDC_GradPointX5_TRNS);
  GradPointX_ED[5] 	= new TEdit	 		(this, IDC_GradPointX6_TRNS);

  GradPointY_ED[0] 	= new TEdit	 		(this, IDC_GradPointY1_TRNS);
  GradPointY_ED[1] 	= new TEdit	 		(this, IDC_GradPointY2_TRNS);
  GradPointY_ED[2] 	= new TEdit	 		(this, IDC_GradPointY3_TRNS);
  GradPointY_ED[3] 	= new TEdit	 		(this, IDC_GradPointY4_TRNS);
  GradPointY_ED[4] 	= new TEdit	 		(this, IDC_GradPointY5_TRNS);
  GradPointY_ED[5] 	= new TEdit	 		(this, IDC_GradPointY6_TRNS);

  ApproxA_ED [0] 		= new TEdit	 		(this, IDC_ApproxA0_TRNS);
  ApproxA_ED [1] 		= new TEdit	 		(this, IDC_ApproxA1_TRNS);
  ApproxA_ED [2] 		= new TEdit	 		(this, IDC_ApproxA2_TRNS);
  ApproxA_ED [3] 		= new TEdit	 		(this, IDC_ApproxA3_TRNS);
  ApproxA_ED [4] 		= new TEdit	 		(this, IDC_ApproxA4_TRNS);
  ApproxA_ED [5] 		= new TEdit	 		(this, IDC_ApproxA5_TRNS);
																					 // График
  Graduir_GB	  		= new TGraduirCharact(this, IDC_Graduir_TRNS, FALSE);

  Help_ICBT   			= new TDrawICBT   (this, IDC_Help_TRNS	  );// Помощь
  Cancel_ICBT 			= new TDrawICBT   (this, IDCANCEL        );// Выход
  Save_BUT  			= new TButton  	(this, IDC_Save_TRNS	  );// Запись
  Delete_BUT  			= new TButton  	(this, IDC_Delete_TRNS );// Удаление

  Message_IB  			= new TBInfoBox	(this, IDC_Message_TRNS);// Статусная строка

// ----------------- Установим фильтры ввода для цифр -----------
  valid = new TFilterValidator("0-9.-");
  if (valid){
		new TEdit(this, IDC_LoBoundIn_TRNS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_HiBoundIn_TRNS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_LoBoundOut_TRNS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_HiBoundOut_TRNS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_Tolerance_TRNS)->SetValidator(valid);
  };
  for (char i = 0; i < 6; i++){
	 valid = new TFilterValidator("0-9.-");
	 if (valid)
		GradPointX_ED[i]->SetValidator(valid);
  };
  for (i = 0; i < 6; i++){
	 valid = new TFilterValidator("0-9.-");
	 if (valid)
		GradPointY_ED[i]->SetValidator(valid);
  };
};
/*
 * 	Нагружаем функцию SetupWindow() своими функциями :
 *		1. Открытие файла данных
 *		2. Заполнение списка и окон
 */
void
TTransducDialog::SetupWindow(){
  Graduir_GB->bShowCharact = FALSE;
  for (char i = 0; i < QUANTVALUES; i++){
		Graduir_GB->LastValuesX[i] = 0.0;
		Graduir_GB->LastValuesY[i] = 0.0;
  };
  bFillList = TRUE;						// Открываем файл	с выбранными аналоговыми
												// каналами для текущего ПК
  TDialog::SetupWindow(); 				// Показываем диалог на экране
  CreateOpenTransduc();					// Создаем новый или открываем
												// существующий файл
  RecCount = pT_TRNS->getRecCount();// Сколько в нем записей
  NameTRDC_CB->AddString("Новый");	// Добавим пункт "Новый" в список
												// для последующего выбора данного
												// пункта для создания нового
												// описания
  for (RECORDNUMBER nNbr = 1; nNbr <= RecCount; nNbr++){
		pT_TRNS->go(nNbr);				// Читаем информацию из файла
		pT_TRNS->FromFileToDynRec();
		NameTRDC_CB->AddString(pcCHAR("NameTransduc"));
  };
  for (i = 0; i < iCountOutDimens; i++){
		DimensOut_CB->AddString(aOutDimens[i]);
  };
  for (i = 0; i < iCountTypesApprox; i++){
		TypeApprox_CB->AddString(aTypesApprox[i]);
  };
  NameTRDC_CB->SetSelIndex(0);
  TRDCNameList();
  bFillList = FALSE;                // Сбросим флаг измемения
  bChangeParamsTrans = FALSE;       // содержимого окон и списка
};
/*
 * 	Отображение списка аналоговых диапазонов по выбранному ПК
 */
void
TTransducDialog::TRDCNameList(){
  bFillList = TRUE;
  SelectIndex_TT = NameTRDC_CB->GetSelIndex();
  if (SelectIndex_TT != 0){
	  pT_TRNS->go(SelectIndex_TT);		// Читаем информацию из файла
	  pT_TRNS->FromFileToDynRec();    	// в динамическую запись
	  FromDynRecToDia();						// и далее - в диалог
	  DrawGraduirCharact();					// Отображение графика
	  Delete_BUT->EnableWindow(TRUE);   // Теперь ее можно и убить
  } else {
	  SetDlgItemText(IDC_CodeTransduc_TRNS, 		"");
	  SetDlgItemText(IDC_LoBoundIn_TRNS, 	"0");
	  SetDlgItemText(IDC_HiBoundIn_TRNS, 	"0");
	  SetDlgItemText(IDC_DimensIn_TRNS, 	"");
	  SetDlgItemText(IDC_LoBoundOut_TRNS, "0");
	  SetDlgItemText(IDC_HiBoundOut_TRNS, "0");
	  DimensOut_CB->SetSelIndex(0);
	  TypeApprox_CB->SetSelIndex(0);
	  SetDlgItemText(IDC_Tolerance_TRNS, 	"0");
	  SetDlgItemText(IDC_DupDimensIn_TRNS, 		"");
													// Показываем размерность еще
													// в одном месте
	  GetDlgItemText(IDC_DimensOut_TRNS, pCHAR("DimensOut"), iLEN("DimensOut"));
	  SetDlgItemText(IDC_DupDimensOut_TRNS,   pcCHAR("DimensOut"));
	  // ??????
	  for (char i = 0; i < 6; i++){
		  GradPointX_ED[i]->DeleteLine(0);
		  GradPointX_ED[i]->Insert("0");
		  GradPointY_ED[i]->DeleteLine(0);
		  GradPointY_ED[i]->Insert("0");
		  ApproxA_ED   [i]->DeleteLine(0);
		  ApproxA_ED   [i]->Insert("0");
	  };
	  Graduir_GB->ClearGraphicWindow();		// Убъем график
	  Delete_BUT->EnableWindow(FALSE);
  };
  Save_BUT->EnableWindow(FALSE);			// УЖЕ Показанную в диалоге информацию
													// можно запомнить в файле
  bChangeParamsTrans = FALSE;		  		// Сбросим флаг изменений
  bFillList = FALSE;
};
/*
 * 	Функция отображает размерность в таблице по уничтожению
 *		фокуса на окнах ввода размерностей
 */
void
TTransducDialog::ChangeDimension(){
													// Возьмем строку размерности входного сигнала
													// из динам. массива
	  GetDlgItemText(IDC_DimensIn_TRNS, 	pCHAR("DimensIn"),  iLEN("DimensIn"));
													// и вставим ее в окно размерности входного сигнала
	  SetDlgItemText(IDC_DupDimensIn_TRNS,     	pcCHAR("DimensIn"));
													// Возьмем строку размерности выходного сигнала
													// из динам. массива
	  GetDlgItemText(IDC_DimensOut_TRNS,	pCHAR("DimensOut"), iLEN("DimensOut"));
													// и вставим ее в окно размерности выходного сигнала
	  SetDlgItemText(IDC_DupDimensOut_TRNS,		pcCHAR("DimensOut"));
};

/*
 * 	Функция запускает выбранный тип аппроксимации данных
 */
void
TTransducDialog::ChangeTypeApprox(){
  if (!bFillList){
	  bChangeParamsTrans = TRUE;		  	// Взводим флаг изменений
													// Кнопка "Сохранить" АКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsTrans);
													// Кнопка "Удалить" НЕАКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsTrans);
	  DrawGraduirCharact();					// Отображение графика
  };
};

/*
 * 	Функция взводит флаг как только было выбрано
 *		что-либо в списках диапазонов и размерностей
 */
void
TTransducDialog::ChangeSelectText(){
  if (!bFillList){
	  bChangeParamsTrans = TRUE;			// Взводим флаг изменений
													// Кнопка "Сохранить" АКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsTrans);
													// Кнопка "Удалить" НЕАКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsTrans);
  };
};

/*
 * 	Функция взводит флаг как только было что-либо изменено или выбрано
 *		в списках диапазонов, размерностей и других окнах параметров
 *		с обновлением градуировочной гривой
 */
void
TTransducDialog::ChangeSelectValues(){
  if (!bFillList){
	  bChangeParamsTrans = TRUE;		 	// Взводим флаг изменений
													// Кнопка "Сохранить" АКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsTrans);
													// Кнопка "Удалить" НЕАКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsTrans);
	  DrawGraduirCharact();
  };
};

/*
 *		Записываем или изменяем данные в файле
 */
void
TTransducDialog::SaveRecord(){
  char MiscString[10]; memset(MiscString, ' ', 10);

  if (bChangeParamsTrans){                     // Получим новые данные :
														// Имя преобразователя
	  FromDlgToDynRec();
	  if (SelectIndex_TT == 0){ 				// Если создаем новое описание
														// Добавим элемент в список
		  NameTRDC_CB->AddString(pCHAR("NameTransduc"));
														// Номер заносимой в файл записи
		  vINTG("IndexCode") = (short)((NameTRDC_CB->GetCount()) - 1);
		  pT_TRNS->FromDynRecToFile();
		  pT_TRNS->append();                // Запись данных - добавление
		  SelectIndex_TT = vINTG("IndexCode");
	  } else {                             // Заменяем старые данные
														// Номер заносимой в файл записи
		  vINTG("IndexCode") = (short)(SelectIndex_TT);
														// Удалим элемент из ComboBox'а
		  NameTRDC_CB->DeleteString(SelectIndex_TT);
														// Вставим элемент в список выбранных
														// диапазонов
		  NameTRDC_CB->InsertString(pCHAR("NameTransduc"), SelectIndex_TT);
		  pT_TRNS->FromDynRecToFile();
		  UpdateTTransduc();						// Запись для БД MContrls данных - замещение
	  };
	  NameTRDC_CB->SetSelIndex(SelectIndex_TT); // Маркируем добавленный элемент
	  bChangeParamsTrans = FALSE;          // Сбросим флаг изменений
														// Кнопка "Сохранить" НЕАКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsTrans);
														// Кнопка "Удалить" АКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsTrans);
  };
};
/*
 * 	Удаление текущей записи из файла
 */
void
TTransducDialog::DeleteRecord(){
  Message_IB->ClearShadeBox();			// Удалить конечно можно, но при условии
													// что на эту запись нет ссылки из других
  if (vINTG("Refrence") == 0) {			// файлов
	  pT_TRNS->delCurRecord();	         // Удаление записи из файла
													// Удаление элемента из списка
	  NameTRDC_CB->DeleteString(SelectIndex_TT);
	  SelectIndex_TT--;                 // Маркируем предыдущий элемент
	  NameTRDC_CB->SetSelIndex(SelectIndex_TT);
	  TRDCNameList();							// Показываем информацию о
  } else {										// предыдущем элементе
													// Сообщение при существующей ссылке
													// на данную запись
	  Message_IB->StringToShadeBox(2, 2, RGB(255, 0, 0),
					  "Нельзя удалить, на него существует ссылка");
  };
};

/*
 *		Закрытие файла и выход
 */
void
TTransducDialog::EvClose(){
	EndSession();
};
void
TTransducDialog::EndSession(){
  if (bChangeParamsTrans){
	  if (MessageBox("Данные не сохранены.\nСохранить перед выходом ?",
						  "Преобразователи",
						  MB_YESNO) == IDYES){
			SaveRecord();				 // Сохраним данные
	  };
  };
  Graduir_GB->ClearGraphicWindow();// Очистим окно графика
  CloseTransduc();              	 // Закроем файл преобразователей
  CloseWindow();
};
BOOL
AppendToTTransduc(){
	pT_TRNS->clearRecordForWrite();
	pT_TRNS->FromDynRecToFile();
	pT_TRNS->append();
	return TRUE;
};
BOOL UpdateTTransduc(){
	pT_TRNS->FromDynRecToFile();
	pT_TRNS->update();
	return TRUE;
};

/*
 *  	Информация из динамической записи в диалог
 *
 */
void
TTransducDialog::FromDynRecToDia   (){
  char MiscString[10]; memset(MiscString, ' ', 10);
													// Марка преобразователя
  SetDlgItemText(IDC_CodeTransduc_TRNS, 				 pcCHAR("CodeTransduc"));
													// Нижняя граница изм. сигнала
  SetDlgItemDoub(this->HWindow, IDC_LoBoundIn_TRNS, vDOUB ("LoBoundIn"));
													// Верхняя граница изм. сигнала
  SetDlgItemDoub(this->HWindow, IDC_HiBoundIn_TRNS, vDOUB ("HiBoundIn"));
													// Размерность изм. сингала
  SetDlgItemText(IDC_DimensIn_TRNS, 					 pcCHAR("DimensIn"));
													// Размерность изм. сингала в градуир. таблицу
  SetDlgItemText(IDC_DupDimensIn_TRNS,						 pcCHAR("DimensIn"));
													// Нижняя граница вых. сингала
  SetDlgItemDoub(this->HWindow, IDC_LoBoundOut_TRNS, vDOUB ("LoBoundOut"));
													// Верхняя граница вых. сингала
  SetDlgItemDoub(this->HWindow, IDC_HiBoundOut_TRNS, vDOUB ("HiBoundOut"));
													// Размерность вых. сингала
  DimensOut_CB->SetSelIndex(DimensOut_CB->FindString(pcCHAR("DimensOut"), 0) );
													// Размерность вых. сингала в градуир. таблицу
  SetDlgItemText(IDC_DupDimensOut_TRNS,						  pcCHAR("DimensOut"));
													// Тип аппроксимации
  TypeApprox_CB->SetSelIndex(TypeApprox_CB->FindString(pcCHAR("TypeApprox"), 0) );
													// Класс точности
  SetDlgItemDoub(this->HWindow, IDC_Tolerance_TRNS, vDOUB("Tolerance"));
													// Градуировочные точки по 'X'
  fnbrB =  pT_TRNS->getFieldNumber("GradPointX1");
  fnbrE =  pT_TRNS->getFieldNumber("GradPointX6");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 gcvt(vDOUB(fnbr), 5, MiscString);
		 GradPointY_ED[fnbr - fnbrB]->DeleteLine(0);
		 GradPointY_ED[fnbr - fnbrB]->Insert(MiscString);
  };												// Градуировочные точки по 'Y'
  fnbrB =  pT_TRNS->getFieldNumber("GradPointY1");
  fnbrE =  pT_TRNS->getFieldNumber("GradPointY6");
  for ( fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		  gcvt(vDOUB(fnbr), 5, MiscString);
		  GradPointX_ED[fnbr - fnbrB]->DeleteLine(0);
		  GradPointX_ED[fnbr - fnbrB]->Insert(MiscString);
  };												// Аппроксимационгные коэффициенты
  fnbrB =  pT_TRNS->getFieldNumber("ApproxA0");
  fnbrE =  pT_TRNS->getFieldNumber("ApproxA5");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 gcvt(vDOUB(fnbr), 5, MiscString);
		 ApproxA_ED[fnbr - fnbrB]->DeleteLine(0);
		 ApproxA_ED[fnbr - fnbrB]->Insert(MiscString);
  };
};

/*
 *		Информация из диалога в динамическую запись
 */
void
TTransducDialog::FromDlgToDynRec(){
  char MiscString[10]; memset(MiscString, ' ', 10);
													// Имя преобразователя
  GetDlgItemText(IDC_NameTransduc_TRNS,	pCHAR("NameTransduc"), iLEN("NameTransduc"));
													// Марка преобразователя
  GetDlgItemText(IDC_CodeTransduc_TRNS,	pCHAR("CodeTransduc"), iLEN("CodeTransduc"));
													// Нижняя граница изм. сигнала
  vDOUB("LoBoundIn")  = GetDlgItemDoub(this->HWindow, IDC_LoBoundIn_TRNS);
													// Верхняя граница изм. сигнала
  vDOUB("HiBoundIn")  = GetDlgItemDoub(this->HWindow, IDC_HiBoundIn_TRNS);
													// Размерность изм. сингала
  GetDlgItemText(IDC_DimensIn_TRNS,	pCHAR("DimensIn"), iLEN("DimensIn"));
													// Нижняя граница вых. сингала
  vDOUB("LoBoundOut") = GetDlgItemDoub(this->HWindow, IDC_LoBoundOut_TRNS);
													// Верхняя граница вых. сигнала
  vDOUB("HiBoundOut") = GetDlgItemDoub(this->HWindow, IDC_HiBoundOut_TRNS);
													// Размерность изм. сингала
  GetDlgItemText(IDC_DimensOut_TRNS, pCHAR("DimensOut"), iLEN("DimensOut"));
													// Тип аппроксимации
  GetDlgItemText(IDC_TypeApprox_TRNS,	pCHAR("TypeApprox"), iLEN("TypeApprox"));
													// Класс точности
  vDOUB("Tolerance")  = GetDlgItemDoub(this->HWindow, IDC_Tolerance_TRNS);
													// Градуировочные точки по 'X'
  fnbrB =  pT_TRNS->getFieldNumber("GradPointX1");
  fnbrE =  pT_TRNS->getFieldNumber("GradPointX6");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 memset(MiscString, ' ', 10);
		 GradPointY_ED[fnbr - fnbrB]->GetLine(MiscString, 5, 0);
		 vDOUB(fnbr) = atof(MiscString);
  };												// Градуировочные точки по 'Y'
  fnbrB =  pT_TRNS->getFieldNumber("GradPointY1");
  fnbrE =  pT_TRNS->getFieldNumber("GradPointY6");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 memset(MiscString, ' ', 10);
		 GradPointX_ED[fnbr - fnbrB]->GetLine(MiscString, 5, 0);
		 vDOUB(fnbr) = atof(MiscString);
  };
};

/*
 *	------------------------------------------------------------------
 *		Г р а ф и ч е с к а я      ч а с т ь
 *	------------------------------------------------------------------
 * 	Отображение графика или сообщение об ошибке в статусной строке,
 *		расчет аппроксимационных коэффициентов
 */
void
TTransducDialog::DrawGraduirCharact(){
	FIELDNUMBER  fnbrBX,	fnbrEX;      // Номера полей значений по X
	FIELDNUMBER  fnbrBY, fnbrEY;      // Номера полей значений по Y
	FIELDNUMBER  fnbrX,  fnbrY;
	BOOL 			 bFlagError = FALSE;  // Флаг ошибки если данные не корректны
	char MiscString[10]; memset(MiscString, ' ', 10);

	FromDlgToDynRec();					 // Принимаем данные из динамического массива
												 // Найдем границы в зависимости
												 // от типа аппроксимации
												 // График ЛИНЕЙНЫЙ
	if (strcmp(pCHAR("TypeApprox"), aTypesApprox[0]) == 0){
		fnbrBX = pT_TRNS->getFieldNumber("LoBoundOut");
		fnbrEX = pT_TRNS->getFieldNumber("HiBoundOut");
		fnbrBY = pT_TRNS->getFieldNumber("LoBoundIn");
		fnbrEY = pT_TRNS->getFieldNumber("HiBoundIn");
	};                              	 // График КУСОЧНО-ЛИНЕЙНЫЙ
												 // График с аппроксимацией формой Лагранжа
												 // График с аппроксимацией методом Гаусса
	for (char i = 1; i < iCountTypesApprox; i++)
		if (strcmp(pCHAR("TypeApprox"), aTypesApprox[i]) == 0){
			fnbrBX = pT_TRNS->getFieldNumber("GradPointX1");
			fnbrEX = pT_TRNS->getFieldNumber("GradPointX6");
			fnbrBY = pT_TRNS->getFieldNumber("GradPointY1");
			fnbrEY = pT_TRNS->getFieldNumber("GradPointY6");
		};										 // Если значение поля меньше значения
												 // предыдущего поля,
	for (fnbrX  = fnbrBX + 1, fnbrY  = fnbrBY + 1;
		  fnbrX <= fnbrEX,     fnbrY <= fnbrEY;
		  fnbrX++,			     fnbrY++){
		 if (vDOUB(fnbrY) <= vDOUB(fnbrY - 1) || vDOUB(fnbrX) <= vDOUB(fnbrX - 1))
			 bFlagError = TRUE;			 // то взведем флаг возможной ошибки
	};											 // при вычислении

  Graduir_GB->bShowCharact = !bFlagError;
//====================== Прорисовываем график =================
	Message_IB->ClearShadeBox();		 // Очистим строку сообщения
	if (!bFlagError){                 // При отсутствии ошибки
		Graduir_GB->ClearGraphicWindow(); 	 // Очистим окно графика
												 // Инициализация графиков
		Graduir_GB->ChangeParamGraphic(vDOUB(fnbrBX), vDOUB(fnbrEX), pCHAR("DimensOut"),
										vDOUB(fnbrBY), vDOUB(fnbrEY), pCHAR("DimensIn"));
												 // Отображение окна графика
		Graduir_GB->ShowWindowGraphics();
		DrawValueGraphic(fnbrBX, fnbrEX, fnbrBY, fnbrEY);
	} else {
		Message_IB->StringToShadeBox(2, 2, RGB(255, 0, 0),
													"Ошибка : Деление на ноль");
	}; // if(){...} else {...}
};
/*
 * 	Функция именно отображения графика по градуировочным
 *		характеристикам преобразователя
 */
void
TTransducDialog::DrawValueGraphic(
						  FIELDNUMBER fnbrBX,// Номера полей значений X
						  FIELDNUMBER fnbrEX,
						  FIELDNUMBER fnbrBY,// Номера полей значений Y
						  FIELDNUMBER fnbrEY){
	double  DeltaX;
													// Динамические массивы данных градуировки
	double* GradPointsX = new double[(fnbrEX - fnbrBX) + 1];
	double* GradPointsY = new double[(fnbrEY - fnbrBY) + 1];
													// Узнаем индекс типа аппроксимации
													// чтобы передать его функции отображения
													// графиков для прорисовки того или
													// иного типа аппроксимации
	Graduir_GB->TypeApprox = TypeApprox_CB->GetSelIndex();
													// Заполнение динамических массивов
	for (char i = 0; i <= (fnbrEX - fnbrBX); i++){
		 GradPointsX[i] = vDOUB(fnbrBX + i);
		 GradPointsY[i] = vDOUB(fnbrBY + i);
	};                                  // Инициализация массива
	for (i = 0; i < QUANTVALUES; i++){
		Graduir_GB->CharBufferX[i] = GradPointsX[0];
		Graduir_GB->CharBufferY[i] = GradPointsY[0];
	};

// ---------- Заполнение массива для ЛИНЕЙНОЙ кривой ---------
	if (Graduir_GB->TypeApprox == 0){
		 Graduir_GB->CharBufferX[QUANTVALUES - 1] = GradPointsX[(fnbrEX - fnbrBX)];
		 Graduir_GB->CharBufferY[QUANTVALUES - 1] = GradPointsY[(fnbrEY - fnbrBY)];
	}; // if () {...}
// ---------- Заполнение массива для КУСОЧНОЛИНЕЙНОЙ кривой ---------
	if (Graduir_GB->TypeApprox == 1){
		 for (i = 0; i <= (fnbrEX - fnbrBX); i++){
			 Graduir_GB->CharBufferX[(QUANTVALUES - 1) - (fnbrEX - fnbrBX) + i]
								 = GradPointsX[i];
			 Graduir_GB->CharBufferY[(QUANTVALUES - 1) - (fnbrEX - fnbrBX) + i]
								 = GradPointsY[i];
		 };
	}; // if () {...}
// ---------- Заполним массив данными С АППРОКСИМАЦИЕЙ -----------
	if (Graduir_GB->TypeApprox  > 1){
													// Дельта 'Х' - приращение по абциссе
		 DeltaX = (GradPointsX[fnbrEX - fnbrBX] - GradPointsX[0])
														/ QUANTVALUES;
		 for (i = 0; i < QUANTVALUES; i++){
													// с учетом первого данного
			 Graduir_GB->CharBufferX[i] = i * DeltaX + GradPointsX[0];
													// Производим вычисление по форме Лагранжа
													// получим точку по 'Y'
			 Graduir_GB->CharBufferY[i] = FunctLagrangForm (
							Graduir_GB->CharBufferX[i], GradPointsX, GradPointsY);
		 };
	};	// if () {...}

// ---------- Отображение графика ------------------------------
	Graduir_GB->ShowValuesGraphics();

	for (i = 0; i < QUANTVALUES; i++){  // Запомним прошлые данные
		Graduir_GB->LastValuesX[i] = Graduir_GB->CharBufferX[i];
		Graduir_GB->LastValuesY[i] = Graduir_GB->CharBufferY[i];
	};
	delete[] GradPointsX;        	      // Уничтожение динамических массивов
	delete[] GradPointsY;
};
/*
 * ---------------------------------------------------------
 *	EoF(transduc.cpp)
 * ----------------------------------------------------------
 */

