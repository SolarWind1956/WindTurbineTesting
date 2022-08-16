#if !defined(__CYCLARRY_H)
#define __CYCLARRY_H
/*
 * cyclarry.h - Handle file for class 'TCycleArray' ---------
 *
 *		Реализация ЦИКЛИЧЕСКОГО массива
 *		( правда без синхронизации доступа к массиву двух или
 *		более процессов - это попозже )
 *
 * Author: Sergey E. Heckel
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 *				Класс 'TCycleArray' является производным от следующих
 *				шаблонных классов:
 *
 *	template <class T> class  	TCycleArray : public
 *									  		TArray<T>
 *	template <class T> class  		TArray : public
 * 	 							  			TArrayAsVector<T>
 *	template <class T> class  			TArrayAsVector : public
 *									    				TMArrayAsVector<T,TStandardAllocator>
 *	template <class T, class Alloc> class 	TMArrayAsVector : public
 *											  				TDArrayAsVectorImp<TMCVectorImp<T,Alloc>,T>
 * template <class Vect, class T> class 		TDArrayAsVectorImp : public
 *											  						TArrayAsVectorImp<Vect,T>
 *	template <class Vect, class T> class 				TArrayAsVectorImp
 *
 *				Класс 'TCycleArrayIterator' является производным от следующих
 *				шаблонных классов:
 *				( не очень уверен, что этот класс пригодится ?..)
 *
 *	template <class T> class TCycleArrayIterator - не является производным
 *
 */

//						H - files

#if !defined( __CLASSLIB_ARRAYS_H )
#include "classlib\arrays.h"
#endif  // __CLASSLIB__H

/*
 *		--------------------------------------------------------------
 *
 *  	template <class T> class TCycleArray
 *
 *			Шаблонный класс циклического массива.
 *		Тип элемента массива Т должен иметь конструктор по умолчанию,
 *		и определение операции ==.
 *
 *		--------------------------------------------------------------
 */
template <class T> class TCycleArrayIterator;

template <class T>
class TCycleArray :	public  TArray<T>
{
	friend class TCycleArrayIterator<T>;

public:

	TCycleArray (int upper) : TArray<T>( upper, 0, 0)
	{
				CountItems  = 0;		// Текущее Количество элементов
											// в циклическом массиве
				HeadOfArray = 0;		// Начало (ГОЛОВА) циклического массива
				TailOfArray = 0;     // Конец  (ХВОСТ)  циклического массива
	};
	void     Add      ( const T& Item ){
				// В пустой буфер элемент заносим без изменения
			// параметров циклического массива
			if (isEmpty()){
				Data[0] = Item;
				CountItems		=	1;	// В массиве ОДИН элемент
				return;
			};
			// Если массив заполнен, начинаем переписывать
			// его с начала
			if (++TailOfArray >= UpperBound())
				 TailOfArray = 0;
			// Корректируем указатель на ГОЛОВУ цикл.массива
			if (TailOfArray   == HeadOfArray)
				 if (++HeadOfArray >= UpperBound())
					  HeadOfArray = 0;
			// Количество элементов не может превышать
			// размера циклического массива
			if (CountItems < UpperBound()) CountItems++;
				// Наконец помещаем элемент в непустой массив
				Data[TailOfArray] = Item;
			return;

	};
	BOOL     isEmpty  () const { return CountItems ? FALSE : TRUE; };
	unsigned Count		() const { return CountItems; };

protected:

	unsigned CountItems;					// Количество элементов в циклическом
												// массиве
	unsigned HeadOfArray;				// Начало (ГОЛОВА) циклического массива
	unsigned	TailOfArray;            // Конец  (ХВОСТ)  циклического массива
};

/*
 *		--------------------------------------------------------------
 *
 *  	template <class T> class TCycleArrayIterator
 *
 *			Образует объект - итератор для обхода объектов ЦИКЛИЧЕСКОГО
 *			массива TCycleArray с учетом разрыва при циклическом добавлении
 *			элементов - см. функцию TCycleArray<T>::Add(T&)
 *
 *		--------------------------------------------------------------
 */
template <class T> class TCycleArrayIterator
{
public:


	TCycleArrayIterator( const TCycleArray<T>& a ) {
				pCycleArray = &a;			//	Указатель на обслуживаемый циклический
												// массив
	};

	operator int() const{
												// Преобразует итератор в целое
												// для определения того, остались
												// ли объекты в итераторе !!!
												// в ИТЕРАТОРЕ а не только в МАССИВЕ
			return !(EndOfIter);
	};
	const T& First() const {
			return (*pCycleArray)[Lower];
	};
	const T& Last() const {
			return (*pCycleArray)[Upper];
	};
												// Получение текущего элемента
	const	T& Current() const{
			return (*pCycleArray)[Cur];
	};
												//	Постфиксная операция
	const T& operator ++ ( int ){
			const T& temp = Current();	// Запомним значение до увеличения указателя
			SetNextCur();					// Установим указатель на следующий элемент
			return temp;
	};
												// Префиксная операция
	const	T& operator ++ ()	{
			SetNextCur();					// Установим указатель на следующий элемент
			return Current();
	};
												// Формирование указателя на последующий
												// элемент итератора
	virtual	void  SetNextCur(){
												// Равенство указателей на голову и хвост
												// при непустом массиве означает
												// наличие в нем только одного элемента
			if (Lower <= Upper){ 		// ГОЛОВА впереди ХВОСТА
				 if (++Cur > Upper){		// Если итератор дошел до ХВОСТА циклического
					  EndOfIter = TRUE;	// массива, взводим флаг конца итерации
												// Итератор не должен пойти
					  Cur = Upper;			// за ХВОСТ циклического массива
				 }
			}
			else{                      // ХВОСТ впереди ГОЛОВЫ
				 if (Cur != Upper){     // Циклический переход в
					  if (++Cur == pCycleArray->UpperBound())
							Cur = 0;     	// начало физического массива
				 }
				 else                 	// Если указатель перед инкрементом
												// уже указывал на хвост, то также
					  EndOfIter = TRUE;  // взводим флаг конца итерации
			}

	};
												// Полный просмотр циклического массива
	virtual	void 	Restart(){
			Lower = pCycleArray->HeadOfArray;
			Upper = pCycleArray->TailOfArray;
			Restart(Lower,Upper);
	};
												// Просмотр указанного фрагмента
												// (надо бы усилить контролем параметров)
	virtual	void 	Restart( unsigned start, unsigned stop ){
			Cur = Lower = start;
			Upper = stop;
			EndOfIter = pCycleArray->isEmpty(); // Пустой массив нечего смотреть
	};

private:

	const    TCycleArray<T> *pCycleArray;	// Указатель на циклический массив
	BOOL		EndOfIter;							// Признак конца итерации
	unsigned Cur;									// Указатель на текущий элемент
	unsigned Lower, Upper;						// Граници просмотра итератора

};

#endif  // __CYCLARRY_H
/*
 * ---------------------------------------------------------
 *	EoF(cyclarry.h)
 * ----------------------------------------------------------
 */

