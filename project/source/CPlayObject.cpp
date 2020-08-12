
#include <CPlayObject.h>
#include <iostream>

using namespace std;

//---------------------------------------------------------------------------
//
//
// Klasse:  CPlayObject
// Methode  Intersect
//
// return:  r = 0: Kein Treffer
//          r = 1: X-Treffer
//          r = 2: Y-Treffer
//
//---------------------------------------------------------------------------

int CPlayObject::Intersect(const CPointObject* ball) const
{
	// Wenn eine der vier Ecken des Rechtecks innerhalb des Balls liegt 
	// oder wenn der Abstand zwischem dem Zentrum des Balls und dem Zentrum
	// des Rechtecks kleiner ist als als die Differenz z
	
	int r = 0;
	
	CRectT<float> NowRect;
	CRectT<float> iRect;
	
	NowRect.CopyFrom(mRect);
	NowRect.Add(mPos.x, mPos.y);
	NowRect.Stretch(1.4f, 1.0f);
	
	if (NowRect.InterSectRect(&ball->mPosRect, &iRect))
	{
	  if (iRect.Width() > iRect.Height())
		{
		  r = 2; 	
		}
		else
		{
		  r = 1;	
		}		
	}
	return r;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CPlayObject
// Methode: AddPos
//
//
//---------------------------------------------------------------------------

void CPlayObject::AddPos(float xrel, float yrel)
{
  mPos.x += xrel;
	mPos.y += yrel;		
}

//---------------------------------------------------------------------------
//
//
// Klasse:  CPlayObject
// Methode: Draw
//
//
//---------------------------------------------------------------------------

void CPlayObject::Draw(CGL_Shader_Blurline* shader,
                     float x1, float y1,
										 float x2, float y2,
						         float LineWidth,
										 const float* color)
{
	
	
  if (!mInit)
  {  
    mRect.Set(x1, y1 - LineWidth/2, x2, y1 + LineWidth/2);
    //cout << " x1=" << x1 << " y1=" << y1 - LineWidth/2
    //     << " x2=" << x2 << " y2=" << y2 + LineWidth/2 << endl;
  }
  CLineData::Draw(shader, x1, y1, x2, y2, LineWidth, color);
}
