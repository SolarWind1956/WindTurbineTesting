/*
 * WritProt - Write primary protocols module -------------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 * WritProt - 	модуль записи различных протоколов опроса датчиков ВЭУ
 *             а также протоколов испытаний
 */
#include "commglob.h"
#include "database.h"
#include "cyclfile.h"
/*
 * 		Операции чтения из циклического файла и записи (добавления)
 *			информации в него
 */
BOOL ReadFromCycleFile() 	{
	// Помещаем значения полей в буфера пользователя
	pT_CYCF->getFieldFromCurrentRec("CyclePC"   ,   (void*)&adwInputData1[25]);
	pT_CYCF->getFieldFromCurrentRec("AtmosPress",	(void*)&adwInputData1[0]);
	pT_CYCF->getFieldFromCurrentRec("AirTemper" ,	(void*)&adwInputData1[2]);
	pT_CYCF->getFieldFromCurrentRec("WindSpeed" ,	(void*)&adwInputData1[6]);
	pT_CYCF->getFieldFromCurrentRec("DirectWnd" ,	(void*)&adwInputData1[9]);
	pT_CYCF->getFieldFromCurrentRec("AzimutHd1" ,	(void*)&adwInputData1[12]);
	pT_CYCF->getFieldFromCurrentRec("AzimutHd2" ,	(void*)&adwInputData1[13]);
	pT_CYCF->getFieldFromCurrentRec("NetPower"  ,	(void*)&adwInputData1[17]);
	pT_CYCF->getFieldFromCurrentRec("ReaPower"  ,	(void*)&adwInputData1[19]);
	pT_CYCF->getFieldFromCurrentRec("CurrentMotor",	(void*)&adwInputData1[21]);

	FIELDNUMBER fnMIN, fnMAX;
	fnMIN = pT_CYCF->getFieldNumber("RSpeed1");
	fnMAX = pT_CYCF->getFieldNumber("RSpeed20");
	for (FIELDNUMBER i = fnMIN; i <= fnMAX; i++)
		pT_CYCF->getFieldFromCurrentRec(i, (void*)&adwInputData2[i - fnMIN]);
	fnMIN = pT_CYCF->getFieldNumber("Stock1St1");
	fnMAX = pT_CYCF->getFieldNumber("Stock1St20");
	for (/*FIELDNUMBER*/ i = fnMIN; i <= fnMAX; i++)
		pT_CYCF->getFieldFromCurrentRec(i, (void*)&adwInputData3[i - fnMIN]);
	fnMIN = pT_CYCF->getFieldNumber("Stock2St1");
	fnMAX = pT_CYCF->getFieldNumber("Stock2St20");
	for (/*FIELDNUMBER*/ i = fnMIN; i <= fnMAX; i++)
		pT_CYCF->getFieldFromCurrentRec(i, (void*)&adwInputData4[i - fnMIN]);
	return TRUE;
};

BOOL AppendToCycleFile() 	{
	static int CyclePCForCycleFile = 0;    // Индекс цикла ПК для проверки
														// Проверка на повторяющиеся записи
	if (CyclePCForCycleFile != (int)adwInputData1[25]){
		PrepareForWriteCycleOrInstant(pT_CYCF);
		pT_CYCF->addAsCycle();              // Запомним индекс прошедшего цикла ПК
		CyclePCForCycleFile = (int)adwInputData1[25];
	}; // if(){...};
	return TRUE;
};
BOOL AppendToInstantFile() 	{
	static int CyclePCForInstant = 0;      // Индекс цикла ПК для проверки
														// Проверка на повторяющиеся записи
	if (CyclePCForInstant != (int)adwInputData1[25]){
		PrepareForWriteCycleOrInstant(pT_INST);
		pT_INST->append();                  // Запомним индекс прошедшего цикла ПК
		CyclePCForInstant = (int)adwInputData1[25];
	}; // if(){...};
	return TRUE;
};
BOOL PrepareForWriteCycleOrInstant(AbstractFile* pFile){
	char timebuf[9];	_strtime(timebuf);   // Определяем текущее время
	char datebuf[9];  _strdate(datebuf);	// Определим текущую дату
	pFile->clearRecordForWrite();				// Очищаем запись под данные
// Помещаем значения полей в буфера пользователя
	int One = 1;
	pFile->putFieldForWrite("Index"     , 	(void*)&One);
	pFile->putFieldForWrite("RecDate"   , 	(void*)&datebuf);
	pFile->putFieldForWrite("RecTime"   , 	(void*)&timebuf);
	pFile->putFieldForWrite("CyclePC"   , 	(void*)&adwInputData1[25]);
	pFile->putFieldForWrite("AtmosPress", 	(void*)&adwInputData1[0]);
	pFile->putFieldForWrite("AirTemper" , 	(void*)&adwInputData1[2]);
	pFile->putFieldForWrite("WindSpeed" , 	(void*)&adwInputData1[6]);
	pFile->putFieldForWrite("DirectWnd" , 	(void*)&adwInputData1[9]);
	pFile->putFieldForWrite("AzimutHd1" , 	(void*)&adwInputData1[12]);
	pFile->putFieldForWrite("AzimutHd2" , 	(void*)&adwInputData1[13]);
	pFile->putFieldForWrite("NetPower"  , 	(void*)&adwInputData1[17]);
	pFile->putFieldForWrite("ReaPower"  , 	(void*)&adwInputData1[19]);
	pFile->putFieldForWrite("CurrentMotor",(void*)&adwInputData1[21]);
	FIELDNUMBER fnMIN, fnMAX;
	fnMIN = pFile->getFieldNumber("RSpeed1");
	fnMAX = pFile->getFieldNumber("RSpeed20");
	for (FIELDNUMBER i = fnMIN; i <= fnMAX; i++)
		pFile->putFieldForWrite(i, (void*)&adwInputData2[i - fnMIN]);
	fnMIN = pFile->getFieldNumber("Stock1St1");
	fnMAX = pFile->getFieldNumber("Stock1St20");
	for (/*FIELDNUMBER*/ i = fnMIN; i <= fnMAX; i++)
		pFile->putFieldForWrite(i, (void*)&adwInputData3[i - fnMIN]);
	fnMIN = pFile->getFieldNumber("Stock2St1");
	fnMAX = pFile->getFieldNumber("Stock2St20");
	for (/*FIELDNUMBER*/ i = fnMIN; i <= fnMAX; i++)
		pFile->putFieldForWrite(i, (void*)&adwInputData4[i - fnMIN]);
	return TRUE;
};
/*
 * ---------------------------------------------------------
 *	EoF(writprot.cpp)
 * ----------------------------------------------------------
 */


