//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CContextLissa.h
// @DATE     :	22.5.2020
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef CContextLissa_H
#define CContextLissa_H


#include <CGL_Context.h>
#include <CGL_Shader_Color.h>
#include <CGL_Shader_Blurline.h>
#include <CGL_Shader_Texture.h>
#include <CGL_Shader_Noise.h>
#include <CLineStrip.h>

class CContextLissa : public CGL_Context
{
    public:

        CContextLissa();


        void Draw2D();
        bool Init(int w, int h);
        void MouseMove(int x, int y, bool lb, bool rb);
        void KeyPress(int key, bool down);


    protected:

        void CalcLissa(TLinePoints& LissaPoints, float horInit, float verInit, float verSpeed, float horSpeed);

        CGL_Shader_Color* mColorShader;
        CGL_Shader_Blurline* mBlurlineShader;
        CGL_Shader_Texture* mTextureShader;
        CGL_Shader_Noise* mNoiseShader;

        int mSpeed;
        int mResolution;
        float mVerSpeed;
        float mHorSpeed;
};

#endif
