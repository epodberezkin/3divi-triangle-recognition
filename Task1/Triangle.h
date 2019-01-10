#pragma once

#include "Constants.h"
#include <fstream>
#include <utility>
#include <time.h>
#include <cmath>
#include <random>

class Triangle
{
public:
	Triangle();
	~Triangle();
	std::pair<double,double> get_vertex_A();
	std::pair<double,double> get_vertex_B();
	std::pair<double,double> get_vertex_C();
	double get_angle_A();
	double get_angle_B();
	double get_angle_C();
	int Save(const std::string&);

private:
	bool IsValid();
	double SideLength(std::pair<double, double>, std::pair<double, double>);
	double VertexAngle(double, double, double);
	
	const static double PI;

	std::uniform_real_distribution<double> unif;
	std::default_random_engine re;
	double side_A, side_B, side_C;
	double angle_A, angle_B, angle_C;
	std::pair<double,double> vertex_A, vertex_B, vertex_C;
};