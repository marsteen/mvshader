//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CGL_Freetype.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CGL_FREETYPE_H
#define CGL_FREETYPE_H

#include <set>
#include <CRectT.h>
#include <CSetPointer.h>
#include <CGL_Shader.h>
#include <NStringTool.h>
#include <CFrameBuffer.h>
#include <ft2build.h>
#include FT_FREETYPE_H


struct SGlyphTexture
{
    bool operator<(const SGlyphTexture& k) const
    {
        return mGlyphIndex < k.mGlyphIndex;
    }


    int				mGlyphIndex;
    unsigned int	mTexHandle;
    CRectT<float>	mTexRect;
    int				mWidth;
    int				mHeight;
    float			mAdvanceX;
    int				mEx;
    int				mEy;
    int				mDesc;
    float			mBitmapLeft;
    float			mBitmapTop;
    float			mBitmapBottom;
};

typedef CSetPointer<SGlyphTexture> TGlyphTexture;

class CGL_Freetype
{
    public:

        void Init(const char* Fontfile, int FontSize);
        int RenderString(const CGL_Shader* glcon, float x, float y, const unsigned char* text, int w);
        float GetStringWidth(const unsigned char* text, int b);
        bool GetGlyphRect(const int c, float x, float y, CRectT<float>& vr);
        int RenderStringFrame(const CGL_Shader* glcon, float x, float y, const float* FillColor, const float* FrameColor, const unsigned char* text, int b);
        bool Umbruch(const CGL_Shader* glcon, const std::string& text, int Limit, stringvector* utext);
        void RenderUmbruch(const CGL_Shader* glcon, int xpos, int ypos, const std::string& text, int wLimit, int LineHeight, int MaxLines);
        void RenderUmbruch2(const CGL_Shader* glcon, int center_xpos, int ypos, const std::string& text, int wLimit, int LineHeight, int MaxLines);

        void RenderToFrameBuffer(const CGL_Shader* glcon, float left, float right, int Scale, const unsigned char* txt);
        void DrawFrameBuffer(const CGL_Shader* glcon, float xpos, float ypos) const;
        void DrawFrameBufferRotated(const CGL_Shader* glcon, float xpos, float ypos) const;
        void DrawFrameBufferRotatedCenter(const CGL_Shader* glcon, const char* txt, float xpos, float ycenter);
        static void ResetFrameBuffer();



        void Reset();

        float LineOffset() const { return mFontSize * 0.6; }



    protected:

        const SGlyphTexture* GetGlyphTex(const int c);
        float RenderGlyph(const CGL_Shader* glcon, float x, float y, const int c, int* LastIndex);
        int GetGlyphWidth(const int c, int* LastIndex);
        void RenderToFrameBuffer(const CGL_Shader* glcon, CFrameBuffer* fb, float left, float right, const unsigned char* txt);

        std::set<TGlyphTexture> mGlyphSet;
        FT_Face mFace;
        int mFontSize;
};


#endif
