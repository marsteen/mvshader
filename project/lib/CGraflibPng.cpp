//------------------------------------------------------------------------------
//
// PROJECT : Imagon
//
// FILE    : CGraflibPng.cpp
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
// Inhalt: Deklaration der Klasse CGraflibPng
//
//------------------------------------------------------------------------------
// Revisionsgeschichte:
//
// 08.01.2001 erstellt - Martin Steen
//
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <CFileIO.h>

#include <CPixel24.h>
#include <SFileReadInfo.h>
#include <CGraflibPng.h>
#ifdef _WIN32
#include <libpng/png.h>
#else
#include <png.h>
#endif
#include <GlobalDebug.h>


using namespace std;

extern "C"
{
  #include <PnglibInterface.h>
};

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   TestPalette
//
//
//---------------------------------------------------------------------------

void CGraflibPng::TestPalette(const void* vpinfo)
{
	const PngInfoStruct* pinfo = (const PngInfoStruct*) vpinfo;

	if (pinfo->info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
	{
		if (pinfo->palette != NULL)
		{
			if (pinfo->trans_pal != NULL)
			{
				ConvertPalToRGBA((CPixel24*) pinfo->palette, pinfo->trans_pal);
			}
			else
			{
				switch (mBits)
				{
					case 1:

						Convert1BitPalToRGB((CPixel24*) pinfo->palette);
						break;

					case 4:

						Convert4BitPalToRGB((CPixel24*) pinfo->palette);
						break;


					case 8:

						Convert8BitPalToRGB((CPixel24*) pinfo->palette);
						break;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   Read
//
//
//---------------------------------------------------------------------------

bool CGraflibPng::Read(const char* Filename, SFileReadInfo* fri)
{
	int r;



	PngInfoStruct pinfo;
	int FileSize;
	unsigned char* PicData = NULL;

	FILE* fp = (FILE*) fopen(Filename, "rb");

	//GlobalDebug("  open ok");

	if (fp == NULL)
	{

	}
	else
	{
		read_png(fp, 0, &pinfo);
		//GlobalDebug("  read_png ok");





		mWidth  = pinfo.info_ptr->width;
		mHeight = pinfo.info_ptr->height;


		mBits   = pinfo.bit_depth *  pinfo.info_ptr->channels;
		int ByteProPixel = mBits / 8;
		mData   = (CPixel*) new unsigned char[pinfo.info_ptr->width *  (pinfo.info_ptr->height * mBits) / 8];

		unsigned char* PicDataPtr = (unsigned char*) mData;

		stringstream mstr;
		mstr << "File:" << Filename << "  W=" << mWidth << " H=" << mHeight << " Bits=" << mBits;
		GlobalDebug(mstr, DBG_TEMP2);

		int BytesPerLine = (mWidth * mBits) / 8;

		for (int i = 0; i < mHeight; i++)
		{
			memcpy(PicDataPtr, pinfo.info_ptr->row_pointers[i], BytesPerLine);
			PicDataPtr += BytesPerLine;
		}
		TestPalette(&pinfo);


		finish_read(&pinfo);
		r = true;



	}




	return r;
}






//---------------------------------------------------------------------------
//
// Klasse:    CGraflibPng
// Methode:   Write
//
//
//---------------------------------------------------------------------------

int CGraflibPng::Write(const char* Filename, bool bComp, CFileIO* fio)
{

	#if 1
	switch (mBits)
	{

		case  8: write_pngGrayscale(Filename, mWidth, mHeight, mData, 1);
		         break;

		case 16: write_pngGrayscale(Filename, mWidth, mHeight, mData, 2);
		         break;

		case 24: write_png(Filename, mWidth, mHeight, 3, mData);
		         break;



		case 32: write_png(Filename, mWidth, mHeight, 4, mData);
		         break;
	}
	#endif
}

