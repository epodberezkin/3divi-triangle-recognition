#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>

class MedianFilter
{
public:
	MedianFilter();
	~MedianFilter();
	static void Load(const uint8_t * src, int a[3]);
	static void Load(const uint8_t * src, size_t stride, int a[9]);
	static void Sort(int & a, int & b);
	static void Sort(int a[9]);
	static void DoFilter(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);
};

