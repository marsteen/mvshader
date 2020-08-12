#ifndef CGL_Shader_Blurline_H
#define CGL_Shader_Blurline_H

#include <CGL_Shader.h>

class CGL_Shader_Blurline : public CGL_Shader
{
    public:

        void InitShader();
        void SetAngle(float w) const;
        void SetPos(float x, float y) const;
        void SetColor(float r, float g, float b, float a) const;
        void SetColor(const float* color) const;

        void SetObjPos(float x, float y) const;
        void SetObjOrigin(float x, float y) const;
        void SetObjAngle(float w) const;
        void SetObjScale(float w) const;

    protected:

        unsigned int uAngle;
        unsigned int uObjAngle;
        unsigned int uObjScale;
        unsigned int uObjPos;
        unsigned int uObjOrigin;
        unsigned int uPos;
        unsigned int uColor;
};

#endif
