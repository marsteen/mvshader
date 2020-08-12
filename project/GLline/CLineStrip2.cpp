

void CLineStrip2::Draw2(CGL_Shader_Blurline* shader, const TLinePoints& points, float lw, const float* color)
{
    int vertCount = (points.size() + 1) * 2;
    SVertArray2D  ar;
    ar.MakeVerts(vertCount);
    ar.MakeTexts(vertCount);

    
    
    const CVector2<float>& p0 = point0[0];
    for (int i = 1; i < points.size(); i++)
    {       
        CVector2<float> c1, c2;
        const CVector2<float>& p1 = points[i];
           
        CGL_Basic::CalcQuadLine(p0, p1, c1, c2, lw);
        ar.AddVert(c1);
        ar.AddVert(c2);
        p0 = p1;
        
    }
    CGL_Basic::DrawTristrip(shader, ar);
}
