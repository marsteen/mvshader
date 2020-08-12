//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGraflib.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <CGraflib.h>

//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SetError
//
// Parameter:
//
//
//---------------------------------------------------------------------------

static EGraflibError  StaticErrCode = EGRAFLIB_ERROR_NONE;
static std::string    StaticErrFunc;
static std::string    StaticErrMsg;

void CGraflib::SetError(EGraflibError er, const char* ef, const char* msg)
{
	StaticErrCode = er;
	StaticErrFunc = ef;
	StaticErrMsg  = msg;
}
