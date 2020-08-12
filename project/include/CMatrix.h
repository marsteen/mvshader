//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CMatrix.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#ifndef CMATRIX_H
#define CMATRIX_H

#include <CVector2T.h>
#include <CVector3T.h>
#include <MathDefines.h>

template<typename T>
class CMatrix
{
    public:

        CMatrix()
        {
            MatSetIdentity();
        }


        void MatSetIdentity();

        void Translate(T tx, T ty, T tz);
        void Scale(T sx, T sy, T sz);
        void RotateZ(T angle);
        void MatTransformVertex(const CVector2<T>* sv, CVector2<T>* dv) const;
        void MatTransformVertex(const CVector3<T>* sv, CVector3<T>* dv) const;


        void Print(char* buff) const;

        const T* Mat() const
        {
            return mMat;
        }


    protected:

        void Multiply(const CMatrix<T>& m2);
        void CopyFrom(const CMatrix<T>& m2);


        T GetElem(int x, int y) const;
        T GetElem2(int x, int y) const;
        void SetElem(T value, int x, int y);
        void AddElem(T value, int x, int y);
        void MultElem(T value, int x, int y);

        T mMat[16];
};

#include "CMatrix.hpp"

#endif
