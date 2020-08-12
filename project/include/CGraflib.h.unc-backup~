//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	CGraflib.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************

#ifndef CGRAFLIB_H
#define CGRAFLIB_H


#include <string>
#include <CFileIO.h>
#include <SFileReadInfo.h>
#include <CDataRect.h>

enum EGraflibError
{
	EGRAFLIB_ERROR_NONE,         // Alles OK
	EGRAFLIB_ERROR_GENERAL,      // Allgemeiner Fehler
	EGRAFLIB_ERROR_SIZE,         // Falsche Groesse
	EGRAFLIB_ERROR_BIT_DEPTH,    // Falsche Bit-Tiefe
	EGRAFLIB_ERROR_READ,         // Lese-Fehler
	EGRAFLIB_ERROR_WRITE,        // Schreib-Fehler
	EGRAFLIB_ERROR_IMAGETYPE     // Falscher Bild-Typ
};

class CGraflib : public CDataRect
{
	public:

		CGraflib() {	}
		virtual ~CGraflib() {	}

		virtual bool Read(const char* Filename, SFileReadInfo* fri = NULL) = 0;
		virtual int  Write(const char* Filename) = 0;

		virtual bool OpenReadLine(const char* Filename) = 0;
		virtual void ReadLine(void* LineBuffer) = 0;
		virtual void CloseReadLine() = 0;

		virtual bool OpenWriteLine(const char* Filename) = 0;
		virtual void WriteLine(const void* LineBuffer) = 0;
		virtual void CloseWriteLine() = 0;

		static void SetError(EGraflibError er, const char* es, const char* msg=NULL);

};

#endif
