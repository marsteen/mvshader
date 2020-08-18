#ifndef CGL_Shader_Sphere_H
#define CGL_Shader_Sphere_H

#include <CGL_Shader.h>

class CGL_Shader_Sphere : public CGL_Shader
{
public:

    //void InitShader();
    void InitShader(
        const std::string& vertShaderFile,
        const std::string& fragShaderFile);
};

#endif
