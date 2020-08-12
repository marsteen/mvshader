/*

 PNG LIB INTERFACE

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define png_infopp_NULL (png_infopp)NULL
#define png_voidp_NULL (png_voidp)NULL
#define int_p_NULL (int*)NULL

#ifdef _WIN32
#include <libpng/png.h>
#else
#include <png.h>
#endif

#include <PnglibInterface.h>

#ifndef png_jmpbuf
#  define png_jmpbuf(png_ptr) ((png_ptr)->jmpbuf)
#endif

void (*PngStatusFunction)(int Status);

static void SendPngStatus(int Status)
{
	if (PngStatusFunction != NULL)
	{
		(*PngStatusFunction)(Status);
	}
}

#define PNG_BYTES_TO_CHECK 4
static int check_if_png(char *file_name, FILE **fp)
{
   char buf[PNG_BYTES_TO_CHECK];

   /* Open the prospective PNG file. */
   if ((*fp = fopen(file_name, "rb")) == NULL)
      return 0;

   /* Read in some of the signature bytes */
   if (fread(buf, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK)
      return 0;

   /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
      Return nonzero (true) if they match */

   return(!png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}



void png_get_info(PngInfoStruct* pinf)
{
	png_uint_32 w, h;

pinf->bit_depth = 0;
 png_get_IHDR(pinf->png_ptr, pinf->info_ptr, &w, &h,
       &pinf->bit_depth, &pinf->color_type, &pinf->interlace_type,
       &pinf->compression_type, &pinf->filter_method);

//	printf("pinf->bit_depth=%d\n", pinf->bit_depth);
}

//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:   read_png
//
//
//---------------------------------------------------------------------------



/*************************************************************************/

/* Read a PNG file.  You may want to return an error code if the read
 * fails (depending upon the failure).  There are two "prototypes" given
 * here - one where we are given the filename, and we need to open the
 * file, and the other where we are given an open file (possibly with
 * some or all of the magic bytes read - see comments above).
 */
void read_png(FILE *fp, unsigned int sig_read, PngInfoStruct* pinf)  /* file is already open */
{
   //png_structp png_ptr;
   //png_infop info_ptr;
   png_uint_32 width, height;
   int bit_depth, color_type, interlace_type;

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also supply the
    * the compiler header file version, so that we know if the application
    * was compiled with a compatible version of the library.  REQUIRED
    */
   //png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
   //   png_voidp user_error_ptr, user_error_fn, user_warning_fn);

	  pinf->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);


   if (pinf->png_ptr == NULL)
   {
      fclose(fp);
      return;
   }

   /* Allocate/initialize the memory for image information.  REQUIRED. */
   pinf->info_ptr = png_create_info_struct(pinf->png_ptr);
   if (pinf->info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_read_struct(&(pinf->png_ptr), png_infopp_NULL, png_infopp_NULL);
      return;
   }

   /* Set error handling if you are using the setjmp/longjmp method (this is
    * the normal method of doing things with libpng).  REQUIRED unless you
    * set up your own error handlers in the png_create_read_struct() earlier.
    */

   if (setjmp(png_jmpbuf(pinf->png_ptr)))
   {
      /* Free all of the memory associated with the png_ptr and info_ptr */
      png_destroy_read_struct(&pinf->png_ptr, &pinf->info_ptr, png_infopp_NULL);
      //png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
      fclose(fp);
      /* If we get here, we had a problem reading the file */
      return;
   }


   /* Set up the input control if you are using standard C streams */
   //png_init_io(png_ptr, fp);
   png_init_io(pinf->png_ptr, fp);


   /* If we have already read some of the signature */
   png_set_sig_bytes(pinf->png_ptr, sig_read);



	 png_read_png(pinf->png_ptr, pinf->info_ptr, 0, png_voidp_NULL);
	 png_get_info(pinf);


	 //
	 // Anfang Palette
	 //

   pinf->palette   = NULL;
   pinf->trans_pal = NULL;

#if 0
		if (pinf->info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
		{
			//cout << "!!! png_get_PLTE" << endl;
			png_get_PLTE(pinf->png_ptr, pinf->info_ptr, &pinf->palette, &pinf->num_palette);
			//cout << "pinf->num_palette=" << pinf->num_palette << endl;
		}
		else
		{
			pinf->palette = NULL;
		}

		if(png_get_valid(pinf->png_ptr, pinf->info_ptr, PNG_INFO_tRNS))
		{
			if (pinf->info_ptr->valid & PNG_INFO_tRNS)
			{
				png_bytep     trans;
				int           num_trans;
				png_color_16p trans_values;

				//cout << "!!! png_get_tRNS"<< endl;

				png_get_tRNS(pinf->png_ptr, pinf->info_ptr, &trans, &num_trans, &trans_values);

				//cout << "num_trans=" << num_trans << endl;
				//cout << "trans[0]=" << (int) trans[0] << endl;
				//cout << "trans_values=" << (int) trans_values[0].index << endl;

        /*
				for (int n = 0; n < num_trans; n++)
				{
					cout << "  " << n << " trans=" << (int) trans[n] << endl;
				}
				*/
				pinf->trans_pal = (unsigned char*) malloc(256);
				memset(pinf->trans_pal, 255, 256);
				pinf->num_trans = num_trans;
				memcpy(pinf->trans_pal, trans, num_trans);
			}
			else
			{
				pinf->trans_pal = NULL;
			}
		}
#endif
   //
	 // Ende Palette
	 //

   fclose(fp);

   SendPngStatus(1);

   /* that's it */
   return;
}

//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:   write_png
//
//
//---------------------------------------------------------------------------

int write_png(const char* file_name, int Width, int Height, int BytePerPixel, void* PixelData)
{
   FILE* fp;
   int   i;
   png_bytepp RowPointers;
   png_structp png_ptr;
   png_infop info_ptr;
   png_colorp palette;
	 int png_transforms = PNG_TRANSFORM_BGR;
   /* open the file */
   fp = fopen(file_name, "wb");
   if (fp == NULL)
   {
     return 0;
	 }

	 RowPointers = (png_bytepp) malloc(Height * sizeof(void*));

	 for (i = 0; i < Height; i++)
	 {
		 RowPointers[i] = ((char*) PixelData) + (i * Width * BytePerPixel);
	 }

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible with the one used at compile time,
    * in case we are using dynamically linked libraries.  REQUIRED.
    */

   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return 0;
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
      return 0;
   }

   png_set_rows(png_ptr, info_ptr, RowPointers);

   switch (BytePerPixel)
   {
		 case 3:

			 png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGB,
					PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
			 break;


		 case 4:

			 png_set_IHDR(png_ptr, info_ptr, Width, Height, 8, PNG_COLOR_TYPE_RGBA,
					PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
			 break;

			default:

				/* unsupported Pixel format */
				break;

		}



   /* Set error handling.  REQUIRED if you aren't supplying your own
    * error handling functions in the png_create_write_struct() call.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* If we get here, we had a problem reading the file */
      fclose(fp);
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return 0;
   }

   png_init_io(png_ptr, fp);

   /* This is the easy way.  Use it if you already have all the
    * image info living info in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */
   png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);


   /* clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   free(RowPointers);

   /* close the file */
   fclose(fp);

   /* that's it */
   return 1;
}

//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:   write_pngGrayscale
//
//
//---------------------------------------------------------------------------

int write_pngGrayscale(const char* file_name, int Width, int Height, void* PixelData, int ByteProPixel)
{
   FILE* fp;
   int   i;
   png_bytepp   RowPointers;
   png_structp  png_ptr;
   png_infop    info_ptr;
   png_colorp   palette;
	 int png_transforms = PNG_TRANSFORM_BGR;
   /* open the file */
   fp = fopen(file_name, "wb");
   if (fp == NULL)
   {
     return 0;
	 }

	 RowPointers = (png_bytepp) malloc(Height * 4);

	 for (i = 0; i < Height; i++)
	 {
		 RowPointers[i] = ((char*) PixelData) + (i * Width * ByteProPixel);
	 }

   /* Create and initialize the png_struct with the desired error handler
    * functions.  If you want to use the default stderr and longjump method,
    * you can supply NULL for the last three parameters.  We also check that
    * the library version is compatible with the one used at compile time,
    * in case we are using dynamically linked libraries.  REQUIRED.
    */

   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (png_ptr == NULL)
   {
      fclose(fp);
      return 0;
   }

   /* Allocate/initialize the image information data.  REQUIRED */
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      fclose(fp);
      png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
      return 0;
   }

   png_set_rows(png_ptr, info_ptr, RowPointers);

   png_set_IHDR(png_ptr, info_ptr, Width, Height, ByteProPixel * 8, PNG_COLOR_TYPE_GRAY,
      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);


   /* Set error handling.  REQUIRED if you aren't supplying your own
    * error handling functions in the png_create_write_struct() call.
    */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      /* If we get here, we had a problem reading the file */
      fclose(fp);
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return 0;
   }

   png_init_io(png_ptr, fp);

   /* This is the easy way.  Use it if you already have all the
    * image info living info in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */
   png_write_png(png_ptr, info_ptr, png_transforms, png_voidp_NULL);


   /* clean up after the write, and free any memory allocated */
   png_destroy_write_struct(&png_ptr, &info_ptr);

   free(RowPointers);

   /* close the file */
   fclose(fp);

   /* that's it */
   return 1;
}





//---------------------------------------------------------------------------
//
// Klasse:    global
// Methode:   finish_read
//
//
//---------------------------------------------------------------------------


void finish_read(PngInfoStruct* pinf)
{
  png_destroy_read_struct(&pinf->png_ptr, &pinf->info_ptr, png_infopp_NULL);
  free(pinf->trans_pal);
}


