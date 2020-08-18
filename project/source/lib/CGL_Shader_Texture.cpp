//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Shader_Texture.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <CGL_Shader_Texture.h>


extern const char* shader_texture_frag;
extern const char* shader_texture_vert;

using namespace std;

void CGL_Shader_Texture::InitShader()
{
    cout << "CGL_Shader_Texture::InitShader START" << endl;
    mName = "Texture Shader";

    CGL_Shader::InitShader(shader_texture_vert, shader_texture_frag);
    uTexture = GetUniformLocation("uTexture");


    cout << "CGL_Shader_Texture::InitShader OK" << endl;
}


void CGL_Shader_Texture::UseTexture() const
{
    glUniform1i(uTexture, 0);
}
