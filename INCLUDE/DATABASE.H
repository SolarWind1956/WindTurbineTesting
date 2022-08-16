#if !defined(__DATABASE_H)
#define __DATABASE_H
/*
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    powercha.exe Application
	 FILE:         database.h
	 AUTHORS:      L. Baybus & N. Mezhenina


	 OVERVIEW
	 ========
*/
#include <time.h>
#include <sys\timeb.h>
#include <dos.h>
#include <memory.h>
#include <mem.h>

#include <bengine.h>
#include <bdatabas.h>
#include <bcursor.h>
#include <brecord.h>

#include "cyclfile.h"
#include "abstfile.h"
/*
 *-------------------------------------------------------------
 *		Прототипы функций
 *-------------------------------------------------------------
 */
BOOL		InitPXEngine(const char* appName);	// Инициализация Borland Paradox
															// Engine & DataBase FrameWork
BOOL		OffPXEngine();								// и выключение этого механизма
BOOL		CreateTestSystTables();					//
BOOL		DeleteTestSystTables();					//
BOOL		CreatePowerCharTables();				//
BOOL		DeletePowerCharTables();				//

BOOL		CreateOpenCycleFile(),    CloseCycleFile();
BOOL		CreateOpenInstantFile(),  CloseInstantFile();
BOOL		CreateOpenTestProtocol(), CloseTestProtocol();

BOOL		CreateOpenMContrls(),	 			CloseMContrls();
BOOL		CreateOpenModuls(int, char*), 	CloseModuls();
BOOL		CreateOpenChanIO(int), 				CloseChanIO();
BOOL		CreateOpenTransduc(), 				CloseTransduc();
BOOL		CreateOpenTSensors(), 				CloseTSensors();
BOOL		CreateOpenTestJrnlFile(), 			CloseTestJrnlFile();
/*
 *				Paradox Engine 3.0 DataBase FrameWork
 *
 */
extern BEngine* 			pPXEngineMachine;	// Указатель на PXEngine information class
extern BDatabase* 		pPXDataBase;		// Указатель на PX BDatabase класс,
														// концепцию реляционной БД
// Указатели на классы таблиц файлов базы данных
extern TCycleFile*  	   pT_CYCF; 			// Указатель на таблицу циклического
extern AbstractFile*		pT_INST;				// - " - на класс мгновенных значений
														// показаний датчиков
extern AbstractFile*		pT_XTST;				// Указатель на таблицу файла
														// протокола испытаний
extern AbstractFile*		pT_CONT;				// - " - на класс типов контроллеров
extern AbstractFile*		pT_MDIO;				// - " - на класс типов модулей ввода-вывода
extern AbstractFile*		pT_CHNL;				// - " - на класс каналов ввода/вывода
extern AbstractFile*		pT_TRNS;				// - " - на класс преобразователей
extern AbstractFile*		pT_SENS;				// - " - на класс датчиков

extern AbstractFile*		pT_TJNL;				// - " - на класс журнального файла
#endif  // __DATABASE_H
/*
 * ---------------------------------------------------------
 *	EoF(database.h)
 * ----------------------------------------------------------
 */

