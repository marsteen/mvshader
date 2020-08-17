//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CHexConv.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#ifndef CHEXCONV
#define CHEXCONV

class CHexConv
{
public:

    static int NibbleToInt(char a);
    static int ByteToInt(unsigned char a, unsigned char b);
    static float ByteToFloat(unsigned char a, unsigned char b);
    static int StringToInt(const char* s);
    static std::string ToHex(int i);
    static std::string ToHexSmall(int i);
};

#include <CHexConv.hpp>

#endif // CHEXCONV
