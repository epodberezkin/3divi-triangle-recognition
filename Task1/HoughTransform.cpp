#include "HoughTransform.h"

const int HoughTransform::r_max = int(sqrt(pow(plot_bound, 2) * 2) * 2);
const double HoughTransform::PI = 3.141592653589793238463;
const int HoughTransform::theta_max = 180;


using namespace std;

HoughTransform::HoughTransform() : accum_array(theta_max * r_max,0)
{
}

HoughTransform::~HoughTransform()
{
}

void HoughTransform::MakeTransform(const PgmImage& original_image)
{
	//проверка фильтра CheckPixel
	//std::vector<unsigned char> check_bitmap(plot_bound * plot_bound, 0);

	int	pixel_density = original_image.CheckPixelDencity();
	// пробегаемся по пикселям изображения контуров
	for (int y = 0; y < original_image.ordinate; y++) {
		for (int x = 0; x < original_image.abscissa; x++) {
			if (original_image.CheckPixel(x, y, pixel_density))  // это возможный пиксель контура?
			{
				//проверка фильтра CheckPixel
				//check_bitmap[x + y * plot_bound] = 255;
				
				// рассмотрим все возможные прямые, которые могут проходить через эту точку
				for (int f = 0; f < theta_max; f++) { //перебираем все возможные углы наклона
					double theta = f * PI / 180.0;
					double r = (y)*sin(theta) + (x)*cos(theta);
					//если r укладывается в диапазон возможных значений, голосуем
					if ( (r < r_max / 2) && (r >= -r_max / 2))
						this->accum_array[static_cast<int>(round(r + r_max / 2) * theta_max + f)]++;
				}
			}
		}
	}
	/* проверка фильтра CheckPixel
	ofstream file("test_check.pgm", ios_base::out | ios::binary | ios_base::trunc);
	if (!file.is_open()) return; // если файл не открыт
	file << "P5\n" << plot_bound << " " << plot_bound << "\n" << gray_scale << "\n";
	file.write((char *)&check_bitmap[0], check_bitmap.size());
	file.close();
	*/
}

void HoughTransform::FindLines(int count)
{
	//находим count максимумов в цикле, зануляя предыдущий в квадрате +-10 
	//зная что углы треугольника точно >30, плюс некоторая погрешность округления, приведения типов и арифметических операций (r может совпадать), точки должныть быть на расстоянии больше 20.
	std::pair<int, int> max_coordinates;
	for (int i = 0; i < count; i++)
	{
		max_coordinates = FindAccumMax();
		this->ErasePeak(max_coordinates.first, max_coordinates.second, 10);
		max_coordinates.second = max_coordinates.second - r_max / 2;
		this->lines.push_back(max_coordinates);
	}	
}

void HoughTransform::CalculateVertexes()
{
	//для каждых двух соседних линий определяем координаты пересечения
	for (size_t i1 = 0; i1 < this->lines.size(); i1++)
	{
		int i2 = (i1 == (this->lines.size() - 1) ? 0 : (i1 + 1));
		//решаем систему 2-х уравнений с 2-мя неизвестными по формуле Крамера
		double det = cos(this->lines[i1].first * PI / 180.0) * sin(this->lines[i2].first * PI / 180.0) - cos(this->lines[i2].first * PI / 180.0) * sin(this->lines[i1].first * PI / 180.0);
		if (0.0 != det)
		{
			double det_x = this->lines[i1].second * sin(this->lines[i2].first * PI / 180.0) - this->lines[i2].second * sin(this->lines[i1].first * PI / 180.0);
			double det_y = cos(this->lines[i1].first * PI / 180.0) * this->lines[i2].second - cos(this->lines[i2].first * PI / 180.0) * this->lines[i1].second;
			// костыль для повышения точности, зная что изначальные координаты > 0
			// если немного промахнулись и координата отрицательная, кладем 0
			double x = (det_x / det > 0.0) ? (det_x / det) : 0.0;
			double y = (det_y / det > 0.0) ? (det_y / det) : 0.0;
			this->vertexes.push_back(std::make_pair(x, y));
		} 
		else
		{
			//линии параллельны, система не имеет решения, кладем нули :`(
			this->vertexes.push_back(std::make_pair(std::numeric_limits<int>::quiet_NaN(), std::numeric_limits<int>::quiet_NaN()));
		}
	}
}

int HoughTransform::SaveVertexes(const string& filename)
{
	ofstream file(filename, ios_base::out | ios_base::trunc);
	if (!file.is_open()) return -1; // если файл не открыт
	for (auto i : this->GetVertexes())
		file << i.first << " " << i.second << endl;
	file.close();
	return 0;
}

const std::vector<std::pair<double, double>>& HoughTransform::GetVertexes()
{
	return vertexes;
}

std::pair<int, int> HoughTransform::FindAccumMax()
{
	int max = this->accum_array[0];
	int x = 0, y = 0;
	//определяем максимум accum_array полным перебором
	for (int fy = 0; fy < r_max; fy++) {
		for (int fx = 0; fx < theta_max; fx++)
		{
			if (max < this->accum_array[fx + theta_max * fy])
			{
				max = this->accum_array[fx + theta_max * fy];
				x = fx;
				y = fy;
			}
		}
	}
	return std::make_pair(x, y);
}    

//зануляем пик (x,y) в квадрате +-peak_area
void HoughTransform::ErasePeak(int x,int y,int peak_area)
{
	// dy и dx проверям на границы массива
	for (int dy = ((y - peak_area) > 0 ? (y - peak_area) : 0); dy <= ((y + peak_area) < (r_max - 1) ? (y + peak_area) : r_max - 1); dy++)
		for (int dx = ((x - peak_area) > 0 ? (x - peak_area) : 0); dx <= ((x + peak_area) < (theta_max - 1) ? (x + peak_area) : theta_max - 1); dx++)
		{
			this->accum_array[dy * theta_max + dx] = 0;
		}
}

void HoughTransform::NormalizeAccumArray()
{
	//если max >  gray_scale, умножаем все поля на коэфициент  * gray_scale / max
	
	std::pair<int,int> max_coordinates = this->FindAccumMax();
	int max = this->accum_array[max_coordinates.first, max_coordinates.second];
	if (max < gray_scale) return;
	for (int y = 0; y < r_max; y++) {
		for (int x = 0; x < theta_max; x++)
		{
			this->accum_array[x + theta_max * y] = (this->accum_array[x + theta_max * y]  * gray_scale / max);
		}
	}
}

int HoughTransform::Save(const	string& filename)
{
	NormalizeAccumArray();
	ofstream hough_image_file(filename, ios_base::out | ios::binary | ios_base::trunc);
	if (!hough_image_file.is_open()) return -1; // если файл не открыт
	hough_image_file << "P5\n" << theta_max << " " << r_max << "\n" << gray_scale << "\n";
	//TODO: буферизировать, ?	
	for (int y = 0; y < r_max; y++) {
		for (int x = 0; x < theta_max; x++)
		{
			hough_image_file << static_cast<unsigned char>(this->accum_array[x + theta_max * y]);
		}
	}
	hough_image_file.close();
	/*
	ofstream lines_text_file("lines" + filename + ".txt", ios_base::out | ios_base::trunc);
	if (!lines_text_file.is_open()) return -1; // если файл не открыт
	for (auto i : this->lines)
		lines_text_file << i.first << " " << i.second << endl;
	lines_text_file.close();
	*/
	return 0;
}