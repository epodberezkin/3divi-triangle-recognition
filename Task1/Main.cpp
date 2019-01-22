// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//#include <sstream>
//#include <time.h>
//#include <iomanip>
#include <stdexcept> // std::invalid_argument
#include <iostream> //std::cerr
#include "InputParser.h"
#include "Triangle.h"
#include "PgmImage.h"
#include "XiaolinWuLineAlgorithm.h"
#include "HoughTransform.h"

using namespace std;

int main(int argc, char **argv) {
	InputParser input(argc, argv);
	if (input.cmdOptionExists("-generate")) {
		const std::string &noise_level = input.getCmdOption("-generate");
		std::string::size_type sz;	// alias of size_t
		double noise_probability = 0.0;
		try
		{
			noise_probability = std::stod(noise_level, &sz);
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << '\n';
		}
		Triangle random_triangle;
		PgmImage triangle_image;

		std::pair<double, double> vertex_A = random_triangle.get_vertex_A();
		std::pair<double, double> vertex_B = random_triangle.get_vertex_B();
		std::pair<double, double> vertex_C = random_triangle.get_vertex_C();

		XiaolinWuLineAlgorithm::DrawWuLine(vertex_A.first, vertex_A.second, vertex_B.first, vertex_B.second, triangle_image);
		XiaolinWuLineAlgorithm::DrawWuLine(vertex_B.first, vertex_B.second, vertex_C.first, vertex_C.second, triangle_image);
		XiaolinWuLineAlgorithm::DrawWuLine(vertex_C.first, vertex_C.second, vertex_A.first, vertex_A.second, triangle_image);

		triangle_image.MakeNoise(noise_probability);
		
		/*
		time_t t = std::time(nullptr);
		#pragma warning(suppress : 4996)
		tm* result = localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(result, "%d-%m-%Y %H-%M-%S") << "-noise-" << noise_probability << ".txt";
		random_triangle.Save(oss.str());
		*/

		triangle_image.Save("image.pgm");
	}

	if (input.cmdOptionExists("-restore")) {
		const std::string &filename = input.getCmdOption("-restore");
		if (!filename.empty()) {
			PgmImage triangle_image;
			triangle_image.Open(filename);
			HoughTransform h_transf;
			h_transf.MakeTransform(triangle_image);
			//h_transf.Save("before.pgm");
			h_transf.FindLines(3);
			h_transf.CalculateVertexes();
			h_transf.SaveVertexes("output.txt");
		}
	}
	return 0;
}