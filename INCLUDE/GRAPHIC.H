#if !defined(__GRAPHIC_H)
#define __GRAPHIC_H
/*-------------------------------------------------------------*\
 * TGraphicsBox - Class like TBGroupBox for output
 *
 * Author: Konstantin A. Davidov
 *  Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
\*-------------------------------------------------------------*/
//						H - files

#include <bwcc.h>

#include <owl\groupbox.h>

#include "usergrph.h"
#include "userdraw.h"
#include "commglob.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TGraphicsBox
 *  -------------------------------------------------------------------------
 */
class TGraphicsBox : public TGroupBox {
  public:
	 BOOL		  bGridCommand;			 // Команда для предотвращения
												 // повторного удаления сетки
	 BOOL      bGridOnOff;			 	 // Наличие сетки
	 TRect     GraphRect;           	 // Размеры и координаты окна графиков
	 int       fnObject;					 // Фон
	 int		  NumGroup;					 // Номер группы графиков

	 Graphics* pGraphics[COUNTGRAPHICS];

												 // Конструктор
	 TGraphicsBox(TWindow* parent, int resourceId, TModule* module = 0);
	 ~TGraphicsBox();
	 void GetWindowClass		(WNDCLASS&);
	 void Paint					(TDC&, BOOL, TRect&);
	 void EvPaint				();
	 void ShowWindowGraphics(int); // Прототип функции отображения окна графиков
	 void ShowValuesGraphics();	 // Прототип функции отображения данных на графиках
	 char far* GetClassName	();

  DECLARE_RESPONSE_TABLE(TGraphicsBox);
};
#endif  // __GRAPHIC_H
/*
 * ---------------------------------------------------------
 *	EoF(graphic.h)
 * ----------------------------------------------------------
 */
