#ifndef CPointObject_H
#define CPointObject_H

#include <CVector2T.h>
#include <CRectT.h>
#include <CPointData.h>
#include <CGL_Shader_Blurline.h>

class CPointObject
{
	public:
		
		void InitRandom();
		void Init(float xpos, float ypos, float size, const float* color);
		void Draw(CGL_Shader_Blurline* shader);
		
		CVector2<float> mPos;
		CVector2<float> mDir;
		CPointData      mPointData;
		float           mColor[4];
		float           mPointSize;
		CRectT<float>   mPosRect;
};
#endif