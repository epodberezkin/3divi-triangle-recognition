   //   alphatrimmedmeanfilter.h - declarations for 
   //   1D and 2D alpha-trimmed mean filter routines
   //
   //   The code is property of LIBROW
   //   You can use it on your own
   //   When utilizing credit LIBROW site

   #ifndef _ALPHATRIMMEDMEANFILTER_H_
   #define _ALPHATRIMMEDMEANFILTER_H_

   //   Signal/image element type
   typedef unsigned char element;

   //   1D ALPHA-TRIMMED MEAN FILTER, window size 5
   //     signal - input signal
   //     result - output signal, NULL for inplace processing
   //     N      - length of the signal
   //     alpha  - filter alpha parameter
   void alphatrimmedmeanfilter(element* signal, element* result, int N, int alpha);

   //   2D ALPHA-TRIMMED MEAN FILTER, window size 3x3
   //     image  - input image
   //     result - output image, NULL for inplace processing
   //     N      - width of the image
   //     M      - height of the image
   //     alpha  - filter alpha parameter
   void alphatrimmedmeanfilter(element* image, element* result, int N, int M, int alpha);

   #endif
