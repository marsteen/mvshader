//***************************************************************************
//
//
// @PROJECT  :  Basic SDL
// @VERSION  :  1.0
// @FILENAME :  CContextLissa.cpp
// @DATE     :  8.1.2015
//
// @AUTHOR   :  Martin Steen
// @EMAIL    :  martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <fstream>
#include <CGL_Basic.h>
#include <NGlobalLog.h>
#include <CContextLissa.h>
#include <MathDefines.h>
#include <CRandom.h>

#include <CLineData.h>
#include <CPointData.h>
#include <CRingData.h>
#include <CPointObject.h>
#include <CPlayObject.h>
#include <CFrameBuffer.h>

using namespace std;
using namespace NGlobalLog;


float GlobalAspect = 1.0;
//static CGL_Freetype* Freetype;
static CPlayObject* block;
static CPlayObject racket;
static CPointObject Ball;
static CFrameBuffer* FrameBuffer;
static float LineWidth = 0.015f;

bool GlobalPolygonMode;

extern void checkGlError(const char* func);

CGL_Context* CGL_Context::CreateContext()
{
    return new CContextLissa;
}


CContextLissa::CContextLissa() : CGL_Context()
{
    mVerSpeed = 1.0f;
    mHorSpeed = 2.0f;
    mResolution = 10;
    mSpeed = 1;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode  CalcLissa
//
//
//---------------------------------------------------------------------------

void CContextLissa::CalcLissa(TLinePoints& LissaPoints, float horInit, float verInit, float verSpeed, float horSpeed)
{
    float AngleHor = horInit;
    float AngleVer = verInit;
    float scale = 0.6f;
    int lim = 360 * mResolution;

    const float f = M_PI*2 / lim;

    for (int i = 0; i < lim; i++)
    {
        CVector2<float> p;
        p.x = sin(AngleVer) * scale;
        p.y = sin(AngleHor) * scale;
        LissaPoints.push_back(p);

        AngleVer += verSpeed * f;
        AngleHor += horSpeed * f;
    }
}


void CContextLissa::KeyPress(int key, bool down)
{
    float w = 1.0f;

    if (down)
    {
        switch (key)
        {
            case 'r':

                mHorSpeed = 1.0f;
                mVerSpeed = 1.0f;
                mResolution = 1;
                mSpeed = 1;
                break;

            case 'a':

                if (mHorSpeed > 1.0f)
                {
                    mHorSpeed -= w;
                }
                break;

            case 'd':

                mHorSpeed += w;
                break;

            case 's':

                if (mVerSpeed > 1.0f)
                {
                    mVerSpeed -= w;
                }
                break;

            case 'w':

                mVerSpeed += w;
                break;

            case 'n':

                if (mResolution > 1)
                {
                    mResolution -= 1;
                }
                break;

            case 'm':

                mResolution++;
                break;

            case 'p':

                GlobalPolygonMode = !GlobalPolygonMode;
                break;

            case 'v':

                LineWidth -= 0.01f;
                break;

            case 'b':

                LineWidth += 0.01f;
                break;

            case 'u':

                if (mSpeed > 1)
                {
                    mSpeed -= 1;
                }
                break;

            case 'i':

                mSpeed++;
                break;

            case '1':
                mHorSpeed = 2;
                mVerSpeed = 1;
                break;

            case '2':
                mHorSpeed = 3;
                mVerSpeed = 2;
                break;

            case '3':
                mHorSpeed = 3;
                mVerSpeed = 4;
                break;

            case '4':
                mHorSpeed = 3;
                mVerSpeed = 5;
                break;

            case '5':
                mHorSpeed = 3;
                mVerSpeed = 7;
                break;

            case '6':
                mHorSpeed = 4;
                mVerSpeed = 7;
                break;

            case '7':
                mHorSpeed = 5;
                mVerSpeed = 7;
                break;

            case '8':
                mHorSpeed = 7;
                mVerSpeed = 8;
                break;
        }
    }

    cout << "HS=" << mHorSpeed << " VS=" << mVerSpeed << " RES=" << mResolution << " SPD=" << mSpeed << endl;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode  Init
//
//
//---------------------------------------------------------------------------


bool CContextLissa::Init(int w, int h)
{
    CGL_Context::Init(w, h);

    mColorShader = new CGL_Shader_Color;
    mColorShader->InitShader();

    mBlurlineShader = new CGL_Shader_Blurline;
    mBlurlineShader->InitShader();

    mNoiseShader = new CGL_Shader_Noise;
    mNoiseShader->InitShader();


    mTextureShader = new CGL_Shader_Texture;
    mTextureShader->InitShader();

    //cout << "Width=" << mWidth << " Height=" << mHeight << endl;

    return true;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode  MouseMove
//
//
//---------------------------------------------------------------------------

void CContextLissa::MouseMove(int xrel, int yrel, bool lb, bool rb)
{
    if (fabs(xrel) < 100)
    {
        racket.mSpeed = float(xrel) / 500.0f;
        racket.mPos.x += racket.mSpeed;
    }
}


#define LINE_W    0.04f



static void DrawTestQuad(CGL_Shader_Blurline* shader)
{
    static const float ColorGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f }; //
    std::vector<CVector2<float> > vec;

    CVector2<float> li_ob(-0.5f, 0.5f);
    CVector2<float> re_ob(0.5f, 0.5f);
    CVector2<float> li_un(-0.5f, -0.5f);
    CVector2<float> re_un(0.5f, -0.5f);

#define LEFT_TO_RIGHT

#ifdef LEFT_TO_RIGHT
    vec.push_back(li_un);
    vec.push_back(li_ob);
    //vec.push_back(re_ob);
    vec.push_back(re_un);
#else
    vec.push_back(re_un);
    vec.push_back(re_ob);
    vec.push_back(li_ob);
    vec.push_back(li_un);
#endif

    CLineStrip::Draw4(shader, vec, 0.05f, ColorGreen);
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode  Draw2D
//
//
//---------------------------------------------------------------------------

void CContextLissa::Draw2D()
{
    //
    //

    static float horInit;
    static float verInit;

    //cout << "Draw2D() START" << endl;
    static const float ColorWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //
    static const float ColorGrey50[] = { 0.5f, 0.5f, 0.5f, 1.0f };  //
    static const float ColorBlue[] = { 0.5f, 0.5f, 0.5f, 1.0f };    //
    static const float ColorGrey[] =
    {
        0.0f, 0.1f, 0.2f, 1.0f, // links unten
        0.0f, 0.1f, 0.2f, 1.0f, // rechts unten
        0.0f, 0.1f, 0.2f, 1.0f, // rechts oben
        0.0f, 0.1f, 0.2f, 1.0f, // links oben
    };



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    mColorShader->UseProgram();
    mColorShader->SetUniformFloat("uAspect", mAspect);
    CGL_Basic::DrawQuad(mColorShader, -1.0, -1.0f, 1.0f, 1.0f, ColorGrey);

    mBlurlineShader->UseProgram();
    mBlurlineShader->SetUniformFloat("uAspect", mAspect);
    checkGlError("SetUniformFloat");

    GlobalAspect = mAspect;


    //static float a;
    //mBlurlineShader->SetObjAngle(a);
    //mBlurlineShader->SetObjPos(p2.x, p2.y);
    //mBlurlineShader->SetObjOrigin(org.x, org.y);
    //a += 0.001f;


    //static CLineData LineData[4];
    //static CRingData RingData[4];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    TLinePoints LissaPoints;

    CalcLissa(LissaPoints, horInit, verInit, mHorSpeed, mVerSpeed);
    CLineStrip::Draw4(mBlurlineShader, LissaPoints, LineWidth, ColorWhite);
    horInit += 0.02 * mSpeed;
    verInit += 0.02 * mSpeed;

    if (horInit > M_PI*2)
    {
        horInit -= M_PI*2;
    }
    if (verInit > M_PI*2)
    {
        verInit -= M_PI*2;
    }

    //DrawTestQuad(mBlurlineShader);

    /*
     * LineData[0].Draw(mBlurlineShader, -1.0f, -1.0f,  1.0f,-1.0f, LINE_W, ColorGreen);
     * LineData[1].Draw(mBlurlineShader,  1.0f, -1.0f,  1.0f, 1.0f, LINE_W, ColorGreen);
     * LineData[2].Draw(mBlurlineShader,  1.0f,  1.0f, -1.0f, 1.0f, LINE_W, ColorGreen);
     * LineData[3].Draw(mBlurlineShader, -1.0f,  1.0f, -1.0f,-1.0f, LINE_W, ColorGreen);
     */

    glUseProgram(0);
    //cout << "Draw2D() END" << endl;
}
