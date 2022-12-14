#if !defined(__ABSTFILE_H)
#define __ABSTFILE_H
/*
	 Crimea Engineering Centre
	 ????????? ????? ? 1996. ??? ????? ????????.

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
 * 	??? ???????? (???????) ????? ??? ????????? ?????? ???? ????? PX Engine
 * 	-----------------------------------------------------------------------
 */
class AbstractFile {
protected:

  BCursor*		cursorPtr;	  			// ????????? ?? ????????? ?????? BCursor
  BDatabase*	dataBase; 	  			// ????????? ?? ???????? ??
  BRecord*		incRecord;				// ????? ?????? ??????????
  void**			pDynamRec;				// ????????? ?? ???????????? ????? ??????
public:
  int isOpen;                     	// ???? - ??????? ???????? ????? ??
  Retcode 	   lastError;           // ??? ????????(??????) ?????-???????
  // ??????????? ? ?????????? ??????.
  AbstractFile(BDatabase *db);
  virtual ~AbstractFile();
  // ????????, ???????? ? ???????? ?????
  virtual Retcode create(char *tableName, FIELDNUMBER  fN, const FieldDesc* fieldD);
  virtual Retcode open  (char *tableName);
  virtual Retcode close ();

  // ??????? ??????????? ?? ?????
  virtual Retcode up();
  virtual Retcode down();
  virtual Retcode home();
  virtual Retcode end();
  virtual Retcode go(RECORDNUMBER recNum);

  virtual Retcode appendBlank();			// ???????? ?????? ?????? ? ????? ?????
  virtual Retcode clearRecordForWrite(); // ???????? ?????????? ??????
													// ??????? ???? ?? ?????????? ??????
  virtual Retcode putFieldForWrite(char* fldName, const void* anyData);
  virtual Retcode putFieldForWrite(FIELDNUMBER fldNum, const void* anyData);
  virtual Retcode append();				// ???????? ??????. ?????????? ??????
													// ? ????? ?????
  virtual Retcode delCurRecord();		// ??????? ??????? ??????
													// ???????? ???? ??????? ??????
  virtual Retcode getFieldFromCurrentRec(char* fldName, void* anyData);
  virtual Retcode getFieldFromCurrentRec(FIELDNUMBER fldNum, void* anyData);
													// ??????? ???????? ? ???? ??????? ??????
  //  virtual Retcode putFieldInCurrentRec  (char* fldName){};
  virtual Retcode update();				// ???????? ??????? ?????? ?????
													// ???????? ????? ???? ??????
  FIELDNUMBER		getFieldNumber(const char* fldName);
  RECORDNUMBER		getRecCount();			// ???????? ?????????? ???????
			 int		fLen(const char * fldName);
  //	??????? ????????????? ?????? ??????
			 Retcode CreateDynamicRec();
			 Retcode DeleteDynamicRec();
			 Retcode FromFileToDynRec();
			 Retcode FromDynRecToFile();
			 void*   fPoint(const char* fldName);
			 void*   fPoint(FIELDNUMBER fldNum);
  //	??????? ????????? ???? 'Refrence'  ? ?????? ??
			 void		IncrRefrence();
			 void		DecrRefrence();
};
#endif  // __ABSTFILE_H
/*
 * ---------------------------------------------------------
 *	EoF(abstfile.h)
 * ----------------------------------------------------------
 */

