
#ifndef CBLOCKOPERATIONS_H
#define CBLOCKOPERATIONS_H

#include <CFileIO.h>
#include <CDataRect.h>

class CBlockOperations
{
	public:

    static void WriteBlock(CFileIO* fio, CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int BytesProPixel);
    static void CopyBlock(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int BytesProPixel);
    static void CopyBlock2(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos, int CopyX, int CopyY, int BytesProPixel);
    static void CopyBlock2(CDataRect* dstRect, const CDataRect* srcRect, int Xpos, int Ypos);

};

#endif
