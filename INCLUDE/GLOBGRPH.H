#if !defined(__GLOBGRPH_H)
#define __GLOBGRPH_H
/*
 * CommGlob.h	-	файл общей области глобальных массивов, переменных и т.д.
 *						для графической части пакета
 *
 * Author: Konstantin A. Davidov
 *  Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
#include <math.h>
#include "cyclarry.h"
//----------------------------------------------------------------------------
#define GRAD_TO_RAD(X) (M_PI * X / 180.0)

#define QUANTVALUES      60   		// Кол-во значений в графике
#define COUNTGRAPHICS    9  			// Кол-во отображаемых графиков
#define COUNTINSTRUMENTS 10 			// Кол-во отображаемых приборов
#define AXISDELTA        3     	   // Расстояние от верхней и нижней границы до оси
#define QUANTRAYS 		 17	 		// Кол-во лучей для солнца
#define DELTASCALE       20 			// Отступ шкалы от окна
#define LENGTHMARK		 6				// Длина засечки
#define QUANTMARKCIRCLE  5	 			// Кол-во делений на круговой шкале
#define QUANTMARKRECT    5.0	   	// Кол-во делений на прямоугольной шкале по Y
#define COUNTMARKX		 5.0	   	// Кол-во делений на прямоугольной шкале по X
#define DELTAFROMSENS  	 5				// Отступ прибора от границы окна или от
												// другого прибора
#define DELTAFROMTERM    2 			// Отступ окна показаний и термометра
												// от общей нижней и правой границы
#define DELTAFROMBAROM 	20	 	   	// Отступ окна показаний от барометра
#define DELTAFROMMULT  	4   			// Отступ окна показаний и термометра
												// от общей нижней и правой границы
#define DELTAFROMSTOCK 	2 				// Отступ окна показаний и шток
												// от общей нижней и правой границы
#define DELTAFROMGRAPH 	2 				// Отступ окна от общей границы

#define COUNTRUMBS  	  	16	  			// Количество румбов
#define RADCORNER22_5  	GRAD_TO_RAD(22.5)
#define RADCORNER45    	GRAD_TO_RAD(45.0)
#define RADCORNER67_5  	GRAD_TO_RAD(67.5)

/*---------------------------------------------------------------------*\
 *  Class TCycleBuffer                                                 *
\*---------------------------------------------------------------------*/
class TCycleBuffer {					// Массив циклических массивов
public:
	TCycleArray<double> CA; 		// Массив показаний текущего прибора
	TCycleArrayIterator<double> CAIter;	// Итератор этого массива
											// Конструктор
	TCycleBuffer() : CA(QUANTVALUES + 1), CAIter(CA)
	{};
};
												// Массив циклических массивов
extern TCycleBuffer  BuffValues [COUNTGRAPHICS];
extern double 			LostValues [COUNTGRAPHICS];
extern double 			ValuesInstruments[COUNTINSTRUMENTS];
												//	Позиция прямоугольной шкалы
enum TypePosScale    {LeftScale, RightScale};
												// Типы рамки
enum TypeBorder      {RAISED, EMBOSSED};
												// Типы объектов по направлению
enum TypeDirectObject{DIRWIND, DIRHEAD, ALLDIRECT};

typedef struct {        			 	// Структура одного канала отображения
	char   		 szName   [30];  		// Название канала отображения
	char     	 szNameVal[10]; 	 	// Измеряемые еденицы
	int    		 iMin;	  			 	// Минимальное допустимое значение
	int    		 iMax;				 	// Максимальное допустимое значение
	COLORREF 	 Color;					// Цвет графика
	int      	 IndexGroupGraph;		// Индекс группы графиков
	int		 	 IndexElementGroup;	// Номер графика в группе
	TypePosScale PosScale;				// К какой шкале относится
} ConfigSignals;
												// Массив структур каналов отображения
												// для отображения определенного
												// набора графиков для левой и правой шкалы
extern ConfigSignals aListParamGraphics[COUNTGRAPHICS];
												// Массив структур каналов отображения
												// для отображения приборов
extern ConfigSignals aListParamInstruments[COUNTINSTRUMENTS];
/*
 * ---------------------------------------------------------
 *	EoF(globgrph.h)
 * ----------------------------------------------------------
 */
#endif  // __GLOBGRPH_H
