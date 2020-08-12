//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CPixel32.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef CPIXEL32_H
#define CPIXEL32_H

#include <CPixel.h>

class CPixel32 : public CPixel
{
    public:

        unsigned char rgba[4];

        void operator=(const CPixel32& p2)
        {
            rgba[0] = p2.rgba[0];
            rgba[1] = p2.rgba[1];
            rgba[2] = p2.rgba[2];
            rgba[3] = p2.rgba[3];
        }


        void operator=(int i)
        {
            rgba[0] = i >> 16;
            rgba[1] = (i >> 8) & 0xFF;
            rgba[2] = i & 0xFF;
            rgba[3] = 0;
        }


        bool operator==(const CPixel32& p2) const;
        bool operator!=(const CPixel32& p2) const;
        void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        int GetInt();
        void SwapRedBlue();
        int Brightness();
};

#endif
