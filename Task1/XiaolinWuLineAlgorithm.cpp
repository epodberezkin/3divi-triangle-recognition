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
	//if ((int)round(x1) == (int)round(x0) || (int)round(y1) == (int)round(y0))
	//{
	//	DrawStraitLine(x0, y0, x1, y1, image);
	//	return;
	//}
	//Для Х-линии (коэффициент наклона < 1)
	if (dy < dx)
	{
		//Первая точка должна иметь меньшую координату Х

		if (x1 < x0)
		{
			x1 += x0; x0 = x1 - x0; x1 -= x0;
			y1 += y0; y0 = y1 - y0; y1 -= y0;
		}

		//Относительное изменение координаты Y
		double gradient = dy / dx;
		if (y1 < y0) gradient = -gradient;
		//Первая точка
		int x_beg = (int)round(x0);
		double y_beg = y0 + gradient * (x_beg - x0);
		double xgap = 1 - FPart(x0 + 0.5); // 0,1 - 0,4; 0,2 - 0,3; 0,3 - 0,2; 0,4 - 0,1; 0,5 - 1; 0,6 - 0,9; 0,7 - 0,8; 0,8 - 0,7, 0,9 - 0,6; 0,0 - 0,5 
		int xpxl0 = x_beg;
		int ypxl0 = IPart(y_beg);
		image.Plot(xpxl0, ypxl0,(int)(255 - FPart(y_beg) * xgap  * 255));
		image.Plot(xpxl0, ypxl0 + 1,(int)(FPart(y_beg) * xgap * 255));
		//image->Plot(xpxl0, ypxl0, 255);
		double intery = y_beg + gradient;

		// последняя точка
		int x_end = x1 < plot_bound - 1 ? IPart(x1) : (int)round(x1);
		double y_end = y1 + gradient * (x_end - x1);
		xgap = FPart(x1 + 0.5);
		int xpxl1 = x_end;
		int ypxl1 = IPart(y_end);
		image.Plot(xpxl1, ypxl1,(int)(255 - FPart(y_end) * xgap * 255));
		image.Plot(xpxl1, ypxl1 + 1, (int)(FPart(y_end) * xgap * 255));
		//image->Plot(xpxl1, ypxl1, 255);
		
		for (int x = xpxl0 + 1; x < xpxl1; x++)
		{
			//Верхняя точка
			image.Plot(x, IPart(intery), (int)(255 - FPart(intery) * 255));
			//Нижняя точка
			image.Plot(x, IPart(intery) + 1, (int)(FPart(intery) * 255));
			//Изменение координаты Y
			intery += gradient;
		}

	}
	//Для Y-линии (коэффициент наклона > 1)
	else
	{
		//Первая точка должна иметь меньшую координату Y
		if (y1 < y0)
		{
			x1 += x0; x0 = x1 - x0; x1 -= x0;
			y1 += y0; y0 = y1 - y0; y1 -= y0;
		}
		//Относительное изменение координаты X
		double gradient = dx / dy;
		if (x1 < x0) gradient = -gradient;
		
		//Первая точка
		int y_beg = (int)round(y0);
		double x_beg = x0 + gradient * (y_beg - y0);
		double ygap = 1 - FPart(y0 + 0.5); 
		int ypxl0 = y_beg;
		int xpxl0 = IPart(x_beg);
		image.Plot(xpxl0, ypxl0,(int)(255 - FPart(x_beg) * ygap  * 255));
		image.Plot(xpxl0 + 1, ypxl0,(int)(FPart(x_beg) * ygap * 255));
		//image->Plot(xpxl0, ypxl0, 255);
		double interx = x_beg + gradient;

		// последняя точка
		int y_end = y1 < plot_bound - 1 ? IPart(y1) : (int)round(y1);
		double x_end = x1 + gradient * (y_end - y1);
		ygap = FPart(y1 + 0.5);
		int ypxl1 = y_end;
		int xpxl1 = IPart(x_end);
		image.Plot(xpxl1, ypxl1, (int)(255 - FPart(x_end) * ygap * 255));
		image.Plot(xpxl1 + 1, ypxl1, (int)(FPart(x_end) * ygap * 255));
		//image->Plot(xpxl1, ypxl1, 255);

		for (int y = ypxl0 + 1; y < ypxl1; y++)
		{
			//Верхняя точка
			image.Plot(IPart(interx), y, 255 - (int)(FPart(interx) * 255));
			//Нижняя точка
			image.Plot(IPart(interx) + 1, y, (int)(FPart(interx) * 255));
			//Изменение координаты X
			interx += gradient;
		}
	}
}

//void XiaolinWuLineAlgorithm::DrawStraitLine(double x0, double y0, double x1, double y1, PgmImage& image)
//{
//}

//Целая часть числа
int XiaolinWuLineAlgorithm::IPart(double x)
{
	return (int)x;
}
//дробная часть числа
double XiaolinWuLineAlgorithm::FPart(double x)
{
	return x - (double)IPart(x);
}

/*int  XiaolinWuLineAlgorithm::round(double x)
{
	return ((int)(((double)(x)) + 0.5));
}*/

