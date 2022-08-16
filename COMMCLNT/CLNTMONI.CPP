/*
 * clntmoni - Data input system module -------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * Cl[i]n[e]tMoni[tory] - модуль, предназначенный для отображения информации,
 * полученной с контроллера Info 73.
 * Информация выводится  в нескольких окнах мониторинга с
 * периодичностью, определенной таймером мониторинга.
 */
#include "commglob.h"
#include "monitory.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TMonitoryWindow
 *  -------------------------------------------------------------------------
 */
DEFINE_RESPONSE_TABLE2(TMonitoryWindow, TMonitoringWindow, TWindow)
  EV_WM_TIMER,
  EV_WM_CLOSE,
END_RESPONSE_TABLE;

TMonitoryWindow::TMonitoryWindow(TWindow* parent)
	: TMonitoringWindow(parent), TWindow(parent)
{																			// Окна мониторинга
//  pTStatusWindow= new TBInfoBox   (this, IDW_STATUSWND); // Статусная строка
};

/*
 *		Функция - триггер включения мониторинга
 */
void
TMonitoryWindow::MonitCycleONOFF() {
	if (!bMonitoryON){                           // Если оператор еще НЕ включил мониторинг
		pMonTimer->SetUserTimer(this->HWindow);	//	Устанавливаем таймер цикла мониторинга
		bMonitoryON = TRUE;								// Взводим флаг ВКЛЮЧЕНИЯ мониторинга
		MonitONECycle();									// И ОДИН раз отображаем информацию
	};
};
/*
 * 	Функция одного (и каждого) цикла мониторинга
 */
void
TMonitoryWindow::MonitONECycle(){
															// Для окна 1
	ShowIntegersInMonitBox(1,
							"Барометр. давление", 		BuffTransfFrom.BuffFromPC.awInputData1[0],  0,
							"Температура воздуха", 		BuffTransfFrom.BuffFromPC.awInputData1[2],  0,
							"Направление ветра", 		BuffTransfFrom.BuffFromPC.awInputData1[9],  0,
							"Азимут головки ВЭУ", 		BuffTransfFrom.BuffFromPC.awInputData1[12], 1,
							"Азимут головки ВЭУ",	 	BuffTransfFrom.BuffFromPC.awInputData1[13], 2,
							"Цикл опроса", 				BuffTransfFrom.BuffFromPC.awInputData1[25], 0);
															// Для окна 2
	ShowIntegersInMonitBox(2,
							"Скорость ветра", BuffTransfFrom.BuffFromPC.awInputData1[6],  0);
															// Для окна 3
	ShowIntegersInMonitBox(3,
							"Активная мощность", 	BuffTransfFrom.BuffFromPC.awInputData1[17],  0,
							"Реактивная мощность", 	BuffTransfFrom.BuffFromPC.awInputData1[19],  0,
							"Ток двигателя раскр", 	BuffTransfFrom.BuffFromPC.awInputData1[21],  0);
															// Для окна 4
	ShowIntegersInMonitBox(4,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[0],  1,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[1],  2,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[2],  3,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[3],  4,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[16], 17,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[17], 18,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[18], 19,
							"Скорость в/колеса", BuffTransfFrom.BuffFromPC.awInputData2[19], 20);
															// Для окна 5
	ShowIntegersInMonitBox(5,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[0],  1,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[1],  2,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[2],  3,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[3],  4,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[4],  5,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[5],  6,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[6],  7,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[7],  8);
															// Для окна 6
	ShowIntegersInMonitBox(6,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[12], 13,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[13], 14,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[14], 15,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[15], 16,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[16], 17,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[17], 18,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[18], 19,
							"Ход штока 1", BuffTransfFrom.BuffFromPC.awInputData3[19], 20);
															// Для окна 7
	ShowIntegersInMonitBox(7,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[0],  1,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[1],  2,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[2],  3,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[3],  4,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[4],  5,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[5],  6,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[6],  7,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[7],  8);
															// Для окна 8
	ShowIntegersInMonitBox(8,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[12], 13,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[13], 14,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[14], 15,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[15], 16,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[16], 17,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[17], 18,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[18], 19,
							"Ход штока 2", BuffTransfFrom.BuffFromPC.awInputData4[19], 20);
};
/*
 *		Завершение работы мониторинга по нажатию кнопки
 *		или по сбою в системе, но не по нахатию 'Alt+F4'
 */
void
TMonitoryWindow::EvClose(){
	if (!bMonitoryON){						// Если мониторинг выключен
		pMonTimer->KillUserTimer();	  	// Уничтожение таймера
		MonitONECycle();      			  	// Последняя отработка цикла мониторинга
		CloseWindow(IDCANCEL);
	};
};
/*
 *		Изменение периода опроса и мониторинга
 */
void
TMonitoryWindow::MonitCycleService(){
	pMonTimer->ChangeUserTimerPeriod();	// В диалоге меняем параметры цикла
	if (bMonitoryON){         				// Перезапуск РАБОТАЮЩЕГО мониторинга
		 pMonTimer->KillUserTimer();  	// Убиваем таймер со старыми параметрами
		 pMonTimer->SetUserTimer(this->HWindow);   // и запускаем его уже с измененными
		 MonitONECycle();	 	 		 		// Цикл мониторинга
	};
};
/*
 *		Циклический перезапуск мониторинга
 */
void
TMonitoryWindow::EvTimer(UINT IDEvent)
{
	if (IDEvent == pMonTimer->IDEvTimer) {			// Если сработал таймер мониторинга
		 if (!bMonitoryON){                       // Проверим, может уже успели выключить мониторинг
			  EvClose();									// Тогда завершаем его работу
		 } else if (pMonTimer->TestUserTimer())	// Если пользоват.таймер готов
			  MonitONECycle(); 				 			// отрабатывается цикл мониторинга
	};
};
/*
 * ---------------------------------------------------------
 *	EoF(clntmoni.cpp)
 * ---------------------------------------------------------
 */