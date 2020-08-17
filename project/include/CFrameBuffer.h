//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CFrameBuffer.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CFrameBuffer_H
#define CFrameBuffer_H


#include <GLinclude.h>

class CFrameBuffer
{
public:


    CFrameBuffer()
    {
        mWidth = 0;
        mHeight = 0;
        mScale = 1.0f;
        mOldFramebuffer = 0;
    }


    void CreateFramebufferObject(int Width, int Height);
    void DeleteFramebuffer();
    void DrawToFrameBuffer(bool Enable);
    void DrawTexture(const CGL_Shader* glcon, float xpos, float ypos) const;
    void DrawTexture(const CGL_Shader* glcon, float xpos, float ypos, float w, float h) const;
    void DrawTextureRotated(const CGL_Shader* glcon, float xpos, float ypos) const;

    void SetScale(float f) { mScale = f; }
    void SetViewport();
    void RestoreViewport() const;
    void ClearBuffer() const;

    float Width()  const { return mScale * mWidth; }
    float Height() const { return mScale * mHeight; }
    float UnscaledWidth()  const { return mWidth; }
    float UnscaledHeight()  const { return mHeight; }


protected:

    GLuint mFBhandle;
    GLuint mColorRenderbuffer;
    GLuint mDepthRenderbuffer;
    GLuint mTexture;
    GLint mOldFramebuffer;
    GLint mOldVp[4];     // Alter Viewport

    unsigned int mWidth;
    unsigned int mHeight;
    float mScale;
};


#endif
