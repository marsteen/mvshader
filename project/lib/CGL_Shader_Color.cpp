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


#include <CGL_Shader_Color.h>


extern const char* shader_color_frag;
extern const char* shader_color_vert;


void CGL_Shader_Color::InitShader()
{
  CGL_Shader::InitShader(shader_color_vert, shader_color_frag);
};
