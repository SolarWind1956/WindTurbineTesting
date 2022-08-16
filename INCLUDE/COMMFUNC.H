#if !defined(__COMMFUNC_H)
#define __COMMFUNC_H
/*
 * CommFunc - Общие вспомогательные или инструментальные функции
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 */
#include <owl\owlpch.h>
#include <bwcc.h>
											//	Среднее пошагового усреднения
double
StepAverageValue  (double StepNumber,		// Номер шага усреднения
						 double PrecedAverage,	// Предудущее Cреднее
						 double CurrentValue); 	// Текущее значение
														// усредняемой величины
int 													//	Индекс массива БИНОВ
IndexBinsArray		(double WindSpeed);     // Скорость ветра
char*
ShortDate(char* StrDate);						// Строка зашифрованнонй даты
/*
 * 	Дополнения к Windows API
 */
void 		SetDlgItemDoub		(HWND hWnd, int childID, double value,
									 BOOL isSigned = TRUE);
double 	GetDlgItemDoub    (HWND hWnd, int childID, BOOL FAR* translated = 0,
									 BOOL isSigned = TRUE);
void 		CalcNewtonC(double* CoordX, double* CoordY, double* NewtonC);
double 	FunctLagrangForm (double CurrentX, double* CoordX, double* CoordY);
#endif	// __COMMFUNC_H
/*
 * ---------------------------------------------------------
 *	EoF(commfunc.h)
 * ----------------------------------------------------------
 */

