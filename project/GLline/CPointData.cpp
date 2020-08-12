
#include <CPointData.h>
#include <CGL_Basic.h>
#include <StaticLineHelp.hpp>
//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode  InitPoint
//
//
//---------------------------------------------------------------------------

static void InitPoint(CGL_Shader_Blurline* shader, SVertArray2D& ar, float PointSize)
{
		CVector2<float> org(0.0f, 0.0f);
		std::vector<CVector2<float> > vec;
		vec.push_back(org);
		GetCirclePoints<float>(0.0f, 360.0f, PointSize, 128, 0.0f, 0.0f, vec);
		CGL_Basic::CreateTrifan(vec, ar);	
}


void CPointData::DrawPointData(CGL_Shader_Blurline* shader)
{
	shader->SetAngle(0.0f);
	shader->SetPos(mP1.x, mP1.y);
	shader->SetColor(mColor);

	CGL_Basic::DrawTrifan(shader, mAr);	
}


void CPointData::Draw(CGL_Shader_Blurline* shader,
                     float x1, float y1,
										 float PointSize,
										 const float* color)
{
  if (!mInit)
	{
		
		mP1.Set(x1, y1);
		mColor = color;
		
		InitPoint(shader, mAr, PointSize);
	  mInit = true;
	}
	DrawPointData(shader);
	
}
