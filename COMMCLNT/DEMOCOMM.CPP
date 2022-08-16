/*
 * DemoComm - Demo communicate program with Info73 module -------------------------
 *
 * Authors: Sergey E. Heckel & Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 * DemoComm - программа без установления связи с контроллером Info 73
 * 		Основные функции:
 *			-	иммитация связи с контроллером с иммитируемыми данными
 */

#include <math.h>

#include <..\include\commglob.h>

void DemoDataProcess();

/*
 *		Переменные для принятия данных с контроллера
 */
/*
 *		Массивы для передачи данных в контроллер
 */

/*
 *  -------------------------------------------------------------------------
 * 	Main Procedure
 *  -------------------------------------------------------------------------
 */
int
InputON(TWindow*)
{
	BuffTransfFrom.BuffFromPC.awInputData1[25] = 0;
	return 1;
};
/*
 * 	Функция разрыва связи
 */
void InputOFF() {
};
/*
 *    Принятие блока данных с контроллера
 */
int InputCycle() {
	DemoDataProcess();						// Обработка данных для DEMO
													// в файле 'techpro.cpp
	return 1;
};
 /*
  * 	---------------------------------------------------------
  *		EoF(democomm.cpp)
  * 	----------------------------------------------------------
  */
