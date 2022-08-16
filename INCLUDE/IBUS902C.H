#if !defined (__IBUS902C_H)
#define __IBUS902C_H
//
extern "C" int INIT_NETWORK  (unsigned char STATION_ADRESS,
			      unsigned char SPEED,
			      unsigned char *STATION_LIST,
			      unsigned char INTAD);
const int OK_INIT       = 0;    // Инициализация сети завершена
const int ERR_ST_ADR    = 29;   // Неразрешенный адрес станции
const int ERR_SPEED     = 30;   // Неразрешенный код скорости
const int ERR_ST_LIST   = 31;   // Неразрешенный список активных станций
const int ERR_INIT      = 32;   // Инициализация сети не удалась
const int ERR_NO_IBUS   = 253;  // Не установлен IBUS модуль сопряжения
const int ERR_ADR_IBUS  = 254;  // Неразрешенный адрес IBUS модуля

extern "C" int WRITE_STATION_LIST  (unsigned char *STATION_LIST,
				    unsigned char INTAD);
const int REQUEST_CFG   = 0;    // Запрос на конфигурацию сети отослан сетевому процессору
const int ERR_ST_SLAVE  = 34;   // Станция не является ведущей
/*
const int ERR_ST_LIST   = 31;   // Неразрешенный список активных станций
const int ERR_NO_IBUS   = 253;  // Не установлен IBUS модуль сопряжения
const int ERR_ADR_IBUS  = 254;  // Неразрешенный адрес IBUS модуля
 */

extern "C" int RESET_44 (unsigned char INTAD);
const int OK_RESET      = 0;    // Отключение от сети завершено
/*
const int ERR_NO_IBUS   = 253;  // Не установлен IBUS модуль сопряжения
const int ERR_ADR_IBUS  = 254;  // Неразрешенный адрес IBUS модуля
 */

extern "C" int CONFIGURE_SAP (unsigned char SAP_NO,
			      unsigned char COMM_TYPE,
			      unsigned char RDR_SERVICE,
			      unsigned char NUM_BUFF,
			      unsigned char INTAD);
const int OK_CFG        = 0;    // Конфигурирование завершено
const int ERR_LSAP      = 18;   // Неразрешенный логический адрес
const int ERR_COMM      = 19;   // Неразрешенный параметр коммуникации
const int ERR_RDR       = 20;   // Неразрешенный параметр RDR услуги
const int ERR_NUM_BUFF  = 21;   // Недопускаемое запрошенное число буферов
const int ERR_CFG_LSAP  = 22;   // Нет конфигурации активного логического адреса
/*
const int ERR_ADR_IBUS  = 254;  // Неразрешенный адрес IBUS модуля
 */

extern "C" int ACTIVATE_SAP (unsigned char SAP_NO,
			     unsigned char INTAD);
const int OK_FUNC       = 0;    // Функция закончена
const int ERR_LSAP_ACT  = 23;   // Логический адрес уже активен
const int ERR_LSAP_CFG  = 24;   // Логический адрес незаконфигурирован
/*
const int ERR_LSAP      = 18;   // Неразрешенный логический адрес
const int ERR_ADR_IBUS  = 254;  // Неразрешенный адрес IBUS модуля
 */

extern "C" int DEACTIVATE_SAP (unsigned char SAP_NO,
			       unsigned char INTAD);
const int ERR_LSAP_DACT = 25;   // Логический адрес уже неактивен
/*
const int OK_FUNC       = 0;    // Функция закончена
const int ERR_LSAP_CFG  = 24;   // Логический адрес незаконфигурирован
const int ERR_LSAP      = 18;   // Неразрешенный логический адрес
const int ERR_ADR_IBUS  = 254;  // Неразрешенный адрес IBUS модуля
 */

extern "C" int CONNECT_SAP(unsigned char SAP_NO,
			   unsigned char RADR,
			   unsigned char RSAP,
			   unsigned char INTAD);
const int OK_NET        = 0;    // Связь установлена
const int ERR_ACT_LSAP  = 1;    // Локальный логический адрес не активизирован
const int ERR_RADR      = 11;   // Недопускаемый физический адрес станции
const int ERR_RSAP      = 12;   // Недопускаемый логический адрес удаленной станции
const int ERR_LSAP_CON  = 26;   // Логический адрес уже привязан
const int ERR_LSAP_NCON = 27;   // Логический адрес незаконфигурирован  для
										  // коммуникации с установлением связи

extern "C" int DISCONNECT_SAP (unsigned char SAP_NO,
			       unsigned char INTAD);
const int OK_NO_NET     = 0;    // Связь расторгнута
/*
const int ERR_ACT_LSAP  = 1;    // Локальный логический адрес не активизирован
const int ERR_LSAP_NCON = 27;   // Логический адрес незаконфигурирован  для
										  // коммуникации с установлением связи
 */
const int ERR_LSAP_NNET = 28;   // Логический адрес не имеет установленной связи

extern "C" int GET_STATION_PAR (unsigned char *STATION_ADRESS,
			  	unsigned char *SPEED,
			  	unsigned char *REVISION,
			  	unsigned char *STATUS,
			  	unsigned char INTAD);

extern "C" int SEND_DATA_ACK   (unsigned char RADR,
			  	unsigned char RSAP,
			  	unsigned char LSAP,
			  	unsigned char PRIOR,
			  	unsigned char NDATA,
			  	unsigned char *UDATA,
			  	unsigned char INTAD);
const int OK_SEND       = 0;    // Запрос принят
/*
const int ERR_ACT_LSAP  = 1;    // Локальный логический адрес не активизирован
 */
const int ERR_LSAP_REZ  = 2;    // Логический адрес локальной станции резервирован
										  // под локальные функции
/*
const int ERR_LSAP_NNET = 4;    // Логический адрес локальной станции законфигурирован
				// для коммуникаций с установлением логической связи,
				// а связи нет
*/
const int ERR_RSAP_NNET = 5;    // Параметр логического адреса удаленной станции
				// несоответствует логическому адресу локальной станции
				// определенный для коммуникации с устанавливанием
				// логической связи, а связь не установлена заданным
									  // параметром логического адреса удаленной станции
const int ERR_RADR_NNET = 6;    // Параметр физического адреса удаленной станции
				// несоответствует логическому адресу локальной станции
				// определенный для коммуникации с устанавливанием
				// логической связи, а связь не установлена заданной станцией,
				// или неразрешенное значение физического адреса удаленной станции
const int ERR_NO_BUF    = 8;    // Нет в распоряжении внутреннего буфера для хранения запроса
const int ERR_UP_BUF    = 9;    // Переполнено общее число внутренних буферов резервированное
										  // для данной локальной станции
const int ERR_PRIOR     = 14;   // Неразрешенный приоритет
const int ERR_NDATA     = 15;   // Неразрешенная длина данных

extern "C" int SEND_DATA       (unsigned char RADR,
			  	unsigned char RSAP,
			  	unsigned char LSAP,
			  	unsigned char PRIOR,
			  	unsigned char NDATA,
			  	unsigned char *UDATA,
			  	unsigned char INTAD);
				// Ошибки те же что и для SEND_DATA_ACK
extern "C" int UPDATE_REQUEST  (unsigned char LSAP,
			  	unsigned char NDATA,
			  	unsigned char *UDATA,
			  	unsigned char INTAD);
				// Ошибки те же что и для SEND_DATA_ACK
const int ERR_LSAP_RDR  = 17;   // Локальная станция незаконфигурирован, а для RDR услуг

extern "C" int REQUEST_REPLY   (unsigned char RADR,
			  	unsigned char RSAP,
			  	unsigned char LSAP,
			  	unsigned char PRIOR,
			  	unsigned char INTAD);
				// Ошибки те же что и для SEND_DATA_ACK										  // Ошибки те же что и для SEND_DATA_ACK

extern "C" int GET_CONFIRM     (unsigned char *RADR,
			  	unsigned char *LADR,
			  	unsigned char *RSAP,
			  	unsigned char *LSAP,
			  	unsigned char *PRIOR,
			  	unsigned char *NDATA,
			  	unsigned char *UDATA,
			  	unsigned char *STATUS,
			  	unsigned char *ANSW_TYPE,
			  	unsigned char INTAD);
const int OK_REQUEST    = 0;    // Запрос корректно выполнен
const int ERR_R_CONFIRM = 1;    // Нет подтверждения от удаленной станции
const int ERR_RSAP_CFG  = 2;    // Удаленный логический адрес незаконфигурирован
const int ERR_R_RESOURC = 3;    // Недостаточный ресурсы удаленной станции

										  // Параметр ANSW_TYPE может принимать одно
										  // из следующих значений:
enum ANSWERS
	{CONF_S_D_ACK,         /* = 0 Подтверждение для запроса заданного
				  функцией SEND_DATA_ACK               */
	 CONF_S_DATA,          /* = 1 Подтверждение для запроса заданного
				  функцией SEND_DATA                   */
	 CONF_R_REPL,          /* = 2 Подтверждение для запроса заданного
				  функцией REQUEST_REPLY               */
	 CONF_U_REQ};          /* = 3 Подтверждение для запроса заданного
				  функцией UPDATE_REQUEST              */


extern "C" int GET_DATA        (unsigned char *LADR,
			  	unsigned char *RADR,
			  	unsigned char *LSAP,
			  	unsigned char *RSAP,
			  	unsigned char *PRIOR,
			  	unsigned char *NDATA,
			  	unsigned char *UDATA,
				unsigned char INTAD);
#endif 	// __IBUS902C_H
