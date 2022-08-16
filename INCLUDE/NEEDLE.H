#if !defined(__NEEDLE_H)
#define __NEEDLE_H
/*------------------------------------------------------------------------*/
/*	 Author: Sergey E. Heckel
/*	   Crimea Engineering Centre
/*	   Phone: (06557) 68-2-39
/*                                                                        */
/*  class SimpleNeedle                                                    */
/*                                                                        */
/*      class SimpleNeedle draws an SimpleNeedle.                         */
/*                                                                        */
/*  Member functions:                                                     */
/*                                                                        */
/*      SimpleNeedle(int StockLength, TPoint Centre);						     */
/*                                                                        */
/* 			Создает простую стрелку длиной 	StockLength и направленную	  */
/*				центр стрелки имеет координаты 	Centre							  */
/*          																				  */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

class SimpleNeedle
{

public:

	 double dCorner;

	 TPoint TPCentre, TPText;
	 TPoint Tip1,  Tip2,  Tip3, Tip4, Tip5, Tip6,
			  Tip7,  Tip8,  Tip9, Tip10, Tip11, Tip12;
	 LOGFONT  lf;							// Structure for create user font
	 HANDLE   hfontHelv;					// Handle user font
	 LOGBRUSH lb;
	 HBRUSH   hbrRed, hbrWhite, hbr;

	 int wStockLength;
	 int wHeadLength;
	 int wHeadWidth;
	 int wTipLength;
	 int wTipWidth;
	 HPEN hPenAxis, hPenCircle, hPen;

	 SimpleNeedle ( int StockLength, TPoint Centre, COLORREF);
	 ~SimpleNeedle ();
	 void DrawNeedle(TDC&);
	 void DrawValueNeedle(TDC&, double, BOOL, const char*, BOOL);
};
#endif  // __NEEDLE_H

