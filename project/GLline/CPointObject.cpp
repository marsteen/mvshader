
#include <cstring>
#include <CPointObject.h>
#include <CRandom.h>
#include <CGL_Shader_Blurline.h>

extern float GlobalAspect;

//---------------------------------------------------------------------------
//
//
// Klasse:  CPointObject
// Methode: InitRandom
//
//
//---------------------------------------------------------------------------


#define SPEED (1.0 / 100.0f)

void CPointObject::InitRandom()
{
	mPos.x = CRandom<float>::Get(-1.0f, 1.0f) * GlobalAspect;
	mPos.y = CRandom<float>::Get(-1.0f, 1.0f);
	
	mDir.x = CRandom<float>::Get(-1.0f, 1.0f) * SPEED * GlobalAspect;
	mDir.y = CRandom<float>::Get(-1.0f, 1.0f) * SPEED;
	
	mColor[0] = CRandom<float>::Get(0.6f, 1.0f);
	mColor[1] = CRandom<float>::Get(0.6f, 1.0f);
	mColor[2] = CRandom<float>::Get(0.6f, 1.0f);
	mColor[3] = CRandom<float>::Get(0.8f, 1.0f);
	
	mPointSize = CRandom<float>::Get(0.1f, 0.4f) * 4.0f;
	
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CPointObject
// Methode: Init
//
//
//---------------------------------------------------------------------------

void CPointObject::Init(float xpos, float ypos, float size, const float* color)
{
	mPos.x = xpos;
	mPos.y = ypos;
	mPointSize = size;
	
	mDir.x = 0.002f; //CRandom<float>::Get(-1.0f, 1.0f) * SPEED * GlobalAspect;
	mDir.y = 0.02f;
	
	memcpy(mColor, color, sizeof(float) * 4);
	
	
}






//---------------------------------------------------------------------------
//
//
// Klasse:  CPointObject
// Methode: Draw
//
//
//---------------------------------------------------------------------------


void CPointObject::Draw(CGL_Shader_Blurline* shader)
{
	float Green[] = { 0.0f, 1.0f, 0.1f, 0.9f };
	
	shader->SetObjAngle(0.0f);
	shader->SetObjPos(mPos.x, mPos.y);
	shader->SetObjOrigin(0.0f, 0.0f);	
	
	mPos.x += mDir.x / GlobalAspect;
  mPos.y += mDir.y;
	mPointData.Draw(shader, 0, 0, mPointSize, mColor);
  
  //mDir.y -= 0.0005f; // Schwerkraft
	
	mPosRect.left   = mPos.x - mPointSize * 0.6f;
	mPosRect.right  = mPos.x + mPointSize * 0.6f;
	
  mPosRect.top    = mPos.y - mPointSize * 0.6f;
	mPosRect.bottom = mPos.y + mPointSize * 0.6f;
	
	
	
	
	if (mPos.x < -GlobalAspect) mDir.x = -mDir.x;
	if (mPos.x >  GlobalAspect) mDir.x = -mDir.x;
	if (mPos.y < -1.0f) mDir.y = -mDir.y;
	if (mPos.y >  1.0f) mDir.y = -mDir.y;
	
}
