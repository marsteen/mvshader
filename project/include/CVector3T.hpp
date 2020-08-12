//------------------------------------------------------------------------------
//
// PROJECT : 3D Globus
//
// FILE    : CVector3<T>.cpp
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
// Inhalt: Implementation von CVector3<T> Klassen
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 11.04.2005 erstellt - Martin Steen
//
//------------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator=(const CVector3<T>& v2)
{
	x = v2.x;
	y = v2.y;
	z = v2.z;

	return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator-(CVector3<T> v2) const
{
	CVector3<T> v1;

	v1.x = x - v2.x;
	v1.y = y - v2.y;
	v1.z = z - v2.z;

	return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator+(CVector3<T> v2) const
{
	CVector3<T> v1;

	v1.x = x + v2.x;
	v1.y = y + v2.y;
	v1.z = z + v2.z;

	return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator*(CVector3<T> v2) const
{
	CVector3<T> v1;

	v1.x = (y * v2.z) - (z * v2.y);
	v1.y = (z * v2.x) - (x * v2.z);
	v1.z = (x * v2.y) - (y * v2.x);

	return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator*(T c) const
{
	CVector3<T> v1;

	v1.x = x * c;
	v1.y = y * c;
	v1.z = z * c;

	return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator/(T c) const
{
	CVector3<T> v1;

	v1.x = x / c;
	v1.y = y / c;
	v1.z = z / c;

	return v1;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator+=(CVector3<T> v2)
{
	x += v2.x;
	y += v2.y;
	z += v2.z;

	return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator+=(CVector2<T> v2)
{
	x += v2.x;
	y += v2.y;

	return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator-=(CVector3<T> v2)
{
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;

	return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
CVector3<T> CVector3<T>::operator/=(T c)
{
	x /= c;
	y /= c;
	z /= c;

	return *this;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::Print()
{
        //cout << "x=" << x << "\ty=" << y << "\tz=" << z << endl;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
T CVector3<T>::Betrag() const
{
	return sqrt((x * x) + (y * y ) + (z * z));
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::Normalize()
{
	T Lenght = Betrag();

  if (Lenght > 0)
  {
		x /= Lenght;
		y /= Lenght;
		z /= Lenght;
	}
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::NormalVector(const CVector3<T>* v1, const CVector3<T>* v2)
{
	x = (v1->y * v2->z) - (v1->z * v2->y);
	y = (v1->z * v2->x) - (v1->x * v2->z);
	z = (v1->x * v2->y) - (v1->y * v2->x);

  Normalize();
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:		NormalFace
//
// Parameter: v1, v2, v3: Drei Eckpunkte des Face
//
// Der Vektor enthaelt anschliessend die Normale des Face-Dreiecks
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::NormalFace(const CVector3<T>& v1, const CVector3<T>& v2, const CVector3<T>& v3)
{
  CVector3<T> d1 = v1 - v2;
  CVector3<T> d2 = v2 - v3;
  
  NormalVector(&d1, &d2);
}


//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::SubtractVector(const CVector3<T>* v1, const CVector3<T>* v2)
{
	x = v1->x - v2->x;
	y = v1->y - v2->y;
	z = v1->z - v2->z;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:		SkalarProdukt
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
T CVector3<T>::SkalarProdukt(const CVector3<T>* v1) const
{
	return (x * v1->x) + (y * v1->y) + (z * v1->z);
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:		Winkel
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
T CVector3<T>::Winkel(const CVector3<T>* v1) const
{
	T w = 0;
	T b0 = Betrag();
	T b1 = v1->Betrag();

	if ((b0 != 0) && (b1 != 0))
	{
		w = SkalarProdukt(v1) / (b0 * b1);
	}
	return w;
}


//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::Set(T vx, T vy, T vz)
{
	x = vx;
	y = vy;
	z = vz;
}


//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:		RotationXY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::RotationXY(const CVector3<T>* v2, T sinAlpha, T cosAlpha)
{
	float xerg, yerg;

  xerg = (cosAlpha * (x - v2->x)) - (sinAlpha * (y - v2->y));
  yerg = (sinAlpha * (x - v2->x)) + (cosAlpha * (y - v2->y));

  x = xerg + v2->x;
  y = yerg + v2->y;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:		RotationXY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::RotationXY(T sinAlpha, T cosAlpha)
{
	float xerg, yerg;

  xerg = (cosAlpha * (x)) - (sinAlpha * (y));
  yerg = (sinAlpha * (x)) + (cosAlpha * (y));

  x = xerg;
  y = yerg;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::RotationXZ(const CVector3<T>* v2, T sinAlpha, T cosAlpha)
{
	float xerg, zerg;

  xerg = (cosAlpha * (x - v2->x)) - (sinAlpha * (z - v2->z));
  zerg = (sinAlpha * (x - v2->x)) + (cosAlpha * (z - v2->z));

  x = xerg + v2->x;
  z = zerg + v2->z;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::RotationXZ(T sinAlpha, T cosAlpha)
{
	float xerg, zerg;

  xerg = (cosAlpha * (x)) - (sinAlpha * (z));
  zerg = (sinAlpha * (x)) + (cosAlpha * (z));

  x = xerg;
  z = zerg;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::RotationYZ(const CVector3<T>* v2, T sinAlpha, T cosAlpha)
{
	float yerg, zerg;

  yerg = (cosAlpha * (y - v2->y)) - (sinAlpha * (z - v2->z));
  zerg = (sinAlpha * (y - v2->y)) + (cosAlpha * (z - v2->z));

  y = yerg + v2->y;
  z = zerg + v2->z;
}


//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::RotationYZ(T sinAlpha, T cosAlpha)
{
	float yerg, zerg;

  yerg = (cosAlpha * (y)) - (sinAlpha * (z));
  zerg = (sinAlpha * (y)) + (cosAlpha * (z));

  y = yerg;
  z = zerg;
}

//---------------------------------------------------------------------------
//
// Klasse:		CVector3<T>
// Methode:
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

template <class T>
void CVector3<T>::operator*=(T f)
{
	x *= f;
	y *= f;
	z *= f;
}


