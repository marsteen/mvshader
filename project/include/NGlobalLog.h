//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	NGlobalLog.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef NGLOBALLOG_H
#define NGLOBALLOG_H

#include <sstream>

extern std::stringstream gdstr;
extern bool GlobalDebugEnabled;

namespace NGlobalLog
{
void gdlog();
void gderr();
}


#endif
