//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CUtf.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CUTF_H
#define CUTF_H

#include <string>

class CUtf
{
    public:

        static void XmlToUtf8(const char* XmlStr, std::string* Utf8Str);
        static std::string XmlToUtf8(const std::string& XmlStr);
};

#endif
