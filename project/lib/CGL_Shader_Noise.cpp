//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Shader_Noise.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <CGL_Shader_Noise.h>

using namespace std;

extern const char* shader_noise_frag;
extern const char* shader_noise_vert;


CGL_Shader_Noise::CGL_Shader_Noise()
{
    mName = "Noise Shader";
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Noise::InitShader()
{
    CGL_Shader::InitShader(shader_noise_vert, shader_noise_frag);

    uObjSeed = GetUniformLocation("uSeed");


    cout << "uObjSeed  =" << uObjSeed << endl;

    UseProgram();
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode
//
//
//---------------------------------------------------------------------------

void CGL_Shader_Noise::SetSeed(float w) const
{
    glUniform1f(uObjSeed, w);
}
