/*
 * 	TestErMs.cpp - Модуль проверки компоновки PXERRMSG.DLL
 *		путем вызова любого сообщения из динамической
 *		библиотеки.
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include "pxengine.h"
void PXErrMesg(int);		// Функция отображения ошибки.
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
	BWCCGetVersion();						// BWCC - support
	PXErrMesg(PXSUCCESS);// Вызов первого сообщения
	return 0;
};
