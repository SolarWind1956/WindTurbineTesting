/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
 *	class Graphics                                                         *
 *	Создает модель наглядного отображения данных в виде графиков           *
\*------------------------------------------------------------------------*/
#include <stdio.h>

#include "usergrph.h"

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
Graphics::Graphics(
		TRect GraphRect,					// размер и положение окна графиков
		double dMinX,			  		 	// минимальное показание по X
		double dMaxX,                 // максимальное показание по X
		char*  sDimensionX,           // текст размерности по X
		double dMinY, 						// минимальное показание по Y
		double dMaxY,  		  			// максимальное показание по Y
		char*  sDimensionY,           // текст размерности по Y
		COLORREF Color,	  				// цвет графика
		COLORREF BackGround,          // цвет фона
		int 		fnPenStyle,				// стиль линии
		BOOL		bLegenda){				// Легенда

	 if (!bLegenda)
		 DeltaForLegenda = 0;			// При отсутствии легенды
	 else
		 DeltaForLegenda = 40;
	 strcpy(sDimX, sDimensionX);
	 dMinValueX = dMinX;  				// Минимальное допустимое значение
	 dMaxValueX = dMaxX;					// Максимальное допустимое значение
	 dRangeX    = dMaxX - dMinX; 		// Диапазон

	 strcpy(sDimY, sDimensionY);
	 dMinValueY = dMinY;  				// Минимальное допустимое значение
	 dMaxValueY = dMaxY;					// Максимальное допустимое значение
	 dRangeY    = dMaxY - dMinY; 		// Диапазон

//	------------------ Размеры фигур ------------------------
	 AllRect 	= GraphRect;
	 FontHeight = 13;						// Высота и
	 FontWidth	= 4;						// ширина шрифта

//	------------------ Координаты фигур ---------------------
												// Размеры окна графика
	 GraphicRect.left    = AllRect.left   + DELTAFROMGRAPH;
	 GraphicRect.top     = AllRect.top    + DELTAFROMGRAPH;
	 GraphicRect.right   = AllRect.right  - DELTAFROMGRAPH;
	 GraphicRect.bottom  = AllRect.bottom - DELTAFROMGRAPH - DeltaForLegenda;
												// Коэффициент отображения графика X
	 dRatioValX = (double)((GraphicRect.right - DELTASCALE) -
								  (GraphicRect.left  + DELTASCALE)) /
					  (double)QUANTVALUES;
												// Коэффициент отображения графика Y
	 dRatioValY = (double)((GraphicRect.bottom - DELTASCALE) -
								  (GraphicRect.top    + DELTASCALE)) /
					  dRangeY;

//	---------- Создание перьев, шрифтов, кистей -------------
												// Перо шкалы
	 hPenScale   = (HPEN)GetStockObject(BLACK_PEN);
												// Перо сетки
	 hPenGrid    = CreatePen(PS_DOT,   0, RGB(0, 128, 0));
												// Перо "ТЕНИ"
	 hPenGray    = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо невидимого контура
	 hPenLtGray  = CreatePen(PS_SOLID, 1, RGB(190, 190, 190));
												// Перо графика
	 hPenGraph   = CreatePen(fnPenStyle, 1, Color);
												// Перо "ОСВЕЩЕНИЯ"
	 hPenWhite   = (HPEN)GetStockObject(WHITE_PEN);
												// Перо "ЗАТИРАНИЯ"
	 hPenErase   = CreatePen(PS_SOLID, 1, BackGround);

	 lb.lbStyle = BS_SOLID;          // Кисть для фона - черного цвета
	 lb.lbColor = BackGround;
	 hbrBlack   = CreateBrushIndirect(&lb);
												// Кисть для фона - черного цвета
	 hbrLtGray   = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

												// Создаем шрифт для показания и шкалы
	 memset(&lf, 0, sizeof(LOGFONT));
	 lf.lfHeight = FontHeight;			// Высота и
	 lf.lfWidth  = FontWidth;        // ширина шрифта
	 lstrcpy((LPSTR)&lf.lfFaceName[0], (LPSTR)"Helvetica");
	 hfontHelv = CreateFontIndirect(&lf);
};

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r															*
\*-------------------------------------------------------------------*/
Graphics::~Graphics(){
	 DeleteObject(hPenScale);      	// Erase block memory for user pen
	 DeleteObject(hPenGrid);      	// Erase block memory for user pen
	 DeleteObject(hPenGray);   	   // Erase block memory for user pen
	 DeleteObject(hPenLtGray);   	   // Erase block memory for user pen
	 DeleteObject(hPenGraph);     	// Erase block memory for user pen
	 DeleteObject(hPenWhite);   	   // Erase block memory for user pen
	 DeleteObject(hPenErase);   	   // Erase block memory for user pen
	 DeleteObject((HBRUSH)hbrBlack);	// Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrLtGray);// Erase block memory for user brush
	 DeleteObject((HFONT)hfontHelv); // Erase block memory for user font
};

/*-------------------------------------------------------------------*\
 *		Отображаем окно графиков													*
\*-------------------------------------------------------------------*/
void
Graphics::DrawWindowGraphics(TDC& 		   dc,       // DC окна
									  TypePosScale PosScale){// Тип шкалы
// --------- Рисуем окно --------------------------------------------
	dc.SelectObject((HBRUSH)hbrLtGray);
	if (PosScale == LeftScale){
		 dc.SelectObject((HPEN)hPenLtGray);
		 dc.Rectangle(AllRect);						// Окно для всего окна графика
															//	Рамка
//		 DrawLightBorder(dc, AllRect, hPenWhite, hPenGray, EMBOSSED/*RAISED*/);

// --------- Рисуем окно графика ------------------------------------
		 dc.SelectObject((HPEN)hPenLtGray);
		 dc.SelectObject((HBRUSH)hbrBlack);
		 dc.Rectangle(GraphicRect.left   + DELTASCALE,	// Окно для графика
						  GraphicRect.top    + DELTASCALE - 1,
						  GraphicRect.right  - DELTASCALE + 1,
						  GraphicRect.bottom - DELTASCALE + 1);
	};

// ---------- Н а р и с у е м  ш к а л у ----------------------------
	dc.SelectObject((HPEN)hPenScale);
	dc.SelectObject((HFONT)hfontHelv);
	dc.SetTextColor(TColor::Black);
	DrawRectScale(dc, PosScale, GraphicRect,
					  sDimX, dMinValueX, dRangeX,
					  sDimY, dMinValueY, dRangeY);
};

/*-------------------------------------------------------------------*\
 *		Изменение параметров окна графика							  			*
\*-------------------------------------------------------------------*/
void
Graphics::ChangeParamGraphWindow(
		TRect GraphRect,					// размер и положение окна графиков
		double dMinX,			  		 	// минимальное показание по X
		double dMaxX,                 // максимальное показание по X
		char*  sDimensionX,           // текст размерности по X
		double dMinY, 						// минимальное показание по Y
		double dMaxY,  		  			// максимальное показание по Y
		char*  sDimensionY){          // текст размерности по Y
	 strcpy(sDimX, sDimensionX);
	 dMinValueX = dMinX;  				// Минимальное допустимое значение
	 dMaxValueX = dMaxX;					// Максимальное допустимое значение
	 dRangeX    = dMaxX - dMinX; 		// Диапазон

	 strcpy(sDimY, sDimensionY);
	 dMinValueY = dMinY;  				// Минимальное допустимое значение
	 dMaxValueY = dMaxY;					// Максимальное допустимое значение
	 dRangeY    = dMaxY - dMinY; 		// Диапазон

//	------------------ Размеры фигур ------------------------
	 AllRect 	= GraphRect;
	 FontHeight = 13;						// Высота и
	 FontWidth	= 4;						// ширина шрифта

//	------------------ Координаты фигур ---------------------
												// Размеры окна графика
	 GraphicRect.left    = AllRect.left   + DELTAFROMGRAPH;
	 GraphicRect.top     = AllRect.top    + DELTAFROMGRAPH;
	 GraphicRect.right   = AllRect.right  - DELTAFROMGRAPH;
	 GraphicRect.bottom  = AllRect.bottom - DELTAFROMGRAPH - DeltaForLegenda;
												// Коэффициент отображения графика X
	 dRatioValX = (double)((GraphicRect.right - DELTASCALE) -
								  (GraphicRect.left  + DELTASCALE)) /
					  (double)QUANTVALUES;
												// Коэффициент отображения графика Y
	 dRatioValY = (double)((GraphicRect.bottom - DELTASCALE) -
								  (GraphicRect.top    + DELTASCALE)) / dRangeY;
};

/*-------------------------------------------------------------------*\
 *		Отображение графика							  								*
\*-------------------------------------------------------------------*/
void
Graphics::DrawGraphic(TDC& dc,      // DC окна
							 TCycleBuffer  OutBuffer,
							 double        LostValue){
	double dx, dy;			   			// Текущие координаты графика
	int    iCount;							// Кол-во данных в массиве
	double dValue;
// ------- П р о р и с о в к а   г р а ф и к а ----------------------

// ------------------- Удаление графика -------------------------
	dc.SelectObject(hPenErase);		// Выбираем перо для удаления графика
												// Получаем кол-во данных в массиве
	iCount = OutBuffer.CA.Count();
	dValue = LostValue;					// Получим потерянное данное
												// Проверка переполнения
	if (dValue < dMinValueY) dValue = dMinValueY;
	if (dValue > dMaxValueY) dValue = dMaxValueY;

	if (iCount == QUANTVALUES + 1){
		dx = 0.0;                     // Вычисление координат 'X' & 'Y'
		dy = (dValue - dMinValueY) * dRatioValY;
												// Установим первую позицию
		dc.MoveTo(GraphicRect.left   + (int)dx +  DELTASCALE,
					 GraphicRect.bottom - (int)dy - (DELTASCALE + 1));
												// Удалим старый график
		for (int i = 0; i < iCount - 1; i++){
				dValue = OutBuffer.CAIter++;
												// Проверка переполнения
				if (dValue < dMinValueY) dValue = dMinValueY;
				if (dValue > dMaxValueY) dValue = dMaxValueY;
												// Вычисление координат 'X' & 'Y'
				dy  = (dValue - dMinValueY) * dRatioValY;
				dx += dRatioValX;		  	// Увеличение 'X'
				dc.LineTo (GraphicRect.left   + (int)dx + DELTASCALE,
							  GraphicRect.bottom - (int)dy - (DELTASCALE + 1));
		};
	};
// ------------------- Прорисовка графика -----------------------
	dc.SelectObject(hPenGraph);		// Выбираем перо для отображения графика
	OutBuffer.CAIter.Restart();
	dValue = OutBuffer.CAIter++;
												// Проверка переполнения
	if (dValue < dMinValueY) dValue = dMinValueY;
	if (dValue > dMaxValueY) dValue = dMaxValueY;

	dx = 0.0;                        // Вычисление координат 'X' & 'Y'
	dy = (dValue - dMinValueY) * dRatioValY;
												// Установим позицию в первую точку
	dc.MoveTo(GraphicRect.left   + (int)dx +  DELTASCALE,
				 GraphicRect.bottom - (int)dy - (DELTASCALE + 1));

	while(OutBuffer.CAIter){ // Рисуем новый график
			dValue = OutBuffer.CAIter++;
												// Проверка переполнения
			if (dValue < dMinValueY) dValue = dMinValueY;
			if (dValue > dMaxValueY) dValue = dMaxValueY;
												// Вычисление координат 'X' & 'Y'
			dy  = (dValue - dMinValueY) * dRatioValY;
			dx += dRatioValX;		// Увеличение 'X'
			dc.LineTo (GraphicRect.left   + (int)dx + DELTASCALE,
						  GraphicRect.bottom - (int)dy - (DELTASCALE + 1));
	};
};

/*-------------------------------------------------------------------*\
 *		Отображение графика							  								*
\*-------------------------------------------------------------------*/
void
Graphics::DrawGraphicChar(TDC&   dc,      	 // DC окна
								  double CharBufferX[],// Данные по X
								  double CharBufferY[],// Данные по Y
								  double dLastValuesX[],
								  double dLastValuesY[],
								  const int	iCount){	 // Кол-во данных в массиве
	double dx, dy;			   			// Текущие координаты графика
	double dValueX, dValueY;
	int	 StartIndex = 0;
												// Изменим коэффициент отображения графика
												// для характеристики, т.е вычисляем не по количеству
												// данных, а по диапазону шкалы Х
	dRatioValX = (double)((GraphicRect.right - DELTASCALE) -
								  (GraphicRect.left  + DELTASCALE)) /
					  dRangeX;
// ------------------- Удаление старого графика -------------------------
	dc.SelectObject(hPenErase);		// Выбираем перо для удаления графика
/*	for (int i = 0; i < iCount - 1; i++){
		if (CharBufferX[i] > 0.0){
			StartIndex = i;            // Запомним индекс старовой точки
			break;
		};
	};
*/	StartIndex = 0;            // Запомним индекс старовой точки
	dValueX = dLastValuesX[StartIndex];
	dValueY = dLastValuesY[StartIndex];
												// Проверка переполнения
	if (dValueX < dMinValueX) dValueX = dMinValueX;
	if (dValueX > dMaxValueX) dValueX = dMaxValueX;
	if (dValueY < dMinValueY) dValueY = dMinValueY;
	if (dValueY > dMaxValueY) dValueY = dMaxValueY;
												// Вычисление координат 'X' & 'Y'
	dx = (dValueX - dMinValueX) * dRatioValX;
	dy = (dValueY - dMinValueY) * dRatioValY;
												// Установим первую позицию по
												// потерянному данному
	dc.MoveTo(GraphicRect.left   + (int)dx +  DELTASCALE,
				 GraphicRect.bottom - (int)dy - (DELTASCALE + 1));
												// Удалим старый график по данным массива
	for (char i = StartIndex + 1; i < iCount; i++){
			dValueX = dLastValuesX[i];
			dValueY = dLastValuesY[i];
												// Проверка переполнения
			if (dValueX < dMinValueX) dValueX = dMinValueX;
			if (dValueX > dMaxValueX) dValueX = dMaxValueX;
			if (dValueY < dMinValueY) dValueY = dMinValueY;
			if (dValueY > dMaxValueY) dValueY = dMaxValueY;
												// Вычисление координат 'X' & 'Y'
			dx = (dValueX - dMinValueX) * dRatioValX;
			dy = (dValueY - dMinValueY) * dRatioValY;
			dc.LineTo (GraphicRect.left   + (int)dx + DELTASCALE,
						  GraphicRect.bottom - (int)dy - (DELTASCALE + 1));
/*
			dc.Ellipse(GraphicRect.left   + (int)dx + DELTASCALE - 1,
						  GraphicRect.bottom - (int)dy - (DELTASCALE + 1) - 1,
						  GraphicRect.left   + (int)dx + DELTASCALE + 1,
						  GraphicRect.bottom - (int)dy - (DELTASCALE + 1) + 1);
*/
	};
// ------------------- Прорисовка графика -----------------------
	dc.SelectObject(hPenGraph);		// Выбираем перо для отображения графика
	dValueX = CharBufferX[StartIndex];
	dValueY = CharBufferY[StartIndex];
												// Проверка переполнения
	if (dValueX < dMinValueX) dValueX = dMinValueX;
	if (dValueX > dMaxValueX) dValueX = dMaxValueX;
	if (dValueY < dMinValueY) dValueY = dMinValueY;
	if (dValueY > dMaxValueY) dValueY = dMaxValueY;
												// Вычисление координат 'X' & 'Y'
	dx = (dValueX - dMinValueX) * dRatioValX;
	dy = (dValueY - dMinValueY) * dRatioValY;
												// Установим позицию в первую точку
	dc.MoveTo(GraphicRect.left   + (int)dx +  DELTASCALE,
				 GraphicRect.bottom - (int)dy - (DELTASCALE + 1));

	for (i = StartIndex + 1; i < iCount; i++){// Рисуем новый график
		 dValueX = CharBufferX[i];
		 dValueY = CharBufferY[i];
												// Проверка переполнения
		 if (dValueX < dMinValueX) dValueX = dMinValueX;
		 if (dValueX > dMaxValueX) dValueX = dMaxValueX;
		 if (dValueY < dMinValueY) dValueY = dMinValueY;
		 if (dValueY > dMaxValueY) dValueY = dMaxValueY;
												// Вычисление координат 'X' & 'Y'
		 dx = (dValueX - dMinValueX) * dRatioValX;
		 dy = (dValueY - dMinValueY) * dRatioValY;
		 dc.LineTo (GraphicRect.left   + (int)dx + DELTASCALE,
						GraphicRect.bottom - (int)dy - (DELTASCALE + 1));
/*
		 dc.Ellipse(GraphicRect.left   + (int)dx + DELTASCALE - 1,
						GraphicRect.bottom - (int)dy - (DELTASCALE + 1) - 1,
						GraphicRect.left   + (int)dx + DELTASCALE + 1,
						GraphicRect.bottom - (int)dy - (DELTASCALE + 1) + 1);
*/
	};
};

/*-------------------------------------------------------------------*\
 *		Отображение сетки на графике												*
\*-------------------------------------------------------------------*/
void
Graphics::DrawGridGraph(TDC& dc,         // DC окна
								BOOL bGridOnOff){// флаг наличия сетки
	 if (bGridOnOff){					// Отображение или удаление сетки
											// Если включили сетку
											// Выбор пера для отображения
			dc.SelectObject((HPEN)hPenGrid);
											// Отображение сетки
			DrawGrid(dc, GraphicRect);
	 } else {			 				// Удаление сетки
											// Выбор пера для удаления сетки
			dc.SelectObject((HPEN)hPenErase);
											// Отображение сетки
			DrawGrid(dc, GraphicRect);
	 }; // if() {...} esle {...};
};
/*-------------------------------------------------------------------*\
 *		Отображение легенды															*
\*-------------------------------------------------------------------*/
void
Graphics::ShowLegenda(TDC&  dc,      	// DC окна
							 char* szName,
							 int   iIndex){ 	// индекс данных в циклическом массиве данных
	int   iLengthLabel = 25;       		// Длина метки
	int 	iY; 									// Координаты метки и текста легенды
	int   iCountStringInBox; 		      // Кол-во строк, помещающиеся в высоту
													// окна легенды
	static int iCountString   = 0;	  	// Кол-во строк легенды
	static int iCountColumn   = 1;		// Кол-во столбцов легенды
	static int MaxLenStr      = 0;
	TSize size;
													// Начальные позиции 'X и 'Y'
	int   iConstX = AllRect.left + DELTAFROMGRAPH;
	int   iConstY = AllRect.bottom - DeltaForLegenda;
	static int iX = iConstX;
													// Вычисляем размеры строки в пикселах
	dc.GetTextExtent(szName, strlen(szName), size);
													// Кол-во строк, помещающиеся в высоту
													// окна легенды
	iCountStringInBox = (DeltaForLegenda) / size.cy;
													// Удалим предыдущую легенду
	if (iIndex == 0){
		iCountString = 0;						// Сбросим кол-во строк
		iCountColumn = 1;						// и столбцов
		MaxLenStr    = 0;
		iX           = iConstX;
		dc.SelectObject((HBRUSH)hbrLtGray);
		dc.SelectObject((HPEN)hPenLtGray);
		dc.Rectangle(GraphicRect.left,  AllRect.bottom - DeltaForLegenda,
						 GraphicRect.right, AllRect.bottom);
	};
	iCountString++;   	               // Увеличим кол-во строк
	if (iCountString > iCountStringInBox){
		iCountString = 1;
		iCountColumn++; 						// Увеличим кол-во столбцов
													// Позиция новай легенды
													// 10 - расстояние между элементами легенды
		iX = iX + (MaxLenStr + iLengthLabel + 10);
	};
													// Координаты метки
	iY = iConstY + size.cy * (iCountString - 1);
	dc.SelectObject((HFONT)hfontHelv);	// Выбираем шрифт
	dc.SelectObject(hPenGraph);			// Выбираем перо
													// Рисуем метку
	dc.MoveTo(iX, iY + size.cy / 2); dc.LineTo(iX + iLengthLabel, iY + size.cy / 2);
													// Текст легенды
	dc.TextOut(iX + iLengthLabel + 3,	iY, szName);
	if (size.cx > MaxLenStr)
		MaxLenStr = size.cx;
};
/* ----------------------------------------------------------
 *	EoF(usergrph.cpp)
 * ----------------------------------------------------------
 */
