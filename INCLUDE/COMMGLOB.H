#if !defined(__COMMGLOB_H)
#define __COMMGLOB_H
/*
 * CommGlob.h	-	файл общей области глобальных массивов, переменных и т.д.
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */

#include "usertime.h"
#define ERR_FILE_NOT_FOUND 2L				// Константа ошибки нахождения файла
													// при выполнении функции ShellExecute
													// для Клиентного приложения		
/*
 * 	Глобальные переменные по "Нижнему" уровню
 */
/*
 *		Переменные для приема данных с контроллера
 */
const unsigned char N_INPUT_DATA   = 26;	// Число данных в пакете ПРИЕМА
const unsigned char N_INPUT_DATA_BYTE    = (N_INPUT_DATA + 2) * 2;
struct TypeFromPC {
													// Логический канал N 39
	int  awInputData1  [N_INPUT_DATA];	// Первый    массив данных
													// Логический канал N 40
	int  awInputData2  [N_INPUT_DATA];	// Второй    массив данных
													// Логический канал N 41
	int  awInputData3  [N_INPUT_DATA];	// Третий    массив данных
													// Логический канал N 42
	int  awInputData4  [N_INPUT_DATA];	// Четвертый массив данных
};
extern TypeFromPC BuffFromPC;

union TypeBFPC {
  char       aBuffTransf[sizeof(TypeFromPC)];
  TypeFromPC BuffFromPC;
};
extern TypeBFPC BuffTransfFrom;

/*
 *		Массивы для передачи данных в контроллер
 */
const unsigned char N_CONTROL_DATA = 25;	// Число данных в пакете УПРАВЛЯЮЩИХ ДАННЫХ
const unsigned char N_CONTROL_DATA_BYTE  = (N_CONTROL_DATA + 2) * 2;
struct TypeToPC{
	int awControlData1	[N_CONTROL_DATA];	// 1-й Массив данных для управления
														// Логический канал N 60
	int awControlData2	[N_CONTROL_DATA];	// 2-й Массив данных для управления
														// Логический канал N 61
};
extern TypeToPC BuffToPC;

union TypeBTPC {
  TypeToPC BuffToPC;
  char     aBuffTransf[sizeof(TypeToPC)];
};
extern TypeBTPC BuffTransfTo;

const  unsigned char N_COMMAND_DATA = 20; 		// Число данных в пакете КОМАНДНЫХ ДАННЫХ
extern char szCommandToClient[N_COMMAND_DATA];	// Управляющее сообщение для клиента
extern char szCommandToServer[N_COMMAND_DATA];	// Управляющее сообщение для сервера

/*
 * 	Глобальные переменные по "Верхнему" уровню
 */
extern int CodeController;			 		// Текущий работающий контроллер

extern double adwInputData1  [N_INPUT_DATA];		// Первый    массив преобразованных данных
extern double adwInputData2  [N_INPUT_DATA];		// Вторй     массив преобразованных данных
extern double adwInputData3  [N_INPUT_DATA];		// Третий    массив преобразованных данных
extern double adwInputData4  [N_INPUT_DATA];		// Четвертый массив преобразованных данных

extern int aiControlData1	  [N_CONTROL_DATA];	// Первый массив управляющей информации
extern int aiControlData2	  [N_CONTROL_DATA];  // Второй массив управляющей информации
													// Массивы усредненных значений
extern double adwAvergData1  [N_INPUT_DATA];		// Первый     массив усредненных данных
extern double adwAvergData2  [N_INPUT_DATA];		// Вторй      массив усредненных данных
extern double adwAvergData3  [N_INPUT_DATA];		// Третий     массив усредненных данных
extern double adwAvergData4  [N_INPUT_DATA];		// Четвертый  массив усредненных данных
extern double adwAddAvergData[N_INPUT_DATA];		// Добавочный массив усредненных данных

extern BOOL bInptDataON;	// Флаг работы коммуникации
extern BOOL bMonitoryON;	// Флаг работы мониторинга
									// Флаг записи протоколов мгновенных значений 
extern BOOL bInstantValInptON;
extern BOOL bCycleFInptON;	// Флаг записи протоколов циклических данных
extern BOOL bViewDataON;	//	Флаг работы подсистемы отображения данных
extern BOOL bTestDataON;	// Флаг работы испытания
extern BOOL bWriteProtON;  // Флаг записи протоколов испытания
/*
 *		Таймеры
 */
												// Начальные номера пользовательских
												// таймеров в различных приложениях
												// системы
const  UINT SERVER_START_NUMBER_UT = 500;
const  UINT CLIENT_START_NUMBER_UT = 500;
const  UINT TESTS_START_NUMBER_UT  = 750;

extern TUserTimerSystem* lpTimersList;	// Список пользовательских таймеров
												// Указатели на ПОЛЬЗОВАТЕЛЬСКИЕ таймеры
extern TUserTimer*	pMPCTimer;		//	Tаймер для опроса контроллера
extern TUserTimer*	pMonTimer; 		// Таймер для отображения данных в мониторинге
extern TUserTimer*	pCFTimer;      // Таймер для доступа к циклическому файлу
extern TUserTimer*	pInstTimer;		// Таймер для записи мгновенных значений
extern TUserTimer*	pTimeTimer;		// Таймер для отображения текущего времени
extern TUserTimer*	pViewTimer;		// Таймер для отображения графиков
extern TUserTimer*	pTPWTimer;		// Таймер для записи протокола испытания
extern TUserTimer*   pPowerTimer;	// Таймер для отображения МХ
extern TUserTimer*   pStocksTimer;	// Таймер для отображения характеристики штоков
/*
 *				Переменные и константы для аппарата DDE
 */
#define Service1  "DDEML_Data_Server"
#define Topic1    "Info73_Data"
#define Item1     "DataFromInfo73"
#define Item2     "DataToInfo73"
#define Item3     "CommandToClient"
#define Item4     "CommandToServer"

#define ONNETPC    "OnNetPC"
#define OFFNETPC   "OffNetPC"
#define PCNETOK    "PCNetIsOk"
#define PCNETFAIL  "PCNetIsFail"
#define PCNETCRASH "PCNetIsCrash"
/*
 * ---------------------------------------------------------
 *	EoF(commglob.h)
 * ----------------------------------------------------------
 */
#endif  // __COMMGLOB_H
