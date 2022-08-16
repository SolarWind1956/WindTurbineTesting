/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class DirWindHead
	Создает модель наглядного отображения данных по направлению ветра
	и головки ВЭУ

	Member functions:
---------------------------------------------------------------------------
	DirWindHead(POINT 			  Center,  центр модели
					int 	 			  iRadius, радиус модули
					TypeDirectObject DirObj)  тип объекта (направление ветра
													  или направление головки ВЭУ)
			(Consrtuctor)
---------------------------------------------------------------------------
	~DirWindHead()
			(Destructor)
---------------------------------------------------------------------------
	DrawDirWindHead(TDC& dc)    	    	DC окна
			(Рисует корпус, окно показания, и т.д)
---------------------------------------------------------------------------
	DrawValueDirWindHead(TDC& 				  dc,			DC окна
								int  				  iIndex,	индекс данного в
																	циклическом массиве данных
								TypeDirectObject DirObj){  Тип объекта
			(Отображает одно значение датчика)
\*------------------------------------------------------------------------*/
#include <stdio.h>
#include "windhead.h"

/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
DirWindHead::DirWindHead(TPoint Center,  // центр модели
								 int 	  iRadius, // радиус модули
								 COLORREF Color){
//	------------------ Размеры фигур ------------------------
	 CenterDirWindHead = Center;		   // Центр
	 iRadiusAll      	 = iRadius;			// Радиус корпуса
													// Радиус окна
	 iRadiusDirWindHead = (int)((double)iRadius / 1.5);
													// Радиус
	 iLengthValueBox = 40;            	// Длина окна показания
	 iWidthValueBox  = 15;            	// Ширина окна показания
	 FontHeight      = 13;				  	// Высота и
	 FontWidth	     = 4;	  				// ширина шрифта
	 bOVER		     = FALSE;			  	// Флаг переполнения
													// Размеры и положение окна показания
													// для направления ветра
	 RectWindValue.right  = CenterDirWindHead.x - 2;
	 RectWindValue.left   = RectWindValue.right - iLengthValueBox;
	 RectWindValue.top    = CenterDirWindHead.y + iRadiusAll;
	 RectWindValue.bottom = RectWindValue.top   + iWidthValueBox;
													// Размеры и положение окна показания
													// для ориентации
	 RectHeadValue.left   = CenterDirWindHead.x + 2;
	 RectHeadValue.top    = CenterDirWindHead.y + iRadiusAll;
	 RectHeadValue.right	 = RectHeadValue.left  + iLengthValueBox;
	 RectHeadValue.bottom = RectHeadValue.top   + iWidthValueBox;

//	---------- Создание перьев, шрифтов, кистей ---------------------

	 hPenErase   	= CreatePen(PS_INSIDEFRAME, 1, RGB(192, 192, 192));
												// Перо "ТЕНИ" - серого цвета
	 hPenGray    	= CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо "СВЕТА" - белого цвета
	 hPenWhite   	= (HPEN)GetStockObject(WHITE_PEN);

	 lb.lbStyle 	= BS_SOLID;          // Кисть для окна показания - светло-серого цвета
	 lb.lbColor 	= RGB(192, 192, 192);
	 hbrLtGray  	= CreateBrushIndirect(&lb);
												// Перо оси
	 hPenAxis    	= CreatePen(PS_SOLID, 0, RGB(0, 128, 0));
												// Перо стрелки - зеленого цвета
	 hPenNeedle 	= CreatePen(PS_INSIDEFRAME, 1, Color);
	 lb.lbStyle 	= BS_SOLID;  		   // Кисть для стрелки - зеленого цвета
	 lb.lbColor 	= Color;
	 hbrNeedle  	= CreateBrushIndirect(&lb);
												// Перо головки - синего цвета
	 hPenHead    	= CreatePen(PS_INSIDEFRAME, 1, RGB(0, 0, 255));
												// Кисть для головки - белого цвета
	 hbrHead     	= (HBRUSH)GetStockObject(WHITE_BRUSH);
												// Создаем шрифт для показания и шкалы
	 memset(&lf, 0, sizeof(LOGFONT));
	 lf.lfHeight 	= FontHeight;			// Высота и
	 lf.lfWidth  	= FontWidth;        // ширина шрифта
	 lstrcpy((LPSTR)&lf.lfFaceName[0], (LPSTR)"Helvetica");
	 hfontHelv 		= CreateFontIndirect(&lf);

	 dOldWindRadCorner = 0.0;
	 strcpy(StOldWindValue, "");
	 dOldHeadRadCorner = 0.0;
	 strcpy(StOldHeadValue, "");
};

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r															*
\*-------------------------------------------------------------------*/
DirWindHead::~DirWindHead(){
	DeleteObject(hPenErase);   	  	// Erase block memory for user pen
	DeleteObject(hPenGray);    	  	// Erase block memory for user pen
	DeleteObject(hPenWhite);  	   	// Erase block memory for user pen
	DeleteObject(hPenAxis);  	   	// Erase block memory for user pen
	DeleteObject(hPenNeedle);	  		// Erase block memory for user pen
	DeleteObject(hPenHead); 	   	// Erase block memory for user pen
	DeleteObject((HBRUSH)hbrLtGray); // Erase block memory for user brush
	DeleteObject((HBRUSH)hbrNeedle); // Erase block memory for user brush
	DeleteObject((HBRUSH)hbrHead);   // Erase block memory for user brush
	DeleteObject((HFONT)hfontHelv);  // Erase block memory for user font
};

/*-----------------------------------------------------------------*\
 * 	Отображение окна направления и ориентации                    *
\*-----------------------------------------------------------------*/
void DirWindHead::DrawDirWindHead(TDC& dc){	// DC окна
	 TSize  size;
// ---------------------- Рисуем окно --------------------------
	 DrawLightCircle(dc, CenterDirWindHead, iRadiusAll, hPenWhite, hPenGray, RAISED);
	 DrawLightCircle(dc, CenterDirWindHead, iRadiusDirWindHead, hPenWhite, hPenGray, EMBOSSED);

// --------------------- Рисуем окна показаний ------------------------
	 dc.SelectObject((HBRUSH)hbrLtGray);
	 dc.Rectangle(RectWindValue);
	 DrawLightBorder(dc, RectWindValue, hPenWhite, hPenGray, EMBOSSED);
	 dc.Rectangle(RectHeadValue);
	 DrawLightBorder(dc, RectHeadValue, hPenWhite, hPenGray, EMBOSSED);
};

/*-----------------------------------------------------------------*\
 * 	Отображение одного значения ориентации головки               *
\*-----------------------------------------------------------------*/
void
DirWindHead::DrawValueDirHead(TDC& dc,				// DC окна
										int  iIndex){	 	// индекс данного в
																// циклическом массиве данных
	TSize  size;
	char   StValue[6] = ""; 		// Строка показания
	double dValue;
	double dHeadRadCorner;

	dValue = ValuesInstruments[iIndex];
	dHeadRadCorner = GRAD_TO_RAD(-dValue);

// ------------ У д а л е н и е   п о к а з а н и й -------------------
	dc.SelectObject((HFONT)hfontHelv);
	dc.SetTextColor(TColor::LtGray); 		// Устанавливаем цвет текста
														// Зарисуем старый текст
	dc.TextOut(RectHeadValue.left + 2, RectHeadValue.top + 2, StOldHeadValue);

// ------------ У д а л е н и е   с т а р о й   г о л о в к и ----------
												// Выбор пера для удаления старой стрелки
	dc.SelectObject((HBRUSH)hbrLtGray);
	dc.SelectObject((HPEN)hPenErase);// Удалим старую
	DrawShapeDirHead(dc, dOldHeadRadCorner);

// ------------------ П о к а з а н и я -------------------------------
	 if (dValue <= 360.0 && dValue >= 0.0){// Сравнение для определения переполнения
		 bOVER = FALSE;                     // Сбросим флаг переполнения
		 dc.SetTextColor(TColor::LtBlue);	// Устанавливаем цвет текста
		 sprintf(StValue, "%3.1f", dValue); // Формируем текст показания
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 dc.SetTextColor(TColor::LtRed);		// Устанавливаем цвет сообщения
		 if (dValue > 360.0)	dValue = 360.0;
		 if (dValue < 0.0	 ) dValue = 0.0;
		 sprintf(StValue, "OVER");
	 };
	dc.TextOut(RectHeadValue.left + 2, RectHeadValue.top + 2, StValue);

// ------------ П р о р и с о в к а   н о в о й   г о л о в к и ----------
	dc.SelectObject((HPEN)hPenHead); // Загрузим перо
	dc.SelectObject((HBRUSH)hbrHead);
	DrawShapeDirHead(dc, dHeadRadCorner);

												 // Запомним данные удаления
	dOldHeadRadCorner = dHeadRadCorner;
	strcpy(StOldHeadValue, StValue);
};

/*-----------------------------------------------------------------*\
 * 	Отображение одного значения направления ветра	             *
\*-----------------------------------------------------------------*/
void
DirWindHead::DrawValueDirWind(TDC& dc,				// DC окна
										int  iIndex){	 	// индекс данного в
	TSize  size;											// циклическом массиве данных
	char   StValue[6] = ""; 							// Строка показания
	double dValue;											// Данное для показания в окне показания
	double dWindRadCorner;								// Угол для показания на приборе

	dValue = ValuesInstruments[iIndex];          // Вычислим угол
	dWindRadCorner = GRAD_TO_RAD(-dValue);

// ------------ У д а л е н и е   п о к а з а н и й -------------------
	dc.SelectObject((HFONT)hfontHelv);
	dc.SetTextColor(TColor::LtGray); 		// Устанавливаем цвет текста
														// Зарисуем старый текст
	dc.TextOut(RectWindValue.left + 2, RectWindValue.top + 2, StOldWindValue);

// ------------ У д а л е н и е   с т а р о й   с т р е л к и ----------
												// Выбор пера для удаления старой стрелки
	dc.SelectObject((HBRUSH)hbrLtGray);
	dc.SelectObject((HPEN)hPenErase);// Удалим старую
	DrawShapeDirWind(dc, dOldWindRadCorner);

// ------------------ П о к а з а н и я -------------------------------
	 if (dValue <= 360.0 && dValue >= 0.0){// Сравнение для определения переполнения
		 bOVER = FALSE;                     // Сбросим флаг переполнения
														// Устанавливаем цвет текста
		 dc.SetTextColor(TColor(lb.lbColor));
		 sprintf(StValue, "%3.1f", dValue); // Формируем текст показания
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 dc.SetTextColor(TColor::LtRed);		// Устанавливаем цвет сообщения
		 if (dValue > 360.0)	dValue = 360.0;
		 if (dValue < 0.0	 ) dValue = 0.0;
		 sprintf(StValue, "OVER");
	 };
	dc.TextOut(RectWindValue.left + 2, RectWindValue.top + 2, StValue);

// ------------------ О с и  ------------------------------------------
	dc.SelectObject((HPEN)hPenAxis);
	for (int i = 0; i < 360; i += 45){
		dc.MoveTo(CenterDirWindHead + AffinedRotate((double)GRAD_TO_RAD(i),
									0, - (iRadiusAll - 3)));
		dc.LineTo(CenterDirWindHead + AffinedRotate((double)GRAD_TO_RAD(i),
									0, - (iRadiusDirWindHead + 3)));
	};
// ------------ П р о р и с о в к а   н о в о й   с т р е л к и -------
											// Загрузим перо
	dc.SelectObject((HBRUSH)hbrNeedle);
	dc.SelectObject((HPEN)hPenNeedle);
											// Отобразим новую
	DrawShapeDirWind(dc, dWindRadCorner);

	dOldWindRadCorner = dWindRadCorner;
	strcpy(StOldWindValue, StValue);
};

/*-----------------------------------------------------------------*\
 * 	Отображение стрелок направления ветра				             *
\*-----------------------------------------------------------------*/
void
DirWindHead::DrawShapeDirWind(TDC&   dc,				// DC окна
										double dAngle){     	// угол поворота
	TPoint TipO = CenterDirWindHead;
	TPoint NeedlePoint[3];

	for (char i = -5; i <= 5; i += 5){
		NeedlePoint[0] = TipO + AffinedRotate(dAngle,  i,     - (iRadiusDirWindHead + 3));
		NeedlePoint[1] = TipO + AffinedRotate(dAngle,  i - 2, - (iRadiusDirWindHead + 7));
		NeedlePoint[2] = TipO + AffinedRotate(dAngle,  i + 2, - (iRadiusDirWindHead + 7));
		dc.Polygon(NeedlePoint, 3);

		dc.MoveTo(TipO + AffinedRotate(dAngle,  i, - (iRadiusAll - 3)));
		dc.LineTo(TipO + AffinedRotate(dAngle,  i, - (iRadiusDirWindHead + 7)));
	};
};

/*-----------------------------------------------------------------*\
 *			  12																		 *
 *			  /\                                                      *
 *		  11 -  13                                                   *
 *		  /10--0\                                                  	 *
 *		 9       1    	Определение точек головки                   	 *
 *		8         2    Определяем точки 0, 1, 2, 3, 4, 5           	 *
 *		|			 |    Точки 6, 7, 8, 9, 10 являются симметричными 	 *
 *		7			 3    точкам 0, 1, 2, 3, 4                        	 *
 *		 \       /                                                 	 *
 *	 	  \     /                                                  	 *
 *			6   4                                                   	 *
 *			 \5/                                                    	 *
\*-----------------------------------------------------------------*/
void
DirWindHead::DrawShapeDirHead(TDC&   dc,				// DC окна
										double dAngle){     	// угол поворота
	const  int CountPoints = 11;                    // Кол-во точек
	int	 R = iRadiusDirWindHead - 2;	            // Радиус окна
	TPoint DrawPoints[CountPoints];                 // массив точек
	TPoint PicPoints [3];
	TPoint TipO;                                    // Точка центра

	TipO   = CenterDirWindHead;
	PicPoints [0]    = AffinedRotate(dAngle,    (double)R * 0.15, -(double)R * 0.8);
	PicPoints [2]    = AffinedRotate(dAngle,  - (double)R * 0.15, -(double)R * 0.8);
	PicPoints [1]    = AffinedRotate(dAngle,  				  0.0,  -(double)R);

	DrawPoints[0]    = AffinedRotate(dAngle,    (double)R * 0.2,  -(double)R * 0.8);
	DrawPoints[10]   = AffinedRotate(dAngle,  - (double)R * 0.2,  -(double)R * 0.8);

	DrawPoints[1]    = AffinedRotate(dAngle,    (double)R * 0.35, -(double)R * 0.7);
	DrawPoints[9]    = AffinedRotate(dAngle,  - (double)R * 0.35, -(double)R * 0.7);

	DrawPoints[2]    = AffinedRotate(dAngle,    (double)R * 0.45, -(double)R * 0.6);
	DrawPoints[8]    = AffinedRotate(dAngle,  - (double)R * 0.45, -(double)R * 0.6);

	DrawPoints[3]    = AffinedRotate(dAngle,    (double)R * 0.43,  (double)R * 0.1);
	DrawPoints[7]    = AffinedRotate(dAngle,  - (double)R * 0.43,  (double)R * 0.1);

	DrawPoints[4]    = AffinedRotate(dAngle,    (double)R * 0.2,   (double)R * 0.8);
	DrawPoints[6]    = AffinedRotate(dAngle,  - (double)R * 0.2,   (double)R * 0.8);

	DrawPoints[5]    = AffinedRotate(dAngle,    0.0,					(double)R * 0.9);

	for (char i = 0; i < CountPoints; i++)
			  DrawPoints[i] += TipO;
	for (		 i = 0; i < 3; i++)
			  PicPoints[i] += TipO;

	dc.Polygon(DrawPoints, CountPoints);
	dc.Polygon(PicPoints, 3);	
};
/* ----------------------------------------------------------
 *	EoF(windhead.cpp)
 * ----------------------------------------------------------
 */
