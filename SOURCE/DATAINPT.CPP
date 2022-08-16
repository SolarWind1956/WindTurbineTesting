/*
 * datainpt - Data input system module -------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 * DataInpt - диспетчер подсистемы сбора информации, выполняет
 * следующие функции:
 * - установление связи с контроллером;
 * - включение подсистемы ввода данных, запуск таймера коммуникации;
 * - выключение подсистемы ввода данных и сброс связи с контроллером;
 * - включение подсистемы мониторинга, запуск таймера мониторинга;
 * - выключение подсистемы мониторинга;
 * - включение подсистемы записи протоколов;
 * - выключение подсистемы записи протоколов;
 * - изменение периода опроса и мониторинга и протоколов.
 */

#include <owl\menu.h>
#include <stdio.h>

#include "testsyst.h"
#include "datainpt.h"

const char sNet      [] = "Связь";
const char sInstant  [] = "Мгновенный";
const char sCycleFile[] = "Циклический";
/*
 *  -------------------------------------------------------------------------
 * 	Class TDInptWindow
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE1(TDInptWindow, TDialogWindow)
  EV_WM_TIMER,
  EV_WM_CLOSE,
  EV_WM_INITMENU,

  EV_COMMAND(CM_ONOFFNET,    InfoInptON), 		// Триггер опроса
//  EV_COMMAND(CM_CHANGENET,   InputCycleService),// Изменение периода опроса

  EV_COMMAND(CM_ONOFFINSTANT,  InstantValInptON),      // Триггер записи мгновенных значений
//  EV_COMMAND(CM_CHANGEINSTANT, InstantValCycleService),// Изменение периода записи мгновенных значений

  EV_COMMAND(CM_ONOFFCYCLE,  CycleFInptON),		// Триггер записи циклических данных
//  EV_COMMAND(CM_CHANGECYCLE, CycleFCycleService),// Изменение периода записи циклических данных

  EV_COMMAND(CM_ONOFFMONITOR,MonitoryONOFF),		// Триггер мониторинга
//  EV_COMMAND(CM_CHANGEMONITP,MonitCycleService),// Период мониторинга

  EV_COMMAND(CM_NETHELP,  	  DataMonitHelp),    // Помощь
  EV_COMMAND(CM_NETEXIT,  	  EndSession),       // Конец работы
END_RESPONSE_TABLE;

//		C o n s t r u c t o r
TDInptWindow::TDInptWindow(TWindow* parent, const char* far Title, TResID MenuName,
									int Width, int Height) :
					TDialogWindow(parent, Title, MenuName, Width, Height){
	DInptWindowCreated = FALSE;				// Диалог создан только как объект класса - его еще
														// нужно создать методом класса Create()
														// Мониторинг
	pTMonitoryWindow = new TMonitoryWindow(this);
// --------------------- Связь с ПК ----------------------------
	ButtonG1 = new TButtonGadget(CM_ONOFFNET, 	 CM_ONOFFNET,
										  TButtonGadget::Exclusive,
										  TRUE, TButtonGadget::Up);
	CntrBar->Insert(*ButtonG1);
														// Изменение периода связи с ПК
	ButtonG2 = new TButtonGadget(CM_CHANGENET, 	 CM_CHANGENET,
										  TButtonGadget::Command,
										  FALSE/*TRUE*/);
	CntrBar->Insert(*ButtonG2);  				// Разделитель
	CntrBar->Insert(*new TSeparatorGadget(6));

// --------------------- Мониторинг ----------------------------
	ButtonG3 = new TButtonGadget(CM_ONOFFMONITOR, CM_ONOFFMONITOR,
										  TButtonGadget::Exclusive,
										  FALSE, TButtonGadget::Up);
	CntrBar->Insert(*ButtonG3);
														// Изменение периода мониторинга
	ButtonG4 = new TButtonGadget(CM_CHANGEMONITP, CM_CHANGEMONITP,
										  TButtonGadget::Command,
										  FALSE/*TRUE*/);
	CntrBar->Insert(*ButtonG4);				// Разделитель
	CntrBar->Insert(*new TSeparatorGadget(6));

// --------------------- Протокол мгновенных значений --------------
	ButtonG5 = new TButtonGadget(CM_ONOFFINSTANT,   CM_ONOFFINSTANT,
										  TButtonGadget::Exclusive,
										  FALSE, TButtonGadget::Up);
	CntrBar->Insert(*ButtonG5);

	//ButtonG6							Резерв для коррекции поправки 2-х секундного протокола
														// Разделитель
	CntrBar->Insert(*new TSeparatorGadget(6));

// ----------------------- Цикл. протокол ----------------------
	ButtonG7 = new TButtonGadget(CM_ONOFFCYCLE,   CM_ONOFFCYCLE,
										  TButtonGadget::Exclusive,
										  FALSE, TButtonGadget::Up);
	CntrBar->Insert(*ButtonG7);
														// Изменение периода цикл. протокола
	ButtonG8 = new TButtonGadget(CM_CHANGECYCLE,  CM_CHANGECYCLE,
										  TButtonGadget::Command,
										  FALSE);
	CntrBar->Insert(*ButtonG8);				// Разделитель
	CntrBar->Insert(*new TSeparatorGadget(12));

// ----------------------- Помощь ------------------------------
	ButtonG9 = new TButtonGadget(CM_NETHELP, 	 	 CM_NETHELP,
										  TButtonGadget::Command,
										  TRUE);
	CntrBar->Insert(*ButtonG9);

// -----------------------	Выход -------------------------------
	ButtonG10 = new TButtonGadget(CM_NETEXIT, 	 CM_NETEXIT,
											TButtonGadget::Command,
											TRUE);
	CntrBar->Insert(*ButtonG10);
													// Включим режим подсказки в
													// StatusBar
	CntrBar->SetHintMode(TGadgetWindow::EnterHints);

													// Формирование и вставка индикаторов
	pNet = new TTextGadget(0, TGadget::None, TTextGadget::Left, strlen(sNet), "");
	StatBar->Insert(*pNet);					// Вставка индикатора сети с ПК

	pInstantVal = new TTextGadget(0, TGadget::None, TTextGadget::Left, strlen(sInstant), "");
	StatBar->Insert(*pInstantVal);		// Вставка индикатора протокола мгновенных значений

	pCycle = new TTextGadget(0, TGadget::None, TTextGadget::Left, strlen(sCycleFile), "");
	StatBar->Insert(*pCycle);	  			// Вставка индикатора циклического протокола
};

/*
 * 	Обновление меню при помощи статуса переменных
 */
void
TDInptWindow::EvInitMenu(HMENU menuHandle)
{
  ShowMessage("");							// Удалим сообщение из статусной строки
  TMenu menu(menuHandle);					// Создадим объект класса меню
													// Делаем элементы меню доступными или недоступными
  menu.EnableMenuItem(CM_ONOFFNET, 		This->HConv ? MF_ENABLED : MF_GRAYED);
  menu.EnableMenuItem(CM_ONOFFMONITOR, bInptDataON ? MF_ENABLED : MF_GRAYED);
  menu.EnableMenuItem(CM_ONOFFINSTANT, bInptDataON ? MF_ENABLED : MF_GRAYED);
  menu.EnableMenuItem(CM_ONOFFCYCLE, 	bInptDataON ? MF_ENABLED : MF_GRAYED);
													// Разрешаем или рапрещаем доступ к кнопкам
  ButtonG3->SetEnabled(bInptDataON);	// Мониторинг
  ButtonG5->SetEnabled(bInptDataON);	// Протокол мгновенных значений
  ButtonG7->SetEnabled(bInptDataON);	// Цикл. протокол
													// Маркируем панели меню или сбрасываем маркировку
  menu.CheckMenuItem (CM_ONOFFNET,   	MF_BYCOMMAND | bInptDataON   ? MF_CHECKED : MF_UNCHECKED);
  menu.CheckMenuItem (CM_ONOFFMONITOR, MF_BYCOMMAND | bMonitoryON   ? MF_CHECKED : MF_UNCHECKED);
  menu.CheckMenuItem (CM_ONOFFINSTANT, MF_BYCOMMAND | bInstantValInptON  ? MF_CHECKED : MF_UNCHECKED);
  menu.CheckMenuItem (CM_ONOFFCYCLE, 	MF_BYCOMMAND | bCycleFInptON ? MF_CHECKED : MF_UNCHECKED);
													// Зажигаем или гасим индикаторы
  pNet->SetText(bInptDataON 					? sNet 	    : "");
  pInstantVal->SetText(bInstantValInptON 	? sInstant 	 : "");
  pCycle->SetText(bCycleFInptON 				? sCycleFile : "");
  DrawMenuBar();								// Перерисовываем меню с происшедшими изменениями
};	// end of function

void
TDInptWindow::ShowMessage(char* StMessage){
	StatBar->SetText(StMessage);
};

/*
 * 	Функция включения системы ввода данных и установления связи
 *		с контроллером. Запуск таймера для коммуникации с
 *		интервалом по умолчанию
 */
void
TDInptWindow::InfoInptON(){
 if (This->HConv){						// Только при наличии связи с Клиентом
		if (!bInptDataON){            // Подключене к ПК
			  bInptDataON = TRUE;		// Взводим флаг включения системы ввода данных
												// Опустим кнопку
			  ButtonG1->SetButtonState(TButtonGadget::Down);
												// Готовим текст команды в буфере для пересылки
			  strcpy(szCommandToClient, ONNETPC);// КЛЕНТУ
			  This->UpdateData();		// Передаем команду в КЛИЕНТ на запуск
												// коммуникации с контроллером
												// Обновим меню
			  PostMessage(WM_INITMENU, WPARAM(GetMenu()), 0);
		} else {
			  InfoInptOFF();           // Отключение от ПК
		};
	} else {									// При отсутствии связи с Клиентом	
												// Поднимем кнопку
		 ButtonG1->SetButtonState(TButtonGadget::Up);
	};
};	// end of function

/*
 * 	Функция выключения системы ввода данных и разрыв связи
 *		с контроллером. ( Выполняется при условии, что система ввода
 *		данных была включена ).
 *    При выключении просматриваем флаги всех подсистем
 *		( мониторинг, записи первичных протоколов мгновенных значений и
 *		циклических данных ) для их автоматического выключения.
 */
void
TDInptWindow::InfoInptOFF(){
											// Если система находится в работе
	  if (bInptDataON){				// Готовим текст команды в буфере для пересылки
		  strcpy(szCommandToClient, OFFNETPC);// КЛИЕНТУ
		  This->UpdateData();		// Передаем команду в КЛИЕНТ на останов
	  };									// коммуникации с контроллером
	  bInptDataON = FALSE;			// Сбросим флаг включения системы ввода данных
											// Поднимем кнопку
	  ButtonG1->SetButtonState(TButtonGadget::Up);
											// Проверка флагов всех подсистем
	  if (bMonitoryON)
			MonitoryONOFF();			// Выключаем мониторинг
	  if (bInstantValInptON)
			InstantValInptOFF();    // Выключаем запись протокола мгновенных значений
	  if (bCycleFInptON)
			CycleFInptOFF();       	// Выключаем запись цикл. протокола
											// Обновим меню
	  PostMessage(WM_INITMENU, WPARAM(GetMenu()), 0);
};	// end of function

/*
 *		Изменение поправки к периоду обмена данными с Клиентом (ввод данных с контроллера)
 */
void
TDInptWindow::InputCycleService(){
	pMPCTimer->ChangeCorrectUserTimer();// В диалоге меняем параметры цикла
	if (bInptDataON){
		 pMPCTimer->KillUserTimer();  	// Убиваем таймер со старыми параметрами
		 pMPCTimer->SetUserTimer(this->HWindow);   // и запускаем его уже с измененными
	};
};	// end of function

/*
 *		Функция - триггер мониторинга
 */
void
TDInptWindow::MonitoryONOFF(){
	if (!bInptDataON){                        // Мониторинг без данных не включаем
															// Поднимем кнопку
		 ButtonG3->SetButtonState(TButtonGadget::Up);
		 return;
	}
	if (!bMonitoryON){
		pTMonitoryWindow->Create();  				// Создаем диалог
		pTMonitoryWindow->MonitCycleONOFF();	//	Устанавливаем таймер цикла мониторинга
	} else {
		bMonitoryON  = FALSE;			  			// Сбросим флаг работы мониторинга
		pTMonitoryWindow->EvClose();				// Закроем окно мониторинга
	};
	ButtonG3->SetButtonState(bMonitoryON ? TButtonGadget::Down : TButtonGadget::Up);
															// Обновим меню
	PostMessage(WM_INITMENU, WPARAM(GetMenu()), 0);
};	// end of function

/*
 *		Изменение периода мониторинга
 */
void
TDInptWindow::MonitCycleService(){
	pTMonitoryWindow->MonitCycleService();
};	// end of function

/*
 *		Запуск циклических программ.
 *    При работе циклических программ просматриваем флаги всех
 *		подсистем ( мониторинг, записи первичных протоколов
 *		мгновенных значений и циклических данных) для их автоматического
 *		выключения.
 */
void
TDInptWindow::EvTimer(UINT IDEvent){
	if (!bInptDataON){						  // Если пользователь выключил систему
		bInstantValInptON  = FALSE;		  // связи с ПК то, сбросим флаги подсистем
		bCycleFInptON      = FALSE;		  // записи протоколов
	};
	if (IDEvent == pInstTimer->IDEvTimer){ // Если сработал таймер записи протоколов мгновенных значений
		 if (!bInstantValInptON){          // проверим, если система не включена,
				InstantValInptOFF();			  // Выключим протокол
		 } else {                      	  // При отработке заданного кол-ва таймеров
			  if (pInstTimer->TestUserTimer()){ // записи протоколов мгновенных значений
					AppendToInstantFile();	  // Запись в протокол мгновенных значений
			  }; 		// if (){...};
		}; 			// if (){...} else {...};
	}; 				// if (){...};
	if (IDEvent == pCFTimer->IDEvTimer){  // Если сработал таймер записи циклических данных
		if (bCycleFInptON){           	  // а система записи протоколов не включена,
			if (pCFTimer->TestUserTimer()){ // таймеров
					AppendToCycleFile();		  // Запись в цикл.протокол
			};       // if (){...};
		};          // if (){...};
	};             // if (){...};
};	// end of function

/*
 *		Выход из диалога с (без) сохранения.
 *		При выходе из диалога могут остаться
 *		включенными программы опроса и мониторинга
 */
void
TDInptWindow::EvClose(){
	 EndSession();
};	// end of function
void
TDInptWindow::EndSession(){
	if (bInptDataON){                // Если система опроса включена,
		 Show(SW_HIDE);		 			// прячем окно диалога.
		 ::SendMessage(GetParent(), WM_PAINT, 0, 0);
	} else {						 			// Если все выключено,
		 DInptWindowCreated = FALSE;  // сбросим флаг создания диалога,
		 CloseWindow(IDCANCEL);       // убиваем диалог.
	};	// if(){...} else {}
};	// end of function
/*
 *		Включение системы записи данных ветроагрегатов
 */
void
TDInptWindow::CycleFInptON(){
	  ShowMessage("");
	  if (!bInptDataON) {
		  ShowMessage("Выключена система опроса.");
													// Снимем кнопку с фиксации
		  ButtonG7->SetButtonState(TButtonGadget::Up);
		  return;                  		// Ключ 'Ветроагрегат' в 'OFF'
	  };
	  if (!bCycleFInptON){
													// Запись данных
			 AppendToCycleFile();
			 pCFTimer->SetUserTimer(this->HWindow);
			 bCycleFInptON = TRUE;    		// Взводим флаг записи данных по ВЭУ
													// Зафиксируем кнопку
			 ButtonG7->SetButtonState(TButtonGadget::Down);
	  } else {
			 CycleFInptOFF();					// Выключим протокол
	  }
	  PostMessage(WM_INITMENU, WPARAM(GetMenu()), 0);
};

/*
 *		Выключение системы записи данных ветра
 */
void
TDInptWindow::CycleFInptOFF(){
		  pCFTimer->KillUserTimer();
		  bCycleFInptON  = FALSE;			// Сбрасываем флаг записи данных по ВЭУ
													// Снимем кнопку с фиксации
		  ButtonG7->SetButtonState(TButtonGadget::Up);
};
/*
 *		Изменение периода записи протокола характеристик ВЭУ
 */
void
TDInptWindow::CycleFCycleService(){
	pCFTimer->ChangeCorrectUserTimer(); // В диалоге меняем параметры цикла
	if (bInptDataON && bCycleFInptON){	// Перезапуск РАБОТАЮЩЕЙ записи
		 pCFTimer->KillUserTimer();  		// Убиваем таймер со старыми параметрами
		 pCFTimer->SetUserTimer(this->HWindow); // и запускаем его уже с измененными
		 AppendToCycleFile();				// Цикл записи
	};
};

/*
 *		Включение системы записи мгновенных значений
 */
void
TDInptWindow::InstantValInptON(){
	  ShowMessage("");
	  if (!bInptDataON) {				// Проверим, включена ли система опроса
			ShowMessage("Выключена система опроса.");
												// Снимем кнопку с фиксации
			ButtonG5->SetButtonState(TButtonGadget::Up);
			return;
	  };
	  if (!bInstantValInptON){
			CreateOpenInstantFile();		// Открываем файл мгновенных значений
			AppendToInstantFile();			// Запись в Мгновенных значений
			pInstTimer->SetUserTimer(this->HWindow);
			bInstantValInptON = TRUE;     // взводим флаг записи мгновенных значений
													// Зафиксируем кнопку
			ButtonG5->SetButtonState(TButtonGadget::Down);
	  } else {
			InstantValInptOFF();				// Выключим протокол
	  };
	  PostMessage(WM_INITMENU, WPARAM(GetMenu()), 0);
};
/*
 *		Выключение системы записи протокола мгновенных значений
 */
void
TDInptWindow::InstantValInptOFF(){
		pInstTimer->KillUserTimer();
		bInstantValInptON  = FALSE;		// Сбрасываем флаг записи протокола мгновенных значений
													// Снимем кнопку с фиксации
		ButtonG5->SetButtonState(TButtonGadget::Up);
		CloseInstantFile();					// Закрываем файл мгновенных значений
};
/*
 *		Изменение периода записи протокола мгновенных значений
 */
void
TDInptWindow::InstantValCycleService(){
	pInstTimer->ChangeCorrectUserTimer();   // В диалоге меняем параметры цикла
	if (bInptDataON && bInstantValInptON){	// Перезапуск РАБОТАЮЩЕЙ записи
		 pInstTimer->KillUserTimer();  	   // Убиваем таймер со старыми параметрами
		 pInstTimer->SetUserTimer(this->HWindow);  // и запускаем его уже с измененными
	};
};
/*
 * ---------------------------------------------------------
 *	EoF(datainpt.cpp)
 * ----------------------------------------------------------
 */

