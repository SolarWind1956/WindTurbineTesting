#if !defined(__USERDRAW_H)
#define __USERDRAW_H
/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	Библиотека графических объектов
\*------------------------------------------------------------------------*/
#include <owl\owlpch.h>
#include <owl\dc.h>
#include "globgrph.h"

void DrawLightBorder(TDC&, TRect&,             HPEN, HPEN, TypeBorder);
void DrawLightBorder(TDC&, int, int, int, int, HPEN, HPEN, TypeBorder);
void DrawLightCircle(TDC&, TPoint&,  int,      HPEN, HPEN, TypeBorder);
void DrawLightCircle(TDC&, int,      int, int, HPEN, HPEN, TypeBorder);
void Sun  			  (TDC&, TPoint&,       int, HBRUSH, HPEN);
void Sun  			  (TDC&, int,		 int, int, HBRUSH, HPEN);
void Cloud 			  (TDC&, TRect&, 				  HBRUSH, HPEN);
void Cloud 			  (TDC&, int, int, int, int, HBRUSH, HPEN);
void DrawCircleScale(TDC&, int, int, int, char*, int,	int);
void DrawCircleScale(TDC&, TPoint&,  int, char*, int,	int);
void DrawRectScale  (TDC&, TypePosScale, int, int, int, int,
											char*,  double, double,
											char*,  double, double);
void DrawRectScale  (TDC&, TypePosScale, TRect&,
											char*,  double, double,
											char*,  double, double);
void DrawGrid  	  (TDC&, int, int, int, int);
void DrawGrid		  (TDC&, TRect& 				);
void DrawBitMap	  (TDC&, TBitmap&, int, int);
void Arc4			  (TDC&, TPoint&, int, TPoint&, TPoint&);
TPoint AffinedRotate(double, TPoint&);
TPoint AffinedRotate(double, int, int);
void SetCenter	(int x, int y);
void SetCenter	(const TPoint& point);
BOOL MoveToCorner(TDC& dc, int cx, int cy, int x, int y, double corner);
BOOL MoveToCorner(TDC& dc, const TPoint& center, const TPoint& point, double corner);
BOOL LineToCorner(TDC& dc, int cx, int cy, int x, int y, double corner);
BOOL LineToCorner(TDC& dc, const TPoint& center, const TPoint& point, double corner);
#endif  // __USERDRAW_H
/* ----------------------------------------------------------
 *	EoF(userdraw.h)
 * ----------------------------------------------------------
 */
