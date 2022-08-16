/*
 * PXErrMsg.cpp - Additional module Errors and messages ----------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 *    PXErrMsg  -  Модуль выдачи сообщений об ошибках при работе
 *		функций Paradox 3.0
 *
 *		Особенность работы этого модуля в том, что массив сообщений
 *		находится в динамической библиотеке 'PXERRMSG.DLL'
 */
#include <stdio.h>

#include "messagew.h"
#include "pxengine.h"

void PXErrMesg(int errCode)		// Функция отображения ошибки.
{
  const char MaxStrSize = 100;	// Максимальный размер в байтах
											// строки сообщений
  HLOCAL hMem;							// Логический номер буфера памяти
											// запрашиваемого у системы в
											// локальной динамической области
  LPVOID pStr;							// Указатель на выделенный системой
											// буфер
											// Запрашиваем буфер у системы
  hMem = LocalAlloc(LMEM_MOVEABLE, MaxStrSize);
  pStr = LocalLock(hMem);			// Заблокируем буфер на время
											// использования
											// Грузим тексты возможных сообщений
  HINSTANCE hInst = LoadLibrary("PXERRMSG.DLL");
  LoadString(hInst, errCode, (LPSTR)pStr, MaxStrSize);
  FreeLibrary(hInst);
											// Отображаем принятое сообщение
  char ErrStr[MaxStrSize + 5];
  sprintf(ErrStr, "%s: %d", (LPSTR)pStr, errCode);
  MessageW("Paradox Engine 3.0", (LPSTR)pStr);
  LocalUnlock(hMem);					// Разблокираем буфер памяти
  LocalFree(hMem);					// Отдаем буфер системе
  return;
}
/*
 * ---------------------------------------------------------
 *	EoF(PXErrMsg.cpp)
 * ----------------------------------------------------------
 */
