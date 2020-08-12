#ifndef CPointData_H
#define CPointData_H

#include <cstdio>
#include <CVector2T.h>
#include <SVertArray.h>
#include <CGL_Shader_Blurline.h>

class CPointData
{
	public:
	
		CPointData()
		{
			mInit = false;	
		}
		CVector2<float> mP1;
		void Draw(CGL_Shader_Blurline* shader, float x1, float y1, float PointSize, const float* color);
	
	protected:
		
		void DrawPointData(CGL_Shader_Blurline* shader);
		
		bool mInit;	
		SVertArray2D mAr;
		const float* mColor;	
};


#endif