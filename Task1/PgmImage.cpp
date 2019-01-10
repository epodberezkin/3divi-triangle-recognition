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
			bitmap[x + y * abscissa] = (unsigned char)gray_Scale;
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
	// Skip over the header
	//file.seekg(15, ios::beg); // Skip over 15 bytes 50 35 0A 35 30 30 20 35 30 30 0A 32 35 35 0A
	// Read in the data at once
	char current_line[256];
	file.getline(current_line, 256);
	file.getline(current_line, 256);
	if ('#' == *current_line)
		file.getline(current_line, 256);
	file.getline(current_line, 256);
	file.read((char *)&bitmap[0], bitmap.size());
	return 0;
}

void PgmImage::MakeNoise(double noise_probability)
{
	if (0.0 == noise_probability) return;
	re.seed((unsigned long)time(nullptr));
	unif_d = uniform_real_distribution<>(0.0, 1.0);
	unif_i = uniform_int_distribution<>(0, gray_scale);
	double probability = unif_d(re);
	double noise_integral;
	double noise_fractional = modf(noise_probability, &noise_integral);
	for(int y = 0; y < plot_bound; y++)
		for (int x = 0; x < plot_bound; x++)
		{
			if (probability < noise_fractional) bitmap[x + y * plot_bound] = unif_i(re);
			probability = unif_d(re);
		}
}

//проверяем, что пиксель закрашен (>gray_depth) а также пытаемся отсечь шум (если плотность пикселей шума ниже плотности пикселей линии для выбранного диапазона).
bool PgmImage::CheckPixel(int x, int y, int pixel_density) const
{
	//если не основной пиксель линии, сразу возвращаем без доп. проверкок.
	if (gray_level >= this->bitmap[x + y * abscissa]) return false;
	// dy и dx проверям на границы массива
	int pixel_count = 0;
	for (int dy = (y > 0 ? (y - 1) : 0); dy <= (y < (ordinate - 1) ? (y + 1) : y); dy++)
		for (int dx = (x > 0 ? (x - 1) : 0); dx <= (x < (abscissa - 1) ? (x + 1) : x); dx++)
		{
			if (this->bitmap[dx + dy * abscissa] > 0)
			pixel_count++;
		}
	return (pixel_count > pixel_density) ? true : false;
}

int PgmImage::CheckPixelDencity() const
{
	// Каждый основной пиксель линии с высокой вероятностью окружен =>4-мя ненулевыми пикселями (без учета шума).
	// Передаем эмпирически подобранное количество пикселей pixel_density в зависимости от уровня шума.
	// считаем число пикселей на 1/16 изображения, потом * 16
	int gray_pixel_count = 0;
	for (int y = 0; y < (this->ordinate >> 2); y++) {
		for (int x = 0; x < (this->abscissa >> 2); x++) {
			if (this->bitmap[x + y * abscissa] > 0) gray_pixel_count++;
		}
	}
	gray_pixel_count = gray_pixel_count << 4;

	int pixel_density = 8;

	if (gray_pixel_count < this->abscissa * this->ordinate * 7 / 16)
	{//для 0.0-0.4
		pixel_density = 4;
	}
	else if (gray_pixel_count < this->abscissa * this->ordinate * 59 / 80)
	{//для 0.5-0.7
		pixel_density = 7;
	}
	return pixel_density;
	//return (gray_pixel_count < this->abscissa * this->ordinate * 7 / 16) ? 4 : 7;
}
