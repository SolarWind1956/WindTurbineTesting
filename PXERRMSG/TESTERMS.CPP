/*
 * 	TestErMs.cpp - ?????? ???????? ?????????? PXERRMSG.DLL
 *		????? ?????? ?????? ????????? ?? ????????????
 *		??????????.
 */
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\framewin.h>
#include <bwcc.h>

#include "pxengine.h"
void PXErrMesg(int);		// ??????? ??????????? ??????.
int
OwlMain(int /*argc*/, char* /*argv*/ [])
{
	BWCCGetVersion();						// BWCC - support
	PXErrMesg(PXSUCCESS);// ????? ??????? ?????????
	return 0;
};
