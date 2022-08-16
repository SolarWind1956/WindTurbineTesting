#if !defined(__CYCLFILE_H)
#define __CYCLFILE_H
/*
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    powercha.exe Application
	 FILE:         cyclfile.h
	 AUTHORS:      L. Baybus & N. Medjenina


	 OVERVIEW
	 ========
*/
#include "abstfile.h"
BOOL ReadFromCycleFile();
BOOL AppendToCycleFile();
BOOL AppendToInstantFile();
BOOL PrepareForWriteCycleOrInstant(AbstractFile* pFile);
/*
 *		TCycleFile --------------------------------------------------------
 *
 *		Класс таблицы циклического файла
 *
 *    -------------------------------------------------------------------
 */
class TCycleFile : public AbstractFile {
public:
	// Constructor.
	TCycleFile(BDatabase *db);
	// Destructor.
//	~TCycleFile();
	// Общедоступные методы класса
	Retcode 	addAsCycle();				// Добавить запись в режиме
												// именно циклического файла
	Retcode 	InitCyclePart();			// Инициализация циклической части
	BOOL     isEmpty  () const { return CFRecCount ? FALSE : TRUE; }
	long     Count		() const { return CFRecCount; }
protected:

	long     CFRecCount;					// Количество записей в циклическом файле
	long 		CFHeadOfFile;				// Начало (ГОЛОВА) циклического файла
	long		CFTailOfFile;	         // Конец  (ХВОСТ)  циклического файла
};
#endif  // __CYCLFILE_H
/*
 * ---------------------------------------------------------
 *	EoF(cyclfile.h)
 * ----------------------------------------------------------
 */

