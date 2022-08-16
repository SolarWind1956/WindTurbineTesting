/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	class Barometr
	Создает модель наглядного отображения данных по барометрическому давлению

	Member functions:
---------------------------------------------------------------------------
		Barometr(TPoint CenterBarom,      центр барометра
					int 	iRadiusBarom,		радиус барометра
					int 	iMin,             минимальное показание
					int 	iMax,             максимальное показание
					char* sDimension) 		текст размерности
			(Consrtuctor)
---------------------------------------------------------------------------
		~Barometr()
			(Destructor)
---------------------------------------------------------------------------
		DrawBarometr(TDC& dc)          	DC окна
			(Рисует корпус, окно показания, и т.д)
---------------------------------------------------------------------------
		DrawValueBarometr(TDC& dc,		DC окна
							int  iIndex)	индекс данного в циклическом массиве данных
			(Отображает одно значение датчика)
\*------------------------------------------------------------------------*/
#include <stdio.h>
#include "barometr.h"
/*-------------------------------------------------------------------*\
 *		C o n s t r u c t o r 														*
\*-------------------------------------------------------------------*/
Barometr::Barometr(TPoint CenterBarom, // центр барометра
						 int iRadiusBarom,  // радиус барометра
						 int iMin,          // минимальное показание
						 int iMax,          // максимальное показание
						 char* sDimension){ // текст размерности
	 strcpy(sDim, sDimension);			// Запомним строку размерности
	 iMinValue = iMin;  					// Минимальное допустимое значение термометра
	 iMaxValue = iMax;		  			// Максимальное допустимое значение термометра
	 iRange    = iMax - iMin; 			// Диапазон термометра

//	------------------ Размеры фигур ------------------------
	 Center		   = CenterBarom;    // Центр барометра
	 iRadius       = iRadiusBarom;  	// Радиус окна барометра
												// Радиус барометра
	 iRadiusTwo    = (int)((double)iRadiusBarom / 1.1);
	 iRadiusOut    = iRadiusBarom / 6;// Радиус крепления стрелки
	 iRadiusSun		= iRadiusBarom / 7;// Радиус солнца
	 iLengthCloud	= iRadiusBarom / 3;// Длина облака
	 iHeightCloud	= iRadiusBarom / 7;// Высота облака
												 // Длина стрелки
	 iLengthNeedle  = (int)((double)iRadiusBarom / 1.2);
	 iLengthTextBox = 35;  			   	// Длина окна показания
	 FontHeight     = 13;					// Высота и
	 FontWidth	    = 4;						// ширина шрифта
	 bOVER		    = FALSE;				// Флаг переполнения

//	---------- Создание перьев, шрифтов, кистей ---------------------
												// Перо "ТЕНИ" - серого цвета
	 hPenGray    = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
												// Перо контура - черного цвета
	 hPenOutline = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
												// Перо "ОСВЕЩЕНИЯ" - белого цвета
	 hPenWhite   = (HPEN)GetStockObject(WHITE_PEN);
												// Перо солнца - желтого цвета
	 hPenSun     = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
												// Перо стрелки и тучи - синего цвета
	 hPenNeedle  = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

												// Кисть для барометра - белого цвета
	 hbrWhite    = (HBRUSH)GetStockObject(WHITE_BRUSH);
												// Кисть для фона - серого цвета
	 hbrGray     = (HBRUSH)GetStockObject(GRAY_BRUSH);
												// Кисть для окна показания - светло-серого цвета
	 hbrLtGray   = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
												// Создаем кисть для СОЛНЦА - желтого цвета
	 lb.lbStyle = BS_SOLID;
	 lb.lbColor = RGB(255, 255, 0);
	 hbrSun     = CreateBrushIndirect(&lb);
												// Создаем кисть для ТУЧИ - синего цвета
	 lb.lbStyle = BS_SOLID;
	 lb.lbColor = RGB(0, 0, 255);
	 hbrCloud   = CreateBrushIndirect(&lb);
												// Создаем шрифт для показания и шкалы
	 memset(&lf, 0, sizeof(LOGFONT));
	 lf.lfHeight = FontHeight;			// Высота и
	 lf.lfWidth  = FontWidth;        // ширина шрифта
	 lstrcpy((LPSTR)&lf.lfFaceName[0], (LPSTR)"Helvetica");
	 hfontHelv = CreateFontIndirect(&lf);

	 dOldRadCorner = 0.0;
	 strcpy(StOldValue, "");
};

/*-------------------------------------------------------------------*\
 *		D e s t r u c t o r															*
\*-------------------------------------------------------------------*/
Barometr::~Barometr()
{
	 DeleteObject(hPenGray);      	// Erase block memory for user pen
	 DeleteObject(hPenOutline);   	   // Erase block memory for user pen
	 DeleteObject(hPenWhite);     	// Erase block memory for user pen
	 DeleteObject(hPenSun);  	   	// Erase block memory for user pen
	 DeleteObject(hPenNeedle);     	// Erase block memory for user pen
	 DeleteObject((HBRUSH)hbrWhite); // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrGray);  // Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrLtGray);// Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrSun);	// Erase block memory for user brush
	 DeleteObject((HBRUSH)hbrCloud);	// Erase block memory for user brush
	 DeleteObject((HFONT)hfontHelv); // Erase block memory for user font
};

/*-----------------------------------------------------------------*\
 * 	Отображение барометра                                        *
\*-----------------------------------------------------------------*/
void Barometr::DrawBarometr(TDC& dc){     // DC окна
	 TSize size;
// ---------------------- Рисуем барометр --------------------------
/*	 dc.SelectObject((HBRUSH)hbrLtGray);   // Рисуем окно барометра
	 dc.Ellipse(Center.x - iRadius, Center.y - iRadius,
					Center.x + iRadius, Center.y + iRadius);
*/	 DrawLightCircle(dc, Center, iRadius,    hPenWhite, hPenGray, RAISED);
														// Рисуем корпус барометра
/*	 dc.Ellipse(Center.x - iRadiusTwo, Center.y - iRadiusTwo,
					Center.x + iRadiusTwo, Center.y + iRadiusTwo);
*/	 DrawLightCircle(dc, Center, iRadiusTwo, hPenWhite, hPenGray, EMBOSSED);
	 dc.SelectObject((HBRUSH)hbrWhite);    // Рисуем окно графика барометра
	 dc.SelectObject((HPEN)hPenOutline);
	 dc.Chord(Center.x - iRadiusTwo + 3, Center.y - iRadiusTwo + 3,
				 Center.x + iRadiusTwo - 3, Center.y + iRadiusTwo - 3,
				 Center.x + iRadiusTwo - 3, Center.y,
				 Center.x - iRadiusTwo + 3, Center.y);
	 dc.SelectObject((HBRUSH)hbrLtGray);   // Рисуем крепление стрелки барометра
	 dc.Ellipse(Center.x - iRadiusOut, Center.y - iRadiusOut,
					Center.x + iRadiusOut, Center.y + iRadiusOut);
	 DrawLightCircle(dc, Center, iRadiusOut, hPenWhite, hPenGray, RAISED);

														// Рисуем окно показания
	 dc.Rectangle(Center.x - iLengthTextBox / 2,
					  Center.y + iRadiusTwo - DELTAFROMBAROM,
					  Center.x + iLengthTextBox / 2,
					  Center.y + iRadiusTwo - DELTAFROMBAROM / 3);
	 DrawLightBorder(dc, Center.x - iLengthTextBox / 2,
								Center.y + iRadiusTwo - DELTAFROMBAROM,
								Center.x + iLengthTextBox / 2,
								Center.y + iRadiusTwo - DELTAFROMBAROM / 3,
								hPenWhite, hPenGray, EMBOSSED);

// ---------------------- С о л н ц е ------------------------------
	 Sun  (dc, Center.x + iRadiusTwo / 2,
				  Center.y + iRadiusSun + 2,
				  iRadiusSun, hbrSun, hPenSun);
// ---------------------- О б л а к о ------------------------------
	 Cloud(dc, Center.x - iRadiusTwo / 2 - iLengthCloud / 2,
				  Center.y + iRadiusSun + 2 - iHeightCloud / 2,
				  Center.x - iRadiusTwo / 2 + iLengthCloud / 2,
				  Center.y + iRadiusSun + 2 + iHeightCloud / 2,
				  hbrCloud, hPenNeedle);

// ------------------ Ш к а л а ------------------------------------
	 dc.SelectObject((HPEN)hPenOutline);
	 dc.SetTextColor(TColor::Black);
	 dc.SelectObject((HFONT)hfontHelv);
	 DrawCircleScale(dc, Center, iLengthNeedle, sDim, iMinValue, iRange);
};

/*-----------------------------------------------------------------*\
 * 	Отображение одного значения барометра                        *
\*-----------------------------------------------------------------*/
void
Barometr::DrawValueBarometr(TDC& dc,      // DC окна
									 int  iIndex){ // индекс данного в циклическом массиве данных
	 double dValue;
	 double dRadCorner;
	 char   StValue[6];

	 dValue = ValuesInstruments[iIndex];
// ------------------ П о к а з а н и е ----------------------------
	 dc.SelectObject((HFONT)hfontHelv);
	 dc.SetTextColor(TColor::LtGray);		// Отображаем текст внизу и по центру барометра
														// Зарисуем старый текст
	 dc.TextOut(Center.x - iLengthTextBox / 2 + 2,
					Center.y + iRadiusTwo - DELTAFROMBAROM, StOldValue);
														// Сравнение для определения переполнения
	 if (dValue <= (double)iMaxValue && dValue >= (double)iMinValue){
		 bOVER = FALSE;                     // Сбросим флаг переполнения
		 dc.SetTextColor(TColor::Black);
		 sprintf(StValue, "%3.1f", dValue); // Формируем текст показания
	 } else {                              // Если переполнение, то сообщаем
		 bOVER = TRUE;								// Взводим флаг переполнения
		 if (dValue > (double)iMaxValue) dValue = (double)iMaxValue;
		 if (dValue < (double)iMinValue) dValue = (double)iMinValue;
		 dc.SetTextColor(TColor::LtRed);
		 sprintf(StValue, "OVER");
	 };												// Отображаем текст внизу и по центру барометра
	 dc.TextOut(Center.x - iLengthTextBox / 2 + 2,
					Center.y + iRadiusTwo - DELTAFROMBAROM, StValue);

// ------------------ Г р а ф и к ----------------------------------
	 dRadCorner = GRAD_TO_RAD((dValue - (double)iMinValue) * 1.8) - GRAD_TO_RAD(90.0);
	 dc.SelectObject((HPEN)hPenWhite);    // Выбор пера для удаления старой стрелки
	 MoveToCorner(dc, Center.x, Center.y, iRadiusOut + 1, iRadiusOut + 1, dOldRadCorner);
	 LineToCorner(dc, Center.x, Center.y, iLengthNeedle,  iLengthNeedle,  dOldRadCorner);

// ------------------ Ш к а л а ------------------------------------
	 dc.SelectObject((HPEN)hPenOutline);
	 dc.SetTextColor(TColor::Black);
	 dc.SelectObject((HFONT)hfontHelv);
	 DrawCircleScale(dc, Center, iLengthNeedle, sDim, iMinValue, iRange);

	 dc.SelectObject((HPEN)hPenNeedle);    // Выбор пера для стрелки
	 MoveToCorner(dc, Center.x, Center.y, iRadiusOut + 1, iRadiusOut + 1, dRadCorner);
	 LineToCorner(dc, Center.x, Center.y, iLengthNeedle,  iLengthNeedle,  dRadCorner);

	 dOldRadCorner = dRadCorner;				// Запомним старое значение
	 strcpy(StOldValue, StValue);
};
/* ----------------------------------------------------------
 *	EoF(barometr.cpp)
 * ----------------------------------------------------------
 */
