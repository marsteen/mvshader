#ifndef CLineStrip_H
#define CLineStrip_H

#include <vector>
#include <CVector2T.h>
#include <CGL_Shader_Blurline.h>

typedef std::vector<CVector2<float> > TLinePoints;

class CLineStrip
{
    public:

        static void Draw(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color);
        static void Draw2(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color);
        static void Draw3(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color);
        static void Draw4(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color);
};

#endif
