#if !defined(__TMODULPC_H)
#define __TMODULPC_H
/*
 * tmodulpc.h - Handle file  ---------------------
 *
 * Author:
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files

#include <owl\radiobut.h>
#include <owl\combobox.h>
#include <owl\edit.h>

#include "binfobox.h"
#include "drawicbt.h"
#include "database.h"
#include "mcontrls.h"
#include "testsyst.rh"
#include "commfunc.h"
#include "userdraw.h"
/*
 *  -------------------------------------------------------------------------
 * 	Class TModulPCDialog
 *  -------------------------------------------------------------------------
 */
class _OWLCLASS TValidator;

class TModulPCDialog : public TDialog {
  public:
	TValidator*		valid; 	  	 	// Контроль ввода только цифр
	TValidator*		validdec;  	 	// Контроль ввода только цифр и точки

	TComboBox*    	TypePC_CB;	 	// Тип программируемого контроллера
	TRadioButton* 	Analog_RB;	 	// Кнопка "Аналоговый тип модуля"
	TRadioButton* 	Discret_RB;  	// Кнопка "Дискретный тип модуля"
	TRadioButton* 	Counter_RB;   	// Кнопка "Счетчик модуль"
	TRadioButton* 	FCounter_RB;  	// Кнопка "Быстрый счетчик модуль"
	TRadioButton* 	User_RB;	    	// Кнопка "Нестандартный тип модуля"
	TComboBox*    	TModuleList_CB;// Cписок существ. модулей
	TEdit*		  	TypeModule_ED;	// Аббревиатура имени модуля
	TRadioButton* 	Current_RB;   	// Кнопка "постоянный ток"
	TRadioButton* 	Voltage_RB;   	// Кнопка "напряжение DC"
	TComboBox*    	Boundares_CB; 	// Список существ. границ уравней
	TComboBox*    	Dimension_CB; 	// Список существ. размерностей
	TDrawICBT*    	Help_ICBT;	 	// Помощь
	TDrawICBT*    	Cancel_ICBT;  	// Выход
	TButton*      	Save_BUT;  		// Запись
	TButton*      	Delete_BUT;	 	// Удаление
	TBInfoBox*    	Message_IB;   	// Статусная строка
	TBitmap*	   	Picture;

	TModulPCDialog(TWindow* parent);		// Class constructor

	void 		EvPaint		  ();
	void 		Paint			  (TDC&, BOOL, TRect&);
	void 		SetupWindow	  ();
	void 		TMicroPCList  ();
	void 		TModuleList	  ();
	void 		TypeModule	  ();
	void 		CurrentVoltage();
	void 		ChangeText	  ();
	void 		SaveRecord    ();
	void 		DeleteRecord  ();
	void 		ChangeMIOName ();
	void 		Help	   	  (){ MessageBeep(0); };
	void 		EndSession	  ();

  DECLARE_RESPONSE_TABLE   (TModulPCDialog);
};

#endif		// __TMODULPC_H
/*
 * ---------------------------------------------------------
 *	EoF(tmodulpc.h)
 * ----------------------------------------------------------
 */
