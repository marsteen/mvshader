//------------------------------------------------------------------------------
//
// PROJECT : 3D Globus
//
// FILE    : CVector2<T>.cpp
//
// VERSION : 1.0
//
// AUTOR   : Martin Steen
//           Imagon GmbH
//
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Implementation von CVector2<T> Klassen
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 11.04.2005 erstellt - Martin Steen
//
//------------------------------------------------------------------------------


#ifndef CVECTOR2_HPP
#define CVECTOR2_HPP

#include <cmath>
#include <MathDefines.h>

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator=(const CVector2<T>& v2)
{
    x = v2.x;
    y = v2.y;

    return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator-(const CVector2<T> v2) const
{
    CVector2<T> v1;

    v1.x = x - v2.x;
    v1.y = y - v2.y;

    return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator+(const CVector2<T> v2) const
{
    CVector2<T> v1;

    v1.x = x + v2.x;
    v1.y = y + v2.y;

    return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator*(const CVector2<T> v2) const
{
    CVector2<T> v1;

    //v1.x = (y * v2.y) - (y * v2.y);
    //v1.y = (yz * v2.x) - (x * v2.y);

    return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator*(T c) const
{
    CVector2<T> v1;

    v1.x = x * c;
    v1.y = y * c;

    return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator/(T c) const
{
    CVector2<T> v1;

    v1.x = x / c;
    v1.y = y / c;

    return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator+=(CVector2<T> v2)
{
    x += v2.x;
    y += v2.y;

    return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator-=(CVector2<T> v2)
{
    x -= v2.x;
    y -= v2.y;

    return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector2<T> CVector2<T>::operator/=(T c)
{
    x /= c;
    y /= c;

    return *this;
}


//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
T CVector2<T>::Betrag() const
{
    return sqrt((x * x) + (y * y ));
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector2<T>::Normalize()
{
    T Lenght = Betrag();

  if (Lenght > 0)
  {
        x /= Lenght;
        y /= Lenght;
    }
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector2<T>::SubtractVector(CVector2<T>* v1, CVector2<T>* v2)
{
    x = v1->x - v2->x;
    y = v1->y - v2->y;
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
T CVector2<T>::SkalarProdukt(const CVector2<T>* v1) const
{
    return (x * v1->x) + (y * v1->y);
}

//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector2<T>::Reverse()
{
    x = -x;
    y = -y;
}



//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector2<T>::operator*=(T f)
{
    x *= f;
    y *= f;
}


//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:   RotationXY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

// Drehen des Vectors in der XY Ebene um den Punkt v2 und den
// Winkel Alpha

template <class T>
void CVector2<T>::RotationXY(const CVector2<T>* v2, T sinAlpha, T cosAlpha)
{
    T xerg, yerg;

    xerg = (cosAlpha * (x - v2->x)) - (sinAlpha * (y - v2->y));
    yerg = (sinAlpha * (x - v2->x)) + (cosAlpha * (y - v2->y));

    x = xerg + v2->x;
    y = yerg + v2->y;
}


//---------------------------------------------------------------------------
//
// Klasse:      CVector2<T>
// Methode:     RotationXY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

// Drehen des Vectors in der XY Ebene und den Winkel Alpha

template <class T>
void CVector2<T>::RotationXY(T sinAlpha, T cosAlpha)
{
    T xerg, yerg;

    xerg = (cosAlpha * x) - (sinAlpha * y);
    yerg = (sinAlpha * x) + (cosAlpha * y);

    x = xerg;
    y = yerg;
}


//---------------------------------------------------------------------------
//
//
// Klasse:  CVector2<T>
// Methode  DriftVector
//
// Berechnung der Position eines Vektors im Abstand ("drift") und
// einem Winkel (sinAlpha, cosAlpha)
//
//
//---------------------------------------------------------------------------


template <class T>
void CVector2<T>::DriftVector(CVector2<T>* v2, T drift, T sinAlpha, T cosAlpha) const
{
  v2->x = x + drift;
  v2->y = y;
  v2->RotationXY(this, sinAlpha, cosAlpha);
}


template <class T>
void CVector2<T>::DriftVector(CVector2<T>* v2, T drift, T deg_angle) const
{
  float rad_angle = DEG_TO_RAD(deg_angle);
    DriftVector(v2, drift, sin(rad_angle), cos(rad_angle));
}


template <class T>
T  CVector2<T>::GetAngleRad(const CVector2<T>* v2) const
{
   float dx = x - v2->x;
   float dy = y - v2->y;

   return atan2(dy, dx);
}

template <class T>
T  CVector2<T>::GetAngleSkalarRad(const CVector2<T>* v2) const
{
   float dx = x - v2->x;
   float dy = y - v2->y;

   return acos(SkalarProdukt(v2) / Betrag() * v2->Betrag());
}
template <class T>
T  CVector2<T>::GetAngleRadAtan2(const CVector2<T>* v2) const
{
    return atan2(v2->y, v2->x) - atan2(y, x);
}

template <class T>
T  CVector2<T>::GetAngleDeg(const CVector2<T>* v2) const
{
   float dx = x - v2->x;
   float dy = y - v2->y;

   return RAD_TO_DEG(atan2(dy, dx));
}

#endif
