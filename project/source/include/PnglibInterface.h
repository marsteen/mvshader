//***************************************************************************
//
//
// @PROJECT  :	Basic SDL
// @VERSION  :	1.0
// @FILENAME :	PnglibInterface.h
// @DATE     :	8.1.2015
//
// @AUTHOR   :	Martin Steen
// @EMAIL    :	martin@martin-steen.de
//
//
//***************************************************************************


#ifndef PNGLIBINTERFACE_H
#define PNGLIBINTERFACE_H

/*
 * typedef struct
 * {
 *  png_structp png_ptr;
 *  png_infop   info_ptr;
 * }
 * PngInfoStruct;
 */

typedef struct
{
    png_structp		png_ptr;
    png_infop		info_ptr;
    png_color*		palette;
    int				num_palette;
    unsigned char*	trans_pal;
    int				num_trans;
    int				bit_depth;
    int				color_type;
    int				interlace_type;
    int				compression_type;
    int				filter_method;
}
PngInfoStruct;



void read_png(FILE* fp, unsigned int sig_read, PngInfoStruct* pinf);
int write_png(const char* file_name, int Width, int Height, int ByteProPixel, void* PixelData);
int write_pngGrayscale(const char* file_name, int Width, int Height, void* PixelData, int ByteProPixel);
void finish_read(PngInfoStruct* pinf);

#endif
