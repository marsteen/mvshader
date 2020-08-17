//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Shader_Blurline.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <CGL_Shader_Blurline.h>

using namespace std;

extern const char* shader_blurline_frag;
extern const char* shader_blurline_vert;

//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Blurline::InitShader()
{
    CGL_Shader::InitShader(shader_blurline_vert, shader_blurline_frag);
    uAngle = GetUniformLocation("uAngle");
    uPos = GetUniformLocation("uPos");
    uColor = GetUniformLocation("uColor");

    uObjAngle = GetUniformLocation("uObjAngle");
    uObjPos = GetUniformLocation("uObjPos");
    uObjOrigin = GetUniformLocation("uObjOrigin");
    uObjScale = GetUniformLocation("uObjScale");


    cout << "uPos       =" << uPos   << endl;
    cout << "uAngle     =" << uAngle << endl;
    cout << "uColor     =" << uColor << endl;
    cout << "uObjAngle  =" << uObjAngle << endl;
    cout << "uObjPos    =" << uObjPos << endl;
    cout << "uObjOrigin =" << uObjOrigin << endl;
    cout << "uObjScale  =" << uObjScale << endl;

    UseProgram();
    SetObjScale(1.0f);
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Blurline::SetAngle(float w) const
{
    glUniform1f(uAngle, w);
}


void CGL_Shader_Blurline::SetObjAngle(float w) const
{
    glUniform1f(uObjAngle, w);
}


void CGL_Shader_Blurline::SetObjScale(float w) const
{
    glUniform1f(uObjScale, w);
}


void CGL_Shader_Blurline::SetObjPos(float x, float y) const
{
    glUniform2f(uObjPos, x, y);
}


void CGL_Shader_Blurline::SetObjOrigin(float x, float y) const
{
    glUniform2f(uObjOrigin, x, y);
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Blurline::SetPos(float x, float y) const
{
    glUniform2f(uPos, x, y);
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Blurline::SetColor(float r, float g, float b, float a) const
{
    glUniform4f(uColor, r, g, b, a);
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Blurline::SetColor(const float* color) const
{
    glUniform4fv(uColor, 1, color);
}
