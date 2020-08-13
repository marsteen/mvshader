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
#include <CContextSphere.h>
#include <MathDefines.h>
#include <CRandom.h>

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
}

void CContextSphere::KeyPress(int key, bool down)
{
    float w = 1.0f;
    if (down)
    {
        //cout << "key=" << key << endl;
        switch (key)
        {
            case 'a':

                break;
                
            case '1':

                uScale = 1.0f;
                break;

            case '2':

                uScale = 0.5f;
                break;

            case '3':

                uScale = 0.25f;
                break;

            case '4':

                uScale = 0.125f;
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
    mVertShaderFile = "../shaders/vert/00.glsl";
    mFragShaderFile = "../shaders/frag/00.glsl";
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
                    mFragShaderFile = (*cmdlineparams)[i+1];
                    cout << "fragment shaderfile=" << mFragShaderFile << endl;
                }
            }
            
        }
    }
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

    mSphereShader->InitShader(mVertShaderFile, mFragShaderFile);

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

void CContextSphere::MouseWheel(bool up)
{
    if (up) mouseZpos *= 1.1f;
    else    mouseZpos /= 1.1f;

}

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

    const float red[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSphereShader->UseProgram();
//    mSphereShader->SetUniformFloat("uAspect", mAspect);

    iTime += 1.0f / 30.0;

    const float vec4[] = { mouseXabs, mouseYabs, mouseZpos, 0.0 };
    
    mSphereShader->SetUniformVec4("iMouse", vec4);
    mSphereShader->SetUniformFloat("iTime", iTime);
    mSphereShader->SetUniformFloat("uScale", uScale);
    mSphereShader->SetUniformVec3("iResolution", mWidth, mHeight, 0.0f);

    CGL_Basic::DrawTextureQuad(mSphereShader, -1.0, -1.0f, 1.0f, 1.0f);


    glUseProgram(0);
    //cout << "Draw2D() END" << endl;
}





