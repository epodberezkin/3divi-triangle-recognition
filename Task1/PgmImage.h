#pragma once
#include "Constants.h"
#include <random>
#include <vector>
#include <fstream>
#include <time.h>
#include <string>


class PgmImage
{
public:
	PgmImage();
	~PgmImage();
	void Plot(int, int, int);
	int Save(const std::string&);
	int Open(const std::string&);
	void MakeNoise(double);
	bool CheckPixel(int, int, int) const;
	int CheckPixelDencity() const;
	const static int abscissa = plot_bound;
	const static int ordinate = plot_bound;

private:

	double noise_probability;
	std::uniform_real_distribution<double> distribution_double;
	std::uniform_int_distribution<int> distribution_int;
	std::default_random_engine random_engine;
	std::vector<unsigned char> bitmap;
	const static int gray_level;
};