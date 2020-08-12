
#include <CLineData.h>
#include <CLineStrip.h>
#include <CGL_Basic.h>
#include <iostream>

using namespace std;
extern bool GlobalPolygonMode;

void CLineStrip::Draw(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color)
{
    for (int i = 1; i < points.size(); i++)
    {
        CLineData LineData;
        LineData.Draw(shader, points[i-1].x, points[i-1].y, points[i].x, points[i].y, lw, color);
    }
}

static void AddPoints(const CVector2<float>& p0, const CVector2<float>& p1, SVertArray2D& arLeft, SVertArray2D& arRight, float lw, int orient)
{
    CVector2<float> t0(0.0f, 0.0f);
    CVector2<float> t1(1.0f, 1.0f);

    CVector2<float> c1, c2;
    CGL_Basic::CalcQuadLine(p0, p1, c1, c2, lw*2, orient);

    arLeft.AddVert(c1);
    arLeft.AddVert(p0);
    arLeft.AddText(t0);
    arLeft.AddText(t1);

    arRight.AddVert(p0);
    arRight.AddVert(c2);
    arRight.AddText(t1);
    arRight.AddText(t0);
}


static void AddPoints(const CVector2<float>& pn, const CVector2<float>& p0, const CVector2<float>& p1, SVertArray2D& arLeft, SVertArray2D& arRight, float lw, int orient)
{
    CVector2<float> t0(1.0f, 1.0f);
    CVector2<float> t1(1.0f, 1.0f);

    CVector2<float> c1, c2;
    CVector2<float> c1a, c2a;
    CVector2<float> c1b, c2b;
    CGL_Basic::CalcQuadLine(p0, pn, c1a, c2a, lw*2, orient * -1);
    CGL_Basic::CalcQuadLine(p0, p1, c1b, c2b, lw*2, orient);

    c1 = (c1a + c1b) / 2;
    c2 = (c2a + c2b) / 2;

    arLeft.AddVert(c1);
    arLeft.AddVert(p0);
    arLeft.AddText(t0);
    arLeft.AddText(t1);

    arRight.AddVert(p0);
    arRight.AddVert(c2);
    arRight.AddText(t1);
    arRight.AddText(t0);
}


static void AddPoints4(const CVector2<float>& p0, const CVector2<float>& p1, const CVector2<float>& p2, SVertArray2D& arLeft, SVertArray2D& arRight, float lw, int orient)
{
    CVector2<float> t0(0.0f, 0.0f);
    CVector2<float> t1(1.0f, 1.0f);

    CVector2<float> c1, c2;
    CGL_Basic::CalcQuadLine2(p0, p1, p2, c1, c2, lw*2, false);


    arLeft.AddVert(c1);
    arLeft.AddVert(p1);
    arLeft.AddText(t0);
    arLeft.AddText(t1);

    arRight.AddVert(p1);
    arRight.AddVert(c2);
    arRight.AddText(t1);
    arRight.AddText(t0);
}


void CLineStrip::Draw2(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color)
{
    CVector2<float> t0, t1;
    int vertCount = (points.size()+1) * 2;
    SVertArray2D  arLeft;
    SVertArray2D  arRight;

    arLeft.mCount = vertCount;
    arLeft.MakeVerts(vertCount);
    arLeft.MakeTexts(vertCount);

    arRight.mCount = vertCount;
    arRight.MakeVerts(vertCount);
    arRight.MakeTexts(vertCount);

    t0.Set(0.0f, 0.0f);
    t1.Set(1.0f, 1.0f);

    for (int i = 0; i < points.size()-1; i++)
    {
        AddPoints(points[i], points[i+1], arLeft, arRight, lw, 1);
    }
    AddPoints(points[points.size()-1], points[points.size()-2], arLeft, arRight, lw, -1);
    AddPoints(points[0], points[points.size()-1], arLeft, arRight, lw, -1);

    shader->SetAngle(0.0f);
    shader->SetPos(0.0f, 0.0f);
    shader->SetColor(color);

    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);

    CGL_Basic::DrawTristrip(shader, arLeft);
    CGL_Basic::DrawTristrip(shader, arRight);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
}



void CLineStrip::Draw3(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color)
{
    CVector2<float> t0, t1;
    int vertCount = (points.size()+1) * 2;
    SVertArray2D  arLeft;
    SVertArray2D  arRight;
    int ilast = points.size()-1;

    arLeft.mCount = vertCount;
    arLeft.MakeVerts(vertCount);
    arLeft.MakeTexts(vertCount);

    arRight.mCount = vertCount;
    arRight.MakeVerts(vertCount);
    arRight.MakeTexts(vertCount);

    t0.Set(0.0f, 0.0f);
    t1.Set(1.0f, 1.0f);


    AddPoints(points[ilast], points[0], points[1], arLeft, arRight, lw, 1);
    for (int i = 1; i < points.size()-1; i++)
    {
        AddPoints(points[i-1], points[i], points[i+1], arLeft, arRight, lw, 1);
    }
    AddPoints(points[ilast-1], points[ilast], points[0], arLeft, arRight, lw, 1);
    AddPoints(points[ilast], points[0], points[1], arLeft, arRight, lw, -1);


    shader->SetAngle(0.0f);
    shader->SetPos(0.0f, 0.0f);
    shader->SetColor(color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    CGL_Basic::DrawTristrip(shader, arLeft);
    CGL_Basic::DrawTristrip(shader, arRight);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
}


void CLineStrip::Draw4(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color)
{
    int vertCount = (points.size()+1) * 2;
    SVertArray2D  arLeft;
    SVertArray2D  arRight;
    int ilast = points.size()-1;

    arLeft.mCount = vertCount;
    arLeft.MakeVerts(vertCount);
    arLeft.MakeTexts(vertCount);

    arRight.mCount = vertCount;
    arRight.MakeVerts(vertCount);
    arRight.MakeTexts(vertCount);


    AddPoints4(points[ilast], points[0], points[1], arLeft, arRight, lw, 1);
    for (int i = 0; i < points.size()-2; i++)
    {
        AddPoints4(points[i], points[i+1], points[i+2], arLeft, arRight, lw, 1);
    }

    AddPoints4(points[ilast-1], points[ilast], points[0], arLeft, arRight, lw, 1);
    AddPoints4(points[ilast], points[0], points[1], arLeft, arRight, lw, 1);


    //AddPoints4(points[ilast-2], points[ilast-1], points[ilast], arLeft, arRight, lw, 1);
    //AddPoints4(points[ilast-1], points[ilast], points[0], arLeft, arRight, lw, 1);


    shader->SetAngle(0.0f);
    shader->SetPos(0.0f, 0.0f);
    shader->SetColor(color);

    int pmode = (GlobalPolygonMode) ? GL_LINE: GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, pmode);

    CGL_Basic::DrawTristrip(shader, arLeft);
    CGL_Basic::DrawTristrip(shader, arRight);
    glPolygonMode(GL_FRONT_AND_BACK , GL_FILL);
}




