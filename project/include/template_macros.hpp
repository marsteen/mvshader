//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	template_macros.hpp
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef TEMPLATE_MACROS_HPP
#define TEMPLATE_MACROS_HPP

template <typename T>
T Tmax(T a, T b)
{	
	return (a > b) ? a : b;
}

template <typename T>
T Tmin(T a, T b)
{	
	return (a < b) ? a : b;
}


#endif
