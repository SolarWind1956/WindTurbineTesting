/*
 * InptDErr - Errors messages module -------------------------
 *
 * Author: Konstantin A. Davidov
 *	   Crimea Engineering Centre
 *	   Phone: (06557) 68-2-39
 * -------------------------------------------------------------
 *    InptDErr  -  ?????? ?????? ????????? ?? ??????? ??? ??????
 *		??????? ???????????? ? ????????????.
 */
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "ibus902c.h"
#include "messagew.h"
struct Message {
	char name[20];
	int  code;
	char msg[250];
};

Message MessageBuffer [] = {
	{"INIT_NETWORK",	ERR_ST_ADR, 	"????????????? ????? ???????"},
	{"INIT_NETWORK", 	ERR_SPEED,  	"????????????? ??? ????????"},
	{"INIT_NETWORK", 	ERR_ST_LIST, 	"????????????? ?????? ???????? ???????"},
	{"INIT_NETWORK", 	ERR_INIT,	"????????????? ???? ?? ???????"},
	{"INIT_NETWORK", 	ERR_NO_IBUS, 	"?? ?????????? IBUS ?????? ??????????"},
	{"INIT_NETWORK", 	ERR_ADR_IBUS, 	"????????????? ????? IBUS ??????"},
	{"WRITE_STATION_LIST", 	REQUEST_CFG,	"?????? ?? ???????????? ???? ??????? ???????? ??????????"},
	{"WRITE_STATION_LIST", 	ERR_ST_SLAVE,	"??????? ?? ???????? ???????"},
	{"WRITE_STATION_LIST", 	ERR_ST_LIST,	"????????????? ?????? ???????? ???????"},
	{"WRITE_STATION_LIST", 	ERR_NO_IBUS,	"?? ?????????? IBUS ?????? ??????????"},
	{"WRITE_STATION_LIST", 	ERR_ADR_IBUS,	"????????????? ????? IBUS ??????"},
	{"RESET_44", 		ERR_NO_IBUS,	"?? ?????????? IBUS ?????? ??????????"},
	{"RESET_44", 		ERR_ADR_IBUS,	"????????????? ????? IBUS ??????"},
	{"CONFIGURE_SAP", 	ERR_LSAP,	"????????????? ?????????? ?????"},
	{"CONFIGURE_SAP", 	ERR_COMM,	"????????????? ???????? ????????????"},
	{"CONFIGURE_SAP", 	ERR_RDR,	"????????????? ???????? RDR ??????"},
	{"CONFIGURE_SAP", 	ERR_NUM_BUFF,	"????????????? ??????????? ????? ???????"},
	{"CONFIGURE_SAP", 	ERR_CFG_LSAP,	"??? ???????????? ????????? ??????????? ??????"},
	{"CONFIGURE_SAP", 	ERR_ADR_IBUS,	"????????????? ????? IBUS ??????"},
	{"ACTIVATE_SAP", 	ERR_LSAP_ACT,	"?????????? ????? ??? ???????"},
	{"ACTIVATE_SAP", 	ERR_LSAP_CFG,	"?????????? ????? ??????????????????"},
	{"ACTIVATE_SAP", 	ERR_LSAP,	"????????????? ?????????? ?????"},
	{"ACTIVATE_SAP", 	ERR_ADR_IBUS,	"????????????? ????? IBUS ??????"},
	{"DEACTIVATE_SAP", 	ERR_LSAP_CFG,	"?????????? ????? ??????????????????"},
	{"DEACTIVATE_SAP", 	ERR_LSAP_DACT,	"?????????? ????? ??? ?????????"},
	{"DEACTIVATE_SAP", 	ERR_LSAP,	"????????????? ?????????? ?????"},
	{"DEACTIVATE_SAP", 	ERR_ADR_IBUS,	"????????????? ????? IBUS ??????"},
	{"CONNECT_SAP",		ERR_ACT_LSAP,	"????????? ?????????? ????? ?? ?????????????"},
	{"CONNECT_SAP", 	ERR_RADR,	"????????????? ?????????? ????? ???????"},
	{"CONNECT_SAP", 	ERR_RSAP,	"????????????? ?????????? ????? ????????? ???????"},
	{"CONNECT_SAP", 	ERR_LSAP_CON,	"?????????? ????? ??? ????????"},
	{"CONNECT_SAP", 	ERR_LSAP_NCON, 	"?????????? ????? ?? ???????????????? ??? ???????????? ? ????????????? ?????"},
	{"DISCONNECT_SAP",	ERR_ACT_LSAP,	"????????? ?????????? ????? ?? ?????????????"},
	{"DISCONNECT_SAP",	ERR_LSAP_NCON,  "?????????? ????? ?? ???????????????? ??? ???????????? ? ????????????? ?????"},
	{"DISCONNECT_SAP",	ERR_LSAP_NNET,	"?????????? ????? ?? ????? ????????????? ?????"},
	{"SEND_DATA_ACK",	ERR_ACT_LSAP,	"????????? ?????????? ????? ?? ?????????????"},
	{"SEND_DATA_ACK",	ERR_LSAP_REZ,	"?????????? ????? ????????? ??????? ???????????? ??? ????????? ???????"},
	{"SEND_DATA_ACK",	ERR_LSAP_NNET,  "?????????? ????? ????????? ??????? ???????????????? ??? ???????????? ? ????????????? ?????????? ?????, ? ????? ???"},
	{"SEND_DATA_ACK",	ERR_RSAP_NNET,	"???????? ??????????? ?????? ????????? ??????? ??????????????? ??????????? ?????? ????????? ???????????? ??? ???????????? ? ??????????????? ?????????? ?????, ? ????? ?? ??????????? ???????? ?????????? ??????????? ?????? ????????? ???????"},
	{"SEND_DATA_ACK",	ERR_RADR_NNET,	"???????? ??????????? ?????? ????????? ??????? ??????????????? ??????????? ?????? ????????? ???????????? ??? ???????????? ? ??????????????? ?????????? ?????, ? ????? ?? ??????????? ???????? ??? ????????????? ???????? ??????????? ??????"},
	{"SEND_DATA_ACK",	ERR_NO_BUF,	"??? ? ???????????? ??????????? ?????? ??? ???????? ???????"},
	{"SEND_DATA_ACK",	ERR_UP_BUF,	"??????????? ????? ????? ?????????? ??????? ??????????????? ??? ?????? ????????? ???????"},
	{"SEND_DATA_ACK",	ERR_PRIOR,	"????????????? ?????????"},
	{"SEND_DATA_ACK",	ERR_NDATA,	"????????????? ????? ??????"},
	{"SEND_DATA",		ERR_ACT_LSAP,	"????????? ?????????? ????? ?? ?????????????"},
	{"SEND_DATA",		ERR_LSAP_REZ,	"?????????? ????? ????????? ??????? ???????????? ??? ????????? ???????"},
	{"SEND_DATA",		ERR_LSAP_NNET,  "?????????? ????? ????????? ??????? ???????????????? ??? ???????????? ? ????????????? ?????????? ?????, ? ????? ???"},
	{"SEND_DATA",		ERR_RSAP_NNET,	"???????? ??????????? ?????? ????????? ??????? ??????????????? ??????????? ?????? ????????? ???????????? ??? ???????????? ? ??????????????? ?????????? ?????, ? ????? ?? ??????????? ???????? ?????????? ??????????? ?????? ????????? ???????"},
	{"SEND_DATA",		ERR_RADR_NNET,	"???????? ??????????? ?????? ????????? ??????? ??????????????? ??????????? ?????? ????????? ???????????? ??? ???????????? ? ??????????????? ?????????? ?????, ? ????? ?? ??????????? ???????? ??? ????????????? ???????? ??????????? ??????"},
	{"SEND_DATA",		ERR_NO_BUF,	"??? ? ???????????? ??????????? ?????? ??? ???????? ???????"},
	{"SEND_DATA",		ERR_UP_BUF,	"??????????? ????? ????? ?????????? ??????? ??????????????? ??? ?????? ????????? ???????"},
	{"SEND_DATA",		ERR_PRIOR,	"????????????? ?????????"},
	{"SEND_DATA",		ERR_NDATA,	"????????????? ????? ??????"},
	{"UPDATE_REQUEST",	ERR_ACT_LSAP,	"????????? ?????????? ????? ?? ?????????????"},
	{"UPDATE_REQUEST",	ERR_LSAP_REZ,	"?????????? ????? ????????? ??????? ???????????? ??? ????????? ???????"},
	{"UPDATE_REQUEST",	ERR_LSAP_NNET,  "?????????? ????? ????????? ??????? ???????????????? ??? ???????????? ? ????????????? ?????????? ?????, ? ????? ???"},
	{"UPDATE_REQUEST",	ERR_RSAP_NNET,	"???????? ??????????? ?????? ????????? ??????? ??????????????? ??????????? ?????? ????????? ???????????? ??? ???????????? ? ??????????????? ?????????? ?????, ? ????? ?? ??????????? ???????? ?????????? ??????????? ?????? ????????? ???????"},
	{"UPDATE_REQUEST",	ERR_RADR_NNET,	"???????? ??????????? ?????? ????????? ??????? ??????????????? ??????????? ?????? ????????? ???????????? ??? ???????????? ? ??????????????? ?????????? ?????, ? ????? ?? ??????????? ???????? ??? ????????????? ???????? ??????????? ??????"},
	{"UPDATE_REQUEST",	ERR_NO_BUF,	"??? ? ???????????? ??????????? ?????? ??? ???????? ???????"},
	{"UPDATE_REQUEST",	ERR_UP_BUF,	"??????????? ????? ????? ?????????? ??????? ??????????????? ??? ?????? ????????? ???????"},
	{"UPDATE_REQUEST",	ERR_PRIOR,	"????????????? ?????????"},
	{"UPDATE_REQUEST",	ERR_NDATA,	"????????????? ????? ??????"},
	{"UPDATE_REQUEST",	ERR_LSAP_RDR,	"????????? ??????? ?? ????????????????? ??? RDR ?????"},
	{"GET_CONFIRM",		ERR_R_CONFIRM,	"??? ????????????? ?? ????????? ??????"},
	{"GET_CONFIRM",		ERR_RSAP_CFG,	"????????? ?????????? ????? ??????????????????"},
	{"GET_CONFIRM",		ERR_R_RESOURC,	"????????????? ??????? ????????? ???????"}
};

void
InptDErr(const char * ProcName, int RetCode)
{
int MessageSize = (sizeof (MessageBuffer) / sizeof(MessageBuffer[0]));
int i;
for (i = 0; i < MessageSize; i++)
  {
	 if (MessageBuffer[i].name == ProcName || MessageBuffer[i].code == RetCode)
	 {
		  MessageW(ProcName, MessageBuffer[i].msg);
		  break;
	 };
  }
}
/*
 * ---------------------------------------------------------
 *	EoF(inptderr.cpp)
 * ----------------------------------------------------------
 */
