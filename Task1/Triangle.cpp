

#include "Triangle.h"


using namespace std;

const double Triangle::PI = 3.141592653589793238463;


Triangle::Triangle() {

	re.seed((unsigned long)time(nullptr));
	unif = uniform_real_distribution<>(0,(double)plot_bound);
	do {
		vertex_A.first = unif(re);
		vertex_A.second = unif(re);
		vertex_B.first = unif(re);
		vertex_B.second = unif(re);
		vertex_C.first = unif(re);
		vertex_C.second = unif(re);
	} while (!(IsValid()));
}

Triangle::~Triangle() {}

pair<double,double> Triangle::get_vertex_A()
{
	return vertex_A;
}

pair<double,double> Triangle::get_vertex_B()
{
	return vertex_B;
}

pair<double,double> Triangle::get_vertex_C()
{
	return vertex_C;
}

double Triangle::get_angle_A()
{
	return angle_A;
}

double Triangle::get_angle_B()
{
	return angle_B;
}

double Triangle::get_angle_C()
{
	return angle_C;
}

int Triangle::Save(const string& file_name)
{
	ofstream file(file_name, ios_base::out | ios_base::trunc);
	if (!file.is_open()) return -1; // если файл не открыт
	file << this->get_vertex_A().first << " " << this->get_vertex_A().second << endl;
	file << this->get_vertex_B().first << " " << this->get_vertex_B().second << endl;
	file << this->get_vertex_C().first << " " << this->get_vertex_C().second << endl;
	//file << this->get_angle_A() << "% " << this->get_angle_B() << "% " << this->get_angle_C() << "%" << endl;
	file.close();
	return 0;
}

bool Triangle::IsValid() {
	
	side_A = SideLength(vertex_B, vertex_C);
	side_B = SideLength(vertex_C, vertex_A);
	side_C = SideLength(vertex_A, vertex_B);
	if (side_A < 100 || side_B < 100 || side_C < 100) return false;

	angle_A = VertexAngle(side_A, side_B, side_C);
	angle_B = VertexAngle(side_B, side_C, side_A);
	angle_C = VertexAngle(side_C, side_A, side_B);
	return !(angle_A < 30 || angle_B < 30 || angle_C < 30);
}

double Triangle::SideLength(pair<double, double> vertex_X, pair<double, double> vertex_Y) {
	return sqrt(pow((vertex_X.first - vertex_Y.first), 2) + pow((vertex_X.second - vertex_Y.second), 2));
};

double Triangle::VertexAngle(double a, double b, double c) {
	return 180 / PI * acos((pow(b, 2) + pow(c, 2) - pow(a, 2))/(2 * b * c));
};