#pragma once
#include "PgmImage.h"

class XiaolinWuLineAlgorithm
{
public:
	XiaolinWuLineAlgorithm();
	~XiaolinWuLineAlgorithm();
	static void DrawWuLine(double, double, double, double, PgmImage&);
	static void DrawStraitLine(double, double, double, double, PgmImage&);
private:
	static int IPart(double);
	static double FPart(double);
	//static int round(double);
};

