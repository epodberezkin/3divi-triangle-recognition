// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MedianFilter.h"

using namespace std;

MedianFilter::MedianFilter()
{
}


MedianFilter::~MedianFilter()
{
}


inline void MedianFilter::Load(const uint8_t * src, int a[3])
{
	a[0] = src[-1];
	a[1] = src[0];
	a[2] = src[1];
}

inline void MedianFilter::Load(const uint8_t * src, size_t stride, int a[9])
{
	Load(src - stride, a + 0);
	Load(src, a + 3);
	Load(src + stride, a + 6);
}

inline void MedianFilter::Sort(int & a, int & b)
{
	int d = a - b;
	int m = ~(d >> 8);
	b += d & m;
	a -= d & m;
}

inline void MedianFilter::Sort(int a[9]) //частично сортирует весь массив
{
	std::sort(a, a + 9);

	/*
	Sort(a[1], a[2]); Sort(a[4], a[5]); Sort(a[7], a[8]);
	Sort(a[0], a[1]); Sort(a[3], a[4]); Sort(a[6], a[7]);
	Sort(a[1], a[2]); Sort(a[4], a[5]); Sort(a[7], a[8]);
	Sort(a[0], a[3]); Sort(a[5], a[8]); Sort(a[4], a[7]);
	Sort(a[3], a[6]); Sort(a[1], a[4]); Sort(a[2], a[5]);
	Sort(a[4], a[7]); Sort(a[4], a[2]); Sort(a[6], a[4]);
	Sort(a[4], a[2]);
	*/
}

void MedianFilter::DoFilter(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride)
{
	int a[9];
	int new_center_index, old_cener_index, old_center;
	for (size_t y = 0; y < height; ++y)
	{
		for (size_t x = 0; x < width; ++x)
		{
			Load(src + x, srcStride, a);
			old_center = a[4];
			Sort(a);
			old_cener_index = std::lower_bound(a, a+9, old_center) - a;
			new_center_index = 4;
			if (old_cener_index != 4)
			{
				new_center_index = (old_cener_index > 4) ? old_cener_index - 1 : old_cener_index + 1;
			}
			dst[x] = (uint8_t)a[4];
		}
		src += srcStride;
		dst += dstStride;
	}
}