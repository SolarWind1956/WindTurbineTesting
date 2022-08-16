#if !defined(__BINFOBOX_H)
#define __BINFOBOX_H
/*
 * binfobox.h - Handle file TBInfoBox Class ---------------------
 *
 *					Класс предназначен для вывода информации в окно
 *             типа Borland Shade Box некоторой информации.
 *					При этом на программу вывода информации накладываются
 *					следующие ограничения:
 *					Прототип функции:
 *
 *						void OutputFunction(TWindow* WinPointer);
 *							  где WinPointer - указатель на окно BorShade
 *													 созданное Resource Workshop
 *
 *					Запуск вывода в окно следует осуществлять следующим
 *					образом:
 *
 * 					void (*pOutputFunction)(TWindow*);
 *						pOutputFunction = OutputFunction;
 *					   pTInfoBox->InfoToShadeBox(pOutputFunction);
 *								где
 *									void OutputFunction(TWindow*) - функция
 *															пользователя, осуществляющая
 *															собственно вывод информации
 *									InfoToShadeBox  - метод класса TBInfoBox
 *                                           запускающий OutputFunction
 *															в TBInfoBox::Paint
 *									pTInfoBox       - указатель на объект
 *														   класса TBInfoBox
 *
 *					Следует отметить, что очистка окна должна осуществляться
 *					методлм класса:
 *                    	 void ClearShadeBox();
 *
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\owlpch.h>
#include <bwcc.h>
#include <owl\groupbox.h>


//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TBInfoBox
 *  -------------------------------------------------------------------------
 */
class TBInfoBox : public TGroupBox{
  public:
	 TDC   	*lpDC; 							// Указатель DC
	 BOOL 	 FlagInfoToShadeBox;			// Флаг разрешения вывода комплексной
													// информации в окно
	 BOOL 	 FlagStringToShadeBox;		// Флаг разрешения вывода строки
													// информации в окно
	 int      PosX, PosY; 	 				// Координаты текста
	 char     OutString[255]; 				// Текст сообщения
	 COLORREF OutColor;						// Цвет текста сообщения

	 int    fnObject;
													// Указатель на функцию,
	 void   (*pInfoFunction)(TBInfoBox*);	// выводящую информацию в окно
													// Конструктор
	 TBInfoBox(TWindow* parent, int resourceId, TModule* module = 0);

	 void GetWindowClass		(WNDCLASS&);
	 void InfoToShadeBox		(void(*)(TBInfoBox*));
	 void StringToShadeBox	(int X, int Y, COLORREF Color, char* TextString);
	 void BitmapToShadeBox	(int X, int Y, char* BitmapNameString);
	 void ClearShadeBox		();
	 void Paint					(TDC&, BOOL, TRect&);
	 void EvPaint				();
	 char far* GetClassName	();

  DECLARE_RESPONSE_TABLE(TBInfoBox);
};

#endif  // __BINFOBOX_H
/*
 * ---------------------------------------------------------
 *	EoF(binfobox.h)
 * ----------------------------------------------------------
 */
