//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGL_Shader_Color.cpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#include <CGL_Shader_Sphere.h>


extern const char* shader_sphere_vert;
extern const char* shader_sphere_frag;


void CGL_Shader_Sphere::InitShader()
{
    CGL_Shader::InitShader(shader_sphere_vert, shader_sphere_frag);
};


void CGL_Shader_Sphere::InitShader(
    const std::string& vertShaderFile,
    const std::string& fragShaderFile)
{
    std::string vertShader = LoadShaderFile(vertShaderFile);
    std::string fragShader = LoadShaderFile(fragShaderFile);
    CGL_Shader::InitShader(vertShader.c_str(), fragShader.c_str());
};
