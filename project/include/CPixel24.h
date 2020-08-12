//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CPixel24.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef CPIXEL24_H
#define CPIXEL24_H


#ifndef byte
typedef unsigned short		word;
typedef unsigned char		byte;
#endif

#include <CPixel.h>

class CPixel24 : public CPixel
{
    public:

        byte rgb[3];

        void operator=(const CPixel24& p2)
        {
            rgb[0] = p2.rgb[0];
            rgb[1] = p2.rgb[1];
            rgb[2] = p2.rgb[2];
        }


        void operator=(int i)
        {
            rgb[0] = i >> 16;
            rgb[1] = (i >> 8) & 0xFF;
            rgb[2] = i & 0xFF;
        }


        unsigned int GetInt()
        {
            return (rgb[2] << 16) | (rgb[1] << 8) | rgb[0];
        }


        bool operator==(const CPixel24& p2) const;
        bool operator!=(const CPixel24& p2) const;
        void Set(word w1, byte m1);
        void Set(byte r, byte g, byte b);
        void Set(unsigned int b);
        void SwapRedBlue(void);
        int Brightness(void);
};



#endif
