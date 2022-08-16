#if !defined(__GRADCHAR_H)
#define __GRADCHAR_H
/*-------------------------------------------------------------*\
 * TGraduirCharact - Class like TBGroupBox for output
 *
 * Author: Konstantin A. Davidov
 *  Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
\*-------------------------------------------------------------*/
//						H - files
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\groupbox.h>

#include "usergrph.h"
#include "userdraw.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TGraduirCharact
 *  -------------------------------------------------------------------------
 */
class TGraduirCharact : public TGroupBox {
  public:
	 BOOL		  bShowCharact;          // Флаг разрешения отображения
	 double    CharBufferX[QUANTVALUES]; 			 // Данные по X
	 double    CharBufferY[QUANTVALUES]; 			 // Данные по Y
	 double    LastValuesX[QUANTVALUES]; 			 // Прошлые данные по X
	 double    LastValuesY[QUANTVALUES]; 			 // Прошлые данные по Y
	 int		  TypeApprox;				 // Индекс аппроскимации
												 // графика и данных
	 TRect     GraphRect;           	 // Размеры и координаты окна графиков
	 int       fnObject;					 // Фон
	 int		  NumGroup;					 // Номер группы графиков

	 Graphics* pGradCharact;
	 Graphics* pApproxCharact;

												 // Конструктор
	 TGraduirCharact(TWindow* parent, int resourceId, BOOL bLegenda,
						  TModule* module = 0);

	 void ChangeParamGraphic(double, double, char*,
									 double, double, char*);
	 void ClearGraphicWindow();
	 void GetWindowClass		(WNDCLASS&);
	 void Paint					(TDC&, BOOL, TRect&);
	 void EvPaint				();
	 void ShowWindowGraphics(); 	 // Прототип функции отображения окна графиков
											 // Прототип функции отображения данных на графиках
	 void ShowValuesGraphics();
	 char far* GetClassName	();

  DECLARE_RESPONSE_TABLE(TGraduirCharact);
};
#endif  // __GRADCHAR_H
/*
 * ---------------------------------------------------------
 *	EoF(gradchar.h)
 * ----------------------------------------------------------
 */
