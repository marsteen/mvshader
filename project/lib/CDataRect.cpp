//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CDataRect.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <cstring>
#include <CDataRect.h>
#include <CPixel8.h>
#include <CPixel24.h>
#include <CPixel32.h>
#include <CBlockOperations.h>

using namespace std;

//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		Yflip
//
//---------------------------------------------------------------------------

void CDataRect::Yflip()
{
    unsigned char* PixelData = (unsigned char*)mData;
    int LineWidth = mWidth * (mBits / 8); // Breite des Bildes in Byte
    unsigned char* LineBuffer = new unsigned char[LineWidth];

    for (int y = (mHeight / 2) - 1; y >= 0; y--)
    {
        int off1 = y * LineWidth;
        int off2 = ((mHeight-1) - y) * LineWidth;

        memcpy(LineBuffer, PixelData + off2, LineWidth);
        memcpy(PixelData + off2, PixelData + off1, LineWidth);
        memcpy(PixelData + off1, LineBuffer, LineWidth);
    }
    delete[] LineBuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		Enhance
//
//---------------------------------------------------------------------------

void CDataRect::Enhance(int twidth, int theight, bool CenterX, bool CenterY)
{
    CDataRect dstRect;
    CBlockOperations Block;

    dstRect.mWidth = twidth;
    dstRect.mHeight = theight;
    dstRect.mBits = mBits;
    dstRect.mData = (CPixel*)new char[dstRect.DataSize()];



    memset(dstRect.mData, 0, dstRect.DataSize());


    int offx = (CenterX)  ? (twidth  - mWidth) / 2 : 0;
    int offy = (CenterY)  ? (theight - mHeight) / 2 : 0;

    Block.CopyBlock2(&dstRect, this, offx, offy, mWidth, mHeight, mBits / 8);

    DeleteData();
    CopyDataFrom(&dstRect);
}


//---------------------------------------------------------------------------
//
// Klasse:		CDataRect
// Methode:   Convert8BitTo24
//
//---------------------------------------------------------------------------

unsigned char* CDataRect::Convert8BitTo24() const
{
    /*
     * CPixel24* Buffer24 = NULL;
     * if (mBits == 8)
     * {
     *  const unsigned char* Buffer8Ptr = (const unsigned char*) mData;
     *  Buffer24 = new CPixel24[mWidth * mHeight];
     *  CPixel24* Buffer24Ptr = Buffer24;
     *  for (int y = 0; y < mHeight; y++)
     *  {
     *      for (int x = 0; x < mWidth; x++)
     *      {
     *          Buffer24Ptr->rgb[0] = *Buffer8Ptr;
     *          Buffer24Ptr->rgb[1] = 0;
     *          Buffer24Ptr->rgb[2] = 0;
     *          Buffer24Ptr++;
     *          Buffer8Ptr++;
     *      }
     *  }
     * }
     *
     * return (unsigned char*) Buffer24;
     */
    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:			static
// Methode:			TestSegmentT
//
// Parameter:
//
// Return:			true = alle Pixel des Segments sind gleich
//
//---------------------------------------------------------------------------

template<typename Tpixel>
static bool TestSegmentT(const Tpixel* PixelData, int Width, int Height)
{
    const Tpixel* PixelPtr = PixelData;
    const Tpixel Pixel0 = PixelData[0];

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            if (PixelPtr[x] != Pixel0)
            {
                return false;
            }
        }
        PixelPtr += Width;
    }
    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:		CDataRect
// Methode:		TestDataEqual
//
// Parameter:
//
// Return:		true = alle Pixel des Segments sind gleich
//
//---------------------------------------------------------------------------

bool CDataRect::TestDataEqual() const
{
    bool r = false;

    switch (mBits)
    {
        case 8:
        {
            const CPixel8* DataPtr = (const CPixel8*)mData;
            r = TestSegmentT(DataPtr, mWidth, mHeight);
        }
        break;

        case 24:
        {
            const CPixel24* DataPtr = (const CPixel24*)mData;
            r = TestSegmentT(DataPtr, mWidth, mHeight);
        }
        break;

        case 32:
        {
            const CPixel32* DataPtr = (const CPixel32*)mData;
            r = TestSegmentT(DataPtr, mWidth, mHeight);
        }
        break;

        default:

            break;
    }
    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		TestZero
//
// Return:    true  = Daten sind 0
//            false = Daten sind ungleich 0
//
//
//---------------------------------------------------------------------------

bool CDataRect::TestZero()
{
    if (mData != NULL)
    {
        int LineSize = mWidth * (mBits/8);
        char* NullLine = new char[LineSize];
        const char* LinePtr = (const char*)mData;
        char Char0 = LinePtr[0];

        memset(NullLine, Char0, LineSize);

        for (int y = 0; y < mHeight; y++)
        {
            if (memcmp(LinePtr, NullLine, LineSize) != 0)
            {
                return false;
            }
            LinePtr += LineSize;
        }
        delete[] NullLine;
    }
    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		Show
//
//
//
//---------------------------------------------------------------------------

void CDataRect::Show() const
{
    cout << "B=" << mBits << " W=" << mWidth  << " H=" << mHeight << " D=" << (ptrdiff_t)mData << " Size=" << DataSize() << endl;
}


std::string CDataRect::Print() const
{
    stringstream mstr;

    mstr << "B=" << mBits << " W=" << mWidth  << " H=" << mHeight << " D=" << (ptrdiff_t)mData << " Size=" << DataSize();
    return mstr.str();
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		Convert32to24
//
//
// Konvertieren einer 32-Bit Grafik nach 24 Bit
//
//---------------------------------------------------------------------------

void CDataRect::Convert32to24()
{
    /*
     * if (mBits == 32)
     * {
     *  if (mData != NULL)
     *  {
     *
     *
     *      const CPixel32* DataPtr32 = (const CPixel32*) mData;
     *      CPixel24* Data24    = new CPixel24[mWidth * mHeight];
     *      CPixel24* DataPtr24 = Data24;
     *
     *      for (int y = 0; y < mHeight; y++)
     *      {
     *          for (int x = 0; x < mWidth; x++)
     *          {
     * DataPtr24 = *DataPtr32;
     *              DataPtr24++;
     *              DataPtr32++;
     *          }
     *      }
     *
     *      delete[] mData;
     *      mData = Data24;
     *      mBits = 24;
     *  }
     * }
     */
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		SwapRedBlue
//
//---------------------------------------------------------------------------

void CDataRect::SwapRedBlue()
{
    switch (mBits)
    {
        case 24:
        {
            CPixel24* PixelPtr = (CPixel24*)mData;

            for (int y = 0; y < mHeight; y++)
            {
                for (int x = 0; x < mWidth; x++)
                {
                    char s = PixelPtr->rgb[0];
                    PixelPtr->rgb[0] = PixelPtr->rgb[2];
                    PixelPtr->rgb[2] = s;
                    PixelPtr++;
                }
            }
        }
        break;

        case 32:
        {
            CPixel32* PixelPtr = (CPixel32*)mData;

            for (int y = 0; y < mHeight; y++)
            {
                for (int x = 0; x < mWidth; x++)
                {
                    char s = PixelPtr->rgba[0];
                    PixelPtr->rgba[0] = PixelPtr->rgba[2];
                    PixelPtr->rgba[2] = s;
                    PixelPtr++;
                }
            }
        }
        break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		EnhanceCenter
//
//---------------------------------------------------------------------------

void CDataRect::EnhanceCenter(int twidth, int theight, int* ex, int* ey)
{
    CDataRect dstRect;
    CBlockOperations Block;

    if (ex != NULL)
    {
        *ex = twidth - dstRect.mWidth;
    }

    if (ey != NULL)
    {
        *ey = theight - dstRect.mHeight;
    }


    dstRect.mWidth = twidth;
    dstRect.mHeight = theight;
    dstRect.mBits = mBits;
    dstRect.mData = (CPixel*)new char[dstRect.mWidth * dstRect.mHeight * (mBits / 8)];



    memset(dstRect.mData, 0, dstRect.DataSize());


    int offx = (twidth  - mWidth) / 2;
    int offy = (theight - mHeight) / 2;

    Block.CopyBlock2(&dstRect, this, offx, offy, mWidth, mHeight, mBits / 8);

    DeleteData();
    CopyDataFrom(&dstRect);
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:		EnhanceBlock
//
// Erweitert die Daten, so dass die Breite durch BlockW und die Hoehe
// durch BlockH teilbar ist (z.B. fuer OpenGL-Texturen)
//
//---------------------------------------------------------------------------

void CDataRect::EnhanceBlock(int BlockW, int BlockH, int* ex, int* ey, bool CenterX, bool CenterY)
{
    int wrest = mWidth % BlockW;
    int hrest = mHeight % BlockH;

    if ((wrest != 0) || (hrest != 0))
    {
        int TargetW;
        int TargetH;

        if (wrest != 0)
        {
            TargetW = ((mWidth / BlockW) + 1) * BlockW;
        }
        else
        {
            TargetW = mWidth;
        }

        if (hrest != 0)
        {
            TargetH = ((mHeight / BlockH) + 1) * BlockH;
        }
        else
        {
            TargetH = mHeight;
        }

        if (ex != NULL)
        {
            *ex = TargetW - mWidth;
        }

        if (ey != NULL)
        {
            *ey = TargetH - mHeight;
        }

        Enhance(TargetW, TargetH, false, false);
    }
    else
    {
        if (ex != NULL)
        {
            *ex = 0;
        }

        if (ey != NULL)
        {
            *ey = 0;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraflib
// Methode:		Convert32to24
//
//
// Konvertieren einer 24-Bit Grafik nach 32 Bit
//
//---------------------------------------------------------------------------

void CDataRect::Convert24to32()
{
    /*
     * if (mBits == 24)
     * {
     *  const CPixel24* DataPtr24 = (const CPixel24*) mData;
     *  CPixel32* Data32    = new CPixel32[mWidth * mHeight];
     *  CPixel32* DataPtr32 = Data32;
     *
     *  for (int y = 0; y < mHeight; y++)
     *  {
     *      for (int x = 0; x < mWidth; x++)
     *      {
     * DataPtr32 = *DataPtr24;
     *          DataPtr32->rgba[3] = 0xFF;
     *          DataPtr24++;
     *          DataPtr32++;
     *      }
     *  }
     *
     *  delete[] mData;
     *  mData = Data32;
     *  mBits = 32;
     * }
     */
}


void CDataRect::Convert24to8()
{
    if (mBits == 24)
    {
        const unsigned char* DataPtr24 = (const unsigned char*)mData;
        unsigned char* NewData8 = new unsigned char[mWidth * mHeight];
        unsigned char* DataPtr8 = NewData8;


        for (int y = 0; y < mHeight; y++)
        {
            for (int x = 0; x < mWidth; x++)
            {
                *DataPtr8 = (DataPtr24[0] + DataPtr24[1] + DataPtr24[2]) / 3;

                DataPtr8 += 1;
                DataPtr24 += 3;
            }
        }

        delete[] mData;
        mData = (CPixel*)NewData8;
        mBits = 8;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:   AlphaTo32
//
// Erzeugt 32 Bit Daten aus 8 Bit Pixeldaten
//
// Return: true  - 32 Bit Daten erzeugt
//         false - Keine Operation durchgefuehrt
//
//---------------------------------------------------------------------------

bool CDataRect::AlphaTo32()
{
    bool r = false;

    if (mBits == 8)
    {
        unsigned char* PixelDataPtr = (unsigned char*)mData;

        int Size = mWidth * mHeight;

        unsigned char* C32Data = new unsigned char[Size * 4];
        unsigned char* C32DataPtr = C32Data;

        for (int i = 0; i < Size; i++)
        {
            C32DataPtr[0] = *PixelDataPtr;
            C32DataPtr[1] = *PixelDataPtr;
            C32DataPtr[2] = *PixelDataPtr;
            C32DataPtr[3] = *PixelDataPtr;

            PixelDataPtr += 1;
            C32DataPtr += 4;
        }
        delete[] mData;
        mData = (CPixel32*)C32Data;
        mBits = 32;
        r = true;
    }
    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDataRect
// Methode:   AlphaTo32
//
// Erzeugt 32 Bit Daten aus 8 Bit Pixeldaten
// Originaldaten werden NICHT geloscht
//
// Return: true  - 32 Bit Daten erzeugt
//         false - Keine Operation durchgefuehrt
//
//---------------------------------------------------------------------------

bool CDataRect::AlphaTo32(unsigned char r, unsigned char g, unsigned char b)
{
    bool d = false;

    if (mBits == 8)
    {
        unsigned char* PixelDataPtr = (unsigned char*)mData;

        int Size = mWidth * mHeight;

        unsigned char* C32Data = new unsigned char[Size * 4];
        unsigned char* C32DataPtr = C32Data;

        for (int i = 0; i < Size; i++)
        {
            C32DataPtr[0] = r;
            C32DataPtr[1] = g;
            C32DataPtr[2] = b;
            C32DataPtr[3] = *PixelDataPtr;

            PixelDataPtr += 1;
            C32DataPtr += 4;
        }
        mData = (CPixel32*)C32Data;
        mBits = 32;
        d = true;
    }
    return d;
}
