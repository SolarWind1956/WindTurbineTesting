/*
 * tsensors - List type sensors module ------------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 *	tsensors - модуль редактирования списка поддерживаемых
 * датчиков с измемением, добавлением и удалением записи
 * Вся информация находиться в файле 'DBNET/TSensors.DB'
 * поддерживаемом пакетом PXEngine 3.0
 */
/*
 * 	Макросы для работы с Paradox Engine
 */
#define 	pVOID(X)	  pT_SENS->fPoint(X)

#define	vDOUB(X)	 *((double*)(pVOID(X)))
#define	pDOUB(X)	  (double*)(pVOID(X))
#define	vUINT(X)	 *((UINT*)(pVOID(X)))
#define	vINTG(X)	 *((INT16*)(pVOID(X)))
#define	pINTG(X)	  (INT16*)(pVOID(X))
#define  pCHAR(X)   (char*)(pVOID(X))
#define  pcCHAR(X)  (const char*)(pVOID(X))
#define  iLEN(X)	  (int)(pT_SENS->fLen(X))

/*
 * 	Включение заголовочных файлов
 */
#include <owl\validate.h>
#include <math.h>
#include <stdio.h>

#include "testsyst.rh"

#include "commfunc.h"
#include "database.h"
#include "tsensors.h"
//----------------------------------------------------------------------------

/*
 * 		Глобальные переменные - поля файла DB
 */
											// Массив размерностей выходного сигнала
const char aOutDimens [][4] =	{"V", "mA"};
const int  iCountOutDimens = sizeof(aOutDimens) / sizeof(aOutDimens[0]);
											// Массив типов аппроксимации
const char aTypesApprox[][30] = {
										"Линейная",
										"Кусочно-линейная",
										"Форма Лагранжа",
										"Форма Ньютона",
										"Метод Гаусса",
										"Метод наименьших квадратов",
										"Приближения Чебышева"
										  };
const int  iCountTypesApprox = sizeof(aTypesApprox) / sizeof(aTypesApprox[0]);

											// Массив типов датчиков
const char aTypesSensors[][20] = {
										"Аналоговый",
										"Дискретный",
										"Счетчик",
										"Быстрый счетчик",
										"Нестандартный"
											};
const int  iCountTypesSensors = sizeof(aTypesSensors) / sizeof(aTypesSensors[0]);

int   SelectIndex_TS = 0;	 		// Номер выбранного элемента списка
											// Флаг изменения в окнах ввода
BOOL  bChangeParamsSens = FALSE;
BOOL  bFillList = FALSE;  			// Флаг прошедшего заполнения списка
/*
 *  -------------------------------------------------------------------------
 * 	Class TSensorsDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TSensorsDialog, TDialog)
  EV_WM_CLOSE,
  EV_CBN_SELCHANGE (IDC_NameSensor_SENS, 	 TSensorsNameList),
  EV_CBN_EDITCHANGE(IDC_NameSensor_SENS, 	 ChangeSelectText),
  EV_EN_CHANGE		 (IDC_CodeSensor_SENS, 	 ChangeSelectText),
  EV_CBN_SELCHANGE (IDC_TypeSensor_SENS, 	 ChangeSelectText),
  EV_EN_CHANGE 	 (IDC_LoBoundIn_SENS, 	 ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_HiBoundIn_SENS, 	 ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_DimensIn_SENS,		 ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_LoBoundOut_SENS, 	 ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_HiBoundOut_SENS, 	 ChangeSelectValues),
  EV_CBN_SELCHANGE (IDC_DimensOut_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE 	 (IDC_Tolerance_SENS,	 ChangeSelectText),
  EV_EN_CHANGE		 (IDC_GradPointX1_SENS,  ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointX2_SENS,  ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointX3_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointX4_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointX5_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointX6_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointY1_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointY2_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointY3_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointY4_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE	 	 (IDC_GradPointY5_SENS,	 ChangeSelectValues),
  EV_EN_CHANGE		 (IDC_GradPointY6_SENS,	 ChangeSelectValues),
  EV_EN_KILLFOCUS	 (IDC_DimensIn_SENS, 	 ChangeDimension),
  EV_CBN_SELENDOK  (IDC_DimensOut_SENS,  	 ChangeDimension),
  EV_CBN_SELCHANGE (IDC_TypeApprox_SENS,	 ChangeTypeApprox),
  EV_COMMAND		 (IDC_Save_SENS, 	 		 SaveRecord),
  EV_COMMAND		 (IDC_Delete_SENS,  	    DeleteRecord),
  EV_COMMAND		 (IDC_Help_SENS, 	 		 Help),
  EV_COMMAND		 (IDCANCEL,  		 	 	 EndSession),
  EV_COMMAND		 (IDOK,		  		 	 	 EndSession),
END_RESPONSE_TABLE;

//								C o n s t r u c t o r
TSensorsDialog::TSensorsDialog(TWindow* parent)
  : TDialog(parent, "SensorDIALOG"), TWindow(parent)  {

  NameSensor_CB 	= new TComboBox	(this, IDC_NameSensor_SENS	);// Имя датчика
  CodeSensor_ED 	= new TEdit	      (this, IDC_CodeSensor_SENS	);// Марка датчика
  TypeSensor_CB 	= new TComboBox   (this, IDC_TypeSensor_SENS	);// Тип датчика
  DimensOut_CB 	= new TComboBox   (this, IDC_DimensOut_SENS	);// Размерности выходного сигнала
  TypeApprox_CB	= new TComboBox   (this, IDC_TypeApprox_SENS );// Тип аппроксимации

  GradPointX_ED[0] = new TEdit	 (this, IDC_GradPointX1_SENS);
  GradPointX_ED[1] = new TEdit	 (this, IDC_GradPointX2_SENS);
  GradPointX_ED[2] = new TEdit	 (this, IDC_GradPointX3_SENS);
  GradPointX_ED[3] = new TEdit	 (this, IDC_GradPointX4_SENS);
  GradPointX_ED[4] = new TEdit	 (this, IDC_GradPointX5_SENS);
  GradPointX_ED[5] = new TEdit	 (this, IDC_GradPointX6_SENS);

  GradPointY_ED[0] = new TEdit	 (this, IDC_GradPointY1_SENS);
  GradPointY_ED[1] = new TEdit	 (this, IDC_GradPointY2_SENS);
  GradPointY_ED[2] = new TEdit	 (this, IDC_GradPointY3_SENS);
  GradPointY_ED[3] = new TEdit	 (this, IDC_GradPointY4_SENS);
  GradPointY_ED[4] = new TEdit	 (this, IDC_GradPointY5_SENS);
  GradPointY_ED[5] = new TEdit	 (this, IDC_GradPointY6_SENS);

  ApproxA_ED [0] 	 = new TEdit	 (this, IDC_ApproxA0_SENS);
  ApproxA_ED [1] 	 = new TEdit	 (this, IDC_ApproxA1_SENS);
  ApproxA_ED [2] 	 = new TEdit	 (this, IDC_ApproxA2_SENS);
  ApproxA_ED [3] 	 = new TEdit	 (this, IDC_ApproxA3_SENS);
  ApproxA_ED [4] 	 = new TEdit	 (this, IDC_ApproxA4_SENS);
  ApproxA_ED [5] 	 = new TEdit	 (this, IDC_ApproxA5_SENS);
																			  // График
  Graduir_GB		 = new TGraduirCharact(this, IDC_Graduir_SENS, FALSE);

  Help_ICBT   		 = new TDrawICBT(this, IDC_Help_SENS	);// Помощь
  Cancel_ICBT 		 = new TDrawICBT(this, IDCANCEL       	);// Выход
  Save_BUT  		 = new TButton  (this, IDC_Save_SENS	);// Запись
  Delete_BUT	    = new TButton  (this, IDC_Delete_SENS	);// Удаление

  Message_IB  		 = new TBInfoBox(this, IDC_Message_SENS);// Статусная строка

// ----------------- Установим фильтры ввода для цифр -----------
  valid = new TFilterValidator("0-9.-");
  if (valid){
		new TEdit(this, IDC_LoBoundIn_SENS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_HiBoundIn_SENS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_LoBoundOut_SENS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_HiBoundOut_SENS)->SetValidator(valid);
  };
  if (valid){
		new TEdit(this, IDC_Tolerance_SENS)->SetValidator(valid);
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
TSensorsDialog::SetupWindow(){
  Graduir_GB->bShowCharact = FALSE;
  for (char i = 0; i < QUANTVALUES; i++){
		Graduir_GB->LastValuesX[i] = 0.0;
		Graduir_GB->LastValuesY[i] = 0.0;
  };
  bFillList = TRUE;						// Открываем файл	с выбранными аналоговыми
												// каналами для текущего ПК
  TDialog::SetupWindow(); 				// Показываем диалог на экране

  CreateOpenTSensors();					// Создаем новый или открываем
												// существующий файл
  RecCount = pT_SENS->getRecCount();// Сколько в нем записей
  NameSensor_CB->AddString("Новый");// Добавим пункт "Новый" в список
												// для последующего выбора данного
												// пункта для создания нового
												// описания
  for (RECORDNUMBER nNbr = 1; nNbr <= RecCount; nNbr++){
		pT_SENS->go(nNbr);				// Читаем информацию из файла
		pT_SENS->FromFileToDynRec();
		NameSensor_CB->AddString(pcCHAR("NameSensor"));
  };
												// Заполняем список типов датчиков
  for (i = 0; i < iCountTypesSensors; i++)
		TypeSensor_CB->AddString(aTypesSensors[i]);

  for (i = 0; i < iCountOutDimens; i++)// Заполняем список размерностей
		DimensOut_CB->AddString(aOutDimens[i]);

  for (i = 0; i < iCountTypesApprox; i++)
		TypeApprox_CB->AddString(aTypesApprox[i]);
  NameSensor_CB->SetSelIndex(0);    // Выберем первый элемент списка имен датчиков
												// Отображение параметров по выбранному датчику
  TSensorsNameList(); 					// Точнее т.к. изначально выбран первый элемент
												// "Занулим" все окна параметров
  bChangeParamsSens = FALSE;	 		// Сбросим флаг измемения
  bFillList = FALSE;                // содержимого окон и списка
};

/*
 * 	Отображение параметров по выбранному датчику
 */
void
TSensorsDialog::TSensorsNameList(){
  char MiscString[10]; memset(MiscString, ' ', 10);

  bFillList = TRUE;						   // Взведем флаг измемения
													// Получим индекс выбранного элемента
  SelectIndex_TS = NameSensor_CB->GetSelIndex();
  if (SelectIndex_TS != 0){            // При выборе не первого элемента
	  pT_SENS->go(SelectIndex_TS);		// Читаем информацию из файла
	  pT_SENS->FromFileToDynRec();    	// в динамическую запись
	  FromDynRecToDia();						// и далее - в диалог
	  DrawGraduirCharact();					// Отображение графика
	  Delete_BUT->EnableWindow(TRUE);   // Кнопка "Удалить" АКТИВНАЯ
													// При выборе первого элемента списка
  } else {										// "Зануляем" все окна
	  SetDlgItemText(IDC_CodeSensor_SENS, 		"");
	  TypeSensor_CB->SetSelIndex(0);
	  SetDlgItemText(IDC_LoBoundIn_SENS, 		"0");
	  SetDlgItemText(IDC_HiBoundIn_SENS, 		"0");
	  SetDlgItemText(IDC_DimensIn_SENS,		    "");
	  SetDlgItemText(IDC_LoBoundOut_SENS, 		"0");
	  SetDlgItemText(IDC_HiBoundOut_SENS, 		"0");
	  DimensOut_CB->SetSelIndex(0);
	  TypeApprox_CB->SetSelIndex(0);
	  SetDlgItemText(IDC_Tolerance_SENS, 		"0");
	  SetDlgItemText(IDC_DupDimensIn_SENS, 			 "");
	  GetDlgItemText(IDC_DimensOut_SENS, MiscString, 4);
	  SetDlgItemText(IDC_DupDimensOut_SENS, MiscString);
	  for (char i = 0; i < 6; i++){
		  GradPointX_ED[i]->DeleteLine(0);
		  GradPointX_ED[i]->Insert("0");
		  GradPointY_ED[i]->DeleteLine(0);
		  GradPointY_ED[i]->Insert("0");
		  ApproxA_ED   [i]->DeleteLine(0);
		  ApproxA_ED   [i]->Insert("0");
	  };
	  Graduir_GB->ClearGraphicWindow();		// Очистим окно графика
	  Delete_BUT->EnableWindow(FALSE);  	// Кнопка "Удалить" НЕАКТИВНАЯ
  };
  Save_BUT->EnableWindow(FALSE);       	// Кнопка "Сохранить" НЕАКТИВНАЯ
  bChangeParamsSens 	= FALSE;					// Сбросим флаг изменений
  bFillList 		= FALSE;
};
/*
 * 	Функция отображает размерность в градуировочной таблице
 *		по уничтожению фокуса на окнах ввода размерностей
 */
void
TSensorsDialog::ChangeDimension(){
													// Возьмем строку размерности входного сигнала
													// из динам. массива
  GetDlgItemText(IDC_DimensIn_SENS, 	pCHAR("DimensIn"),  iLEN("DimensIn"));
													// и вставим ее в окно размерности входного сигнала
  SetDlgItemText(IDC_DupDimensIn_SENS,	pcCHAR("DimensIn"));
													// Возьмем строку размерности выходного сигнала
													// из динам. массива
  GetDlgItemText(IDC_DimensOut_SENS, 	pCHAR("DimensOut"), iLEN("DimensOut"));
													// и вставим ее в окно размерности выходного сигнала
  SetDlgItemText(IDC_DupDimensOut_SENS,pcCHAR("DimensOut"));
};

/*
 * 	Функция запускает выбранный тип аппроксимации данных
 */
void
TSensorsDialog::ChangeTypeApprox(){
  if (!bFillList){
	  bChangeParamsSens = TRUE;		  	// Взводим флаг изменений
													// Кнопка "Сохранить" АКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsSens);
													// Кнопка "Удалить" НЕАКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsSens);
	  DrawGraduirCharact();					// Отображение графика
  };
};

/*
 * 	Функция взводит флаг как только было что-либо изменено или выбрано
 *		в окнах имен, типов, и др. текстовых полях
 */
void
TSensorsDialog::ChangeSelectText(){
  if (!bFillList){
	  bChangeParamsSens = TRUE;		  	// Взводим флаг изменений
													// Кнопка "Сохранить" АКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsSens);
													// Кнопка "Удалить" НЕАКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsSens);
  };
};
/*
 * 	Функция взводит флаг как только было что-либо изменено или выбрано
 *		в списках диапазонов, размерностей и других окнах параметров
 *		с обновлением градуировочной гривой
 */
void
TSensorsDialog::ChangeSelectValues(){
  if (!bFillList){
	  bChangeParamsSens = TRUE; 		 	// Взводим флаг изменений
													// Кнопка "Сохранить" АКТИВНА
	  Save_BUT->EnableWindow(bChangeParamsSens);
													// Кнопка "Удалить" НЕАКТИВНА
	  Delete_BUT->EnableWindow(!bChangeParamsSens);
	  DrawGraduirCharact();
  };
};

/*
 *		Записываем или изменяем данные в файле
 */
void
TSensorsDialog::SaveRecord(){
  char MiscString[10]; memset(MiscString, ' ', 10);

  FromDlgToDynRec();                   // Получим новые данные из диалога
  if (SelectIndex_TS == 0){ 				// Если создаем новое описание
													// Добавим имя в список элементов
	  NameSensor_CB->AddString(pCHAR("NameSensor"));
													// Номер заносимой в файл записи
	  vINTG("IndexCode") = (short)((NameSensor_CB->GetCount()) - 1);
	  AppendToTSensors();         		// Запись данных - добавление
	  SelectIndex_TS = vINTG("IndexCode");
  } else {                             // Заменяем старые данные
													// Номер заносимой в файл записи
	  vINTG("IndexCode") = (short)(SelectIndex_TS);
													// Удалим элемент из ComboBox'а имен
	  NameSensor_CB->DeleteString(SelectIndex_TS);
													// Вставим имя в список элементов
	  NameSensor_CB->InsertString(pCHAR("NameSensor"), SelectIndex_TS);
	  UpdateTSensors();  				  	// Обновим данные в файле
  };                                   // Маркируем добавленный элемент
  NameSensor_CB->SetSelIndex(SelectIndex_TS);
  bChangeParamsSens = FALSE;           // Сбросим флаг изменений
													// Кнопка "Сохранить" НЕАКТИВНА
  Save_BUT->EnableWindow(bChangeParamsSens);
													// Кнопка "Удалить" АКТИВНА
  Delete_BUT->EnableWindow(!bChangeParamsSens);
};
/*
 * 	Удаление текущей записи из файла
 */
void
TSensorsDialog::DeleteRecord(){
  Message_IB->ClearShadeBox();			// Удалить конечно можно, но при условии
													// что на эту запись нет ссылки из других
  if (vINTG("Refrence") == 0) {        // файлов
	  pT_SENS->delCurRecord();		      // Удаление записи из файла
													// Удаление элемента из списка имен
	  NameSensor_CB->DeleteString(SelectIndex_TS);
	  SelectIndex_TS--;                 // Кол-во элементов уменьшилось
													// Маркируем предыдущий элемент
	  NameSensor_CB->SetSelIndex(SelectIndex_TS);
	  TSensorsNameList();					// Показываем информацию о
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
TSensorsDialog::EvClose(){
	EndSession();
};
void
TSensorsDialog::EndSession(){
  if (bChangeParamsSens){
	  if (MessageBox("Данные не сохранены.\nСохранить перед выходом ?",
						  "Информационные датчики",
						  MB_YESNO) == IDYES){
			SaveRecord();				 // Сохраним данные
	  };
  };
  Graduir_GB->ClearGraphicWindow();// Очистим окно графика
  CloseTSensors();                // Закроем файл датчиков
  CloseWindow();
};
/*
 *	 Добавление записи в файл
 */
BOOL
AppendToTSensors(){
	pT_SENS->clearRecordForWrite();
	pT_SENS->FromDynRecToFile();
	pT_SENS->append();
	return TRUE;
};
/*
 *	 Обновление записи в файле
 */
BOOL UpdateTSensors(){
	pT_SENS->FromDynRecToFile();
	pT_SENS->update();
	return TRUE;
};
/*
 *					Информация из динамической записи в диалог
 */
void
TSensorsDialog::FromDynRecToDia   (){
  char MiscString[10]; memset(MiscString, ' ', 10);
											 // Марка датчика
  SetDlgItemText(IDC_CodeSensor_SENS,	pcCHAR("CodeSensor"));
											 // Тип датчика
  TypeSensor_CB->SetSelIndex(TypeSensor_CB->FindString(pcCHAR("TypeSensor"), 0));
											 // Нижняя граница изм. сигнала
  SetDlgItemDoub(this->HWindow, IDC_LoBoundIn_SENS, vDOUB("LoBoundIn"));
											 // Верхняя граница изм. сигнала
  SetDlgItemDoub(this->HWindow, IDC_HiBoundIn_SENS, vDOUB("HiBoundIn"));
											 // Размерность изм. сингала
  SetDlgItemText(IDC_DimensIn_SENS,   	pcCHAR("DimensIn"));
											 // Размерность изм. сингала в градуир. таблицу
  SetDlgItemText(IDC_DupDimensIn_SENS,	pcCHAR("DimensIn"));

											 // Нижняя граница вых. сингала
  SetDlgItemDoub(this->HWindow, IDC_LoBoundOut_SENS, 	vDOUB("LoBoundOut"));
											 // Верхняя граница вых. сингала
  SetDlgItemDoub(this->HWindow, IDC_HiBoundOut_SENS, 	vDOUB("HiBoundOut"));
											 // Размерность вых. сингала
  DimensOut_CB->SetSelIndex(DimensOut_CB->FindString(pcCHAR("DimensOut"), 0) );
											 // Размерность вых. сингала в градуир. таблицу
  SetDlgItemText(IDC_DupDimensOut_SENS,pcCHAR("DimensOut"));
											 // Тип аппроксимации
  TypeApprox_CB->SetSelIndex(TypeApprox_CB->FindString(pcCHAR("TypeApprox"), 0) );

											 // Класс точности
  SetDlgItemDoub(this->HWindow, IDC_Tolerance_SENS, vDOUB("Tolerance"));
											 // Градуировочные точки по 'X'
  fnbrB =  pT_SENS->getFieldNumber("GradPointX1");
  fnbrE =  pT_SENS->getFieldNumber("GradPointX6");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 gcvt(vDOUB(fnbr), 5, MiscString);
		 GradPointY_ED[fnbr - fnbrB]->DeleteLine(0);
		 GradPointY_ED[fnbr - fnbrB]->Insert(MiscString);
  };										 // Градуировочные точки по 'Y'
  fnbrB =  pT_SENS->getFieldNumber("GradPointY1");
  fnbrE =  pT_SENS->getFieldNumber("GradPointY6");
  for ( fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		  gcvt(vDOUB(fnbr), 5, MiscString);
		  GradPointX_ED[fnbr - fnbrB]->DeleteLine(0);
		  GradPointX_ED[fnbr - fnbrB]->Insert(MiscString);
  };										 // Аппроксимационгные коэффициенты
  fnbrB =  pT_SENS->getFieldNumber("ApproxA0");
  fnbrE =  pT_SENS->getFieldNumber("ApproxA5");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 gcvt(vDOUB(fnbr), 5, MiscString);
		 ApproxA_ED[fnbr - fnbrB]->DeleteLine(0);
		 ApproxA_ED[fnbr - fnbrB]->Insert(MiscString);
  };
};
/*
 *			Информация из диалога в динамическую запись
 */
void
TSensorsDialog::FromDlgToDynRec(){
  char MiscString[10]; memset(MiscString, ' ', 10);
											 // Имя датчика
  GetDlgItemText(IDC_NameSensor_SENS,	pCHAR("NameSensor"), iLEN("NameSensor"));
											 // Марка датчика
  GetDlgItemText(IDC_CodeSensor_SENS,	pCHAR("CodeSensor"), iLEN("CodeSensor"));
											 // Тип датчика
  GetDlgItemText(IDC_TypeSensor_SENS,	pCHAR("TypeSensor"), iLEN("TypeSensor"));
											 // Нижняя граница изм. сигнала
  vDOUB("LoBoundIn")  = GetDlgItemDoub(this->HWindow, IDC_LoBoundIn_SENS);
											 // Верхняя граница изм. сигнала
  vDOUB("HiBoundIn")  = GetDlgItemDoub(this->HWindow, IDC_HiBoundIn_SENS);
											 // Размерность изм. сингала
  GetDlgItemText(IDC_DimensIn_SENS,	pCHAR("DimensIn"), iLEN("DimensIn"));
											 // Нижняя граница вых. сигнала
  vDOUB("LoBoundOut") = GetDlgItemDoub(this->HWindow, IDC_LoBoundOut_SENS);
											 // Верхняя граница вых. сигнала
  vDOUB("HiBoundOut") = GetDlgItemDoub(this->HWindow, IDC_HiBoundOut_SENS);
											 // Размерность изм. сингала
  GetDlgItemText(IDC_DimensOut_SENS,	pCHAR("DimensOut"), iLEN("DimensOut"));
											 // Тип аппроксимации
  GetDlgItemText(IDC_TypeApprox_SENS,	pCHAR("TypeApprox"), iLEN("TypeApprox"));
											 // Класс точности
  vDOUB("Tolerance")  = GetDlgItemDoub(this->HWindow, IDC_Tolerance_SENS);
											 // Градуировочные точки по 'X'
  fnbrB =  pT_SENS->getFieldNumber("GradPointX1");
  fnbrE =  pT_SENS->getFieldNumber("GradPointX6");
  for (fnbr = fnbrB; fnbr <= fnbrE; fnbr++){
		 memset(MiscString, ' ', 10);
		 GradPointY_ED[fnbr - fnbrB]->GetLine(MiscString, 5, 0);
		 vDOUB(fnbr) = atof(MiscString);
  };										 // Градуировочные точки по 'Y'
  fnbrB =  pT_SENS->getFieldNumber("GradPointY1");
  fnbrE =  pT_SENS->getFieldNumber("GradPointY6");
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
TSensorsDialog::DrawGraduirCharact(){
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
		fnbrBX = pT_SENS->getFieldNumber("LoBoundOut");
		fnbrEX = pT_SENS->getFieldNumber("HiBoundOut");
		fnbrBY = pT_SENS->getFieldNumber("LoBoundIn");
		fnbrEY = pT_SENS->getFieldNumber("HiBoundIn");
	};                              	 // График КУСОЧНО-ЛИНЕЙНЫЙ
												 // График с аппроксимацией формой Лагранжа
												 // График с аппроксимацией методом Гаусса
	for (char i = 1; i < iCountTypesApprox; i++)
		if (strcmp(pCHAR("TypeApprox"), aTypesApprox[i]) == 0){
			fnbrBX = pT_SENS->getFieldNumber("GradPointX1");
			fnbrEX = pT_SENS->getFieldNumber("GradPointX6");
			fnbrBY = pT_SENS->getFieldNumber("GradPointY1");
			fnbrEY = pT_SENS->getFieldNumber("GradPointY6");
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
		Graduir_GB->ClearGraphicWindow();// Очистим окно графика
												 // Инициализация нового графика
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
TSensorsDialog::DrawValueGraphic(
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
 *	EoF(tsensors.cpp)
 * ----------------------------------------------------------
 */
