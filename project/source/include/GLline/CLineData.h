#ifndef CLineData_H
#define CLineData_H

#include <cstdio>
#include <CVector2T.h>
#include <SVertArray.h>
#include <CGL_Shader_Blurline.h>


class CLineData
{
public:

    CLineData()
    {
        mInit = false;
    }


    void Draw(
        CGL_Shader_Blurline* shader,
        float x1, float y1,
        float x2, float y2,
        float LineWidth,
        const float* color);

protected:

    void DrawLineData(CGL_Shader_Blurline* shader);

    bool mInit;
    SVertArray2D mAr[4];

    CVector2<float> mP1;
    CVector2<float> mP2;

    float mLineAngle;
    const float* mColor;
};

#endif
