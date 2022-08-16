#if !defined(__SHAPEBOX_H)
#define __SHAPEBOX_H
/*
 * shapebox.h - Handle file TShapeBox Class ---------------------
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\groupbox.h>

#include "thermom.h"
#include "barometr.h"
#include "rosewind.h"

#include "multmetr.h"
#include "stock.h"

#include "usergrph.h"
#include "userdraw.h"
#include "globgrph.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TShapeBox
 *  -------------------------------------------------------------------------
 */
class TShapeBox : public TGroupBox{
  public:
    int		  ResourceId;				 // Индетификатор ресурса	

	 BOOL      bGridOnOff;			 	 // Наличие сетки
	 TRect	  ThermoRect; 	 	 		 // Размеры и координаты термометра
	 POINT	  CenterBarom;         	 // Центр барометра
	 int		  RadiusBarom;				 // Диаметр барометра
	 POINT	  CenterRoseWind;      	 // Центр розы ветров
	 int		  RadiusRoseWind;			 // Диаметр розы ветров

	 TRect     ActivePowerRect;    	 // Размеры и координаты мультиметра для мощности
	 TRect     ReactivePowerRect;   	 // Размеры и координаты мультиметра для мощности
	 TRect	  StockRect1;				 // Размеры и координаты датчика хода штока 1
	 TRect	  StockRect2;				 // Размеры и координаты датчика хода штока 2
	 TRect     GraphRect;           	 // Размеры и координаты окна графиков
	 int       fnObject;					 // Фон
	 int		  NumGroup;					 // Номер группы графиков

	 Thermom		*pThermom;
	 Barometr	*pBarometr;
	 RoseWind	*pRoseWind;
//------------------------------------------

	 Multimetr	*pActivePower;
	 Multimetr	*pReactivePower;
	 Stock		*pStock1;
	 Stock		*pStock2;
	 Graphics	*pGraphics[COUNTGRAPHICS];

												 // Конструктор
	 TShapeBox(TWindow* parent, int resourceId, TModule* module = 0);
	 ~TShapeBox();

	 void GetWindowClass		(WNDCLASS&);
	 void Paint					(TDC&, BOOL, TRect&);
	 void EvPaint				();
	 void ShowWindowGraphics(int); 	 // Прототип функции отображения окна графиков
	 void ShowSensors			();	 	 // Прототип функции отображения датчиков
	 void ShowValuesGraphics();	 	 // Прототип функции отображения данных на графиках
	 void ShowValuesSensors	();	 	 // Прототип функции отображения данных на датчиках
	 char far* GetClassName	();

  DECLARE_RESPONSE_TABLE(TShapeBox);
};
#endif  // __SHAPEBOX_H
/*
 * ---------------------------------------------------------
 *	EoF(shapebox.h)
 * ----------------------------------------------------------
 */
