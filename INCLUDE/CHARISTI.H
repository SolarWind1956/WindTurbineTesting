#if !defined(__CHARISTI_H)
#define __CHARISTI_H
/*
 * Char [acter] isti [c] -
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 */
#include <owl\owlpch.h>
#include <classlib\arrays.h>

#include "commglob.h"
#include "ClBINAtt.h"
#include "commfunc.h"

/*
 * 	Константы
 */
const int BINQuant = 51;				// Количество BIN для характеристики
/*
 * 	Переменные
 */
extern const TArray<TBinAttributs>&			TB;
/*
 * 	Прототипы фукций
 */
void Characteristic();

#endif	// __CHARISTI_H
