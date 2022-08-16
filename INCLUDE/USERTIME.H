#if !defined(__USERTIME_H)
#define __USERTIME_H
/*
 * usertime.h - Handle file for  ---------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include "settimep.h"
#include <owl\owlpch.h>
#include <bwcc.h>
//----------------------------------------------------------------------------
const UINT MAX_QUANT_USER_TIMERS = 1000;	// Максимальное количество элементов списка
const UINT	DEFAULT_IDEV	 =	1;		// Номер таймера по умолчанию
const UINT	DEFAULT_VALUE	 =	600;	// Период таймера в десятках ms
const int	DEFAULT_CORRECT = 0;		// Поправка к таймеру
const int	DEFAULT_SCALE	 = SCALE_MSEC;	// Масштаб - миллисекунды
/*
 *  -------------------------------------------------------------------------
 * 	Class TUserTimerSystem
 *  -------------------------------------------------------------------------
 */
class TUserTimer {
	friend 	class TUserTimerSystem;	// Списковый класс будет другом для элементов
												// списка
	public:
	HWND		  	  myParentHWND;
	TUserTimer far* Next;				// Указатель на следующий элемент списка
	TUserTimer far* Prev;				// Указатель на предыдущий элемент списка
	UINT 			  IDEvTimer;  			// Идентификатор таймера
	UINT			  uTimerPeriod;	 	// Период таймера
	int			  iTimerCorrect;		//	Поправка на срабатывание таймера в Windows
	int  			  iMaxCountPeriod;	// Расчетное количество запусков таймера сложного интервала
	int  			  iCurCountPeriod;	// Текущий номер отработки таймера сложного интервала
	int  			  iTimerScale;			// Шкала временного интервала сложного таймера
	int  			  iTimerValue;			// Интервал времени запуска таймера в единицах шкалы
	HWND			  hSetWindow; 			// ЛН окна, в котором УСТАНОВЛЕН таймер
	TWindow*		  tCreateWindow;		// Указатель на окно, в котором СОЗДАН таймер

	// C o n s t r u c t o r s
	TUserTimer();							// Конструктор по умолчанию
												// Основной конструктор
	TUserTimer(TWindow*, int TimerScale = SCALE_SEC,
								int TimerValue = 2,
								int TimerCorrect = 0);
	TUserTimer(TUserTimer&);			// Конструктор копии
	TUserTimer& operator= (TUserTimer&);	// Операция присваивания

	//  		М е т о д ы     к л а с с а

	// Инициализация пользовательского таймера
	void InitUserTimer(int, unsigned int, int);
	// Установка пользовательского таймера
	void SetUserTimer(HWND, int, unsigned int,int);
	void SetUserTimer(HWND);
	// Сброс системного таймера, обслуживающего пользовательский
	void KillUserTimer();
	// Проверка срабатывания сложного таймера
	BOOL TestUserTimer();
	// Проверка срабатывания сложного таймера
	// с последующим запуском нужной программы
	BOOL TestUserTimerAndRun();
	// Перевод из единиц таймера в единицы времени:
	//	Изменение в диалоге периода срабатывания пользовательского таймера
	BOOL ChangeUserTimerPeriod();
	//	Изменение в диалоге периода срабатывания пользовательского таймера
	//	с последующим запуском некоторых программ обработки
	BOOL ChangeUTPeriodAndRun(void (*pFunction)());
	BOOL ChangeUTPeriodAndRun(void (*pFunction)(TWindow*),
									  TWindow*);
	//	Изменение в диалоге ТОЛЬКО ПОПРАВКИ к таймеру пользователя
	BOOL ChangeCorrectUserTimer();
};
/*
 *  -------------------------------------------------------------------------
 * 	Class TUserTimerSystem
 *  -------------------------------------------------------------------------
 */
void ScaleAndPeriodToValue(TUserTimer& UserTimer);
void ScaleAndValueToPeriod(TUserTimer& UserTimer);

class TUserTimerSystem {
	public:
		UINT START_TIMER_NUMBER; 	// Начальное значение идентификатора
											// пользовательского таймера
		// C o n s t r u c t o r
		TUserTimerSystem(UINT StartTimerNumber){
			START_TIMER_NUMBER = StartTimerNumber;
			InitUserTimersList(); 	// Инициализация списка таймеров
		};
		// D e s t r u c t o r
//		~TUserTimerSystem();
		// Создание ПУСТОГО списка пользовательских таймеров
		// ( создаются только ГОЛОВА и ХВОСТ списка )
		void InitUserTimersList();	// Инициализация списка таймеров
		// Добавление элемента в ПУСТОЙ список пользовательских таймеров
		// МЕЖДУ ГОЛОВОЙ и ХВОСТОМ списка или в КОНЕЦ непустого списка, если
		// список не имеет ДЫР, т.е. нумерация таймеров спложная, или же
		// перед первым элементом, следующим сразу же после ДЫРЫ
		TUserTimer* AddUserTimer(TWindow* tCreatedWindow);
		// Удаление элемента из списка и уничтожение его
		void DetachAndDelete(TUserTimer* pUserTimer);
		// Получения указателей соответственно на ГОЛОВУ и ХВОСТ списка таймеров
		TUserTimer* PeekHead(){return &Head;};
		TUserTimer* PeekTail(){return &Tail;};
		// Проверка наличия в списке хотя бы одного элемента
		BOOL isEmpty();
		// Удаление всех элементов из списка пользовательских таймеров
		void Flush();
		// Кратность периодов таймеров
		int MultipleUTPeriod(TUserTimer* pTUserTimerBig,
											  TUserTimer* pTUserTimerSmall);
	protected:
		TUserTimer	Head;					// Голова списка таймеров пользователя
		TUserTimer	Tail;					// Хвост  списка таймеров пользователя
		int ItemsInContainer;			// Количество элементов в списке

		// Добавление элемента в НАЧАЛО (сразу после "ГОЛОВЫ") списка пользовательских таймеров,
		TUserTimer* AddUserTimerAtHead(TWindow* tCreatedWindow);
		// Добавление элемента в конец (перед "ХВОСТОМ") списка пользовательских таймеров,
		// если номера смежных таймеров в списке отличаются не больше чем на ЕДИНИЦУ
		TUserTimer* AddUserTimerAtTail(TWindow* tCreatedWindow);
		// Добавление элемента в список элемента
		// пользовательских таймеров перед элементом,
		// указатель на который передается в качестве параметра
		TUserTimer* AddUserTimerBefor(TWindow* tCreatedWindow, TUserTimer* NextTimer);
		// Поиск элемента - пользовательского таймера
		// номер которого отличается от номера предыдущего
		// БОЛЕЕ, чем на ЕДИНИЦУ (последнего после "ДЫРЫ")
		TUserTimer* FirstThatAfterHole();
};
#endif  // __USERTIME_H
/*
 * ---------------------------------------------------------
 *	EoF(usertime.h)
 * ----------------------------------------------------------
 */

