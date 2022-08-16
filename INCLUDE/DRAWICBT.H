#if !defined(__DRAWICBT_H)
#define __DRAWICBT_H
/*
 * DrawICBT ----------------------------------------------------*
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
#include <owl\button.h>
#include <owl\dialog.h>
#include <owl\module.h>
#include <string.h>

#define BD_BORDERWIDTH	1 	// Width of the line bordering the button bitmap
#define BD_ICONHEIGHT   32	// Icon height
/* class TDrawICBT : public TButton ---------------------------- *
 *								 *
 *								 *
 * ------------------------------------------------------------- */
// _CLASSDEF(TDrawICBT)
class TDrawICBT : public TButton
{
public:
  LOGFONT   ButnFontRec;
  typedef   LOGFONT FAR * LPLOGFONT;
  LPLOGFONT hButnFontRec;
  COLORREF  ButnFGColor;

  HICON hIcon1, hIcon2;		// Button's icons handlers
  DWORD bWidth1, bHeight1, bWidth2, bHeight2;

  TDrawICBT(TWindow* AParent, int ResourceId) :
											TButton(AParent, ResourceId)
  {
					// Set button text foregrownd color
	ButnFGColor = RGB(0, 0, 0);
										  // Set button font parameters
	ButnFontRec.lfHeight = 14;
	ButnFontRec.lfWidth = 8;
	ButnFontRec.lfEscapement = 0;
	ButnFontRec.lfOrientation = 0;
	ButnFontRec.lfWeight = FW_NORMAL;
	ButnFontRec.lfItalic = 0;
	ButnFontRec.lfUnderline = 0;
	ButnFontRec.lfStrikeOut = 0;
	ButnFontRec.lfCharSet = ANSI_CHARSET;
	ButnFontRec.lfOutPrecision = OUT_DEFAULT_PRECIS;
	ButnFontRec.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	ButnFontRec.lfQuality = PROOF_QUALITY;
	ButnFontRec.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(ButnFontRec.lfFaceName, "Helv");

	SetBkgndColor(RGB(170, 170, 170));	// LTGRAY_BRUSH
  }
  virtual void ODADrawEntire(DRAWITEMSTRUCT far & DrawInfo);
  virtual void ODAFocus(DRAWITEMSTRUCT far & DrawInfo);
  virtual void ODASelect(DRAWITEMSTRUCT far & DrawInfo);
  void DrawBorder(DRAWITEMSTRUCT far & DrawInfo);
  void FocusBorder(DRAWITEMSTRUCT far & DrawInfo);
  void SelectBorder(DRAWITEMSTRUCT far & DrawInfo);
  void PaintButton(DRAWITEMSTRUCT far & DrawInfo);
};
#endif  // __DRAWICBT_H
/* ------------------------------------------------------------- *
 *	EoF(DrawICBT.h)                                          *
 * ------------------------------------------------------------- */