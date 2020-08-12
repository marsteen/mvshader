//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	NUtf.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Revisionsgeschichte:
//
// 05.11.2013 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

#include <iostream>
#include <cstring>

using namespace std;

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ror
//
// a wird um n Stellen nach rechts verschoben.
// Die herausgeschobenen Stellen werden in b hineingeschoben.
//
// Bespiel:
// Vorher: a=00001110 b=00001101
// 
// ror(a, b, 2)
//
// Nachher: a=00000011 b=10001101
//
//---------------------------------------------------------------------------

static void ror(unsigned char& a, unsigned char& b, int n)
{
	unsigned char k = 0;
	unsigned char m = 0xFF;

	for (int i = 0; i < n; i++)
	{
		k >>= 1;
		m >>= 1;
    if (a & 1)
    {
			k |= 0x80;
		}
		a >>= 1;
	}
	b &= m; // Ausmaskieren 
	b |= k;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   UtfToLong
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int UtfToLong(const unsigned char* from, int n)
{
	unsigned int Utf32 = 0;
	unsigned char* UtfPtr = (unsigned char*) &Utf32;

	for (int i = 0; i < n; i++)
	{
    UtfPtr[i] = from[n - i - 1];
  }
  return Utf32;
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftBits2
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int ShiftBits2(const unsigned char* from)
{
	unsigned char tmp[2];
	memcpy(tmp, from, 2);

	tmp[0] &= 0x1F;
  ror(tmp[0], tmp[1], 2);
	
	//cout << "tmp[0]=" << (unsigned int) tmp[0] << endl;
	//cout << "tmp[1]=" << (unsigned int) tmp[1] << endl;
	
	
  return UtfToLong(tmp, 2);
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftBits3
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int ShiftBits3(const unsigned char* from)
{
	unsigned char  tmp[3];
	memcpy(tmp, from, 3);


	tmp[0] &= 0x0F;
  ror(tmp[1], tmp[2], 2);
  ror(tmp[0], tmp[1], 4);

  return UtfToLong(tmp, 3);
}

//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:   ShiftBits4
//
//
//
//
//---------------------------------------------------------------------------

static unsigned int ShiftBits4(const unsigned char* from)
{
	unsigned char  tmp[4];
	memcpy(tmp, from, 4);

	tmp[0] &= 0x07;
  ror(tmp[2], tmp[3], 2);
  ror(tmp[1], tmp[2], 4);
  ror(tmp[0], tmp[1], 6);

  return UtfToLong(tmp, 4);
}

//---------------------------------------------------------------------------
//
// namespace: NUtf
// Methode:   Utf8_to_Utf32
//
//
//
//
//---------------------------------------------------------------------------

namespace NUtf
{
	int Utf8_char_len(const unsigned char Utf8)
	{
		int Size = 0;
		int n = Utf8;
		//cout << std::hex << "n=" << n << " nibble=" << (Utf8 & 0xF0) << endl;
		if ((Utf8 & 0x80) == 0) // Einstellig, ASCII-Code
		{
			 Size = 1;
		}
		else if ((Utf8 & 0xE0) == 0xC0) // zweistellig, beginnt mit 110
		{
			Size = 2;
		}
		else if ((Utf8 & 0xF0) == 0xE0) // dreistellig, beginnt mit 1110
		{
			Size = 3;
		}
		else if ((Utf8 & 0xF8) == 0xF0) // vierstellig, beginnt mit 11110
		{
			Size = 4;
		}
		return Size;
	}


	//---------------------------------------------------------------------------
	//
	// namespace: NUtf2
	// Methode:   Utf8_to_Utf32
	//
	//
	//
	//
	//---------------------------------------------------------------------------

	unsigned int Utf8_to_Utf32(const unsigned char* Utf8, int* Size)
	{
		 unsigned int Utf32 = 0;
		 *Size = Utf8_char_len(Utf8[0]);

		 /*
		 unsigned int (*ShiftBitsTab[])(const unsigned char*) =
		 {
				ShiftBits2,
				ShiftBits3,
				ShiftBits4
		 };
 		Utf32 = ShiftBitsTab[*Size](Utf8);
 		*/

		switch (*Size)
		{
      case 1:  Utf32 = Utf8[0]; break;
      case 2:  Utf32 = ShiftBits2(Utf8); break;
      case 3:  Utf32 = ShiftBits3(Utf8); break;
      case 4:  Utf32 = ShiftBits4(Utf8); break;
		}
		return Utf32;
	}
	
	/*
	unsigned int Utf8_to_Utf32(const unsigned char* Utf8, int* Size)
	{
		unsigned int Utf32 = 0;
		if ((Utf8[0] & 0x80) == 0) // Einstellig, ASCII-Code
		{
			 Utf32 = Utf8[0];
			 *Size = 1;
		}
		else if ((Utf8[0] & 0xC0) == 0xC0) // zweistellig, beginnt mit 110
		{			
			Utf32 = ShiftBits2(Utf8);
			*Size = 2;
		}
		else if ((Utf8[0] & 0xE0) == 0xE0) // dreistellig, beginnt mit 1110
		{
			Utf32 = ShiftBits3(Utf8);
			*Size = 3;

		}
		else if ((Utf8[0] & 0xF0) == 0xF0) // vierstellig, beginnt mit 11110
		{
			Utf32 = ShiftBits4(Utf8);
			*Size = 4;
		}
		return Utf32;

	}
	*/


};