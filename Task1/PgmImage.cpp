// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "PgmImage.h"

using namespace std;

PgmImage::PgmImage() : bitmap(abscissa * ordinate, 0) , noise_probability(0.0)
{
}

PgmImage::~PgmImage()
{
}

const int PgmImage::gray_level = 127;

const std::vector<unsigned char>& PgmImage::GetBitmap() const
{
	return bitmap;
}

void PgmImage::Plot(int x, int y, int gray_Scale)
{
	if(x <= abscissa && y <=ordinate)
		if(bitmap[x + y * abscissa] < gray_Scale)
			bitmap[x + y * abscissa] = static_cast<unsigned char>(gray_Scale);
}

int PgmImage::Save(const string& file_name) 
{
	ofstream file(file_name, ios_base::out | ios::binary | ios_base::trunc);
	if (!file.is_open()) return -1; // ���� ���� �� ������
	file << "P5\n" << abscissa << " " << ordinate << "\n" << gray_scale << "\n";
	file.write((char *)bitmap.data(), bitmap.size());
	file.close();
	return 0;
}

int PgmImage::Open(const string& filename)
{
	ifstream file(filename, ios::binary | ios::in);
	if (!file.is_open()) return -1; // ���� ���� �� ������
	char current_line[256];
	file.getline(current_line, 256); // ��������� ���������
	file.getline(current_line, 256); // ��������� ����������� ���� ������������
	if ('#' == *current_line)		// ���� �����������, �� ��������� ������ - ����������� 
		file.getline(current_line, 256);
	file.getline(current_line, 256); // ��������� ��������
	// ��������� ���������� ����� �� ���.
	file.read((char *)bitmap.data(), bitmap.size());
	return 0;
}

void PgmImage::SetBitmap(std::vector<unsigned char>& new_bitmap)
{
	this->bitmap = new_bitmap;
}

void PgmImage::SetBitmap(unsigned char * new_bitmap)
{
	std::copy(new_bitmap, new_bitmap + abscissa * ordinate, this->bitmap.data());
}

void PgmImage::MakeNoise(double noise)
{
	if (0.0 == noise) return;
	random_engine.seed(static_cast<unsigned long>(time(nullptr)));
	distribution_double = uniform_real_distribution<>(0.0, 1.0);
	distribution_int = uniform_int_distribution<>(0, gray_scale);
	double probability = distribution_double(random_engine);
	double noise_integral;
	double noise_fractional = modf(noise, &noise_integral); //���� �������� noise > 0 , ����� ������ ������� ����� [0, 1)
	for(int y = 0; y < plot_bound; y++)
		for (int x = 0; x < plot_bound; x++)
		{
			if (probability < noise_fractional) bitmap[x + y * plot_bound] = distribution_int(random_engine);
			probability = distribution_double(random_engine);
		}
}

//���������, ��� ������� �������� (>gray_depth) � ����� �������� ������ ��� (���� ��������� �������� ���� ���� ��������� �������� ����� ��� ���������� ���������).
bool PgmImage::CheckPixel(int x, int y, int& pixel_density) const
{
	//pixel_density = bitmap[x + y * PgmImage::abscissa];
	//return (bitmap[x + y * PgmImage::abscissa] > 127) ? true : false;
	//���� �� �������� ������� �����, ����� ���������� ��� ���. ���������.
	if (PgmImage::gray_level >= bitmap[x + y * PgmImage::abscissa]) return false;
	else
	{
		// dy � dx �������� �� ������� �������
		int pixel_count = 0;
		for (int dy = (y > 1 ? (y - 2) : 0); dy <= (y < (PgmImage::ordinate - 2) ? (y + 2) : y); dy++)
			for (int dx = (x > 1 ? (x - 2) : 0); dx <= (x < (PgmImage::abscissa - 2) ? (x + 2) : x); dx++)
			{
				if (bitmap[dx + dy * PgmImage::abscissa] > 0)
					pixel_count++;
			}
		return (pixel_count > pixel_density) ? true : false;
	}
}

int PgmImage::CheckPixelDencity() const
{
	// ������ �������� ������� ����� � ������� ������������ ������� =>8-� ���������� ��������� � �������� 5x5 (��� ����� ����).
	// �������� ����������� ����������� ���������� �������� pixel_density, ��� ����������� ����������� ��������.
	// ��� ����������� ������ ���� ������� ����� ��������� �������� �� 1/16 �����������, ����� * 16
	int gray_pixel_count = 0;
	for (int y = 0; y < (PgmImage::ordinate >> 2); y++) {
		for (int x = 0; x < (PgmImage::abscissa >> 2); x++) {
			if (bitmap[x + y * PgmImage::abscissa] > 0) gray_pixel_count++;
		}
	}
	int pixel_count = PgmImage::abscissa * PgmImage::ordinate;
	gray_pixel_count = gray_pixel_count << 4;

	//��� P > 0.8
	int pixel_density = 20;

	if (gray_pixel_count < pixel_count * 19 / 80)
	{//��� 0.0-0.2
		pixel_density = 8;
	}
	else if (gray_pixel_count < pixel_count * 27 / 80)
	{//��� 0.3
		pixel_density = 10;
	}
	else if (gray_pixel_count < pixel_count * 7 / 16)
	{//��� 0.4
		pixel_density = 14;
	}
	else if (gray_pixel_count < pixel_count * 43 / 80)
	{//��� 0.5
		pixel_density = 17;
	}
	else if (gray_pixel_count < pixel_count * 51 / 80)
	{//��� 0.6
		pixel_density = 18;
	}
	else if (gray_pixel_count < pixel_count * 59 / 80)
	{//��� 0.7
		pixel_density = 20;
	}
	return pixel_density;
}
