//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CSetPointer.h
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************


#ifndef CSETPOINTER_H
#define CSETPOINTER_H

//
//
//

template<typename T>
class CSetPointerConst
{
    public:

        CSetPointerConst(const T& ptr) { mPtr = &ptr; }
        CSetPointerConst(const T* ptr) { mPtr = ptr; }

        bool operator<(const CSetPointerConst& k) const
        {
            return *mPtr < *(k.mPtr);
        }


        bool operator==(const CSetPointerConst& k) const
        {
            return *mPtr == *(k.mPtr);
        }


        const T* mPtr;
};

//
//
//

template<typename T>
class CSetPointer
{
    public:

        CSetPointer(T& ptr) { mPtr = &ptr; }
        CSetPointer(T* ptr) { mPtr = ptr; }

        bool operator<(const CSetPointer& k) const
        {
            return *mPtr < *(k.mPtr);
        }


        bool operator==(const CSetPointer& k) const
        {
            return *mPtr == *(k.mPtr);
        }


        T* mPtr;
};


#endif
