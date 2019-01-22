//   alphatrimmedmeanfilter.cpp - impelementation of 
//   1D and 2D alpha-trimmed mean filter routines
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#include <memory.h>
#include "AlphaTrimmedMeanFilter.h"

//   1D ALPHA-TRIMMED MEAN FILTER implementation
//     signal - input signal
//     result - output signal
//     N      - length of the signal
//     alpha  - filter alpha parameter
void _alphatrimmedmeanfilter(const element* signal, element* result, int N, int alpha)
{
   //   Start of the trimmed ordered set
   const int start = alpha >> 1;
   //   End of the trimmed ordered set
   const int end = 5 - (alpha >> 1);
   //   Move window through all elements of the signal
   for (int i = 2; i < N - 2; ++i)
   {
      //   Pick up window elements
      element window[5];
      for (int j = 0; j < 5; ++j)
         window[j] = signal[i - 2 + j];
      //   Order elements (only necessary part or them)
      for (int j = 0; j < end; ++j)
      {
         //   Find position of minimum element
         int min = j;
         for (int k = j + 1; k < 5; ++k)
            if (window[k] < window[min])
               min = k;
         //   Put found minimum element in its place
         const element temp = window[j];
         window[j] = window[min];
         window[min] = temp;
      }
      //   Get result - the mean value of the elements in trimmed set
      result[i - 2] = window[start];
      for (int j = start + 1; j < end; ++j)
         result[i - 2] += window[j];
      result[i - 2] /= N - alpha;
   }
}

//   1D ALPHA-TRIMMED MEAN FILTER wrapper
//     signal - input signal
//     result - output signal
//     N      - length of the signal
//     alpha  - filter alpha parameter
void alphatrimmedmeanfilter(element* signal, element* result, int N, int alpha)
{
   //   Check arguments
   if (!signal || N < 1 || alpha < 0 || 4 < alpha || alpha & 1)
      return;
   //   Treat special case N = 1
   if (N == 1)
   {
      if (result)
         result[0] = signal[0];
      return;
   }
   //   Allocate memory for signal extension
   element* extension = new element[N + 4];
   //   Check memory allocation
   if (!extension)
      return;
   //   Create signal extension
   memcpy(extension + 2, signal, N * sizeof(element));
   for (int i = 0; i < 2; ++i)
   {
      extension[i] = signal[1 - i];
      extension[N + 2 + i] = signal[N - 1 - i];
   }
   //   Call alpha-trimmed mean filter implementation
   _alphatrimmedmeanfilter(extension + 2, result ? result : signal, N + 4, alpha);
   //   Free memory
   delete[] extension;
}

//   2D ALPHA-TRIMMED MEAN FILTER implementation
//     image  - input image
//     result - output image
//     N      - width of the image
//     M      - height of the image
//     alpha  - filter alpha parameter
void _alphatrimmedmeanfilter(element* image, element* result, int N, int M, int alpha)
{
   //   Start of the trimmed ordered set
   const int start = alpha >> 1;
   //   End of the trimmed ordered set
   const int end = 9 - (alpha >> 1);
   //   Move window through all elements of the image
   for (int m = 1; m < M - 1; ++m)
      for (int n = 1; n < N - 1; ++n)
      {
         //   Pick up window elements
         int k = 0;
         element window[9];
         for (int j = m - 1; j < m + 2; ++j)
            for (int i = n - 1; i < n + 2; ++i)
               window[k++] = image[j * N + i];
         //   Order elements (only necessary part of them)
         for (int j = 0; j < end; ++j)
         {
            //   Find position of minimum element
            int min = j;
            for (int l = j + 1; l < 9; ++l)
            if (window[l] < window[min])
               min = l;
            //   Put found minimum element in its place
            const element temp = window[j];
            window[j] = window[min];
            window[min] = temp;
         }
         //   Target index in result image
         const int target = (m - 1) * (N - 2) + n - 1;
         //   Get result - the mean value of the elements in trimmed set
         result[target] = window[start];
         for (int j = start + 1; j < end; ++j)
            result[target] += window[j];
         result[target] /= 9 - alpha;
      }
}

//   2D ALPHA-TRIMMED MEAN FILTER wrapper
//     image  - input image
//     result - output image
//     N      - width of the image
//     M      - height of the image
//     alpha  - filter alpha parameter
void alphatrimmedmeanfilter(element* image, element* result, int N, int M, int alpha)
{
   //   Check arguments
   if (!image || N < 1 || M < 1 || alpha < 0 || 8 < alpha || alpha & 1)
      return;
   //   Allocate memory for signal extension
   element* extension = new element[(N + 2) * (M + 2)];
   //   Check memory allocation
   if (!extension)
      return;
   //   Create image extension
   for (int i = 0; i < M; ++i)
   {
      memcpy(extension + (N + 2) * (i + 1) + 1, image + N * i, N * sizeof(element));
      extension[(N + 2) * (i + 1)] = image[N * i];
      extension[(N + 2) * (i + 2) - 1] = image[N * (i + 1) - 1];
   }
   //   Fill first line of image extension
   memcpy(extension, extension + N + 2, (N + 2) * sizeof(element));
   //   Fill last line of image extension
   memcpy(extension + (N + 2) * (M + 1), extension + (N + 2) * M, (N + 2) * sizeof(element));
   //   Call alpha-trimmed mean filter implementation
   _alphatrimmedmeanfilter(extension, result ? result : image, N + 2, M + 2, alpha);
   //   Free memory
   delete[] extension;
}
