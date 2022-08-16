/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	Библиотека графических процедур и функций
\*------------------------------------------------------------------------*/
#include <stdio.h>

#include "userdraw.h"

/*-------------------------------------------------------------------*\
 *		Рисуем световой круг															*
\*-------------------------------------------------------------------*/
void
DrawLightCircle(TDC& dc, 				// Дисплейный контекст
					 int iCenterX, 		// Х центра
					 int iCenterY, 		// У центра
					 int iRadius,			// радиус
					 HPEN hPenWhite, 		// Перо "света"
					 HPEN hPenGray, 		// Перо "тени"
					 TypeBorder TypeB){	// Тип (выпуклая, вогнутая, и т.д)
	TPoint Center;
	Center.x = iCenterX;
	Center.y = iCenterY;
	DrawLightCircle(dc, Center, iRadius, hPenWhite, hPenGray, TypeB);
};
void
DrawLightCircle(TDC& dc, TPoint& Center, int iRadius,
		 HPEN hPenWhite, HPEN hPenGray, TypeBorder TypeB){
	if (TypeB == RAISED)						 // ВЫПУКЛАЯ
		 dc.SelectObject((HPEN)hPenGray); // Выбор света "ТЕНИ"
	else
		 dc.SelectObject((HPEN)hPenWhite);// Выбор цвета "ОСВЕЩЕНИЯ"
													 // ПРАВАЯ и НИЖНЯЯ часть
	dc.Arc(Center.x - iRadius, Center.y - iRadius,
			 Center.x + iRadius, Center.y + iRadius,
			 Center.x - iRadius, Center.y + iRadius,
			 Center.x + iRadius, Center.y - iRadius);

	if (TypeB == EMBOSSED)               // ВОГНУТАЯ
		 dc.SelectObject((HPEN)hPenGray); // Выбор света "ТЕНИ"
	else
		 dc.SelectObject((HPEN)hPenWhite);// Выбор цвета "ОСВЕЩЕНИЯ"
													 // ЛЕВАЯ и ВЕРХНЯЯ часть
	dc.Arc(Center.x - iRadius, Center.y - iRadius,
			 Center.x + iRadius, Center.y + iRadius,
			 Center.x + iRadius, Center.y - iRadius,
			 Center.x - iRadius, Center.y + iRadius);
};

/*-------------------------------------------------------------------*\
 *		Рисуем световую рамку														*
\*-------------------------------------------------------------------*/
void
DrawLightBorder(TDC& dc,		 		// Дисплейный контекст
												// Координаты и размер
					int ix1, int iy1, int ix2, int iy2,
					HPEN 		  hPenWhite,// Перо "света"
					HPEN 		  hPenGray,	// Перо "тени"
					TypeBorder TypeB){	// Тип (выпуклая, вогнутая, и т.д)
	TRect RectB;
	RectB.left = ix1; RectB.top = iy1; RectB.right = ix2; RectB.bottom = iy2;
	DrawLightBorder(dc, RectB, hPenWhite, hPenGray, TypeB);
};
void
DrawLightBorder(TDC& dc, TRect& RectB,
		 HPEN hPenWhite, HPEN hPenGray, TypeBorder TypeB){
	if (TypeB == RAISED)						 // ВЫПУКЛАЯ
		 dc.SelectObject((HPEN)hPenGray); // Выбор света "ТЕНИ"
	else
		 dc.SelectObject((HPEN)hPenWhite);// Выбор цвета "ОСВЕЩЕНИЯ"
													 // ПРАВАЯ и НИЖНЯЯ часть
	dc.MoveTo(RectB.right - 1, RectB.top);
	dc.LineTo(RectB.right - 1, RectB.bottom - 1);
	dc.LineTo(RectB.left,      RectB.bottom - 1);

	if (TypeB == EMBOSSED)               // ВОГНУТАЯ
		 dc.SelectObject((HPEN)hPenGray); // Выбор света "ТЕНИ"
	else
		 dc.SelectObject((HPEN)hPenWhite);// Выбор цвета "ОСВЕЩЕНИЯ"

	dc.MoveTo(RectB.right, RectB.top);	 // ЛЕВАЯ и ВЕРХНЯЯ часть
	dc.LineTo(RectB.left,  RectB.top);
	dc.LineTo(RectB.left,  RectB.bottom);
};

/*-------------------------------------------------------------------*\
 *		Рисуем солнце																	*
 *		Диск солнца и лучи (в виде звезды, с кол-вом лучей					*
 *								  равным QUANTRAYS - 1)                      *
\*-------------------------------------------------------------------*/
void Sun(TDC& dc,					// Дисплейный контекст
			int iCenterX, 			// Х центра
			int iCenterY, 			// У центра
			int iRadiusSun,		// Радиус
			HBRUSH hbrSun, 		// Кисть
			HPEN hPenSun){			// Перо
	TPoint Center;
	Center.x = iCenterX;
	Center.y = iCenterY;
	Sun(dc, Center, iRadiusSun, hbrSun, hPenSun);
};
void Sun(TDC& dc, TPoint& Center, int iRadiusSun,
			HBRUSH hbrSun, HPEN hPenSun){
												// Угол между лучами
	double dCornerRay = 360.0 / (double)(QUANTRAYS - 1);
	TPoint RaysPoint[QUANTRAYS];  	// Массив точек лучей
	div_t  t;								// Выбираем перо и кисть для солнца
	dc.SelectObject((HBRUSH)hbrSun); dc.SelectObject((HPEN)hPenSun);
												// Заполняем массив точек лучей
	for (char i = 0; i < QUANTRAYS; i++){
		double dRadCorner = ((double)i * dCornerRay / 360.0) * (2.0 * 3.14159);
		t = div(i, 2);
		if (t.rem == 0){					// Высокая точка луча
			RaysPoint[i].x = Center.x + (int)((double)iRadiusSun * sin(dRadCorner));
			RaysPoint[i].y = Center.y - (int)((double)iRadiusSun * cos(dRadCorner));
		} else {                      // Низкая точка луча
			RaysPoint[i].x = Center.x + (int)((double)iRadiusSun / 2 * sin(dRadCorner));
			RaysPoint[i].y = Center.y - (int)((double)iRadiusSun / 2 * cos(dRadCorner));
		};
	};                               // Рисуем лучи
	dc.Polyline(RaysPoint, QUANTRAYS);
												// Рисуем диск солнца
	dc.Ellipse(Center.x - iRadiusSun / 3, Center.y - iRadiusSun / 3,
				  Center.x + iRadiusSun / 3, Center.y + iRadiusSun / 3);
};

/*-------------------------------------------------------------------*\
 *		Рисуем облако																	*
\*-------------------------------------------------------------------*/
void Cloud(TDC& dc,		 		// Дисплейный контекст
										// Координаты и размер
			  int ix1, int iy1, int ix2, int iy2,
			  HBRUSH hbrCloud, 	// Кисть
			  HPEN hPenCloud){	// Перо
	TRect RectCloud;
	RectCloud.left  = ix1; RectCloud.top    = iy1;
	RectCloud.right = ix2; RectCloud.bottom = iy2;
	Cloud(dc, RectCloud, hbrCloud, hPenCloud);
};
void Cloud(TDC& dc, TRect& CloudRect, HBRUSH hbrCloud, HPEN hPenCloud){
	div_t t;									// Диаметр эллипса
	int   iDiametrE = (int)((double)(CloudRect.bottom - CloudRect.top) / 1.5);
	int   iQuantE;                    // Кол-во эллипсов
	t = div(CloudRect.right - CloudRect.left, iDiametrE);
	iQuantE = t.quot;                 // Выбираем перо и кисть для облака
	dc.SelectObject((HBRUSH)hbrCloud); dc.SelectObject((HPEN)hPenCloud);
												// Рисуем верх облака
	for (char i = 0; i < iQuantE; i++){
		dc.Ellipse(CloudRect.left + i * iDiametrE,
					  CloudRect.top,
					  CloudRect.left + (i+1) * iDiametrE,
					  CloudRect.top + iDiametrE);
	};											// Рисуем низ облака
	for (i = 0; i < iQuantE; i++){
		dc.Ellipse(CloudRect.left + i * iDiametrE + iDiametrE / 2,
					  CloudRect.bottom - iDiametrE,
					  CloudRect.left + (i+1) * iDiametrE + iDiametrE / 2,
					  CloudRect.bottom);
	};
};

/*-----------------------------------------------------------------*\
 * 	Отображение круговой шкалы										       *
\*-----------------------------------------------------------------*/
void DrawCircleScale(TDC&  dc,		// Дисплейный контекст
			  int   ix,						// Центр шкалы X
			  int   iy,          		// Центр шкалы Y
			  int   iLengthNeedle,		// Длина стрелки
			  char* sDim,					// Размерность
			  int   iMinValue,			// Минимум
			  int   iRange){     		// Диапазон
	TPoint Center;
	Center.x = ix; Center.y = iy;
	DrawCircleScale(dc, Center, iLengthNeedle, sDim, iMinValue, iRange);
};
void DrawCircleScale(TDC&  dc,
					TPoint& Center,					// Центр шкалы
					int   iLengthNeedle,       // Длина стрелки
					char* sDim,						// Размерность
					int   iMinValue,				// Минимум
					int	iRange){			    	// Диапазон
	double dCorner;                        // Деления шкалы
	char   StScale[17] = "";
	TSize  size;
	TPoint  PosText;

	dc.Arc(Center.x - iLengthNeedle + 2, Center.y - iLengthNeedle + 2,
			 Center.x + iLengthNeedle - 2, Center.y + iLengthNeedle - 2,
			 Center.x + iLengthNeedle - 2, Center.y,
			 Center.x - iLengthNeedle + 2, Center.y);

	for (char i = 0; i <= QUANTMARKCIRCLE; i ++){
		dCorner = (((double)i * (180.0 / (double)QUANTMARKCIRCLE) - 90.0) / 360.0) *
					 (2.0 * 3.14159);
		dc.MoveTo(Center.x + (int)((double)(iLengthNeedle - 4) * sin(dCorner)),
					 Center.y - (int)((double)(iLengthNeedle - 4) * cos(dCorner)));
		dc.LineTo(Center.x + (int)((double)(iLengthNeedle + 2) * sin(dCorner)),
					 Center.y - (int)((double)(iLengthNeedle + 2) * cos(dCorner)));
														// Преобразуем значения шкалы в строку
		itoa(iMinValue + i * iRange / QUANTMARKCIRCLE, StScale, 10);
														// Получим длину и ширину строки в пикселах
		dc.GetTextExtent(StScale, strlen(StScale), size);
		switch (i){									// Вычислим позицию текста
			case 0 : PosText.x =  Center.x 					 + (int)((double)(iLengthNeedle - 4) * sin(dCorner));
						PosText.y = (Center.y - size.cy)		 - (int)((double)(iLengthNeedle - 4) * cos(dCorner));
						break;
			case 1 : PosText.x = (Center.x + 2) 			 + (int)((double)(iLengthNeedle - 4) * sin(dCorner));
						PosText.y =  Center.y                - (int)((double)(iLengthNeedle - 4) * cos(dCorner));
						break;
			case 2 : PosText.x = (Center.x - size.cx / 2) + (int)((double)(iLengthNeedle - 4) * sin(dCorner));
						PosText.y = (Center.y + 2)           - (int)((double)(iLengthNeedle - 4) * cos(dCorner));
						break;
			case 3 : PosText.x = (Center.x - size.cx / 2) + (int)((double)(iLengthNeedle - 4) * sin(dCorner));
						PosText.y = (Center.y + 2)           - (int)((double)(iLengthNeedle - 4) * cos(dCorner));
						break;
			case 4 : PosText.x = (Center.x - size.cx - 2) + (int)((double)(iLengthNeedle - 4) * sin(dCorner));
						PosText.y =  Center.y                - (int)((double)(iLengthNeedle - 4) * cos(dCorner));
						break;
			case 5 : PosText.x = (Center.x - size.cx) 	 + (int)((double)(iLengthNeedle - 4) * sin(dCorner));
						PosText.y = (Center.y - size.cy)		 - (int)((double)(iLengthNeedle - 4) * cos(dCorner));
						break;
		};
														// Отобразим значения шкалы
		dc.TextOut(PosText, StScale);
	};                                     // Рисуем текст размерности
	dc.GetTextExtent(sDim, strlen(sDim), size);
	dc.TextOut(Center.x - size.cx / 2,
				  Center.y - (int)((double)iLengthNeedle / 2.0), sDim);
};

/*-----------------------------------------------------------------*\
 * 	Отображение прямоугольной шкалы										 *
\*-----------------------------------------------------------------*/
void DrawRectScale(TDC&  dc,					// Дисплейный контекст
					TypePosScale PosScale,		// Позиция шкалы
					int    ix1, int	iy1,		// X1, Y1
					int	 ix2, int	iy2,     // X2, Y2
					char*  sDimX,  				// Размерность
					double dMinX,  				// Минимальное значение
					double dRangeX,	  			// Диапазон
					char*  sDimY,  				// Размерность
					double dMinY,  				// Минимальное значение
					double dRangeY){    			// Диапазон
	TRect RectScale;
	RectScale.left  = ix1; RectScale.top    = iy1;
	RectScale.right = ix2; RectScale.bottom = iy2;
	DrawRectScale(dc, PosScale, RectScale,
							sDimX, dMinX, dRangeX,
							sDimY, dMinY, dRangeY);
};
void DrawRectScale(TDC&  dc,
					TypePosScale PosScale,		// Позиция шкалы
					TRect& RectScale,				// Размер шкалы
					char*  sDimX, 					// Размерность
					double dMinX,  				// Минимальное значение
					double dRangeX,	  			// Диапазон
					char*  sDimY,  				// Размерность
					double dMinY,  				// Минимальное значение
					double dRangeY){    			// Диапазон
	char  StScale[17] = "";
	TSize size;
	double dDeltaMarkY, dDeltaMarkX, dCountMarkX;
														// Расстояние между засечками шкалы Y
	dDeltaMarkY = (double)((RectScale.bottom - DELTASCALE) -
								  (RectScale.top    + DELTASCALE)) /
								  QUANTMARKRECT;
														// Расстояние между засечками шкалы X
	dDeltaMarkX	= (double)((RectScale.right  - DELTASCALE) -
								  (RectScale.left   + DELTASCALE)) /
								  COUNTMARKX;
														// Кол-во маркеров по 'X'
	dCountMarkX = (double)((RectScale.right  - DELTASCALE) -
								  (RectScale.left   + DELTASCALE)) /
								  dDeltaMarkX;
//----------------- Ш к а л а   N 1  по Y -------------------------
	if (PosScale == LeftScale){
		 dc.MoveTo(RectScale.left   + DELTASCALE,
					  RectScale.top    + DELTASCALE);
		 dc.LineTo(RectScale.left   + DELTASCALE,
					  RectScale.bottom - DELTASCALE);
	};

//----------------- Ш к а л а  по X -------------------------------
	dc.MoveTo(RectScale.left   + DELTASCALE,
				 RectScale.bottom - DELTASCALE);
	dc.LineTo(RectScale.right  - DELTASCALE,
				 RectScale.bottom - DELTASCALE);

//----------------- Ш к а л а   N 2  по Y -------------------------
	if (PosScale == RightScale){
		 dc.LineTo(RectScale.right  - DELTASCALE,
					  RectScale.top	 + DELTASCALE);
	};

	for (char yi = 0; yi <= QUANTMARKRECT; yi++){
//------- М а р к е р ы  по  Y   N 1 ------------------------------
		 if (PosScale == LeftScale){
			 dc.MoveTo(RectScale.left   + DELTASCALE - LENGTHMARK,
						  RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY);
			 dc.LineTo(RectScale.left   + DELTASCALE,
						  RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY);
//------- З н а ч е н и я  по  Y   N 1 ----------------------------
			 sprintf(StScale, "%.f", dMinY + (double)yi * dRangeY / QUANTMARKRECT);
			 dc.GetTextExtent(StScale, strlen(StScale), size);
			 dc.TextOut(RectScale.left   + DELTASCALE - 1 - size.cx,
							RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy,
							StScale);
			 if (yi == QUANTMARKRECT){    // Еденицы измерения
				 dc.GetTextExtent(sDimY, strlen(sDimY), size);
				 dc.TextOut(RectScale.left,
								RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy * 2,
								sDimY);
			 };
		 };

//------- М а р к е р ы  по  Y   N 2 ------------------------------
		 if (PosScale == RightScale){
			 dc.MoveTo(RectScale.right   - DELTASCALE,
						  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
			 dc.LineTo(RectScale.right   - DELTASCALE + LENGTHMARK,
						  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
//------- З н а ч е н и я  по  Y   N 2 ----------------------------
			 sprintf(StScale, "%.f", dMinY + (double)yi * dRangeY / QUANTMARKRECT);
			 dc.GetTextExtent(StScale, strlen(StScale), size);
			 dc.TextOut(RectScale.right  - DELTASCALE + 2,
							RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy,
							StScale);
			 if (yi == QUANTMARKRECT){		// Еденицы измерения
				 dc.GetTextExtent(sDimY, strlen(sDimY), size);
				 dc.TextOut(RectScale.right  - size.cx,
								RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy * 2,
								sDimY);
			 };
		 };
	 };
//------- М а р к е р ы   и   з н а ч е н и я  по  X --------------
	 for (char xi = 1; xi <= (char)dCountMarkX; xi++){
		 dc.MoveTo(RectScale.left   + DELTASCALE + xi * (int)dDeltaMarkX,
					  RectScale.bottom - DELTASCALE);
		 dc.LineTo(RectScale.left   + DELTASCALE + xi * (int)dDeltaMarkX,
					  RectScale.bottom - DELTASCALE + LENGTHMARK);

		 sprintf(StScale, "%.1f", dMinX + (double)xi * dRangeX / dCountMarkX);
		 dc.GetTextExtent(StScale, strlen(StScale), size);
		 dc.TextOut(RectScale.left   + DELTASCALE +
								xi * (int)dDeltaMarkX - size.cx / 2,
						RectScale.bottom - DELTASCALE + LENGTHMARK,
						StScale);
		 if (xi == QUANTMARKRECT){    // Еденицы измерения
				  dc.GetTextExtent(sDimX, strlen(sDimX), size);
				  dc.TextOut(RectScale.left + DELTASCALE - size.cx / 2,
				  RectScale.bottom - DELTASCALE + LENGTHMARK,
				  sDimX);
		 };
	};
};

/*-----------------------------------------------------------------*\
 * 	Отображение сетки															 *
\*-----------------------------------------------------------------*/
void DrawGrid(TDC&  dc,				  	// Дисплейный контекст
				  int ix1, int iy1,	  	// X1, Y1
				  int	ix2, int	iy2){    // X2, Y2
	TRect RectScale;
	RectScale.left  = ix1; RectScale.top    = iy1;
	RectScale.right = ix2; RectScale.bottom = iy2;
	DrawGrid(dc, RectScale);
};
void DrawGrid(TDC&   dc,
				  TRect& RectScale){				// Размер шкалы
	 double dDeltaMarkY, dDeltaMarkX;
	 double dCountMarkX;							// Кол-во маркеров по 'X'
														// Расстояние между засечками шкалы Y
	 dDeltaMarkY  = (double)((RectScale.bottom - DELTASCALE) -
									 (RectScale.top    + DELTASCALE)) /
									 QUANTMARKRECT;
														// Расстояние между засечками шкалы X
	 dDeltaMarkX  = (double)((RectScale.right  - DELTASCALE) -
									 (RectScale.left   + DELTASCALE)) /
									 COUNTMARKX;
	 dCountMarkX  = (double)((RectScale.right  - DELTASCALE) -
									 (RectScale.left   + DELTASCALE)) /
									 dDeltaMarkX;

//----------------- С е т к а   по Y ------------------------------
	 for (char yi = 1; yi <= QUANTMARKRECT; yi++){
		 dc.MoveTo(RectScale.left    + DELTASCALE,
					  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
		 dc.LineTo(RectScale.right   - DELTASCALE,
					  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
	};

//----------------- С е т к а  по X -------------------------------
	 for (char xi = 1; xi <= (char)dCountMarkX; xi++){
		 dc.MoveTo(RectScale.left   + DELTASCALE + xi * (int)dDeltaMarkX,
					  RectScale.bottom - DELTASCALE);
		 dc.LineTo(RectScale.left   + DELTASCALE + xi * (int)dDeltaMarkX,
					  RectScale.top    + DELTASCALE);
	};
};
/*-----------------------------------------------------------------*\
 * 	Функция отображения картинки (BitMap'а)							 *
\*-----------------------------------------------------------------*/
void
DrawBitMap(TDC &dc, TBitmap& bitmap,// Указатель на объект-катринку
			  int x, int y){				// Позиция картинки
  TMemoryDC memDC(dc);
  memDC.SelectObject(bitmap);

  BITMAP bm;
  bitmap.GetObject(bm);
  dc.BitBlt(x, y,                   // Координаты картинки
				bm.bmWidth,	bm.bmHeight,
				memDC, 0, 0, SRCCOPY);
};
/*-----------------------------------------------------------------*\
 * 	Функция дуги по центру, радиусу и двум точкам					 *
\*-----------------------------------------------------------------*/
void Arc4(TDC& dc, 		// Дисплейный контекст
	  TPoint& Center, 	// Центр дуги
	  int Radius, 			// Радиус дуги
	  TPoint& Tip1, 		// Точка 1
	  TPoint& Tip2){		// Точка 1
	dc.Arc(Center.x - Radius, Center.y - Radius,	// левая верхняя точка описанного
																// прямоугольника,
			 Center.x + Radius, Center.y + Radius, // правая нижняя точка описанного
																// прямоугольника
			 Tip2.x, Tip2.y, Tip1.x, Tip1.y);		// точки дуги
};
/*-----------------------------------------------------------------*\
 * 	Функция поворота местоположения точки по заданному углу 		 *
 *		относительно нуля															 *
\*-----------------------------------------------------------------*/
TPoint
AffinedRotate(double AngleRad, 	// Угол поворота
				  int x, int y){		// Точка подвергающаяся повороту
	TPoint OldPoint;
	OldPoint.x = x; OldPoint.y = y;
	return AffinedRotate(AngleRad, OldPoint);
};
TPoint
AffinedRotate(double AngleRad, TPoint& OldPoint){
	TPoint NewPoint;
	NewPoint.x =  OldPoint.x * cos(AngleRad) + OldPoint.y * sin(AngleRad);
	NewPoint.y = -OldPoint.x * sin(AngleRad) + OldPoint.y * cos(AngleRad);
	return NewPoint;
};

/*-------------------------------------------------------------------*\
 *		Установим курсор на заданную позицию									*
\*-------------------------------------------------------------------*/
BOOL
MoveToCorner(TDC& dc, int cx, int cy, int x, int y, double corner){
	TPoint point;
	TPoint center;
	center.x = cx; center.y = cy;
	point.x = x; point.y = y;
	return MoveToCorner(dc, center, point, corner);
};
BOOL
MoveToCorner(TDC& dc, const TPoint& center, const TPoint& point, double corner){
	return dc.MoveTo(center.x + (int)((double)point.x * sin(corner)),
						  center.y - (int)((double)point.y * cos(corner)));
};

/*-------------------------------------------------------------------*\
 *		Прорисуем линию от курсором заданной позиции 						*
 *		до указанной позиции                                           *
\*-------------------------------------------------------------------*/
BOOL
LineToCorner(TDC& dc, int cx, int cy, int x, int y, double corner){
	TPoint point;
	TPoint center;
	center.x = cx; center.y = cy;
	point.x = x; point.y = y;
	return LineToCorner(dc, center, point, corner);
};
BOOL
LineToCorner(TDC& dc, const TPoint& center, const TPoint& point, double corner){
	return dc.LineTo(center.x + (int)((double)point.x * sin(corner)),
						  center.y - (int)((double)point.y * cos(corner)));
};
/* ----------------------------------------------------------
 *	EoF(userdraw.cpp)
 * ----------------------------------------------------------
 */
