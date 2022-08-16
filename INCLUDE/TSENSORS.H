#if !defined(__TSENSORS_H)
#define __TSENSORS_H
/*
 * tsensors.h - Handle file AnalogRange Module ---------------------
 *
 * Author:
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 */
//						H - files

#include <owl\combobox.h>
#include <owl\edit.h>

#include "gradchar.h"
#include "binfobox.h"
#include "drawicbt.h"

/*
 *	Functions prototype
 */
BOOL UpdateTSensors();
BOOL AppendToTSensors();
/*
 *  -------------------------------------------------------------------------
 * 	Class TSensorsDialog
 *  -------------------------------------------------------------------------
 */
class _OWLCLASS TValidator;

class TSensorsDialog : public TDialog {
  public:
	FIELDNUMBER 	fnbrBX, fnbrEX, fnbrBY, fnbrEY;
	FIELDNUMBER 	fnbr, fnbrX, fnbrY, fnbrB, fnbrE;
	RECORDNUMBER 	RecCount;		 // Количество записей в файле
	TValidator*		valid;		  	 // Контроль ввода только цифр
	TValidator*		validdec;  	 	 // Контроль ввода только цифр и точки

	TDrawICBT* 	  	Help_ICBT;		 // Помощь
	TDrawICBT* 	   Cancel_ICBT;	 // Выход
	TComboBox* 	   NameSensor_CB;	 // Имя датчика
	TEdit* 	  	   CodeSensor_ED;	 // Марка датчика
	TComboBox* 	   TypeSensor_CB;	 // Тип датчика

	TEdit* 	  	   GradPointX_ED[6];  // Входные данные
	TEdit* 	  	   GradPointY_ED[6];  // Выходные данные
	TEdit* 	  	   ApproxA_ED [6]; // Аппроксимационные коэффициенты

	TComboBox* 	   TypeApprox_CB;	 // Тип аппроксимации 
	TGraduirCharact* Graduir_GB;	 // График
	TComboBox* 	   DimensOut_CB;	 // Размерности выходного сигнала
	TButton* 	   Save_BUT;		 // Запись
	TButton* 	   Delete_BUT;		 // Удаление
	TBInfoBox* 	   Message_IB;		 // Статусная строка

	TSensorsDialog(TWindow* parent);// Class constructor

	void SetupWindow		  ();
	void TSensorsNameList  ();
	void FromDlgToDynRec	  ();		// Информация из диалога в динамическую запись
	void FromDynRecToDia   ();		// Информация из динамической записи в диалог
	void SaveRecord   	  ();
	void DeleteRecord 	  ();
	void ChangeSelectText  ();
	void ChangeSelectValues();
	void ChangeDimension	  ();
	void ChangeTypeApprox  ();	
	void DrawGraduirCharact();
	void DrawValueGraphic  (FIELDNUMBER, FIELDNUMBER, FIELDNUMBER, FIELDNUMBER );
	void Help	  			  (){ MessageBeep(0); };
	void EvClose			  ();
	void EndSession		  ();
  DECLARE_RESPONSE_TABLE  (TSensorsDialog);
};
#endif  // __TSENSORS_H
/*
 * ---------------------------------------------------------
 *	EoF(tsensors.h)
 * ----------------------------------------------------------
 */
