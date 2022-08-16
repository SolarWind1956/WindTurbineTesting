#if !defined(__ABSTFILE_H)
#define __ABSTFILE_H
/*
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    testsyst.exe & powercha.exe Applications
	 FILE:         abstfile.h
	 AUTHORS:      Ludmila M. Baybus & Nina Y. Mezhenina


	 OVERVIEW
	 ========
*/
#include <bengine.h>
#include <bdatabas.h>
#include <bcursor.h>
#include <brecord.h>
#include <pxengine.h>
/*
 *		AbstractFile -----------------------------------------------------------------------
 * 	Это основной (базовый) класс для различных файлов базы данны PX Engine
 * 	-----------------------------------------------------------------------
 */
class AbstractFile {
protected:

  BCursor*		cursorPtr;	  			// Указатель на экземпляр класса BCursor
  BDatabase*	dataBase; 	  			// Указатель на активную БД
  BRecord*		incRecord;				// Буфер записи встроенный
  void**			pDynamRec;				// Указатель на динамический буфер записи
public:
  int isOpen;                     	// флаг - признак открытия файла БД
  Retcode 	   lastError;           // код возврата(ошибки) члена-функции
  // Конструктор и деструктор класса.
  AbstractFile(BDatabase *db);
  virtual ~AbstractFile();
  // Создание, открытие и закрытие файла
  virtual Retcode create(char *tableName, FIELDNUMBER  fN, const FieldDesc* fieldD);
  virtual Retcode open  (char *tableName);
  virtual Retcode close ();

  // Функции перемещения по файлу
  virtual Retcode up();
  virtual Retcode down();
  virtual Retcode home();
  virtual Retcode end();
  virtual Retcode go(RECORDNUMBER recNum);

  virtual Retcode appendBlank();			// Добавить ПУСТУЮ запись в конец файла
  virtual Retcode clearRecordForWrite(); // Очистить встроенную запись
													// Занести поле во встроенную запись
  virtual Retcode putFieldForWrite(char* fldName, const void* anyData);
  virtual Retcode putFieldForWrite(FIELDNUMBER fldNum, const void* anyData);
  virtual Retcode append();				// Добавить ЗАПОЛН. встроенную запись
													// в конец файла
  virtual Retcode delCurRecord();		// Удалить текущую запись
													// Получить поле текущей записи
  virtual Retcode getFieldFromCurrentRec(char* fldName, void* anyData);
  virtual Retcode getFieldFromCurrentRec(FIELDNUMBER fldNum, void* anyData);
													// Занести значение в поле текущей записи
  //  virtual Retcode putFieldInCurrentRec  (char* fldName){};
  virtual Retcode update();				// Обновить текущую запись файла
													// Получить номер поля записи
  FIELDNUMBER		getFieldNumber(const char* fldName);
  RECORDNUMBER		getRecCount();			// Получить количество записей
			 int		fLen(const char * fldName);
  //	Функции динамического буфера записи
			 Retcode CreateDynamicRec();
			 Retcode DeleteDynamicRec();
			 Retcode FromFileToDynRec();
			 Retcode FromDynRecToFile();
			 void*   fPoint(const char* fldName);
			 void*   fPoint(FIELDNUMBER fldNum);
  //	Функции изменения поля 'Refrence'  в файлах БД
			 void		IncrRefrence();
			 void		DecrRefrence();
};
#endif  // __ABSTFILE_H
/*
 * ---------------------------------------------------------
 *	EoF(abstfile.h)
 * ----------------------------------------------------------
 */

