//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CHexConv.hpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************

#include <string>
#include <algorithm>

//---------------------------------------------------------------------------
//
// Klasse:    CHexConv
// Methode:
//
//---------------------------------------------------------------------------

inline int CHexConv::NibbleToInt(char a)
{
	int b;
	if ((a >= 'a') && (a <= 'f'))
	{
		b = a - 'a' + 10;
	}
	else if ((a >= 'A') && (a <= 'F'))
	{
		b = a - 'A' + 10;
	}
	else if ((a >= '0') && (a <= '9'))
	{
		b = a - '0';
	}
	else
	{
		b = 0;
	}
	return b;
}

//---------------------------------------------------------------------------
//
// Klasse:    CHexConv
// Methode:   ByteToInt
//
//---------------------------------------------------------------------------

inline int CHexConv::ByteToInt(unsigned char a, unsigned char b)
{
  return (NibbleToInt(a) << 4) + NibbleToInt(b);
}

//---------------------------------------------------------------------------
//
// Klasse:    CHexConv
// Methode:   ByteToFloat
//
//---------------------------------------------------------------------------

inline float CHexConv::ByteToFloat(unsigned char a, unsigned char b)
{
  return ((float) ByteToInt(a, b)) / 255.0;
}

//---------------------------------------------------------------------------
//
// Klasse:    CHexConv
// Methode:   StringToInt
//
//---------------------------------------------------------------------------

inline int CHexConv::StringToInt(const char* s)
{
	int i = 0;
	while (*s != 0)
	{
		i <<= 4;
		i += NibbleToInt(*(s++));
	}
	return i;
}

//---------------------------------------------------------------------------
//
// Klasse:    CHexConv
// Methode:   ToHex
//
//---------------------------------------------------------------------------


inline std::string CHexConv::ToHex(int d)
{
	const char* HexTab = "0123456789ABCDEF";

	std::string h;


	if (d == 0)
	{
		h = "0";
	}
	else
	{
		while (d)
		{
			h += HexTab[d & 0x0F];
			d /= 16;
		}
		std::reverse(h.begin(), h.end());
	}
	return h;
}

//---------------------------------------------------------------------------
//
// Klasse:    CHexConv
// Methode:   ToHexSmall
//
//---------------------------------------------------------------------------


inline std::string CHexConv::ToHexSmall(int d)
{
	const char* HexTab = "0123456789abcdef";

	std::string h;


	if (d == 0)
	{
		h = "0";
	}
	else
	{
		while (d)
		{
			h += HexTab[d & 0x0F];
			d /= 16;
		}
		std::reverse(h.begin(), h.end());
	}
	return h;
}