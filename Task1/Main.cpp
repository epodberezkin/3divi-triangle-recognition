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

		XiaolinWuLineAlgorithm::DrawWuLine(random_triangle.get_vertex_A().first, random_triangle.get_vertex_A().second, random_triangle.get_vertex_B().first, random_triangle.get_vertex_B().second, triangle_image);
		XiaolinWuLineAlgorithm::DrawWuLine(random_triangle.get_vertex_B().first, random_triangle.get_vertex_B().second, random_triangle.get_vertex_C().first, random_triangle.get_vertex_C().second, triangle_image);
		XiaolinWuLineAlgorithm::DrawWuLine(random_triangle.get_vertex_C().first, random_triangle.get_vertex_C().second, random_triangle.get_vertex_A().first, random_triangle.get_vertex_A().second, triangle_image);

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
			//h_transf.Save("after.pgm");
			h_transf.CalculateVertexes();
			h_transf.SaveVertexes("output.txt");
		}
	}
	return 0;
}