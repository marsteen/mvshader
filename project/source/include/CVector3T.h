//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CVector3T.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#ifndef CVECTOR3T_H
#define CVECTOR3T_H

#include <cmath>

#include <CVector2T.h>

template<class T>
class CVector3
{
public:

    T x, y, z;

    const T* v(int a = 0) const { return &x + a; }
    T* vm(int a = 0) { return &x + a; }

    CVector3<T> operator=(const CVector3<T>& v2);
    CVector3<T> operator+(CVector3<T> v2) const;
    CVector3<T> operator-(CVector3<T> v2) const;
    CVector3<T> operator*(CVector3<T> v2) const;
    CVector3<T> operator*(T f) const;
    void operator*=(T f);

    CVector3<T> operator+=(CVector3<T> v2);
    CVector3<T> operator-=(CVector3<T> v2);
    CVector3<T> operator+=(CVector2<T> v2);

    CVector3<T> operator/(T c) const;
    CVector3<T> operator/=(T c);

    void Print();

    void NormalVector(const CVector3<T>* v1, const CVector3<T>* v2);
    void NormalFace(const CVector3<T>& v1, const CVector3<T>& v2, const CVector3<T>& v3);

    void SubtractVector(const CVector3<T>* v1, const CVector3<T>* v2);

    T Betrag() const;
    void Normalize();
    void Reverse();
    void Set(T vx, T vy, T vz);

    void RotationXY(const CVector3<T>* v2, T sinAlpha, T cosAlpha);
    void RotationXZ(const CVector3<T>* v2, T sinAlpha, T cosAlpha);
    void RotationYZ(const CVector3<T>* v2, T sinAlpha, T cosAlpha);

    void RotationXY(T sinAlpha, T cosAlpha);
    void RotationXZ(T sinAlpha, T cosAlpha);
    void RotationYZ(T sinAlpha, T cosAlpha);

    CVector3() { x = 0; y = 0; z = 0; }
    CVector3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }


    bool operator==(CVector3<T> v2) const
    {
        return (x == v2.x) && (y == v2.y) && (z == v2.z);
    }


    T Abstand(const CVector3<T>* v1) const
    {
        return (*this - *v1).Betrag();
    }


    T Abstand(const CVector3<T>& v1) const
    {
        return (*this - v1).Betrag();
    }


    T SkalarProdukt(const CVector3<T>* v1) const;
    T Winkel(const CVector3<T>* v1) const;
};

template<class T>
class CVector4 : public CVector3<T>
{
public:

    T w;
};

#include <CVector2T.hpp>
#include <CVector3T.hpp>

#endif
