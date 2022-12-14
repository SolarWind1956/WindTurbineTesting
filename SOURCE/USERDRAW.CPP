/*------------------------------------------------------------------------*\
 *	 Author: Konstantin A. Davidov                                         *
 *	   Crimea Engineering Centre                                           *
 *	   Phone: (06557) 68-2-39                                              *
\*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*\
	?????????? ??????????? ???????? ? ???????
\*------------------------------------------------------------------------*/
#include <stdio.h>

#include "userdraw.h"

/*-------------------------------------------------------------------*\
 *		?????? ???????? ????															*
\*-------------------------------------------------------------------*/
void
DrawLightCircle(TDC& dc, 				// ?????????? ????????
					 int iCenterX, 		// ? ??????
					 int iCenterY, 		// ? ??????
					 int iRadius,			// ??????
					 HPEN hPenWhite, 		// ???? "?????"
					 HPEN hPenGray, 		// ???? "????"
					 TypeBorder TypeB){	// ??? (????????, ????????, ? ?.?)
	TPoint Center;
	Center.x = iCenterX;
	Center.y = iCenterY;
	DrawLightCircle(dc, Center, iRadius, hPenWhite, hPenGray, TypeB);
};
void
DrawLightCircle(TDC& dc, TPoint& Center, int iRadius,
		 HPEN hPenWhite, HPEN hPenGray, TypeBorder TypeB){
	if (TypeB == RAISED)						 // ????????
		 dc.SelectObject((HPEN)hPenGray); // ????? ????? "????"
	else
		 dc.SelectObject((HPEN)hPenWhite);// ????? ????? "?????????"
													 // ?????? ? ?????? ?????
	dc.Arc(Center.x - iRadius, Center.y - iRadius,
			 Center.x + iRadius, Center.y + iRadius,
			 Center.x - iRadius, Center.y + iRadius,
			 Center.x + iRadius, Center.y - iRadius);

	if (TypeB == EMBOSSED)               // ????????
		 dc.SelectObject((HPEN)hPenGray); // ????? ????? "????"
	else
		 dc.SelectObject((HPEN)hPenWhite);// ????? ????? "?????????"
													 // ????? ? ??????? ?????
	dc.Arc(Center.x - iRadius, Center.y - iRadius,
			 Center.x + iRadius, Center.y + iRadius,
			 Center.x + iRadius, Center.y - iRadius,
			 Center.x - iRadius, Center.y + iRadius);
};

/*-------------------------------------------------------------------*\
 *		?????? ???????? ?????														*
\*-------------------------------------------------------------------*/
void
DrawLightBorder(TDC& dc,		 		// ?????????? ????????
												// ?????????? ? ??????
					int ix1, int iy1, int ix2, int iy2,
					HPEN 		  hPenWhite,// ???? "?????"
					HPEN 		  hPenGray,	// ???? "????"
					TypeBorder TypeB){	// ??? (????????, ????????, ? ?.?)
	TRect RectB;
	RectB.left = ix1; RectB.top = iy1; RectB.right = ix2; RectB.bottom = iy2;
	DrawLightBorder(dc, RectB, hPenWhite, hPenGray, TypeB);
};
void
DrawLightBorder(TDC& dc, TRect& RectB,
		 HPEN hPenWhite, HPEN hPenGray, TypeBorder TypeB){
	if (TypeB == RAISED)						 // ????????
		 dc.SelectObject((HPEN)hPenGray); // ????? ????? "????"
	else
		 dc.SelectObject((HPEN)hPenWhite);// ????? ????? "?????????"
													 // ?????? ? ?????? ?????
	dc.MoveTo(RectB.right - 1, RectB.top);
	dc.LineTo(RectB.right - 1, RectB.bottom - 1);
	dc.LineTo(RectB.left,      RectB.bottom - 1);

	if (TypeB == EMBOSSED)               // ????????
		 dc.SelectObject((HPEN)hPenGray); // ????? ????? "????"
	else
		 dc.SelectObject((HPEN)hPenWhite);// ????? ????? "?????????"

	dc.MoveTo(RectB.right, RectB.top);	 // ????? ? ??????? ?????
	dc.LineTo(RectB.left,  RectB.top);
	dc.LineTo(RectB.left,  RectB.bottom);
};

/*-------------------------------------------------------------------*\
 *		?????? ??????																	*
 *		???? ?????? ? ???? (? ???? ??????, ? ???-??? ?????					*
 *								  ?????? QUANTRAYS - 1)                      *
\*-------------------------------------------------------------------*/
void Sun(TDC& dc,					// ?????????? ????????
			int iCenterX, 			// ? ??????
			int iCenterY, 			// ? ??????
			int iRadiusSun,		// ??????
			HBRUSH hbrSun, 		// ?????
			HPEN hPenSun){			// ????
	TPoint Center;
	Center.x = iCenterX;
	Center.y = iCenterY;
	Sun(dc, Center, iRadiusSun, hbrSun, hPenSun);
};
void Sun(TDC& dc, TPoint& Center, int iRadiusSun,
			HBRUSH hbrSun, HPEN hPenSun){
												// ???? ????? ??????
	double dCornerRay = 360.0 / (double)(QUANTRAYS - 1);
	TPoint RaysPoint[QUANTRAYS];  	// ?????? ????? ?????
	div_t  t;								// ???????? ???? ? ????? ??? ??????
	dc.SelectObject((HBRUSH)hbrSun); dc.SelectObject((HPEN)hPenSun);
												// ????????? ?????? ????? ?????
	for (char i = 0; i < QUANTRAYS; i++){
		double dRadCorner = ((double)i * dCornerRay / 360.0) * (2.0 * 3.14159);
		t = div(i, 2);
		if (t.rem == 0){					// ??????? ????? ????
			RaysPoint[i].x = Center.x + (int)((double)iRadiusSun * sin(dRadCorner));
			RaysPoint[i].y = Center.y - (int)((double)iRadiusSun * cos(dRadCorner));
		} else {                      // ?????? ????? ????
			RaysPoint[i].x = Center.x + (int)((double)iRadiusSun / 2 * sin(dRadCorner));
			RaysPoint[i].y = Center.y - (int)((double)iRadiusSun / 2 * cos(dRadCorner));
		};
	};                               // ?????? ????
	dc.Polyline(RaysPoint, QUANTRAYS);
												// ?????? ???? ??????
	dc.Ellipse(Center.x - iRadiusSun / 3, Center.y - iRadiusSun / 3,
				  Center.x + iRadiusSun / 3, Center.y + iRadiusSun / 3);
};

/*-------------------------------------------------------------------*\
 *		?????? ??????																	*
\*-------------------------------------------------------------------*/
void Cloud(TDC& dc,		 		// ?????????? ????????
										// ?????????? ? ??????
			  int ix1, int iy1, int ix2, int iy2,
			  HBRUSH hbrCloud, 	// ?????
			  HPEN hPenCloud){	// ????
	TRect RectCloud;
	RectCloud.left  = ix1; RectCloud.top    = iy1;
	RectCloud.right = ix2; RectCloud.bottom = iy2;
	Cloud(dc, RectCloud, hbrCloud, hPenCloud);
};
void Cloud(TDC& dc, TRect& CloudRect, HBRUSH hbrCloud, HPEN hPenCloud){
	div_t t;									// ??????? ???????
	int   iDiametrE = (int)((double)(CloudRect.bottom - CloudRect.top) / 1.5);
	int   iQuantE;                    // ???-?? ????????
	t = div(CloudRect.right - CloudRect.left, iDiametrE);
	iQuantE = t.quot;                 // ???????? ???? ? ????? ??? ??????
	dc.SelectObject((HBRUSH)hbrCloud); dc.SelectObject((HPEN)hPenCloud);
												// ?????? ???? ??????
	for (char i = 0; i < iQuantE; i++){
		dc.Ellipse(CloudRect.left + i * iDiametrE,
					  CloudRect.top,
					  CloudRect.left + (i+1) * iDiametrE,
					  CloudRect.top + iDiametrE);
	};											// ?????? ??? ??????
	for (i = 0; i < iQuantE; i++){
		dc.Ellipse(CloudRect.left + i * iDiametrE + iDiametrE / 2,
					  CloudRect.bottom - iDiametrE,
					  CloudRect.left + (i+1) * iDiametrE + iDiametrE / 2,
					  CloudRect.bottom);
	};
};

/*-----------------------------------------------------------------*\
 * 	??????????? ???????? ?????										       *
\*-----------------------------------------------------------------*/
void DrawCircleScale(TDC&  dc,		// ?????????? ????????
			  int   ix,						// ????? ????? X
			  int   iy,          		// ????? ????? Y
			  int   iLengthNeedle,		// ????? ???????
			  char* sDim,					// ???????????
			  int   iMinValue,			// ???????
			  int   iRange){     		// ????????
	TPoint Center;
	Center.x = ix; Center.y = iy;
	DrawCircleScale(dc, Center, iLengthNeedle, sDim, iMinValue, iRange);
};
void DrawCircleScale(TDC&  dc,
					TPoint& Center,					// ????? ?????
					int   iLengthNeedle,       // ????? ???????
					char* sDim,						// ???????????
					int   iMinValue,				// ???????
					int	iRange){			    	// ????????
	double dCorner;                        // ??????? ?????
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
														// ??????????? ???????? ????? ? ??????
		itoa(iMinValue + i * iRange / QUANTMARKCIRCLE, StScale, 10);
														// ??????? ????? ? ?????? ?????? ? ????????
		dc.GetTextExtent(StScale, strlen(StScale), size);
		switch (i){									// ???????? ??????? ??????
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
														// ????????? ???????? ?????
		dc.TextOut(PosText, StScale);
	};                                     // ?????? ????? ???????????
	dc.GetTextExtent(sDim, strlen(sDim), size);
	dc.TextOut(Center.x - size.cx / 2,
				  Center.y - (int)((double)iLengthNeedle / 2.0), sDim);
};

/*-----------------------------------------------------------------*\
 * 	??????????? ????????????? ?????										 *
\*-----------------------------------------------------------------*/
void DrawRectScale(TDC&  dc,					// ?????????? ????????
					TypePosScale PosScale,		// ??????? ?????
					int    ix1, int	iy1,		// X1, Y1
					int	 ix2, int	iy2,     // X2, Y2
					char*  sDimX,  				// ???????????
					double dMinX,  				// ??????????? ????????
					double dRangeX,	  			// ????????
					char*  sDimY,  				// ???????????
					double dMinY,  				// ??????????? ????????
					double dRangeY){    			// ????????
	TRect RectScale;
	RectScale.left  = ix1; RectScale.top    = iy1;
	RectScale.right = ix2; RectScale.bottom = iy2;
	DrawRectScale(dc, PosScale, RectScale,
							sDimX, dMinX, dRangeX,
							sDimY, dMinY, dRangeY);
};
void DrawRectScale(TDC&  dc,
					TypePosScale PosScale,		// ??????? ?????
					TRect& RectScale,				// ?????? ?????
					char*  sDimX, 					// ???????????
					double dMinX,  				// ??????????? ????????
					double dRangeX,	  			// ????????
					char*  sDimY,  				// ???????????
					double dMinY,  				// ??????????? ????????
					double dRangeY){    			// ????????
	char  StScale[17] = "";
	TSize size;
	double dDeltaMarkY, dDeltaMarkX, dCountMarkX;
														// ?????????? ????? ????????? ????? Y
	dDeltaMarkY = (double)((RectScale.bottom - DELTASCALE) -
								  (RectScale.top    + DELTASCALE)) /
								  QUANTMARKRECT;
														// ?????????? ????? ????????? ????? X
	dDeltaMarkX	= (double)((RectScale.right  - DELTASCALE) -
								  (RectScale.left   + DELTASCALE)) /
								  COUNTMARKX;
														// ???-?? ???????? ?? 'X'
	dCountMarkX = (double)((RectScale.right  - DELTASCALE) -
								  (RectScale.left   + DELTASCALE)) /
								  dDeltaMarkX;
//----------------- ? ? ? ? ?   N 1  ?? Y -------------------------
	if (PosScale == LeftScale){
		 dc.MoveTo(RectScale.left   + DELTASCALE,
					  RectScale.top    + DELTASCALE);
		 dc.LineTo(RectScale.left   + DELTASCALE,
					  RectScale.bottom - DELTASCALE);
	};

//----------------- ? ? ? ? ?  ?? X -------------------------------
	dc.MoveTo(RectScale.left   + DELTASCALE,
				 RectScale.bottom - DELTASCALE);
	dc.LineTo(RectScale.right  - DELTASCALE,
				 RectScale.bottom - DELTASCALE);

//----------------- ? ? ? ? ?   N 2  ?? Y -------------------------
	if (PosScale == RightScale){
		 dc.LineTo(RectScale.right  - DELTASCALE,
					  RectScale.top	 + DELTASCALE);
	};

	for (char yi = 0; yi <= QUANTMARKRECT; yi++){
//------- ? ? ? ? ? ? ?  ??  Y   N 1 ------------------------------
		 if (PosScale == LeftScale){
			 dc.MoveTo(RectScale.left   + DELTASCALE - LENGTHMARK,
						  RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY);
			 dc.LineTo(RectScale.left   + DELTASCALE,
						  RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY);
//------- ? ? ? ? ? ? ? ?  ??  Y   N 1 ----------------------------
			 sprintf(StScale, "%.f", dMinY + (double)yi * dRangeY / QUANTMARKRECT);
			 dc.GetTextExtent(StScale, strlen(StScale), size);
			 dc.TextOut(RectScale.left   + DELTASCALE - 1 - size.cx,
							RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy,
							StScale);
			 if (yi == QUANTMARKRECT){    // ??????? ?????????
				 dc.GetTextExtent(sDimY, strlen(sDimY), size);
				 dc.TextOut(RectScale.left,
								RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy * 2,
								sDimY);
			 };
		 };

//------- ? ? ? ? ? ? ?  ??  Y   N 2 ------------------------------
		 if (PosScale == RightScale){
			 dc.MoveTo(RectScale.right   - DELTASCALE,
						  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
			 dc.LineTo(RectScale.right   - DELTASCALE + LENGTHMARK,
						  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
//------- ? ? ? ? ? ? ? ?  ??  Y   N 2 ----------------------------
			 sprintf(StScale, "%.f", dMinY + (double)yi * dRangeY / QUANTMARKRECT);
			 dc.GetTextExtent(StScale, strlen(StScale), size);
			 dc.TextOut(RectScale.right  - DELTASCALE + 2,
							RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy,
							StScale);
			 if (yi == QUANTMARKRECT){		// ??????? ?????????
				 dc.GetTextExtent(sDimY, strlen(sDimY), size);
				 dc.TextOut(RectScale.right  - size.cx,
								RectScale.bottom - DELTASCALE - yi * (int)dDeltaMarkY - size.cy * 2,
								sDimY);
			 };
		 };
	 };
//------- ? ? ? ? ? ? ?   ?   ? ? ? ? ? ? ? ?  ??  X --------------
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
		 if (xi == QUANTMARKRECT){    // ??????? ?????????
				  dc.GetTextExtent(sDimX, strlen(sDimX), size);
				  dc.TextOut(RectScale.left + DELTASCALE - size.cx / 2,
				  RectScale.bottom - DELTASCALE + LENGTHMARK,
				  sDimX);
		 };
	};
};

/*-----------------------------------------------------------------*\
 * 	??????????? ?????															 *
\*-----------------------------------------------------------------*/
void DrawGrid(TDC&  dc,				  	// ?????????? ????????
				  int ix1, int iy1,	  	// X1, Y1
				  int	ix2, int	iy2){    // X2, Y2
	TRect RectScale;
	RectScale.left  = ix1; RectScale.top    = iy1;
	RectScale.right = ix2; RectScale.bottom = iy2;
	DrawGrid(dc, RectScale);
};
void DrawGrid(TDC&   dc,
				  TRect& RectScale){				// ?????? ?????
	 double dDeltaMarkY, dDeltaMarkX;
	 double dCountMarkX;							// ???-?? ???????? ?? 'X'
														// ?????????? ????? ????????? ????? Y
	 dDeltaMarkY  = (double)((RectScale.bottom - DELTASCALE) -
									 (RectScale.top    + DELTASCALE)) /
									 QUANTMARKRECT;
														// ?????????? ????? ????????? ????? X
	 dDeltaMarkX  = (double)((RectScale.right  - DELTASCALE) -
									 (RectScale.left   + DELTASCALE)) /
									 COUNTMARKX;
	 dCountMarkX  = (double)((RectScale.right  - DELTASCALE) -
									 (RectScale.left   + DELTASCALE)) /
									 dDeltaMarkX;

//----------------- ? ? ? ? ?   ?? Y ------------------------------
	 for (char yi = 1; yi <= QUANTMARKRECT; yi++){
		 dc.MoveTo(RectScale.left    + DELTASCALE,
					  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
		 dc.LineTo(RectScale.right   - DELTASCALE,
					  RectScale.bottom  - DELTASCALE - yi * (int)dDeltaMarkY);
	};

//----------------- ? ? ? ? ?  ?? X -------------------------------
	 for (char xi = 1; xi <= (char)dCountMarkX; xi++){
		 dc.MoveTo(RectScale.left   + DELTASCALE + xi * (int)dDeltaMarkX,
					  RectScale.bottom - DELTASCALE);
		 dc.LineTo(RectScale.left   + DELTASCALE + xi * (int)dDeltaMarkX,
					  RectScale.top    + DELTASCALE);
	};
};
/*-----------------------------------------------------------------*\
 * 	??????? ??????????? ???????? (BitMap'?)							 *
\*-----------------------------------------------------------------*/
void
DrawBitMap(TDC &dc, TBitmap& bitmap,// ????????? ?? ??????-????????
			  int x, int y){				// ??????? ????????
  TMemoryDC memDC(dc);
  memDC.SelectObject(bitmap);

  BITMAP bm;
  bitmap.GetObject(bm);
  dc.BitBlt(x, y,                   // ?????????? ????????
				bm.bmWidth,	bm.bmHeight,
				memDC, 0, 0, SRCCOPY);
};
/*-----------------------------------------------------------------*\
 * 	??????? ???? ?? ??????, ??????? ? ???? ??????					 *
\*-----------------------------------------------------------------*/
void Arc4(TDC& dc, 		// ?????????? ????????
	  TPoint& Center, 	// ????? ????
	  int Radius, 			// ?????? ????
	  TPoint& Tip1, 		// ????? 1
	  TPoint& Tip2){		// ????? 1
	dc.Arc(Center.x - Radius, Center.y - Radius,	// ????? ??????? ????? ??????????
																// ??????????????,
			 Center.x + Radius, Center.y + Radius, // ?????? ?????? ????? ??????????
																// ??????????????
			 Tip2.x, Tip2.y, Tip1.x, Tip1.y);		// ????? ????
};
/*-----------------------------------------------------------------*\
 * 	??????? ???????? ?????????????? ????? ?? ????????? ???? 		 *
 *		???????????? ????															 *
\*-----------------------------------------------------------------*/
TPoint
AffinedRotate(double AngleRad, 	// ???? ????????
				  int x, int y){		// ????? ?????????????? ????????
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
 *		????????? ?????? ?? ???????? ???????									*
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
 *		????????? ????? ?? ???????? ???????? ??????? 						*
 *		?? ????????? ???????                                           *
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
