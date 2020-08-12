//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CGraflibTga.h
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
// Inhalt: Deklaration der Klasse CGraflibTga
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 12.05.2006 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CDataRect.h>
#include <CGraflib.h>

#ifndef CGRAFLIBPNG_H
#define CGRAFLIBPNG_H

class CGraflibPng : public CGraflib
{
	public:



		bool Read(const char* Filename, SFileReadInfo* fri = NULL);
		int  Write(const char* Filename,  bool bCompFlag, CFileIO* fio = NULL);

		bool OpenReadLine(const char* Filename) { return false; }
		void ReadLine(void* LineBuffer) { }
		void CloseReadLine() { }

		bool OpenWriteLine(const char* Filename) { return false; }
		void WriteLine(const void* LineBuffer) { }
		void CloseWriteLine() { }

	protected:

		void TestPalette(const void* vpinfo);



};

#endif
