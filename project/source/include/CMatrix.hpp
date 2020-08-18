//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CMatrix.hpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#include <cstring>

/***************************************************************************


Aufbau einer OpenGL-Matrix:


	| 0  4  8  12 |
	| 1  5  9  13 |
	| 2  6  10 14 |
	| 3  7  11 15 |


http://wiki.delphigl.com/index.php/glMultMatrix

Die letzte Spalte enthaelt die Verschiebung


****************************************************************************/

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		MatSetIdentity
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::MatSetIdentity()
{
	memset(mMat, 0, sizeof(CMatrix<T>));

	for (int i = 0; i < 4; i++)
	{
		SetElem(1.0, i, i);
	}
/*
	mMat[0]  = 1;
	mMat[1]  = 0;
	mMat[2]  = 0;
	mMat[3]  = 0;

	mMat[4]  = 0;
	mMat[5]  = 1;
	mMat[6]  = 0;
	mMat[7]  = 0;

	mMat[8]  = 0;
	mMat[9]  = 0;
	mMat[10] = 1;
	mMat[11] = 0;

	mMat[12] = 0;
	mMat[13] = 0;
	mMat[14] = 0;
	mMat[15] = 1;
*/
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		
//
//
//---------------------------------------------------------------------------

template <class T>
T CMatrix<T>::GetElem(int x, int y) const
{
	return mMat[x * 4 + y];
}


template <class T>
T CMatrix<T>::GetElem2(int x, int y) const
{
	return mMat[y * 4 + x];
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		
//
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::SetElem(T v, int x, int y)
{
	mMat[x * 4 + y] = v;
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		
//
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::AddElem(T v, int x, int y)
{
	mMat[x * 4 + y] += v;
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:   MultElem
//
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::MultElem(T v, int x, int y)
{
	mMat[x * 4 + y] *= v;
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		Multiply
//
// http://www.programmingsimplified.com/c-program-multiply-matrices
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::Multiply(const CMatrix<T>& m2)
{
	CMatrix<T> m3;
	
	T sum = 0;
	for (int c = 0 ; c < 4 ; c++)
	{
		for (int d = 0 ; d < 4 ; d++)
		{
			for (int k = 0 ; k < 4 ; k++)
			{
				//sum = sum + GetElem(c, k) * m2.GetElem(k, d);
				sum = sum + GetElem(k, c) * m2.GetElem(d, k);
			}
			m3.SetElem(sum, d, c);
			sum = 0;
		}
	}
	CopyFrom(m3);
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		CopyFrom
//
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::CopyFrom(const CMatrix<T>& m2)
{
	memcpy(this, &m2, sizeof(CMatrix<T>));
}


//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		RotateZ
//
// Berechnen einer Rotationsmatrix
//
// angle: Drehwinkel in Grad (0..360)
//
//
// http://de.wikipedia.org/wiki/Drehmatrix
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::RotateZ(T angle)
{
	T ra = DEG_TO_RAD(angle);
	T cs = cos(ra);
	T ss = sin(ra);
	
  CMatrix<T> m2;
	m2.SetElem( cs, 0, 0);
	m2.SetElem(-ss, 1, 0);
	m2.SetElem( ss, 0, 1);
	m2.SetElem( cs, 1, 1);
	
	Multiply(m2);
	
}

// Transformieren eines Vertex durch die Matrix
// sv = Source Vertex
// dv = Ziel Vertex

template <class T>
void CMatrix<T>::MatTransformVertex(const CVector2<T>* sv, CVector2<T>* dv) const
{
	dv->x = (GetElem(0,0) * sv->x) + (GetElem(1,0) * sv->y) + (GetElem(3,0));
	dv->y = (GetElem(0,1) * sv->x) + (GetElem(1,1) * sv->y) + (GetElem(3,1));
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
//
//---------------------------------------------------------------------------



template <class T>
void CMatrix<T>::MatTransformVertex(const CVector3<T>* sv, CVector3<T>* dv) const
{
	dv->x = (GetElem(0,0) * sv->x) + (GetElem(1,0) * sv->y) + (GetElem(2,0) * sv->z) + GetElem(3,0);
	dv->y = (GetElem(0,1) * sv->x) + (GetElem(1,1) * sv->y) + (GetElem(2,1) * sv->z) + GetElem(3,1);
	dv->z = (GetElem(0,2) * sv->x) + (GetElem(1,2) * sv->y) + (GetElem(2,2) * sv->z) + GetElem(3,2);
}


// dv->x = (mMat[0] * sv->x) + (mMat[4] * sv->y) + (mMat[8]  * sv->z) + mMat[12];
// dv->y = (mMat[1] * sv->x) + (mMat[5] * sv->y) + (mMat[9]  * sv->z) + mMat[13];
// dv->z = (mMat[2] * sv->x) + (mMat[6] * sv->y) + (mMat[10] * sv->z) + mMat[14];




//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		Translate
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::Translate(T tx, T ty, T tz)
{
/*
	CMatrix<T> TransMatrix;

	TransMatrix.SetElem(tx, 3, 0);
	TransMatrix.SetElem(ty, 3, 1);
	TransMatrix.SetElem(tz, 3, 2);

	Multiply(TransMatrix);
*/
	AddElem(tx, 3, 0);
	AddElem(ty, 3, 1);
	AddElem(tz, 3, 2);
}

//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		Scale
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::Scale(T sx, T sy, T sz)
{
/*
	CMatrix<T> ScaleMatrix;
	ScaleMatrix.SetElem(sx, 0,0);
	ScaleMatrix.SetElem(sy, 1,1);
	ScaleMatrix.SetElem(sz, 2,2);

	Multiply(ScaleMatrix);
*/
	MultElem(sx, 0,0);
	MultElem(sy, 1,1);
	MultElem(sz, 2,2);


}




//---------------------------------------------------------------------------
//
// Klasse:    CMatrix
// Methode:		Print
//
//---------------------------------------------------------------------------

template <class T>
void CMatrix<T>::Print(char* buff) const
{
	sprintf(buff, "%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
	 mMat[0],  mMat[1],  mMat[2],  mMat[3],
	 mMat[4],  mMat[5],  mMat[6],  mMat[7],
	 mMat[8],  mMat[9],  mMat[10], mMat[11],
	 mMat[12], mMat[13], mMat[14], mMat[15]);
}
