#pragma once
#include "Constants.h"
#include "PgmImage.h"
#include <cmath>
#include <fstream>

class HoughTransform
{
public:
	HoughTransform();
	~HoughTransform();
	int Save(const std::string&);
	void MakeTransform(const PgmImage&);
	void FindLines(int);
	void CalculateVertexes();
	int SaveVertexes(const std::string&);
	const std::vector<std::pair<double, double>>& GetVertexes();
private:
	std::vector<std::pair<double, double>> vertexes;
	std::vector<std::pair<int, int>> lines;
	std::vector<int> accum_array;
	void NormalizeAccumArray();
	std::pair<int, int> FindAccumMax();
	void ErasePeak(int,int,int);
	const static int r_max;
	const static int theta_max;
	const static double PI;
};