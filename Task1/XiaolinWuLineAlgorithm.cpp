#include "XiaolinWuLineAlgorithm.h"


XiaolinWuLineAlgorithm::XiaolinWuLineAlgorithm()
{
}


XiaolinWuLineAlgorithm::~XiaolinWuLineAlgorithm()
{
}


void XiaolinWuLineAlgorithm::DrawWuLine(double x0, double y0, double x1, double y1, PgmImage& image)
{
	double dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	double dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);

	//Если линия параллельна одной из осей, рисуем обычную линию - заполняем все пикселы в ряд
	if (round(x1) == round(x0) || round(y1) == round(y0))
	{
		DrawStraitLine(x0, y0, x1, y1, image);
		return;
	}

	//Для Х-линии (коэффициент наклона < 1)
	if (dy < dx)
	{
		//Первая точка должна иметь меньшую координату Х
		if (x1 < x0) std::swap(x0, x1);
		
		//Относительное изменение координаты Y
		double gradient = dy / dx;
		if (y1 < y0) gradient = -gradient;
		//Первая точка
		int x_beg = static_cast<int>(round(x0));
		double y_beg = y0 + gradient * (x_beg - x0);
		double xgap = 1 - FPart(x0 + 0.5); 
		int xpxl0 = x_beg;
		int ypxl0 = IPart(y_beg);
		image.Plot(xpxl0, ypxl0, static_cast<int>(PgmImage::grayscale - FPart(y_beg) * xgap  * PgmImage::grayscale));
		image.Plot(xpxl0, ypxl0 + 1, static_cast<int>(FPart(y_beg) * xgap * PgmImage::grayscale));
		double intery = y_beg + gradient;

		// последняя точка
		int x_end = (x1 > PgmImage::abscissa - 1) ? IPart(x1) : static_cast<int>(round(x1));
		double y_end = y1 + gradient * (x_end - x1);
		xgap = FPart(x1 + 0.5);
		int xpxl1 = x_end;
		int ypxl1 = IPart(y_end);
		image.Plot(xpxl1, ypxl1, static_cast<int>((1 - FPart(y_end) * xgap) * PgmImage::grayscale));
		image.Plot(xpxl1, ypxl1 + 1, static_cast<int>(FPart(y_end) * xgap * PgmImage::grayscale));
		
		for (int x = xpxl0 + 1; x < xpxl1; x++)
		{
			//Верхняя точка
			image.Plot(x, IPart(intery), static_cast<int>((1 - FPart(intery)) * PgmImage::grayscale));
			//Нижняя точка
			image.Plot(x, IPart(intery) + 1, static_cast<int>(FPart(intery) * PgmImage::grayscale));
			//Изменение координаты Y
			intery += gradient;
		}

	}
	//Для Y-линии (коэффициент наклона > 1)
	else
	{
		//Первая точка должна иметь меньшую координату Y
		if (y1 < y0) std::swap(y0, y1);
		//Относительное изменение координаты X
		double gradient = dx / dy;
		if (x1 < x0) gradient = -gradient;
		
		//Первая точка
		int y_beg = static_cast<int>(round(y0));
		double x_beg = x0 + gradient * (y_beg - y0);
		double ygap = 1 - FPart(y0 + 0.5); 
		int ypxl0 = y_beg;
		int xpxl0 = IPart(x_beg);
		image.Plot(xpxl0, ypxl0, static_cast<int>((1 - FPart(x_beg) * ygap) * PgmImage::grayscale));
		image.Plot(xpxl0 + 1, ypxl0, static_cast<int>(FPart(x_beg) * ygap * PgmImage::grayscale));
		double interx = x_beg + gradient;

		// последняя точка
		int y_end = (y1 > PgmImage::ordinate - 1) ? IPart(y1) : static_cast<int>(round(y1));
		double x_end = x1 + gradient * (y_end - y1);
		ygap = FPart(y1 + 0.5);
		int ypxl1 = y_end;
		int xpxl1 = IPart(x_end);
		image.Plot(xpxl1, ypxl1, static_cast<int>((1 - FPart(x_end) * ygap) * PgmImage::grayscale));
		image.Plot(xpxl1 + 1, ypxl1, static_cast<int>(FPart(x_end) * ygap * PgmImage::grayscale));

		for (int y = ypxl0 + 1; y < ypxl1; y++)
		{
			//Верхняя точка
			image.Plot(IPart(interx), y, static_cast<int>((1 - FPart(interx)) * PgmImage::grayscale));
			//Нижняя точка
			image.Plot(IPart(interx) + 1, y, static_cast<int>(FPart(interx) * PgmImage::grayscale));
			//Изменение координаты X
			interx += gradient;
		}
	}
}

void XiaolinWuLineAlgorithm::DrawStraitLine(double x0, double y0, double x1, double y1, PgmImage& image)
{
	if (round(x1) == round(x0))
	{
		int dx = (x0 < PgmImage::abscissa - 1) ? static_cast<int>(round(x0)) : IPart(x0);
		if (y0 > y1) std::swap(y0, y1);
		int y_max = (y1 < PgmImage::ordinate - 1) ? static_cast<int>(round(y1)) : IPart(y1);
		for (int dy = static_cast<int>(round(y0)); dy <= y_max; dy++)
		{
			image.Plot(dx, dy, PgmImage::grayscale);
		}
	}
	else if (round(y1) == round(y0))
	{
		int dy = (y0 < PgmImage::ordinate - 1) ? static_cast<int>(round(y0)) : IPart(y0);
		if (x0 > x1) std::swap(x0, x1);
		int x_max = (x1 < PgmImage::abscissa - 1) ? static_cast<int>(round(x1)) : IPart(x1);
		for (int dx = static_cast<int>(round(x0)); dx <= x_max; dx++)
		{
			image.Plot(dx, dy, PgmImage::grayscale);
		}
	}
}

//Целая часть числа
int XiaolinWuLineAlgorithm::IPart(double x)
{
	return static_cast<int>(x);
}
//дробная часть числа
double XiaolinWuLineAlgorithm::FPart(double x)
{
	return x - IPart(x);
}
