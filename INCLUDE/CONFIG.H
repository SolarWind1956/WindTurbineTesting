#if !defined(__CONFIG_H)
#define __CONFIG_H
/*
 * config.h - Handle file CopyDisk Module ---------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include <owl\radiobut.h>

#include "testsyst.rh"
/*
 *		Function's prototips
 */
//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TConfigurationDialog
 *  -------------------------------------------------------------------------
 *//*
class TConfigurationDialog : public TDialog {
  public:
	TDrawICBT*    	  Button1;
	TDrawICBT*    	  Button2;
	TDrawICBT*    	  Button3;
	TRadioButton*    RadioButton1;
	TRadioButton*    RadioButton2;
	TRadioButton*    RadioButton3;
	TRadioButton*    RadioButton4;
	TRadioButton*    RadioButton5;
	TRadioButton*    RadioButton6;

	TConfigurationDialog(TWindow* parent);		// Class constructor

	void SizeCopyRest144();
	void SizeCopyRest121();
	void SizeCopyRest720();
	void SizeCopyRest360();
	void DiskCopyRestA();
	void DiskCopyRestB();
	void SaveConfig();
	void ConfigHelp()     {MessageBeep(0);}
	void EndSession();
  DECLARE_RESPONSE_TABLE(TConfigurationDialog);
};   */
#endif  // __CONFIG_H
/*
 * ---------------------------------------------------------
 *	EoF(config.h)
 * ----------------------------------------------------------
 */
