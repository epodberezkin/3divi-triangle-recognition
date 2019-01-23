#include "Sobel.h"



Sobel::Sobel()
{
}


Sobel::~Sobel()
{
}

inline void Sobel::Load(const uint8_t * src, uint8_t * a, size_t width, int pos)
{
	a[0] = src[pos - width -1];
	a[1] = src[pos - width];
	a[2] = src[pos - width + 1];
	a[3] = src[pos -1];
	a[4] = src[pos];
	a[5] = src[pos + 1];
	a[6] = src[pos + width -1];
	a[7] = src[pos + width];
	a[8] = src[pos + width + 1];
}

void Sobel::operate(const uint8_t * src, uint8_t * dst, int width, int height)
{
	//TODO: переписать с Load
	//uint8_t a[9];
	int* accum = new int[width*height]();
	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			//Load(src, a, width, x + y * width);
			int pixel_x = GX[0][0] * src[x - 1 + (y - 1)*width] + GX[0][1] * src[x + (y - 1)*width] + GX[0][2] * src[x + 1 + (y - 1)*width] +
				GX[1][0] * src[x - 1 + y * width] + GX[1][1] * src[x + y * width] + GX[1][2] * src[x + 1 + y * width] +
				GX[2][0] * src[x - 1 + (y + 1)*width] + GX[2][1] * src[x + (y + 1)*width] + GX[2][2] * src[x + 1 + (y + 1)*width];

			int pixel_y = GY[0][0] * src[x - 1 + (y - 1)*width] + GY[0][1] * src[x + (y - 1)*width] + GY[0][2] * src[x + 1 + (y - 1)*width] +
				GY[1][0] * src[x - 1 + y * width] + GY[1][1] * src[x + y * width] + GY[1][2] * src[x + 1 + y * width] +
				GY[2][0] * src[x - 1 + (y + 1)*width] + GY[2][1] * src[x + (y + 1)*width] + GY[2][2] * src[x + 1 + (y + 1)*width];

			accum[x + y * width] = static_cast<int>(sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y)));
		}
	}
	int * max = std::max_element(accum, accum + width * height);
	for (int x = 0; x < width*height; x++)
	{
		dst[x] = accum[x] * 255 / *max;
	}
}
