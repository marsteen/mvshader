//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CMatrixStack.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef CMatrixStack_H
#define CMatrixStack_H


#include <vector>
#include <CMatrix.h>

class CMatrixStack
{
  public:

		void MatPushMatrix()
		{
			mStack.push_back(mMatrix);
		}
		void MatPopMatrix()
		{
      mMatrix = mStack.back();
			mStack.pop_back();
		}
	
		void MatTranslate(float tx, float ty, float tz)
		{
			mMatrix.Translate(tx, ty, tz);
		}
		
		void RotateZ(float angle)
		{
			mMatrix.RotateZ(angle);
		}	
		void MatScale(float sx, float sy, float sz)
		{
			mMatrix.Scale(sx, sy, sz);
		}
    void Transform(CVector2<float>* v)
    {
    	CVector2<float> dv;
    	mMatrix.MatTransformVertex(v, &dv);
    	*v = dv;
    }
    
    void MatLoadIdentity()
		{
			mMatrix.MatSetIdentity();
		}

	  const float* Mat() const
	  {
			return mMatrix.Mat();
		}
	
	protected:
		
		std::vector<CMatrix<float> > mStack;
		CMatrix<float>               mMatrix;
};

#endif
