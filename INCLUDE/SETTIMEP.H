#if !defined(__SETTIMEP_H)
#define __SETTIMEP_H
/*
 * settimep.h - Handle file for Set Period Time Module ---------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\validate.h>
#include <owl\radiobut.h>
#include <owl\dialog.h>
#include <owl\edit.h>

#include "binfobox.h"
#include "drawicbt.h"
#include "testsyst.rh"
/*
 *		 Определение констант едениц времени
 */
const char SCALE_MSEC = 1;
const char SCALE_SEC  = 2;
const char SCALE_MIN  = 3;
const char SCALE_HOUR = 4;
//----------------------------------------------------------------------------
/*
 *  -------------------------------------------------------------------------
 * 	Class TSetTimePeriodDialog
 *  -------------------------------------------------------------------------
 */

 class  TSetTimePeriodDialog : public TDialog {
  public:
	 TValidator*	valid1;					// Контроль ввода только цифр
	 TValidator*	valid2;
	 char far 		Buffer[4];				// Буфер ввода временного интервала
	 char far 		BufferCorrect[6];	 	// Буфер ввода поправки
	 int  far*		lpRBScaleTime;       // Указатель на масштаб времени
	 int  far*		lpInptQuantTime;     // Указатель на интервал времени
	 int     *		lpInptCorrectTimer;  // Указатель на поправку

	 int  far		aiValueScaleTime[4]; // Массив значений для разных
													// едениц времени
	 int  far      MiscScaleTime;	 		// Переменная для промежуточного сохранения
													// едениц времени

	 TDrawICBT*    Button1;					// Cancel
	 TDrawICBT*    Button2;					// Ok

	 TEdit*			TimeInput;
	 TEdit*			CorrectInput;

	 TRadioButton* RButton1;				// Миллисекунды
	 TRadioButton* RButton2;      	   // Секунды
	 TRadioButton* RButton3;         	// Минуты
	 TRadioButton* RButton4;         	// Часы

	 TBInfoBox*    pTMessage;           // Статусная строка
													// К о н с т р у к т о р
	 TSetTimePeriodDialog(TWindow* parent,
								 int far* lpScaleTime,
								 int far* lpQuantTime,
								 int    * lpCorrectTimer);

//	~TSetTimePeriodDialog();            // Д е с т р у к т о р

	void RangeValidator	  ();
	void SetTimePeriodMSEC ();
	void SetTimePeriodSEC  ();
	void SetTimePeriodMIN  ();
	void SetTimePeriodHOUR ();
	void ShowSetTimeMessage();
	void CancelEndSession  ();
	void OkEndSession		  ();

	//
	// Override TWindow virtual member functions
	//
	void TransferData(TTransferDirection);

  protected:
	//
	// Override TWindow virtual member functions
	//
	void SetupWindow();

  private:
	 //
	 // hidden to prevent accidental copying or assignment
	 //
	 TSetTimePeriodDialog(const TSetTimePeriodDialog&);
	 TSetTimePeriodDialog& operator=(const TSetTimePeriodDialog&);

  DECLARE_RESPONSE_TABLE(TSetTimePeriodDialog);
};
#endif  // __SETTIMEP_H
/*
 * ---------------------------------------------------------
 *	EoF(settimep.h)
 * ----------------------------------------------------------
 */

