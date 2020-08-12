
#include <CLineData.h>
#include <CGL_Basic.h>
#include <StaticLineHelp.hpp>

#define LINE_W (1.0f / 25.0f)


//---------------------------------------------------------------------------
//
//
// Klasse:  CContextLissa
// Methode  DrawLine
//
//
//---------------------------------------------------------------------------


static void InitRing(CGL_Shader_Blurline* shader,
                     float x1, float y1,
										 float ri, float ro,
										 const float* color,
										 SVertArray2D* ar)
{
	
	
//		CGL_Basic::CreateQuadTexCoords(0.0f, 0.0f, LINE_W, len,  textcoordsLeft,  ar[0]);
//		CGL_Basic::CreateQuadTexCoords(-LINE_W, 0.0f, 0.0f, len, textcoordsRight, ar[1]);
		
		/*
		glEnableVertexAttribArray(shader->VertAttrib());	
		glEnableVertexAttribArray(shader->TextAttrib());
		
		CGL_Basic::DrawQuadTexCoords(shader, ar[0]);
		CGL_Basic::DrawQuadTexCoords(shader, ar[1]);
		
		
		glDisableVertexAttribArray(shader->VertAttrib());
		glDisableVertexAttribArray(shader->TextAttrib());
		*/

	
	
	
	//CGL_Basic::DrawQuadTexCoords(shader, 0.0f, 0.0f, LINE_W, len,  textcoordsLeft);
	
	//CGL_Basic::DrawQuadTexCoords(shader, -LINE_W, 0.0f, 0.0f, len, textcoordsRight);
	
#if 0
	{ 
		// 1. Kappe
		CVector2<float> org(0.0f, 0.0f);
		std::vector<CVector2<float> > vec;
		vec.push_back(org);
		GetCirclePoints<float>(180.0, 360.0f, LINE_W, 16, 0.0f, 0.0f, vec);
		CGL_Basic::CreateTrifan(vec, ar[2]);
		//CGL_Basic::DrawTrifan(shader, ar);	
		
	}
	
	{ 
		// 2. Kappe
		CVector2<float> org(0.0f, len);
		std::vector<CVector2<float> > vec;
		vec.push_back(org);
		GetCirclePoints<float>(0.0, 180.0f, LINE_W, 16, 0.0f, len, vec);
		CGL_Basic::CreateTrifan(vec, ar[3]);
		//CGL_Basic::DrawTrifan(shader, ar);	
		
	}
#endif
	
	
}


void CRingData::Draw(CGL_Shader_Blurline* shader,
                     float x1, float y1,
                     float InnerRadius, float OuterRadius,
										 const float* color)
{
  if (!mInit)
	{
		
		mP1.Set(x1, y1);
		mColor = color;
		
		InitRing(shader, x1, y1, x2, y2, color, &mLineAngle, mAr);
	  mInit = true;
	}
	DrawRingData(shader);
	
}

void CRingData::DrawLineData(CGL_Shader_Blurline* shader)
{
	
	shader->SetPos(mP1.x, mP1.y);
	shader->SetColor(mColor);

	glEnableVertexAttribArray(shader->VertAttrib());	
	glEnableVertexAttribArray(shader->TextAttrib());
	
	CGL_Basic::DrawTristrip(shader, mAr[0]);
  CGL_Basic::DrawTristrip(shader, mAr[1]);
  CGL_Basic::DrawTristrip(shader, mAr[2]);
	
	
	glDisableVertexAttribArray(shader->VertAttrib());
	glDisableVertexAttribArray(shader->TextAttrib());
	
}
