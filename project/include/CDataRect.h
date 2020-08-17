#ifndef CDATARECT_H
#define CDATARECT_H

#include <CPixel.h>
#include <string>
#include <cstring>

class CDataRect
{
public:

    CDataRect()
    {
        mData = (CPixel*)NULL;
        mPalette = NULL;
        mWidth = 0;
        mHeight = 0;
        mBits = 0;
    }


    void operator=(CDataRect& dr)
    {
        mPalette = dr.mPalette;
        mWidth = dr.mWidth;
        mHeight = dr.mHeight;
        mBits = dr.mBits;
    }


    unsigned int DataSize() const
    {
        return mWidth * mHeight * (mBits / 8);
    }


    void SetData(int b)
    {
        memset(mData, 0, mWidth * mHeight * (mBits/8));
    }


    void CopyDataFrom(const CDataRect* dr)
    {
        mData = dr->mData;
        mPalette = dr->mPalette;
        mWidth = dr->mWidth;
        mHeight = dr->mHeight;
        mBits = dr->mBits;

        //mData = new unsigned char[mWidth * mHeight * (mBits/8)];
    }


    bool InterleaveAlpha(const CDataRect* Alpha);
    bool InterleaveAlpha();
    bool AlphaTo32();
    bool AlphaTo32(unsigned char r, unsigned char g, unsigned char b);
    void Enhance(int twidth, int theight, bool CenterX = false, bool CenterY = false);
    void EnhanceBlock(int BlockW, int BlockH, int* ex = (int*)NULL, int* ey = (int*)NULL, bool CenterX = false, bool CenterY = false);
    void EnhanceCenter(int twidth, int theight, int* ex = (int*)NULL, int* ey = (int*)NULL);
    void Blur(int stages);

    int LineSize() const { return mWidth * (mBits / 8); }
    bool TestDataEqual() const;
    unsigned char* Convert8BitTo24() const;
    bool TestZero();
    void Show() const;
    std::string Print() const;
    void Convert32to24();
    void Convert24to32();
    void Convert24to8();



    void SwapRedBlue();
    void Yflip();
    void ExtractAlpha();

    void DeleteData()
    {
        delete[] mData;
        mData = 0L;
    }


    CPixel* mData;
    void* mPalette;
    int mWidth;
    int mHeight;
    int mBits;

protected:

    int Offset(int x, int y);
    int Offset(int x, int y, int c, int b);
};
#endif
