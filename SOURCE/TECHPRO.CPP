/*
 * TechPro - communicate program with Info73 module -------------------------
 *
 * Authors: Sergey E. Heckel & Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * TechPro -  модуль технологической обработки полученной информации для помещения
 *		в файлы БД и для отображения во временном окне мониторинга
 */
#include <windows.h>
#include <math.h>

#include "commglob.h"

double adwInputData1  [N_INPUT_DATA];		// Первый    массив преобразованных данных
double adwInputData2  [N_INPUT_DATA];		// Вторй     массив преобразованных данных
double adwInputData3  [N_INPUT_DATA];		// Третий    массив преобразованных данных
double adwInputData4  [N_INPUT_DATA];		// Четвертый массив преобразованных данных

/*
 *    Константы используемые в технологических расчетах.
 *    Т.к в контроллере применяется только целочисленная арифметика
 *		данные получаемые с датчиков были умножены на коэффициенты для
 *		уменьшения потери точности.
 *		Соответственно для получения реальных значений на "Верхнем
 *		уровне" мы должны разделить полученные данные на те же
 *		коэффициенты.
 */
const double IMPULS   = 0.76444 / 2.0;	// переводной коэффициент импульсов
													// в м/с согласно документации по датчику
													// импульсов скорости ветра.
const double WST_DISK = 1250.0 * 60.0;	// Комплекс для обработки показаний
													//	датчика скорости ветроколеса (счетчика
													// импульсов)												// для скорости ветра

/*
 * 	Поправки, связанные с растяжением шкал при параметрировании модулей
 *		контроллера
 */
const double K_POWER	  	= 5.0;	// для мощностей
											// для двигателя раскрутки головки
const double K_CURRENTMOTOR = 100.0;
const double K_ATMPRES 	= 2.0;   // для барометрического давления
const double K_AIRTMP  	= 5.0;	// для температуры
const double STOCK  		= 100.0; // для хода штока
const double AZIMUT 		= 10.0; 	// направление
const double DIRECT 		= 10.0; 	// азимут

//----------------------------------------------------------------------------
/*
 *		Предварительные технологические расчеты для отображения
 *						во временном окне мониторинга
 */
void TechnolProcess(){

	for (char i = 0; i < N_INPUT_DATA; i++) adwInputData1 [i] = 0.0;
	for (		 i = 0; i < N_INPUT_DATA; i++) adwInputData2 [i] = 0.0;
	for (		 i = 0; i < N_INPUT_DATA; i++) adwInputData3 [i] = 0.0;
	for (		 i = 0; i < N_INPUT_DATA; i++) adwInputData4 [i] = 0.0;

										// Цикл контроллера
	adwInputData1[25] = BuffTransfFrom.BuffFromPC.awInputData1[25];
										// Вычисление скорости ветра
	adwInputData1[6]  = (double)BuffTransfFrom.BuffFromPC.awInputData1[6] * IMPULS;
										// Вычисление активной мощности генератора ВЭУ
										// P = NetPower / K_POWER
	adwInputData1[17] = BuffTransfFrom.BuffFromPC.awInputData1[17] / K_POWER;
										// Вычисление реактивной мощности генератора ВЭУ
	adwInputData1[19] = BuffTransfFrom.BuffFromPC.awInputData1[19] / K_POWER;
									// Вычисление тока двигателя раскрутки головки
									// W = CW / K_CURRENTMOTOR
	adwInputData1[21] = BuffTransfFrom.BuffFromPC.awInputData1[21] / K_CURRENTMOTOR;
									// Вычисление атмосферного давления
									// B = CB / K_ATMPRES
	adwInputData1[0]  = BuffTransfFrom.BuffFromPC.awInputData1[0] / K_ATMPRES;
									// Вычисление температуры
									// T = CT / K_AIRTMP
	adwInputData1[2]  = BuffTransfFrom.BuffFromPC.awInputData1[2] / K_AIRTMP - 3.0;
									// Вычисление направления ветра
									// D
	adwInputData1[9]  = (double)BuffTransfFrom.BuffFromPC.awInputData1[9] / DIRECT;
									// Вычисление азимута головки ВЭУ Датчик 1
	adwInputData1[12] = (double)BuffTransfFrom.BuffFromPC.awInputData1[12]
																/ AZIMUT;
	if (adwInputData1[12] > 360.0) adwInputData1[12] -= 360.0;
								// Вычисление азимута головки ВЭУ Датчик 2
	adwInputData1[13] = (double)BuffTransfFrom.BuffFromPC.awInputData1[13]
																	/ AZIMUT;
	if (adwInputData1[13] < 0.0) adwInputData1[13] += 360.0;
									// Если направление ветра попадает в диапазон
									// от 90 до 270 то берем данные азимут головки
									// ВЭУ вычисляем по первому датчику
	if (adwInputData1[9] > 90.0 && adwInputData1[9] < 270.0){
									// Вычисление азимута головки ВЭУ Датчик 1
		adwInputData1[14] = adwInputData1[12];
		if (adwInputData1[14] > 360.0) adwInputData1[14] -= 360.0;
	} else {						// Если направление ветра попадает в диапазон
									// от 0 до 90 и от 270 до 360 то берем азимут головки
									// ВЭУ вычисляем по второму датчику
		adwInputData1[14] = adwInputData1[13];
		if (adwInputData1[14] < 0.0  ) adwInputData1[14] += 360.0;
	};

									// Массив данных с датчика скорости поворота
	for (i = 0; i < 20; i++)// ветроколеса в об/мин
		if (BuffTransfFrom.BuffFromPC.awInputData2[i] != 0)
			adwInputData2[i] = WST_DISK /
									(double long)BuffTransfFrom.BuffFromPC.awInputData2[i];
	for (i = 0; i < 20; i++)// Массив данных с первого датчика хода штока ветроколеса
		 adwInputData3[i] =
			(double long)BuffTransfFrom.BuffFromPC.awInputData3[i] / STOCK;
	for (i = 0; i < 20; i++)// Массив данных со второго датчика хода штока ветроколеса
		 adwInputData4[i] =
			(double long)BuffTransfFrom.BuffFromPC.awInputData4[i] / STOCK;
};

/*
 * 		Обработка данных для DEMO
 */
void DemoDataProcess(){
	double adCorrectData [100] = {
				13.2, 13.2, 13.2, 13.6, 14.0, 12.9, 12.5, 12.1, 12.1, 12.1,
				12.5, 14.0, 13.6, 12.5, 12.1, 12.5, 12.5, 12.1, 12.1, 12.5,
				11.3, 10.9, 10.6, 10.2,	11.3, 11.7,  9.8, 12.5, 12.9, 12.9,
				13.2, 12.5,	12.9, 12.9, 10.6, 12.9, 12.5, 12.1, 11.7, 12.5,
				13.6, 13.2, 12.1, 12.1, 12.9, 11.7, 11.3, 12.1, 11.7, 11.7,
				11.3, 10.9, 10.9, 10.6, 10.6, 10.6, 11.3, 10.6, 10.6,  9.8,
				 9.8, 10.2, 11.7, 11.7,	10.9, 11.3, 11.7,  9.8, 10.2, 11.7,
				11.7, 11.3, 10.9, 10.9, 11.7, 10.6, 10.9, 11.3, 10.6,  9.8,
				 9.0,  8.3, 12.1, 13.2, 14.8, 14.4, 13.6, 13.2,	14.0, 14.0,
				13.2, 12.9, 12.1, 12.1, 12.9, 12.9,	12.9, 13.6, 14.0, 13.2
										  };
	static int IndexCorrect;

	if (IndexCorrect > 99)
		IndexCorrect = 0;

	// Цикл контроллера
	BuffTransfFrom.BuffFromPC.awInputData1[25]++;
	if (BuffTransfFrom.BuffFromPC.awInputData1[25] > 30)
										BuffTransfFrom.BuffFromPC.awInputData1[25] = 1;
	// 	Атмосферное давление = 760 мм. рт. ст.
	BuffTransfFrom.BuffFromPC.awInputData1[0]    = 1520 +
		(int)((double)1520 * adCorrectData[IndexCorrect] / 10000);
	// 	Температура = 20 C
	BuffTransfFrom.BuffFromPC.awInputData1[2]    = 100 +
		(int)((double)100 * adCorrectData[IndexCorrect] / 1000);
	// 	Направление
	BuffTransfFrom.BuffFromPC.awInputData1[9]    = 900 +
		(int)((double)900 * adCorrectData[IndexCorrect] / 100);
	if (BuffTransfFrom.BuffFromPC.awInputData1[9] > 3600){
		BuffTransfFrom.BuffFromPC.awInputData1[9] -= 3600;
	};
	if (BuffTransfFrom.BuffFromPC.awInputData1[9] < 0){
		BuffTransfFrom.BuffFromPC.awInputData1[9] += 3600;
	};
	// 	Ориентация головки Датчик 1
	BuffTransfFrom.BuffFromPC.awInputData1[12]   = 900 +
		(int)((double)900 * adCorrectData[IndexCorrect] / 100);
	// 	Ориентация головки Датчик 2
	BuffTransfFrom.BuffFromPC.awInputData1[13]   = 900 +
		(int)((double)900 * adCorrectData[IndexCorrect] / 100);
	//		Ток двигателя раскрутки головки
	BuffTransfFrom.BuffFromPC.awInputData1[21]   = 400 +
		(int)((double)400 * adCorrectData[IndexCorrect] / 100);
	//		Скорость ветра
	BuffTransfFrom.BuffFromPC.awInputData1[6]    =
						(int)(adCorrectData[IndexCorrect] / (double)IMPULS);
	//		Активная мощность
	double ApproxNetPower[6] = {0.0, 	  -4.948954, 0.13819,
										 0.292197, -0.01738,  0.000201};
	BuffTransfFrom.BuffFromPC.awInputData1[17]   =
				(int)(poly(adCorrectData[IndexCorrect], 5, ApproxNetPower) *
						(double)K_POWER);
	if (adCorrectData[IndexCorrect] > 14.0)
			BuffTransfFrom.BuffFromPC.awInputData1[17] =
						(int)(200.0 * (double)K_POWER);
	//		Реактивная мощность = 0
	BuffTransfFrom.BuffFromPC.awInputData1[19]   = 0.0;
	// 	Массив данных с датчика скорости поворота ветроколеса
	for (char i = 0; i < 20; i++)
		BuffTransfFrom.BuffFromPC.awInputData2[i] = 1540 +
						(int)((double)1540 * adCorrectData[IndexCorrect] /
						((double)1000 * (i+1)));
	//		Массивы данных с датчиков хода штоков ~ 7 мм
	for (i = 0; i < 20; i++)
		BuffTransfFrom.BuffFromPC.awInputData3[i] = 700 +
						(int)((double)700 * adCorrectData[IndexCorrect] /
						((double)100 * (i+1)));
	for (i = 0; i < 20; i++)
		BuffTransfFrom.BuffFromPC.awInputData4[i] = 700 +
						(int)((double)700 * adCorrectData[IndexCorrect] /
						((double)100 * (i+1)));
	IndexCorrect++;
};
/*
 * ---------------------------------------------------------
 *	EoF(TechPro.cpp)
 * ----------------------------------------------------------
 */

