#if !defined(__CLBINATT_H)
#define __CLBINATT_H
/*
 * Cl[ass] BIN Att[ributs] - Класс аттрибутов БИНА
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * ------------------------------------------------------------
 */
class TBinAttributs {

	public:

		double		QuantPoints;		// Количество точек в БИНЕ
		double		StepNumber;			//	Номер шага усреднения

		double		MinWindSpeed;		// Скорости ветра
		double		AvrWindSpeed;
		double		MaxWindSpeed;

		double		WindDirection;		//	Направление ветра

		double 		AirTemperature;	// Температура наружного воздуха
		double		AtmosPressure;		// Атмосферное давление
		double		Precipitation;		// Осадки

		double		MinNetPower;		// Полезные мощности
		double		AvrNetPower;
		double		MaxNetPower;
		double		NormNetPower;

		double 		RotorSpeed;				// Скорость вращения ветроколеса

		TBinAttributs 	();					// Конструктор класса атрибутов БИНА
		TBinAttributs 	(const TBinAttributs& BinAttr); // Конструктор копии
													// Операция присваивания
		TBinAttributs& operator =(const TBinAttributs& BinAttr);

													// Операторы сравнения
		BOOL        	operator ==(const TBinAttributs& other) const;
		BOOL        	operator !=(const TBinAttributs& other) const;

		void   InitBINAttributs	();		// Инициализация атрибутов БИНА
													// Обработка БИНА по скорости ветра
		void   WindSpeedProcess	(double WindSpeed);
													// Обработка БИНА по активной мощности
		void   NetPowerProcess	(double NetPower);
		void   NormNetPowerProcess(double WindSpeed);
};

#endif	// __ClBINAtt_H
