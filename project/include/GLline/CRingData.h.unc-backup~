#ifndef CRingData_H
#define CRingData_H

#include <CVector2T.h>
#include <SVertArray.h>
#include <CGL_Shader_Blurline.h>

class CRingData
{
	public:
	
		CRingData()
		{
			mInit = false;	
		}
		CVector2<float> mP1;		
		void Draw(CGL_Shader_Blurline* shader,
										float x1, float y1,
										float ri, float ro,
										int Segments,
										const float* color);
	
	protected:
		
		void DrawRingData(CGL_Shader_Blurline* shader);
		
		bool         mInit;	
		SVertArray2D mAr[2];		
		const float* mColor;
};

#endif



