//***************************************************************************
//
//
// @PROJECT  :  Basic SDL
// @VERSION  :  1.0
// @FILENAME :  CGL_Basic.cpp
// @DATE     :  8.1.2015
//
// @AUTHOR   :  Martin Steen
// @EMAIL    :  martin@martin-steen.de
//
//
//***************************************************************************

#include <iostream>
#include <NGlobalLog.h>
#include <GLinclude.h>
#include <SVertArray.h>
#include <CGL_Basic.h>
#include <CVector2T.h>
#include <CVector3T.h>
#include <CGL_Shader.h>
#include <cmath>

extern void checkGlError(const char* func);

using namespace std;
using namespace NGlobalLog;

// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuad(const CGL_Shader* glcon, float x1, float y1, float x2, float y2)
{
    SVertArray ar;
    CVector3<float> v;

    ar.MakeVerts(4);

    v.Set(x1, y1, 0);
    ar.AddVert(v);

    v.Set(x2, y1, 0);
    ar.AddVert(v);

    v.Set(x2, y2, 0);
    ar.AddVert(v);

    v.Set(x1, y2, 0);
    ar.AddVert(v);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glEnableVertexAttribArray(glcon->VertAttrib());


    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());

    checkGlError("DrawQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad

//  const float* Colors: Zeiger auf 4*4 floats
//  vier Farbewerte fuer die vier Ecken des Rechtecks
//
//  Die ersten beiden Farbwerte sind den UNTEREN Ecken zugeordnet
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuad(const CGL_Shader* glcon, float x1, float y1, float x2, float y2, const float* Colors)
{
    SVertArray ar;
    CVector3<float> v;

    ar.MakeVerts(4);

    v.Set(x1, y1, 0);
    ar.AddVert(v);

    v.Set(x2, y1, 0);
    ar.AddVert(v);

    v.Set(x2, y2, 0);
    ar.AddVert(v);

    v.Set(x1, y2, 0);
    ar.AddVert(v);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->ColorAttrib(), 4, GL_FLOAT, GL_FALSE, 0, Colors);

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->ColorAttrib());


    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->ColorAttrib());

    //checkGlError("DrawQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuadTexCoords(const CGL_Shader* glcon, float vx1, float vy1, float vx2, float vy2, const float* textcoords)
{
    SVertArray ar;
    CVector3<float> v;


    ar.MakeVerts(4);
    ar.MakeTexts(4);

    v.Set(vx1, vy1, 0);
    ar.AddVert(v);

    v.Set(vx2, vy1, 0);
    ar.AddVert(v);

    v.Set(vx2, vy2, 0);
    ar.AddVert(v);

    v.Set(vx1, vy2, 0);
    ar.AddVert(v);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, textcoords);

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());


    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    //checkGlError("DrawQuad");
}


void CGL_Basic::DrawQuadTexCoords(const CGL_Shader* glcon, const SVertArray2D& ar)
{
    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    //checkGlError("DrawQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : CreateQuadTexCoords

//
// ---------------------------------------------------------------------------

void CGL_Basic::CreateQuadTexCoords(float vx1, float vy1, float vx2, float vy2, const float* textcoords, SVertArray2D& ar)
{
    CVector2<float> v;
    CVector2<float> t;


    ar.MakeVerts(4);
    ar.MakeTexts(4);

    v.Set(vx1, vy1);
    ar.AddVert(v);
    t.Set(textcoords[0], textcoords[1]);
    ar.AddText(t);

    v.Set(vx2, vy1);
    ar.AddVert(v);
    t.Set(textcoords[2], textcoords[3]);
    ar.AddText(t);


    v.Set(vx2, vy2);
    ar.AddVert(v);
    t.Set(textcoords[4], textcoords[5]);
    ar.AddText(t);


    v.Set(vx1, vy2);
    ar.AddVert(v);
    t.Set(textcoords[6], textcoords[7]);
    ar.AddText(t);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad

//  const float* Colors: Zeiger auf 16 floats
//  vier Farbewerte fuer die vier Ecken des Rechtecks
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawQuad(const CGL_Shader* glcon, const CRectT<float>& vr, const float* Colors)
{
    DrawQuad(glcon, vr.left, vr.top, vr.right, vr.bottom, Colors);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawQuad
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawQuad(const CGL_Shader* glcon, const CRectT<float>& vr)
{
    DrawQuad(glcon, vr.left, vr.top, vr.right, vr.bottom);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawPolygon
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawPolygon(const CGL_Shader* glcon, const CPolygon* Polygon, float sx, float sy, float& TexOffset)
{
    static bool sFirst = true;
    SVertArray ar;
    CVector3<float> v;
    CVector2<float> t;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ar.MakeVerts(Polygon->Size());
    ar.MakeTexts(Polygon->Size());

    for (int i = 0; i < Polygon->Size(); i++)
    {
        float px = Polygon->ix(i) * sx;
        float py = Polygon->iy(i) * sy;
        float tx = Polygon->it(i);

        if (sFirst)
        {
            //gdstr << "px=" << px << " py=" << py;
            //gdlog();
        }

        v.Set(px, py, 0);

        if (i & 1)
        {
            t.Set(1.0f, TexOffset);
        }
        else
        {
            t.Set(0.0f, TexOffset);
        }


        ar.AddVert(v);
        ar.AddText(t);
    }

    /*
     * TexOffset += 0.002;
     * if (TexOffset >= 1.0)
     * {
     * TexOffset -= 1.0;
     * }
     */


    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());


    glDrawArrays(GL_LINE_STRIP, 0, Polygon->Size());

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    glDisable(GL_BLEND);

    checkGlError("DrawPolygon");
    sFirst = false;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawLine
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawLine(const CGL_Shader* glcon, float x1, float y1, float x2, float y2)
{
    SVertArray ar;
    CVector3<float> v;

    ar.MakeVerts(2);

    v.Set(x1, y1, 0);
    ar.AddVert(v);
    v.Set(x2, y2, 0);
    ar.AddVert(v);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glEnableVertexAttribArray(glcon->VertAttrib());
    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(glcon->VertAttrib());

    checkGlError("DrawLine");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawLines
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawLines(const CGL_Shader* glcon, const std::vector<CVector2<float> >& LineVec, bool Closed)
{
    SVertArray2D ar;
    CVector2<float> v;

    ar.MakeVerts(LineVec.size());
    for (int i = 0; i < LineVec.size(); i++)
    {
        ar.AddVert(LineVec[i]);
    }

    int Mode = (Closed) ? GL_LINE_LOOP : GL_LINE_STRIP;

    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glEnableVertexAttribArray(glcon->VertAttrib());
    glDrawArrays(Mode, 0, LineVec.size());
    glDisableVertexAttribArray(glcon->VertAttrib());

    checkGlError("DrawLine");
}


// -------------------- -------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : CreateLines
//
// ---------------------------------------------------------------------------

static const SVertArray2D* CreateLines(const CGL_Shader* glcon, const std::vector<CVector2<float> >& LineVec)
{
    SVertArray2D* ar;
    CVector2<float> v;

    ar->mCount = LineVec.size();
    ar->MakeVerts(LineVec.size());
    for (int i = 0; i < LineVec.size(); i++)
    {
        ar->AddVert(LineVec[i]);
    }
    return ar;
}


// -------------------- -------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawLines
//
// ---------------------------------------------------------------------------

static void DrawLines(const CGL_Shader* glcon, const SVertArray2D* ar, bool Closed = false)
{
    int Mode = (Closed) ? GL_LINE_LOOP : GL_LINE_STRIP;

    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar->mVert->v());
    glEnableVertexAttribArray(glcon->VertAttrib());
    glDrawArrays(Mode, 0, ar->mCount);
    glDisableVertexAttribArray(glcon->VertAttrib());

    checkGlError("DrawLine");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawFrame
//
// ---------------------------------------------------------------------------


void CGL_Basic::DrawFrame(const CGL_Shader* glcon, const CRectT<float>& rc)
{
    SVertArray ar;
    CVector3<float> v;

    ar.MakeVerts(4);

    v.Set(rc.left, rc.top, 0);
    ar.AddVert(v);
    v.Set(rc.right, rc.top, 0);
    ar.AddVert(v);
    v.Set(rc.right, rc.bottom, 0);
    ar.AddVert(v);
    v.Set(rc.left, rc.bottom, 0);
    ar.AddVert(v);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());

    glEnableVertexAttribArray(glcon->VertAttrib());


    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());

    checkGlError("DrawFrame");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuad(const CGL_Shader* glcon, float x1, float y1, float x2, float y2)
{
    SVertArray ar;
    CVector3<float> v;
    CVector2<float> t;

    ar.MakeVerts(4);
    ar.MakeTexts(4);

    v.Set(x1, y1, 0);
    t.Set(0.0f, 0.0f);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(x2, y1, 0);
    t.Set(1.0f, 0.0f);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(x2, y2, 0);
    t.Set(1.0f, 1.0f);
    ar.AddVert(v);
    ar.AddText(t);

    v.Set(x1, y2, 0);
    t.Set(0.0f, 1.0f);
    ar.AddVert(v);
    ar.AddText(t);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    //checkGlError("DrawTextureQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuadUpsideDown
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuadUpsideDown(const CGL_Shader* glcon, float x1, float y1, float x2, float y2)
{
    SVertArray ar;
    CVector3<float> v;
    CVector2<float> t;

    ar.MakeVerts(4);
    ar.MakeTexts(4);

    v.Set(x1, y1, 0);
    t.Set(0.0f, 1.0f);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(x2, y1, 0);
    t.Set(1.0f, 1.0f);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(x2, y2, 0);
    t.Set(1.0f, 0.0f);
    ar.AddVert(v);
    ar.AddText(t);

    v.Set(x1, y2, 0);
    t.Set(0.0f, 0.0f);
    ar.AddVert(v);
    ar.AddText(t);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    //checkGlError("DrawTextureQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuad(const CGL_Shader* glcon, const std::vector<CVector2<float> >& verts, const std::vector<CVector2<float> >& texts)
{
    SVertArray2D ar;

    ar.MakeVerts(4);
    ar.MakeTexts(4);

    for (int i = 0; i < 4; i++)
    {
        ar.AddText(texts[i]);
        ar.AddVert(verts[i]);
    }

    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, verts.size());


    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTrifan
//
// Trifan mit Textur-Koordinaten erzeugen
//
// Der 1. Punkt (Mittelpunkt) erhält die Texturkoordinate (1.0, 1.0), alle
// weiteren Punkte haben die Koordinate (0.0, 0.0)
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTrifan(const CGL_Shader* glcon, const std::vector<CVector2<float> >& verts)
{
    SVertArray2D ar;
    CVector2<float> t;


    t.Set(1.0, 1.0);

    ar.MakeVerts(verts.size() * 2);
    ar.MakeTexts(verts.size() * 2);

    ar.AddText(t);
    for (int i = 0; i < verts.size(); i++)
    {
        ar.AddVert(verts[i]);
        if (i > 0)
        {
            t.Set(0.0, 0.0);
            ar.AddText(t);
        }
    }
    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, verts.size());

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    //checkGlError("DrawTextureQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : CreateRingTriStrip
//
//
// ---------------------------------------------------------------------------

void CGL_Basic::CreateRingTriStrip(float InnerRadius, float OuterRadius, int Segments, float InnerTexKoor, float OuterTexKoor, SVertArray2D& ar)
{
    CVector2<float> CirclePoint;
    CVector2<float> InnerTexKoorVec;
    CVector2<float> OuterTexKoorVec;

    float Alpha = 0;
    float AlphaPlus = DEG_TO_RAD(360.0f / Segments);

    //glBegin(GL_TRIANGLE_STRIP);

    int VertCount = (Segments + 1) * 2;

    ar.mCount = VertCount;
    ar.MakeVerts(VertCount);
    ar.MakeTexts(VertCount);

    OuterTexKoorVec.Set(OuterTexKoor, 0.0f);
    InnerTexKoorVec.Set(InnerTexKoor, 0.0f);

    //cout << "Segments=" << Segments << endl;

    for (int i = 0; i <= Segments; i++)
    {
        CirclePoint.x = cos(Alpha) * OuterRadius;
        CirclePoint.y = sin(Alpha) * OuterRadius;

        ar.AddVert(CirclePoint);
        ar.AddText(InnerTexKoorVec);


        CirclePoint.x = cos(Alpha) * InnerRadius;
        CirclePoint.y = sin(Alpha) * InnerRadius;

        ar.AddVert(CirclePoint);
        ar.AddText(OuterTexKoorVec);

        Alpha += AlphaPlus;
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : CreateTrifan
//
// Trifan mit Textur-Koordinaten erzeugen
//
// Der 1. Punkt (Mittelpunkt) erhält die Texturkoordinate (1.0, 1.0), alle
// weiteren Punkte haben die Koordinate (0.0, 0.0)
//
// ---------------------------------------------------------------------------

void CGL_Basic::CreateTrifan(const std::vector<CVector2<float> >& verts, SVertArray2D& ar)
{
    //SVertArray2D* ar = new SVertArray2D;
    CVector2<float> t;

    t.Set(1.0, 1.0);

    ar.mCount = verts.size();
    ar.MakeVerts(verts.size() * 2);
    ar.MakeTexts(verts.size() * 2);

    ar.AddText(t);
    for (int i = 0; i < verts.size(); i++)
    {
        ar.AddVert(verts[i]);

        if (i > 0)
        {
            t.Set(0.0, 0.0);
            ar.AddText(t);
        }
    }
}


/*
 *
 * // WORKS! 270 Grad fuer alle winkel
 * static float CalcVectorAngle(
 *  const CVector2<float>& v20,
 *  const CVector2<float>& v2a,
 *  const CVector2<float>& v2b)
 * {
 *  CVector2<float> v2diff1  = v20 - v2a;
 *  CVector2<float> v2diff2  = v2a - v2b;
 *  float r = v2diff2.GetAngleSkalarRad(&v2diff1);
 *  return r + DEG_TO_RAD(180.0f);
 * }
 *
 */


/*
 * // WORKS! 270 Grad fuer alle winkel
 * static float CalcVectorAngle(
 *  const CVector2<float>& v20,
 *  const CVector2<float>& v2a,
 *  const CVector2<float>& v2b)
 * {
 *  CVector2<float> v2diff1  = v20 - v2a;
 *  CVector2<float> v2diff2  = v2a - v2b;
 *  float r = v2diff2.GetAngleRadAtan2(&v2diff1) + DEG_TO_RAD(180.0f);
 *
 *  if (r < 0.0)                r += DEG_TO_RAD(360.0);
 *  if (r > DEG_TO_RAD(360.0))  r -= DEG_TO_RAD(360.0);
 *  return r;
 * }
 */
static void ShowAngle(const char* txt, float va)
{
    static int b;

    if (b < 10)
    {
        cout << txt << "=" << RAD_TO_DEG(va) << endl;
        b++;
    }
}


#define RAD_90		(M_PI/2.0)
#define RAD_180		(M_PI)
#define RAD_360		(M_PI*2)


static void AlignAngle(float& va)
{
    if (va < 0.0)
    {
        va += RAD_360;
    }
    if (va > RAD_360)
    {
        va -= RAD_360;
    }
}


#define ATAN2_STACK_SIZE    3
inline float GetAtan2(const CVector2<float>& v)
{
    static int b = 0;
    static CVector2<float> vstack[ATAN2_STACK_SIZE];
    static float tstack[ATAN2_STACK_SIZE];

    for (int i = ATAN2_STACK_SIZE-1; i >= 0; i--)
    {
        if (v == vstack[i])
        {
            if (b < 1000)
            {
                cout << "found atan2!:" << i << endl;
                b++;
            }
            return tstack[i];
        }
    }

    for (int i = 1; i < ATAN2_STACK_SIZE; i++)
    {
        vstack[i-1] = vstack[i];
        tstack[i-1] = tstack[i];
    }
    vstack[ATAN2_STACK_SIZE-1] = v;
    tstack[ATAN2_STACK_SIZE-1] = atan2(v.y, v.x);
    return tstack[ATAN2_STACK_SIZE-1];
}


static inline float CalcLineAngle(
    const CVector2<float>& v2a,
    const CVector2<float>& v2b)
{
    CVector2<float> v2diff = v2a - v2b;

    return atan2(v2diff.y, v2diff.x);
}


static float CalcVectorAngle(
    const CVector2<float>& v20,
    const CVector2<float>& v2a,
    const CVector2<float>& v2b,
    float& vac,
    bool useCorrection
    )
{
    float la1 = CalcLineAngle(v2a, v2b);
    float la2 = CalcLineAngle(v20, v2a);

    //AlignAngle(la1);
    //AlignAngle(la2);

    float va = la2 - la1;

    //if (va < 0.0)               va += DEG_TO_RAD(360.0f);
    //if (va > DEG_TO_RAD(180.0)) va -= DEG_TO_RAD(360.0f);

    if (va < 0.0)
    {
        va += RAD_360;
    }
    if (va > RAD_180)
    {
        va -= RAD_360;
    }

    float r = la1 - RAD_90 + (va * 0.5f);

    // vac: korrekturwert, damit die Linie immer dieselbe Dicke hat
    float vac1 = cos(va * 0.5f);

    if (useCorrection)
    {
        if (fabs(vac1) > 0.2f) // hoechstens 5 fach
        {
            vac = 1.0f / vac1;
        }
        else
        {
            vac = 1.0f;
        }
    }


    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : CalcQuadLine2
//
//
// ---------------------------------------------------------------------------

void CGL_Basic::CalcQuadLine2(
    const CVector2<float>& v20,
    const CVector2<float>& v2a,
    const CVector2<float>& v2b,
    CVector2<float>& v2c1,
    CVector2<float>& v2c2,
    float linewidth,
    bool useCorrection)
{
    float vac;
    float LineAngle = CalcVectorAngle(v20, v2a, v2b, vac, useCorrection);
    float xoff = linewidth / 2;

    if (useCorrection)
    {
        xoff *= vac;
    }

    v2c1.Set(v2a.x + xoff, v2a.y);
    v2c2.Set(v2a.x - xoff, v2a.y);

    const float sinAlpha = sin(LineAngle);
    const float cosAlpha = cos(LineAngle);

    v2c1.RotationXY(&v2a, sinAlpha, cosAlpha);
    v2c2.RotationXY(&v2a, sinAlpha, cosAlpha);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : CalcQuadLine
//
//
// ---------------------------------------------------------------------------

void CGL_Basic::CalcQuadLine(
    const CVector2<float>& v2a,
    const CVector2<float>& v2b,
    CVector2<float>& v2c1,
    CVector2<float>& v2c2,
    float linewidth,
    int orient)
{
    CVector2<float> v2diff = v2a - v2b;
    float LineAngle = atan2(v2diff.y, v2diff.x) - DEG_TO_RAD(90.0f) * orient;
    float xoff = linewidth / 2;

    v2c1.Set(v2a.x + xoff, v2a.y);
    v2c2.Set(v2a.x - xoff, v2a.y);

    const float sinAlpha = sin(LineAngle);
    const float cosAlpha = cos(LineAngle);

    v2c1.RotationXY(&v2a, sinAlpha, cosAlpha);
    v2c2.RotationXY(&v2a, sinAlpha, cosAlpha);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTrifan
//
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTrifan(const CGL_Shader* glcon, const SVertArray2D& ar)
{
    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    //glEnableVertexAttribArray(glcon->VertAttrib());
    //glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, ar.mCount);

    //glDisableVertexAttribArray(glcon->VertAttrib());
    //glDisableVertexAttribArray(glcon->TextAttrib());
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTristrip
//
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTristrip(const CGL_Shader* glcon, const SVertArray2D& ar)
{
    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, ar.mCount);

//    cout << "ar.mCount=" << ar.mCount << endl;

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTrifan
//
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTrifanColors(const CGL_Shader* glcon, const SVertArray2D* ar, const float* Colors)
{
    glVertexAttribPointer(glcon->VertAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar->mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar->mText->v());
    glVertexAttribPointer(glcon->ColorAttrib(), 4, GL_FLOAT, GL_FALSE, 0, Colors);

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());
    glEnableVertexAttribArray(glcon->ColorAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, ar->mCount);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());
    glDisableVertexAttribArray(glcon->ColorAttrib());
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuadRotated
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuadRotated(const CGL_Shader* glcon, float x1, float y1, float x2, float y2)
{
    SVertArray ar;
    CVector3<float> v;
    CVector2<float> t;

    float w = x2 - x1;
    float h = y2 - y1;


    x2 = x1 + h;
    y2 = y1 + w;


    ar.MakeVerts(4);
    ar.MakeTexts(4);

    v.Set(x1, y1, 0);
    t.Set(0.0f, 1.0f);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(x2, y1, 0);
    t.Set(0.0f, 0.0f);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(x2, y2, 0);
    t.Set(1.0f, 0.0f);
    ar.AddVert(v);
    ar.AddText(t);

    v.Set(x1, y2, 0);
    t.Set(1.0f, 1.0f);
    ar.AddVert(v);
    ar.AddText(t);

    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    //checkGlError("DrawTextureQuad");
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CGL_Basic
// METHODE       : DrawTextureQuad
//
// ---------------------------------------------------------------------------

void CGL_Basic::DrawTextureQuad(const CGL_Shader* glcon, const CRectT<float>& vr, const CRectT<float>& tr)
{
    SVertArray ar;
    CVector3<float> v;
    CVector2<float> t;

    ar.MakeVerts(4);
    ar.MakeTexts(4);

    v.Set(vr.left, vr.top, 0);
    t.Set(tr.left, tr.top);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(vr.right, vr.top, 0);
    t.Set(tr.right, tr.top);
    ar.AddVert(v);
    ar.AddText(t);


    v.Set(vr.right, vr.bottom, 0);
    t.Set(tr.right, tr.bottom);
    ar.AddVert(v);
    ar.AddText(t);

    v.Set(vr.left, vr.bottom, 0);
    t.Set(tr.left, tr.bottom);
    ar.AddVert(v);
    ar.AddText(t);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glVertexAttribPointer(glcon->VertAttrib(), 3, GL_FLOAT, GL_FALSE, 0, ar.mVert->v());
    glVertexAttribPointer(glcon->TextAttrib(), 2, GL_FLOAT, GL_FALSE, 0, ar.mText->v());

    glEnableVertexAttribArray(glcon->VertAttrib());
    glEnableVertexAttribArray(glcon->TextAttrib());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(glcon->VertAttrib());
    glDisableVertexAttribArray(glcon->TextAttrib());

    glDisable(GL_BLEND);

    // checkGlError("DrawTextureQuad");
}
