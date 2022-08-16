/*
 * TestMath -
 *
 * Authors: Sergey E. Heckel & Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * TestMath -  модуль технологической обработки полученной информации
 *	из циклического файла для помещения в файл БД и для отображения
 */
#include <windows.h>
#include <math.h>
#include <time.h>

#include "commglob.h"
#include "database.h"
double StepAverageValue   (double StepNumber,	// Номер шага усреднения
									double PrecedAverage,// Предудущее Cреднее
									double CurrentValue);// Текущее значение
																// усредняемой величины
void	 AvergProcess(double StepNumber);
double AvergRotorSpeed, AvergStockStrk1, AvergStockStrk2;
int    ReadFromCycleFile();
//----------------------------------------------------------------------------
/*
 *		Предварительные технологические расчеты для отображения
 *						во временном окне мониторинга
 *		Исходной является информация взятая из буфера циклического файла и
 *		усредняемая от шага к шагу
 */
BOOL ReadAndAvergProcess(double StepNumber) {
	static int CyclePCForCycleFile = 0; // Индекс цикла ПК для проверки

	pT_CYCF->home();		  					// Чтение информации из начальной записи
	ReadFromCycleFile();
													// Проверка на повторяющиеся записи
													// и выход, если записи повторились
	if (CyclePCForCycleFile == (int)adwInputData1[25])
		return FALSE;				 			// При повторном чтении одной и той-же записи
													// Запомним индекс прошедшего цикла ПК
	CyclePCForCycleFile = (int)adwInputData1[25];
													// Атмосферное давление
	adwAvergData1[0] = StepAverageValue(StepNumber,
													adwAvergData1[0],
													adwInputData1[0]);
													// Температура воздуха
	adwAvergData1[2] = StepAverageValue(StepNumber,
													adwAvergData1[2],
													adwInputData1[2]);
													// Скорость ветра
	adwAvergData1[6] = StepAverageValue(StepNumber,
													adwAvergData1[6],
													adwInputData1[6]);
													// Направление ветра
	adwAvergData1[9] = StepAverageValue(StepNumber,
													adwAvergData1[9],
													adwInputData1[9]);
													// Азимут головки - датчик 1
	adwAvergData1[12] = StepAverageValue(StepNumber,
													adwAvergData1[12],
													adwInputData1[12]);
													// Азимут головки - датчик 2
	adwAvergData1[13] = StepAverageValue(StepNumber,
													adwAvergData1[13],
													adwInputData1[13]);
													// Мощность активная
	adwAvergData1[17] = StepAverageValue(StepNumber,
													adwAvergData1[17],
													adwInputData1[17]);
													// Мощность реактивная
	adwAvergData1[19] = StepAverageValue(StepNumber,
													adwAvergData1[19],
													adwInputData1[19]);
											// Скорость вращения ветроколеса
	{										// и оба датчика хода штока СУВК
		AvergRotorSpeed = AvergStockStrk1 = AvergStockStrk2 = 0.0;
		for (char i = 0; i < 20; i++){	// Усредним 100 мс значе                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                		adwAvergData4[0] = StepAverageValue(StepNumber,
														adwAvergData4[0],
														AvergStockStrk2);
	};
	return TRUE;                // При чтении новой записи
};
/*
 * 		Операции чтения из файла протокола испытания и записи (добавления)
 *			информации в него
 */
/*
 *		функция записи усредненной информации в файл протокола испытаний
 *    прямо в буфер записи файла. После этого запись помещается в конец
 *		файла - протокола испытаний
 */
int
AppendAvergToTP(void){
	char timebuf[9];	_strtime(timebuf);   	// Определяем текущее время
	pT_XTST->clearRecordForWrite();		// Очищаем запись под данные
															// Помещаем данные в чистую запись
	pT_XTST->putFieldForWrite("RecTime"     , (void*)timebuf);
	pT_XTST->putFieldForWrite("AAtmosPress" , (void*)&adwAvergData1[0]);
	pT_XTST->putFieldForWrite("AAirTemper"  ,	(void*)&adwAvergData1[2]);
	pT_XTST->putFieldForWrite("AWindSpeed"  ,	(void*)&adwAvergData1[6]);
	pT_XTST->putFieldForWrite("ADirectWnd"  ,	(void*)&adwAvergData1[9]);
	pT_XTST->putFieldForWrite("AAzimutHd1"  ,	(void*)&adwAvergData1[12]);
	pT_XTST->putFieldForWrite("AAzimutHd2"  ,	(void*)&adwAvergData1[13]);
	pT_XTST->putFieldForWrite("ANetPower"   ,	(void*)&adwAvergData1[17]);
	pT_XTST->putFieldForWrite("AReaPower"   ,	(void*)&adwAvergData1[19]);
	pT_XTST->putFieldForWrite("ARSpeed"     , (void*)&adwAvergData2[0]);
	pT_XTST->putFieldForWrite("AStock1St"   ,	(void*)&adwAvergData3[0]);
	pT_XTST->putFieldForWrite("AStock2St"   ,	(void*)&adwAvergData4[0]);

	pT_XTST->append();                 // Добавляем заполненную запись в файл
	return 1;
};
int
ReadFromTP(){
	// Помещаем значения полей в буфера пользователя
	pT_XTST->getFieldFromCurrentRec("AAtmosPress", (void*)&adwAvergData1[0]);
	pT_XTST->getFieldFromCurrentRec("AAirTemper" ,	(void*)&adwAvergData1[2]);
	pT_XTST->getFieldFromCurrentRec("AWindSpeed" ,	(void*)&adwAvergData1[6]);
	pT_XTST->getFieldFromCurrentRec("ADirectWnd" ,	(void*)&adwAvergData1[9]);
	pT_XTST->getFieldFromCurrentRec("AAzimutHd1" ,	(void*)&adwAvergData1[12]);
	pT_XTST->getFieldFromCurrentRec("AAzimutHd2" ,	(void*)&adwAvergData1[13]);
	pT_XTST->getFieldFromCurrentRec("ANetPower"  ,	(void*)&adwAvergData1[17]);
	pT_XTST->getFieldFromCurrentRec("AReaPower"  ,	(void*)&adwAvergData1[19]);
	pT_XTST->getFieldFromCurrentRec("ARSpeed"    ,	(void*)&adwAvergData2[0]);
	pT_XTST->getFieldFromCurrentRec("AStock1St"  ,	(void*)&adwAvergData3[0]);
	pT_XTST->getFieldFromCurrentRec("AStock2St"  ,	(void*)&adwAvergData4[0]);
	return 1;
};

/*
 * ---------------------------------------------------------
 *	EoF(TetsMath.cpp)
 * ----------------------------------------------------------
 */