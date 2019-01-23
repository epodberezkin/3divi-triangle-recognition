#pragma once
#include <algorithm>

static const int GX[3][3] = { { -1, 0, 1 },{-2, 0, 2 },{ -1, 0, 1 } };
static const int GY[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };

class Sobel
{
public:
	Sobel();
	~Sobel();
	static void Load(const uint8_t *, uint8_t *, size_t, int);
	void static operate(const uint8_t *, uint8_t*, int, int);
private:

};