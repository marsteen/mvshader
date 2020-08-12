//***************************************************************************
//
//
// @PROJECT  :	Diercke Digital PAD
// @VERSION  :	1.0
// @FILENAME :	CBlockOperations.cpp
// @DATE     :	12.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	msteen@imagon.de
//
//
//***************************************************************************
//
// Inhalt:
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 30.06.2006 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

//#include <io.h>

#include <CFileIO.h>
#include <CDataRect.h>
#include <CBlockOperations.h>

//---------------------------------------------------------------------------
//
// Klasse:   CBlockOperations
// Methode:
//
// Schreibt den Source-Block an der Stelle Xpos/Ypos in die
// GEÖFFNETE Datei
//
//---------------------------------------------------------------------------

void CBlockOperations::WriteBlock(CFileIO* fio, CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int BytesProPixel)
{
	int DstBytesProLine = dstRect->mWidth * BytesProPixel;
	int SrcBytesProLine = srcRect->mWidth * BytesProPixel;

	unsigned int dstPtr = (DstBytesProLine * Ypos) + (Xpos * BytesProPixel);
	unsigned char* srcPtr = (unsigned char*) srcRect->mData;

	for (int i = 0; i < dstRect->mHeight; i++)
	{
		fio->SetWritePos(dstPtr);
		fio->WriteBytes(srcPtr, SrcBytesProLine);
		dstPtr += DstBytesProLine;
		srcPtr += SrcBytesProLine;
	}
}

//---------------------------------------------------------------------------
//
// Klasse:   CBlockOperations
// Methode:  CopyBlock
//
// Kopiert einen Block aus dem GROSSEN Sourcerechteck in ein KLEINES Dest-
// Rechteck
//
//---------------------------------------------------------------------------

void CBlockOperations::CopyBlock(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int BytesProPixel)
{
	int DstBytesProLine = dstRect->mWidth * BytesProPixel;
	int SrcBytesProLine = srcRect->mWidth * BytesProPixel;
	int Offset = (SrcBytesProLine * Ypos) + (Xpos * BytesProPixel);

	unsigned char* dstPtr = (unsigned char*) dstRect->mData;
	unsigned char* srcPtr = (unsigned char*) srcRect->mData + Offset;

	for (int i = 0; i < dstRect->mHeight; i++)
	{
		memcpy(dstPtr, srcPtr, dstRect->mWidth * BytesProPixel);
		dstPtr += DstBytesProLine;
		srcPtr += SrcBytesProLine;
	}
}


//---------------------------------------------------------------------------
//
// Klasse:   CBlockOperations
// Methode:  CopyBlock2
//
// Kopiert einen Block aus dem KLEINEN Source-Rechteck in ein GROSSES
// Dest-Rechteck
//
//---------------------------------------------------------------------------

void CBlockOperations::CopyBlock2(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos,
 int CopyX, int CopyY, int BytesProPixel)
{
	int DstBytesProLine = dstRect->mWidth * BytesProPixel;
	int SrcBytesProLine = srcRect->mWidth * BytesProPixel;
	int Offset = (DstBytesProLine * Ypos) + (Xpos * BytesProPixel);

	unsigned char* dstPtr = (unsigned char*) dstRect->mData + Offset;
	unsigned char* srcPtr = (unsigned char*) srcRect->mData;

	for (int i = 0; i < CopyY; i++)
	{
		memcpy(dstPtr, srcPtr, CopyX * BytesProPixel);
		dstPtr += DstBytesProLine;
		srcPtr += SrcBytesProLine;
	}
}

/*
//---------------------------------------------------------------------------
//
// Klasse:   CBlockOperations
// Methode:  CopyBlock2
//
// Kopiert einen Block aus dem KLEINEN Source-Rechteck in ein GROSSES
// Dest-Rechteck
//
//---------------------------------------------------------------------------

void CBlockOperations::CopyBlock2(CDataRect* dstRect, CDataRect* srcRect, int Xpos, int Ypos,
 CRectT<int>* srcPar, int BytesProPixel)
{
	int DstBytesProLine = dstRect->mWidth * BytesProPixel;
	int SrcBytesProLine = srcRect->mWidth * BytesProPixel;
	int DstOffset = (DstBytesProLine * Ypos) + (Xpos * BytesProPixel);
	int SrcOffset = (SrcBytesProLine * Ypos)

	unsigned char* dstPtr = (unsigned char*) dstRect->mData + DstOffset;
	unsigned char* srcPtr = (unsigned char*) srcRect->mData + SrcOffset;

	for (int i = 0; i < CopyY; i++)
	{
		memcpy(dstPtr, srcPtr, CopyX * BytesProPixel);
		dstPtr += DstBytesProLine;
		srcPtr += SrcBytesProLine;
	}
}
*/
