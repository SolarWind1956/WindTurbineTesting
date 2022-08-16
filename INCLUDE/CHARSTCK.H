#if !defined(__charstck_h)              // Sentry, use file only if it's not already included.
#define __charstck_h

/*  Project powercha
	 Crimea Engineering Centre
	 Авторское право © 1996. Все права защищены.

	 SUBSYSTEM:    powercha.exe Application
	 FILE:         grahpwin.h
	 AUTHOR:       Heckel & Davidov


	 OVERVIEW
	 ========
	 Class definition for CharactStocksMDIChild (TMDIChild).
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include "powercha.rh"            // Definition of all resources.
#include "usergrph.h"

//{{TMDIChild = CharactStocksMDIChild}}
class CharactStocksMDIChild : public TMDIChild {
public:
	 int 		  iMin, iMax;				 // Минимум и максимум для Y шкалы
	 char		  sDimension[5];			 // Размерность для Y шкалы
	 TRect     GraphRect;           	 // Размеры и координаты окна графиков
	 int       fnObject;					 // Фон
	 int		  NumGroup;					 // Номер группы графиков

	 Graphics* pGraphic;
												 // Конструктор
	 CharactStocksMDIChild (TMDIClient 		&parent,
									const char far *title,
									TWindow 			*clientWnd,
									BOOL 				shrinkToClient = FALSE,
									TModule* 		module 			= 0);
												 // Деструктор
	 virtual ~CharactStocksMDIChild ();

    void SetupWindow			();
	 void GetWindowClass		(WNDCLASS&);
	 void EvSize				(UINT, TSize& size);
	 void Paint					(TDC&, BOOL, TRect&);
	 void EvPaint				();
	 void ShowWindowGraphics();	 // Прототип функции отображения окна графиков
	 void ShowValuesGraphics();	 // Прототип функции отображения данных на графиках
	 void EvTimer				(UINT);
	 char far* GetClassName	();
	 void EvClose				();

	 DECLARE_RESPONSE_TABLE(CharactStocksMDIChild);
};
#endif                                      // __charstck_h sentry.
