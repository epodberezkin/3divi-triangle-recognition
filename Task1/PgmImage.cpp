#include "PgmImage.h"


using namespace std;

PgmImage::PgmImage() : bitmap(abscissa * ordinate, 0)
{
}

PgmImage::~PgmImage()
{
}

const int PgmImage::gray_level = 127;

void PgmImage::Plot(int x, int y, int gray_Scale)
{
	if(x <= abscissa && y <=ordinate)
		if(bitmap[x + y * abscissa] < gray_Scale)
			bitmap[x + y * abscissa] = static_cast<unsigned char>(gray_Scale);
}

int PgmImage::Save(const string& file_name) 
{
	ofstream file(file_name, ios_base::out | ios::binary | ios_base::trunc);
	if (!file.is_open()) return -1; // если файл не открыт
	file << "P5\n" << abscissa << " " << ordinate << "\n" << gray_scale << "\n";
	file.write((char *)&bitmap[0], bitmap.size());
	file.close();
	return 0;
}

int PgmImage::Open(const string& filename)
{
	ifstream file(filename, ios::binary | ios::in);
	if (!file.is_open()) return -1; // если файл не открыт
	char current_line[256];
	file.getline(current_line, 256); // считываем заголовок
	file.getline(current_line, 256); // считываем размерность либо комментариий
	if ('#' == *current_line)		// если комментарий, то следующая строка - размерность 
		file.getline(current_line, 256);
	file.getline(current_line, 256); // колчиство оттенков
	// Считываем оставшиеся байты за раз.
	file.read((char *)&bitmap[0], bitmap.size());
	return 0;
}

void PgmImage::MakeNoise(double noise_probability)
{
	if (0.0 == noise_probability) return;
	this->random_engine.seed(static_cast<unsigned long>(time(nullptr)));
	this->distribution_double = uniform_real_distribution<>(0.0, 1.0);
	this->distribution_int = uniform_int_distribution<>(0, gray_scale);
	double probability = this->distribution_double(random_engine);
	double noise_integral;
	double noise_fractional = modf(noise_probability, &noise_integral); //если передали noise > 0 , берем только дробную часть [0, 1)
	for(int y = 0; y < plot_bound; y++)
		for (int x = 0; x < plot_bound; x++)
		{
			if (probability < noise_fractional) bitmap[x + y * plot_bound] = this->distribution_int(random_engine);
			probability = this->distribution_double(random_engine);
		}
}

//проверяем, что пиксель закрашен (>gray_depth) а также пытаемся отсечь шум (если плотность пикселей шума ниже плотности пикселей линии для выбранного диапазона).
bool PgmImage::CheckPixel(int x, int y, int pixel_density) const
{
	//если не основной пиксель линии, сразу возвращаем без доп. проверкок.
	if (PgmImage::gray_level >= this->bitmap[x + y * PgmImage::abscissa]) return false;
	else
	{
		// dy и dx проверям на границы массива
		int pixel_count = 0;
		for (int dy = (y > 1 ? (y - 2) : 0); dy <= (y < (PgmImage::ordinate - 2) ? (y + 2) : y); dy++)
			for (int dx = (x > 1 ? (x - 2) : 0); dx <= (x < (PgmImage::abscissa - 2) ? (x + 2) : x); dx++)
			{
				if (this->bitmap[dx + dy * PgmImage::abscissa] > 0)
					pixel_count++;
			}
		return (pixel_count > pixel_density) ? true : false;
	}
}

int PgmImage::CheckPixelDencity() const
{
	// Каждый основной пиксель линии с высокой вероятностью окружен =>8-и ненулевыми пикселями в квадрате 5x5 (без учета шума).
	// Передаем эмпирически подобранное количество пикселей pixel_density, для определения учитываемых пикселей.
	// Для определения уровня шума считаем число ненулевых пикселей на 1/16 изображения, потом * 16
	int gray_pixel_count = 0;
	for (int y = 0; y < (PgmImage::ordinate >> 2); y++) {
		for (int x = 0; x < (PgmImage::abscissa >> 2); x++) {
			if (this->bitmap[x + y * PgmImage::abscissa] > 0) gray_pixel_count++;
		}
	}
	int pixel_count = PgmImage::abscissa * PgmImage::ordinate;
	gray_pixel_count = gray_pixel_count << 4;

	//Для P > 0.8
	int pixel_density = 23;

	if (gray_pixel_count < PgmImage::abscissa * PgmImage::ordinate * 19 / 80)
	{//для 0.0-0.2
		pixel_density = 8;
	}
	else if (gray_pixel_count < PgmImage::abscissa * PgmImage::ordinate * 27 / 80)
	{//для 0.3
		pixel_density = 10;
	}
	else if (gray_pixel_count < PgmImage::abscissa * PgmImage::ordinate * 7 / 16)
	{//для 0.4
		pixel_density = 14;
	}
	else if (gray_pixel_count < PgmImage::abscissa * PgmImage::ordinate * 43 / 80)
	{//для 0.5
		pixel_density = 17;
	}
	else if (gray_pixel_count < PgmImage::abscissa * PgmImage::ordinate * 51 / 80)
	{//для 0.6
		pixel_density = 18;
	}
	else if (gray_pixel_count < PgmImage::abscissa * PgmImage::ordinate * 59 / 80)
	{//для 0.7
		pixel_density = 20;
	}
	return pixel_density;
}
