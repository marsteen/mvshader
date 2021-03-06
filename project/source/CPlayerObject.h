#ifndef CPlayObject_H
#define CPlayObject_H

#include <CPointObject.h>
#include <CLineData.h>

class CPlayObject : public CLineData
{
public:

    CPlayObject()
    {
        mPos.x = 0.0f;
        mPos.y = 0.0f;
        mVisible = true;
    }


    bool Intersect(CPointObject* ball) const;

    void Draw(CGL_Shader_Blurline* shader,
        float x1, float y1,
        float x2, float y2,
        float LineWidth,
        const float* color);

    void AddPos(float xrel, float yrel);

    bool mVisible;

protected:


    CVector2<float> mPos;
    CRectT<float> mRect;
};

#endif
