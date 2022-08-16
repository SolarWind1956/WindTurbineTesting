/*
 * testctrl - Модуль запуска испытаний
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 * TestCtrl - диспетчер подсистемы запуска испытаний.
 * Формируется три ListBox'а:
 *			- список работающих испытаний;
 *			- список типов испытаний;
 *			- список типов ВЭУ.
 * Список работающих испытаний образуется путем выбора типа испытания
 * и ВЭУ из соответствующих списков м проверкой на дубликатность.
 * Для выбранного испытания может быть включена запись данных ипытания
 * в дисковый файл с помощью переключателя. В списке для каждого испытания
 * имеется отметка о записи протокола испытания и времени начала записи.
 * Для создания ListBox'ов используются функции описанные в 'dlistvie.h'
 */
 /*
 * 	Макросы для работы с Paradox Engine
 */
#define 	pVOID(X)	pT_TJNL->fPoint(X)

#define	vDOUB(X)	*((double*)(pVOID(X)))
#define	pDOUB(X)	  (double*)(pVOID(X))
#define	vUINT(X)	*((UINT*)(pVOID(X)))
#define	vINTG(X)	*((INT16*)(pVOID(X)))
#define	pINTG(X)	  (INT16*)(pVOID(X))
#define  pCHAR(X)   (char*)(pVOID(X))
#define  pcCHAR(X)  (const char*)(pVOID(X))
#define  iLEN(X)	  (int)(pT_TJNL->fLen(X))
/*
 * 	Включение заголовочных файлов
 */
#include "testctrl.h"
#include "database.h"
char 		TypesTests[6][40] =				// Типы испытаний
			{"Определение мощностной характеристики",
			 "Направление ветра и оси головки ВЭУ",
			 "Проверка качества электроэнергии",
			 "Виброрежимы работы ВЭУ",
			 "Параметры потока за лопастями",
			 "Тепловые режимы работы ВЭУ"};
char AbbrevTypesTests[6][5] =	{  	// Аббревиатера типов испытаний для имени файла протокола
										 "PwCh", "WnAz", "    ",
										 "    ", "    ", "    "
										};
char TypesWindTurbines[4][12] =     // Испытуемые ВЭУ
		{"АВЭ-250 N5",
		 "АВЭ-250 N7",
		 "АВЭ-250 N10",
		 "АВЭ-250 N11"};
typedef struct {                    // Структура испытания
	unsigned int 	Index;				// Номер испытания
	char 				TimeTest  [9];	  	// Время запуска
	char 				TypeTest  [40];   // Тип
	char 				NumWStTest[12];   // ВЭУ
	char 				StTest    [80];	// Строка в списке
	HWND 				hChldWnd;		 	// Лог. номер окна испытания
	HINSTANCE		hInstTestApp;
} StructTest;
StructTest*	WorkTests;					// Работающие испытания
StructTest*	MiscTest;               // Промежуточный массив работающих испытаний

int	iTypeTest   	= 0;
int 	iNumWStTest 	= 0;
												// Позиции табуляторов в списке
int 	Tab_Stops[4] 	= {23, 60, 247, 297};
												// работающих испытаний
int  	iQuantTests 	= 0;    	 		// Кол-во РАБОТАЮЩИХ испытаний
int  	iQuantRec   	= 0;				// Кол-во запсией в журнале испытаний
int  	iNumTests   	= 0;				// Кол-во ЗАПУЩЕНЫХ испытаний
/*
 *  -------------------------------------------------------------------------
 * 	Class TTestCtrlDialog
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TTestCtrlDialog, TDialog)
  EV_WM_CLOSE,
  EV_LBN_SELCHANGE(IDC_TLISTBOX2, ListTypeTests),// Список типов испытаний
																 // Список типов ВЭУ
  EV_LBN_SELCHANGE(IDC_TLISTBOX3, ListTypeWindTurbine),
  EV_LBN_DBLCLK	(IDC_TLISTBOX1, MaximizeTestApp	  ),
  EV_COMMAND		(CM_TESTHELP,   TestCtrlHelp),  // Помощь
  EV_COMMAND		(CM_TESTEXIT,   EndSession),	  // Выход
  EV_COMMAND		(CM_TESTSTART,  TestCtrlStart), // Запуск испытания
  EV_COMMAND		(CM_TESTSTOP,   TestCtrlStop),  // Остановка испытания
END_RESPONSE_TABLE;
//								C o n s t r u c t o r
TTestCtrlDialog::TTestCtrlDialog(TWindow* parent)
  : TDialog(parent, "TestCtrlDIALOG"),	TWindow(parent) {
  TestCtrlDialogCreated = FALSE;
  Button1   = new TDrawICBT    (this, CM_TESTHELP);   // Помощь
  Button2   = new TDrawICBT    (this, CM_TESTEXIT);   // Выход
  Button3   = new TDrawICBT    (this, CM_TESTSTART);  // Запуск испытания
  Button4   = new TDrawICBT    (this, CM_TESTSTOP);   // Остановка испытания
  ListBox1  = new TDrawListView(this, IDC_TLISTBOX1); // Список работающих испытаний
  ListBox2  = new TDrawListView(this, IDC_TLISTBOX2); // Список типов испытаний
  ListBox3  = new TDrawListView(this, IDC_TLISTBOX3); // Список типов ВЭУ
};
void
TTestCtrlDialog::MaximizeTestApp(){
	::ShowWindow(WorkTests[ListBox1->GetSelIndex()].hChldWnd, SW_RESTORE);
};
void
TTestCtrlDialog::ListTypeTests(){
												// Выбранный тип испытания
	iTypeTest = ListBox2->GetSelIndex();
};
void
TTestCtrlDialog::ListTypeWindTurbine(){
												// Выбранный ВЭУ
	iNumWStTest = ListBox3->GetSelIndex();
};
/*
 * 	Заполнение списков ListBox
 */
									// Список работающих испытаний
void
CreateListBox1(TListBox* ListBox1){
  for (char i = 0; i < iQuantTests; i++){
		ListBox1->AddString(WorkTests[i].StTest);
  };
  if (ListBox1->GetCount() > 0) { // Если список не пустой
		ListBox1->SetSelIndex(0);   // Выбираем первую строку
  };
};
									// Список типов испытаний
void
CreateListBox2(TListBox* ListBox2){
  for (char i = 0; i < 6; i++)
		ListBox2->AddString(TypesTests[i]);
  ListBox2->SetSelIndex(0);		 // Выбираем первую строку
											 // Заносим в массив
											 // выбранный тип
};
									// Список ВЭУ
void
CreateListBox3(TListBox* ListBox3){
  for (char i = 0; i < 4; i++)
		ListBox3->AddString(TypesWindTurbines[i]);
  ListBox3->SetSelIndex(0);         // Выбираем первую строку
												// Заносим в массив выбранный ВЭУ
};
												// Нагружаем 'SetupWindow'
void											// дополнительными функциями
TTestCtrlDialog::SetupWindow(){
	CreateOpenTestJrnlFile();  		// Откроем файл-журнал испытаний
	TDialog::SetupWindow();				// Функции родные ... а также ...
												// Устанавливаем позиции табулятора
	ListBox1->SetTabStops(4, Tab_Stops);
												// Инициируем список типов испытаний
	void (*pCreateListBox2)(TListBox*);
	pCreateListBox2 = CreateListBox2;
	ListBox2->CreateListBox(pCreateListBox2);
												// Инициируем список ВЭУ
	void (*pCreateListBox3)(TListBox*);
	pCreateListBox3 = CreateListBox3;
	ListBox3->CreateListBox(pCreateListBox3);
};
/*
 * Формирование строки для вывода в ListBox работающих испытаний
 */
void
TTestCtrlDialog::FormatString(int NumItem){

	strcpy(WorkTests[NumItem].StTest,     " ");
	strcpy(WorkTests[NumItem].TypeTest,   TypesTests[iTypeTest]);
	strcpy(WorkTests[NumItem].NumWStTest, TypesWindTurbines[iNumWStTest]);

	sprintf (WorkTests[NumItem].StTest,
				" %d\x09%s\x09%s\x09%s\x09%s",
				iNumTests,
				WorkTests[NumItem].TimeTest,
				WorkTests[NumItem].TypeTest,
				WorkTests[NumItem].NumWStTest);
						// Готовим запись в журнальный файл
	vINTG("IndexCode") = iNumTests; // Номер по порядку
												// Время окончания испытания
	strcpy(pCHAR("EndTime"),	WorkTests[NumItem].TimeTest);
												// Наименование испытания
	strcpy(pCHAR("TestName"),  WorkTests[NumItem].TypeTest);
	vINTG("TestMark") = 0;				// Маркировка испытания
	vINTG("TestNumber") = 0;        	// Номер серии испытаний
	vINTG("SeriesNumber") = 0;       // Номер испытания в серии
	sprintf(pCHAR("PerdCyclTime"), "00:00:00");
	strcpy(pCHAR("WindTurbine"), WorkTests[NumItem].NumWStTest);
	vINTG("CodeTypeWT") = 0;        // Код ВЭУ
};

void InitItemStruct(int NumItem){
	WorkTests[NumItem].Index 				= 	iNumTests;

	strcpy(WorkTests[NumItem].TimeTest,   	" ");
	strcpy(WorkTests[NumItem].TypeTest,   	" ");
	strcpy(WorkTests[NumItem].NumWStTest, 	" ");
	strcpy(WorkTests[NumItem].StTest, 		" ");

	WorkTests[NumItem].hChldWnd  			=  NULL;
};
void               						// Запуск испытания
TTestCtrlDialog::TestCtrlStart(){
	const char ProgName [14] = "powercha.exe";
	char ProgParam[70];
	strcpy(ProgParam, " ");
	if (iQuantTests == 0){				// Для первого испытания
		WorkTests = new StructTest[iQuantTests + 1];
		WorkTests[iQuantTests].Index = 0;
		iNumTests++;
		InitItemStruct(iQuantTests);
	} else {									// Создадим промежуточный динам. массив с
												// размером iQuantTests - 1
		MiscTest = new StructTest[iQuantTests];
												// Передадим массив данных по испытаниям во
												// времменный массив
		memmove(MiscTest, WorkTests, size_t(sizeof(StructTest) * (iQuantTests)));
		delete[] WorkTests;           // удалим массив работающих испытания
												// Создадим динам. массив работающих испытаний с
												// размером iQuantTests
		WorkTests = new StructTest[iQuantTests + 1];
		iNumTests++;
		InitItemStruct(iQuantTests);
												// Передадим массив данных по испытаниям из
												// времменного массива в работающий массив
		memmove(WorkTests, MiscTest, size_t(sizeof(StructTest) * (iQuantTests)));
		delete[] MiscTest;            // удалим временный массив работающих испытания
	};
	iQuantTests++; 						// Увеличим количество испытаний
	_strdate(pCHAR("BeginDate"));    // Присвоим дате начала испытания системную дату
												// Присвоим дате окончания испытания системную дату
												// для того чтобы в файл журнала испытания не попало
												// пустое значение даты окончания испытания
												// По завершению испытания это значение заменим на
												// реальную дату окончания испытания
	_strdate(pCHAR("EndDate"));		// Время начала испытания
	strcpy(WorkTests[iQuantTests - 1].TimeTest, _strtime(pCHAR("BeginTime")));
	FormatString(iQuantTests - 1);	// Подготавливаем строку для занесения ее с список
												// заносим выбранное испытание
												// в список работающих испытаний
	ListBox1->AddString(WorkTests[iQuantTests - 1].StTest);
												// Выберем в списке работающих испытаний
												// запущеное испытание
	ListBox1->SetSelIndex(iQuantTests - 1);
/*
 *			-----------------------------------------------------
 *			Запуск дочернего приложение для запущеного испытания
 *			-----------------------------------------------------
 */
												//	Параметры для приложениея формируются как :
												//	"Тип испытания + номер ВЭУ"
												//	х22 - символ " для границ параметра
	sprintf(ProgParam, "\x22%d %s %s\x22 \x22%s\x22",
							 iNumTests,
							 WorkTests[iQuantTests - 1].TypeTest,
							 WorkTests[iQuantTests - 1].NumWStTest,
							 AbbrevTypesTests[0/*iTypeTest*/]);
	WorkTests[iQuantTests - 1].hInstTestApp =
				ShellExecute(HWindow,	  	  		// Handle нашего окна
								 NULL,			 	 	// Флаг для запуска приложения
								 ProgName,		  		// Имя приложения
								 ProgParam,			  	// Параметры
								 NULL,			  		// Текущий каталог
								 SW_SHOWNORMAL);		// Статус окна приложения
															// Получим handle окна запущенного приложения
	WorkTests[iQuantTests - 1].hChldWnd = GetActiveWindow();
//	---------------------------------------------------------
	SetActiveWindow();					// Активизируем наше окно
	pT_TJNL->FromDynRecToFile();     // Запишем данные о начале испытания в журнал испытаний
	pT_TJNL->append();               // в журнальный файл испытаний
};
void                                // Остановка испытаний
TTestCtrlDialog::TestCtrlStop(){
	int iSelectTest;
	if (iQuantTests != 0) {
		iQuantTests--;						// Уменьшим число работающих испытаний
												// Прочитаем данные по останавливаемому испытанию из
												// журнала испытаний
		iSelectTest = ListBox1->GetSelIndex();
												// Читаем информацию из журнального
												// файла
		pT_TJNL->go(iQuantRec + WorkTests[iSelectTest].Index);
		pT_TJNL->FromFileToDynRec();
		_strdate(pCHAR("EndDate"));   // Дата окончания испытания
		_strtime(pCHAR("EndTime"));   // Время окончания испытания
		pT_TJNL->FromDynRecToFile();
		pT_TJNL->update();            // Запишем данные об окончании испытания в журнал испытаний
/*
 *			Останов дочернего приложения для остановленного испытания
 */
												// Пошлем сообщение дочернему приложению
												// соответстсвующее данному испытанию для
												// закрытия приложения данного испытания
		::SendMessage(WorkTests[iSelectTest].hChldWnd, WM_CLOSE, 0, 0);

		ListBox1->DeleteString(iSelectTest);

		if (iSelectTest == 0) {       // Аппарат маркировки испытания перед удаленным из
			ListBox1->SetSelIndex(0);  // списка испытаний
		} else {
			ListBox1->SetSelIndex(iSelectTest - 1);
		};                            // Уничтожим удаляемый элемент из
												// динам. массива работающих испытаний
		for (char i = iSelectTest; i < iQuantTests; i++){
			WorkTests[i] = WorkTests[i + 1];
		};    								// Создадим промежуточный динам. массив с
												// размером iQuantTests
		if (iQuantTests != 0){
			MiscTest = new StructTest[iQuantTests];
												// Передадим массив данных по испытаниям
												// во времменный массив
			memmove(MiscTest, WorkTests, size_t(sizeof(StructTest) * iQuantTests));
			delete[] WorkTests;        // удалим массив работающих испытания
												// Создадим динам. массив работающих испытаний с
												// размером iQuantTests
			WorkTests = new StructTest[iQuantTests];
			if (WorkTests == NULL) MessageBox("Error WorkTests", "Error", MB_OK);
												// Передадим массив данных по испытаниям из
												// времменного массива в работающий массив
			memmove(WorkTests, MiscTest, size_t(sizeof(StructTest) * iQuantTests));
			delete[] MiscTest;         // удалим временный массив работающих испытания
		};
	};
};

void
TTestCtrlDialog::EvClose(){
	 EndSession();
};
void
TTestCtrlDialog::EndSession(){
	if (iQuantTests != 0){
		 EnableWindow(FALSE); 						// Запрещаем доступ к окну диалога
		 Show(SW_HIDE);					 			// Гасим окно диалога
		 ::SendMessage(GetParent(), WM_PAINT, 0, 0);
	 } else {											// Если все выключено, убиваем диалог
		 CloseTestJrnlFile();							// Закроем файл-журнал испытаний
		 TestCtrlDialogCreated = FALSE;
		 CloseWindow(IDCANCEL);
	 };
};
/*
 * ---------------------------------------------------------
 *	EoF(testctrl.cpp)
 * ----------------------------------------------------------
 */

