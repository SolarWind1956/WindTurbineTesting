#if !defined(__INSTRUM_H)
#define __INSTRUM_H
/*-------------------------------------------------------------*\
 * viewbox - Class like TBGroupBox for output
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

#include "thermom.h"
#include "barometr.h"
#include "rosewind.h"
#include "windhead.h"

#include "multmetr.h"
#include "stock.h"

#include "userdraw.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TInstrumentsBox
 *  -------------------------------------------------------------------------
 */
class TInstrumentsBox : public TGroupBox {
  public:
	 TRect	  ThermoRect; 	 	 		 // Размеры и координаты термометра
	 POINT	  CenterBarom;         	 // Центр барометра
	 int		  RadiusBarom;				 // Диаметр барометра
	 POINT	  CenterRoseWind;      	 // Центр розы ветров
	 int		  RadiusRoseWind;			 // Диаметр розы ветров
	 POINT	  CenterDirWindHead;   	 // Центр окна направления ветра и ориентации
	 int		  RadiusDirWindHead;	  	 // Диаметр окна направления ветра и ориентации

	 TRect     ActivePowerRect;    	 // Размеры и координаты мультиметра для мощности
	 TRect     ReactivePowerRect;   	 // Размеры и координаты мультиметра для мощности
	 TRect	  StockRect1;				 // Размеры и координаты прибора хода штока 1
	 TRect	  StockRect2;				 // Размеры и координаты прибора хода штока 2
	 TRect	  SpeedWindRect;			 // Размеры и координаты прибора скорости ветра
	 TRect	  SpeedWndRtrRect;		 // Размеры и координаты прибора скорости ветроколеса
	 int       fnObject;					 // Фон
	 int		  NumGroup;					 // Номер группы графиков

	 Thermom		 *pThermom;
	 Barometr	 *pBarometr;
	 RoseWind	 *pRoseWind;
	 DirWindHead *pDirWindHead;

	 Multimetr	 *pActivePower;
	 Multimetr	 *pReactivePower;
	 Stock		 *pStock1;
	 Stock		 *pStock2;
	 Stock		 *pSpeedWind;
	 Stock		 *pSpeedWndRtr;	 

												 // Конструктор
	 TInstrumentsBox(TWindow* parent, int resourceId, TModule* module = 0);
    ~TInstrumentsBox();

	 void GetWindowClass			(WNDCLASS&);
	 void Paint						(TDC&, BOOL, TRect&);
	 void EvPaint					();
	 void ShowInstruments		();	 // Прототип функции отображения приборов
	 void ShowValuesInstruments();	 // Прототип функции отображения данных на приборах
	 char far* GetClassName		();

  DECLARE_RESPONSE_TABLE(TInstrumentsBox);
};
#endif  // __INSTRUM_H
/*
 * ---------------------------------------------------------
 *	EoF(instrum.h)
 * ----------------------------------------------------------
 */
