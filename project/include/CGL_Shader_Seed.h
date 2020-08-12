#ifndef CGL_Shader_Noise_H
#define CGL_Shader_Noise_H

#include <CGL_Shader.h>

class CGL_Shader_Noise : public CGL_Shader
{
    public:

        void InitShader();
        void SetObjSeed(float w) const;

    protected:

        unsigned int uObjSeed;
};

#endif
