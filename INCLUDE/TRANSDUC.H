#if !defined(__TRANSDUC_H)
#define __TRANSDUC_H
/*
 * transduc.h - Handle file AnalogRange Module ---------------------
 *
 * Author:
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files
#include <owl\combobox.h>
#include <owl\edit.h>

#include "binfobox.h"
#include "drawicbt.h"
#include "gradchar.h"
/*
 *	Functions prototype
 */
BOOL UpdateTTransduc();
BOOL AppendTTransduc();
/*
 *  -------------------------------------------------------------------------
 * 	Class TTransducDialog
 *  -------------------------------------------------------------------------
 */
class _OWLCLASS TValidator;

class TTransducDialog : public TDialog {
  public:
	FIELDNUMBER fnbrBX, fnbrEX, fnbrBY, fnbrEY;
	FIELDNUMBER fnbr, fnbrX, fnbrY, fnbrB, fnbrE;
	RECORDNUMBER RecCount;				// Количество записей в файле
	TValidator*		valid;		  	 	// Контроль ввода только цифр
	TValidator*		validdec;  	 	 	// Контроль ввода только цифр и точки

	TDrawICBT* 	  	Help_ICBT;			// Помощь
	TDrawICBT* 	  	Cancel_ICBT;		// Выход

	TComboBox* 	  	NameTRDC_CB;		// Список Имен преобразователей
	TEdit* 	  	  	CodeTRDC_ED;		// Марка преобразователя

	TEdit* 	  	  	GradPointX_ED[6];	// Входные данные - координаты Х
	TEdit* 	  	  	GradPointY_ED[6]; // Выходные данные - координаты Y
	TEdit* 	  	  	ApproxA_ED   [6]; // Аппроксимационные коэффициенты

	TComboBox* 	  	TypeApprox_CB;		// Тип аппроксимации
	TGraduirCharact* Graduir_GB;		// График
	TComboBox* 	  	DimensOut_CB;		// Размерности выходного сигнала
	TButton* 	  	Save_BUT;			// Запись
	TButton* 	  	Delete_BUT;			// Удаление
	TBInfoBox* 	  	Message_IB;			// Статусная строка

	TTransducDialog(TWindow* parent);// Class constructor

	void SetupWindow		  ();
	void TRDCNameList		  ();
	void SaveRecord   	  ();
	void DeleteRecord 	  ();		// Удаление объекта
	void ChangeSelectText  ();
	void ChangeSelectValues();
	void ChangeDimension	  ();
	void ChangeTypeApprox  ();
	void DrawGraduirCharact();
	void DrawValueGraphic  (FIELDNUMBER, FIELDNUMBER, FIELDNUMBER, FIELDNUMBER);
	void FromDlgToDynRec	  ();		// Информация из диалога в динамическую запись
	void FromDynRecToDia   ();		// Информация из динамической записи в диалог

	void Help	  			  (){ MessageBeep(0); };
	void EvClose			  ();
	void EndSession		  ();

  DECLARE_RESPONSE_TABLE  (TTransducDialog);
};
#endif  // __TRANSDUC_H
/*
 * ---------------------------------------------------------
 *	EoF(transduc.h)
 * ----------------------------------------------------------
 */
