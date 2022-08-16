#if !defined(__TCHANNIO_H)
#define __TCHANNIO_H
/*
 * tchannio.h - Handle file ------------------------------------
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

#include "drawicbt.h"
#include "binfobox.h"

/*
 *  -------------------------------------------------------------------------
 * 	Class TChannelIODialog
 *  -------------------------------------------------------------------------
 */
class _OWLCLASS TValidator;

class TChannelIODialog : public TDialog {
  public:
	TValidator*	valid;		  	  	   // Контроль ввода только цифр

	TDrawICBT* 	  Help_ICBT;			// Помощь
	TDrawICBT* 	  Cancel_ICBT;			// Выход
	TComboBox* 	  NameChanIO_CB; 		// Имя канала измерений
	TEdit* 	  	  CodeChanIO_ED;		// Марка канала измерений
	TRadioButton* ChanIO_A_RB;			// Аналоговый тип канала измерений
	TRadioButton* ChanIO_D_RB;       // Дискретный тип канала измерений
	TRadioButton* ChanIO_C_RB;       // Тип канала измерений - счетчик
	TRadioButton* ChanIO_F_RB;       // Тип канала измерений - быстрый  счетчик
	TRadioButton* ChanIO_N_RB;			// Нестандартный тип канала измерений
	TComboBox* 	  TypeChanIO_CB; 		// Тип канала измерений

	TEdit* 	  	  NumLogChan_ED;		// Номер логического канала передачи данных
	TEdit* 	  	  IndexArray_ED;		// Индекс в массиве общей области данных

	TComboBox* 	  MicroPC_CB;			// Список контроллеров
	TComboBox* 	  ModuleIC_CB;			// Список модулей
	TComboBox* 	  TransducIC_CB;		// Список преобразователей
	TComboBox* 	  SensorIC_CB; 		// Список датчиков

	TButton* 	  Save_BUT;				// Запись
	TButton* 	  Delete_BUT;			// Удаление
	TBInfoBox* 	  Message_IB;			// Статусная строка

	TChannelIODialog(TWindow* parent);// Class constructor

	void SetupWindow		  		();
	void ListChannelIOName 		();
	void ListMicroPC				();
	void CreateOpenModuleFile	();
	void ChannelIOType	  		();
	void SaveRecord   	  		();
	void DeleteRecord 	  		();
	void ChangeSelect		  		();
	void ListComponentChannel	();
	void TChannelIOHelp    		(){ MessageBeep(0); };
	void EndSession		  		();
	void FromDynRecToDia			();
  DECLARE_RESPONSE_TABLE  (TChannelIODialog);
};
#endif  // __TCHANNIO_H
/*
 * ---------------------------------------------------------
 *	EoF(tchannio.h)
 * ----------------------------------------------------------
 */
