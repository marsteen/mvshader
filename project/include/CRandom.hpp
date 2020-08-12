//---------------------------------------------------------------------------
//
// PROJECT : thiefs
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include <cstdlib>
#include <time.h>



template <typename T>
T CRandom<T>::Get01()
{
	static bool StaticInitSeed;

	if (!StaticInitSeed)
	{
		srand (time(NULL));
		StaticInitSeed = true;
	}

	return T(rand()) / T(RAND_MAX);
}

//---------------------------------------------------------------------------
//
// KLASSE        : CRandom
// METHODE       : Get
//
//
//
//---------------------------------------------------------------------------

template <typename T>
T CRandom<T>::Get(T Low, T High)
{
	 return (Get01() * (High - Low)) + Low;
}


//---------------------------------------------------------------------------
//
// KLASSE        : CRandom
// METHODE       : GetInt
//
//
//
//---------------------------------------------------------------------------

template <typename T>
int CRandom<T>::GetInt(int Low, int High)
{
	 return  (int) ((Get01() * (High - Low + 1)) + Low);
}

