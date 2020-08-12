#ifndef CGL_Basic_H
#define CGL_Basic_H

#include <GLinclude.h>
#include <CGL_Shader.h>
#include <CPolygon.h>
#include <CRectT.h>
#include <CVector2T.h>
#include <SVertArray.h>

class CGL_Basic
{
    public:

        // Flaechen:

        static void DrawQuad(const CGL_Shader* glcon, float x1, float y1, float x2, float y2);
        static void DrawQuad(const CGL_Shader* glcon, const CRectT<float>& vr);
        static void DrawQuad(const CGL_Shader* glcon, float x1, float y1, float x2, float y2, const float* Colors);
        static void DrawQuad(const CGL_Shader* glcon, const CRectT<float>& vr, const float* Colors);

        // Flaechen mit Textur:

        static void DrawTextureQuad(const CGL_Shader* glcon, float x1, float y1, float x2, float y2);
        static void DrawTextureQuadUpsideDown(const CGL_Shader* glcon, float x1, float y1, float x2, float y2);
        static void DrawTextureQuad(const CGL_Shader* glcon, const CRectT<float>& vr, const CRectT<float>& tr);
        static void DrawTextureQuadRotated(const CGL_Shader* glcon, float x1, float y1, float x2, float y2);
        static void DrawTextureQuad(const CGL_Shader* glcon, const std::vector<CVector2<float> >& verts, const std::vector<CVector2<float> >& texts);


        // Linien:

        static void DrawLine(const CGL_Shader* glcon, float x1, float y1, float x2, float y2);
        static void DrawLines(const CGL_Shader* glcon, const std::vector<CVector2<float> >& LineVec, bool Closed = false);
        static void DrawPolygon(const CGL_Shader* glcon, const CPolygon* Polygon, float sx, float sy, float& TexOffset);
        static void DrawFrame(const CGL_Shader* glcon, const CRectT<float>& rc);

        static const SVertArray2D* CreateLines(const CGL_Shader* glcon, const std::vector<CVector2<float> >& LineVec);
        static void DrawLines(const CGL_Shader* glcon, const SVertArray2D* verts, bool Closed = false);

        static void CalcQuadLine(
            const CVector2<float>& v2a,
            const CVector2<float>& v2b,
            CVector2<float>& v2c1,
            CVector2<float>& v2c2,
            float linewidth,
            int orient);

        static void CalcQuadLine2(
            const CVector2<float>& v20,
            const CVector2<float>& v2a,
            const CVector2<float>& v2b,
            CVector2<float>& v2c1,
            CVector2<float>& v2c2,
            float linewidth,
            bool useCorrection);


        // Trifans

        static void DrawTrifan(const CGL_Shader* glcon, const SVertArray2D& ar);
        static void CreateTrifan(const std::vector<CVector2<float> >& verts, SVertArray2D& ar);
        static void DrawTrifan(const CGL_Shader* glcon, const std::vector<CVector2<float> >& verts);
        static void DrawTrifanColors(const CGL_Shader* glcon, const SVertArray2D* ar, const float* Colors);

        // Texture & Colors

        static void DrawQuadTexCoords(const CGL_Shader* glcon, float vx1, float vy1, float vx2, float vy2,
            const float* textcoords);


        static void CreateQuadTexCoords(float vx1, float vy1, float vx2, float vy2, const float* textcoords, SVertArray2D& ar);
        static void DrawQuadTexCoords(const CGL_Shader* glcon, const SVertArray2D& ar);


        static void CreateRingTriStrip(float InnerRadius, float OuterRadius, int Segments, float InnerTexKoor, float OuterTexKoor, SVertArray2D& ar);
        static void DrawTristrip(const CGL_Shader* glcon, const SVertArray2D& ar);
};

#endif
