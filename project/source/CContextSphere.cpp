//***************************************************************************
//
//
// @PROJECT  :  Basic SDL
// @VERSION  :  1.0
// @FILENAME :  CContextSphere.cpp
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
#include <MathDefines.h>
#include <CRandom.h>
#include <CFrameBuffer.h>
#include "CContextSphere.h"
#include <NStringTool.h>


using namespace std;
using namespace NGlobalLog;

static float mouseXabs;
static float mouseYabs;
static float mouseZpos = 4.0f;
static float iTime;
static float uScale = 1.0f;

float GlobalAspect = 1.0;

extern void checkGlError(const char* func);

CGL_Context* CGL_Context::CreateContext()
{
    return new CContextSphere;
}


CContextSphere::CContextSphere() : CGL_Context()
{
    mFrameBuffer = nullptr;
    mUseFrameBuffer = false;
    mAktShaderNr = 0;
}


void CContextSphere::KeyPress(int key, bool down)
{
    float w = 1.0f;

    if (down)
    {
        cout << "key=" << key << endl;

        if (mFrameBuffer != nullptr)
        {
            mFrameBuffer->ClearBuffer();
        }

        switch (key)
        {
            case 'a':

                break;

            case '1':

                uScale = 1.0f;
                break;

            case '2':

                uScale = 0.75f;
                break;

            case '3':

                uScale = 0.5f;
                break;

            case '4':

                uScale = 0.25f;
                break;

            case ' ':
            {
                iTime = 0;
                if (++mAktShaderNr >= mFragShaderFileVec.size())
                {
                    mAktShaderNr = 0;
                }
                const std::string& newShaderFile = mFragShaderFileVec[mAktShaderNr];
                cout << "new shader:" << newShaderFile << endl;
                mSphereShader->InitShader(mVertShaderFile, newShaderFile);
            }
            break;

        }
    }
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  ParseParams
//
//
//---------------------------------------------------------------------------

void CContextSphere::ParseParams(const std::vector<std::string>* cmdlineparams)
{
    cout << "ParseParams START" << endl;
    mVertShaderFile = "../shaders/vert/00.glsl";

    if (cmdlineparams != nullptr)
    {
        for (int i = 0; i < cmdlineparams->size(); i++)
        {
            const std::string& arg = (*cmdlineparams)[i];
            if (arg == "-vshader")
            {
                if (i < cmdlineparams->size()-1)
                {
                    mVertShaderFile = (*cmdlineparams)[i+1];
                    cout << "vertex shaderfile=" << mVertShaderFile << endl;
                }
            }
            else
            if (arg == "-fshader")
            {
                if (i < cmdlineparams->size()-1)
                {
                    mFragShaderFileVec.push_back((*cmdlineparams)[i+1]);
                    cout << "fragment shaderfile=" << (*cmdlineparams)[i+1] << endl;
                }
            }
            else
            if (arg == "-fb")
            {
                if (i < cmdlineparams->size()-2)
                {
                    mUseFrameBuffer = true;
                    mFbResW = NStringTool::Cast<int>((*cmdlineparams)[i+1]);
                    mFbResH = NStringTool::Cast<int>((*cmdlineparams)[i+2]);
                    cout << "Framebuffer width:  " << mFbResW << endl;
                    cout << "            height: " << mFbResH << endl;
                    i += 2;
                }
            }

        }
    }
    if (mFragShaderFileVec.size() == 0)
    {
        mFragShaderFileVec.push_back("../shaders/frag/00.glsl");
    }
    else
    {
       cout << "fragment shaders:" << mFragShaderFileVec.size() << endl;
    }
    cout << "ParseParams END" << endl;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  Init
//
//
//---------------------------------------------------------------------------


bool CContextSphere::Init(int w, int h, const std::vector<std::string>* cmdlineparams)
{
    CGL_Context::Init(w, h, cmdlineparams);
    ParseParams(cmdlineparams);

    mSphereShader = new CGL_Shader_Sphere;
    mSphereShader->InitShader(mVertShaderFile, mFragShaderFileVec[0]);


    mTexturShader = new CGL_Shader_Sphere;
    mTexturShader->InitShader("./shaders/vert/00.glsl", "./shaders/frag/texture-fragshader.glsl");


    //cout << "Width=" << mWidth << " Height=" << mHeight << endl;

    return true;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  MouseMove
//
//
//---------------------------------------------------------------------------

void CContextSphere::MouseMove(int xrel, int yrel, bool lb, bool rb)
{
    //cout << "xrel=" << xrel << " yrel=" << yrel << endl;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  MouseMoveAbs
//
//
//---------------------------------------------------------------------------

void CContextSphere::MouseMoveAbs(int xabs, int yabs, bool lb, bool rb)
{
    //cout << "xabs=" << xabs << " yabs=" << yabs << endl;

    mouseXabs = xabs;
    mouseYabs = mHeight - yabs;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  MouseWheel
//
//
//---------------------------------------------------------------------------

void CContextSphere::MouseWheel(bool up)
{
    if (up)
    {
        mouseZpos *= 1.1f;
    }
    else
    {
        mouseZpos /= 1.1f;
    }
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  Timer
//
//
//---------------------------------------------------------------------------

void CContextSphere::Timer()
{
    iTime += 0.01f;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextSphere
// Methode  Draw2D
//
//
//---------------------------------------------------------------------------

void CContextSphere::Draw2D()
{
     //cout << "Draw2D() START" << endl;
    const float red[] = { 1.0f, 0.0f, 0.0f, 1.0f };


    mSphereShader->UseProgram();
//    mSphereShader->SetUniformFloat("uAspect", mAspect);
    iTime += 1.0f / 30.0;

    const float vec4[] = { mouseXabs, mouseYabs, mouseZpos, 0.0 };

    mSphereShader->SetUniformVec4("iMouse", vec4);
    checkGlError("SetUniformVec4");

    mSphereShader->SetUniformFloat("iTime", iTime);
    checkGlError("SetUniformFloat");

    mSphereShader->SetUniformFloat("uScale", uScale);
    checkGlError("SetUniformFloat");

    mSphereShader->SetUniformVec3("iResolution", mWidth, mHeight, 0.0f);
    checkGlError("SetUniformVec3");

    if (!mUseFrameBuffer)
    {
        CGL_Basic::DrawTextureQuad(mSphereShader, -1.0, -1.0f, 1.0f, 1.0f);
    }
    else
    {
        if (mFrameBuffer == nullptr)
        {
            mFrameBuffer = new CFrameBuffer;
            mFrameBuffer->CreateFramebufferObject(mFbResW, mFbResH);
        }

        mFrameBuffer->DrawToFrameBuffer(true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mFrameBuffer->SetViewport();



        CGL_Basic::DrawTextureQuad(mSphereShader, -1.0, -1.0f, 1.0f, 1.0f);
        checkGlError("DrawTextureQuad");
        mFrameBuffer->DrawToFrameBuffer(false);
        mFrameBuffer->RestoreViewport();


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        mTexturShader->UseProgram();
        mTexturShader->SetUniformFloat("uScale", 1.0);
        mFrameBuffer->BindTexture();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    // GL_NEAREST oder GL_LINEAR
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);    // GL_NEAREST
        CGL_Basic::DrawTextureQuad(mTexturShader, -1.0, -1.0f, 1.0f, 1.0f);
        glDisable(GL_BLEND);
    }

    checkGlError("DrawTextureQuad");
    glUseProgram(0);
    //


    //cout << "Draw2D() END" << endl;
}
