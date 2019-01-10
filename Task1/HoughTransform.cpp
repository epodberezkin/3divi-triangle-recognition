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
	//�������� ������� CheckPixel
	//std::vector<unsigned char> check_bitmap(plot_bound * plot_bound, 0);

	int	pixel_density = original_image.CheckPixelDencity();
	// ����������� �� �������� ����������� ��������
	for (int y = 0; y < original_image.ordinate; y++) {
		for (int x = 0; x < original_image.abscissa; x++) {
			if (original_image.CheckPixel(x, y, pixel_density))  // ��� ��������� ������� �������?
			{
				//�������� ������� CheckPixel
				//check_bitmap[x + y * plot_bound] = 255;
				
				// ���������� ��� ��������� ������, ������� ����� ��������� ����� ��� �����
				for (int f = 0; f < theta_max; f++) { //���������� ��� ��������� ���� �������
					double theta = f * PI / 180.0;
					double r = (y)*sin(theta) + (x)*cos(theta);
					//���� r ������������ � �������� ��������� ��������, ��������
					if ( (r < r_max / 2) && (r >= -r_max / 2))
						accum_array[(int)round(r + r_max / 2) * theta_max + f]++;
				}
			}
		}
	}
	/* �������� ������� CheckPixel
		ofstream file("test_check.pgm", ios_base::out | ios::binary | ios_base::trunc);
	if (!file.is_open()) return; // ���� ���� �� ������
	file << "P5\n" << plot_bound << " " << plot_bound << "\n" << gray_scale << "\n";
	file.write((char *)&check_bitmap[0], check_bitmap.size());
	file.close();
	*/
}

void HoughTransform::FindLines(int count)
{
	//������� count ���������� � �����, ������� ���������� � �������� +-10 
	//���� ��� ���� ������������ ����� >30, ���� ��������� ����������� ����������, ���������� ����� � �������������� �������� (r ����� ���������), ����� �������� ���� �� ���������� ������ 20.
	int max_th, max_rd = 0;
	for (int i = 0; i < count; i++ )
	{
		FindAccumMax(max_th, max_rd);
		this->lines.push_back(std::make_pair(max_th, (max_rd - r_max / 2)));
		ErasePeak(max_th, max_rd, 10);
	}	
}

void HoughTransform::CalculateVertexes()
{
	//��� ������ ���� �������� ����� ���������� ���������� �����������
	for (size_t i1 = 0; i1 < lines.size(); i1++)
	{
		int i2 = (i1 == (lines.size() - 1) ? 0 : (i1 + 1));
		//������ ������� 2-� ��������� � 2-�� ������������ �� ������� �������
		double det = cos(lines[i1].first * PI / 180.0) * sin(lines[i2].first * PI / 180.0) - cos(lines[i2].first * PI / 180.0) * sin(lines[i1].first * PI / 180.0);
		if (0.0 == det)
		{
			//����� �����������, ������� �� ����� �������, ������ ���� :`(
			vertexes.push_back(std::make_pair(std::numeric_limits<int>::quiet_NaN(), std::numeric_limits<int>::quiet_NaN()));
			continue;
		} 
		double det_x = lines[i1].second * sin(lines[i2].first * PI / 180.0) - lines[i2].second * sin(lines[i1].first * PI / 180.0);
		double det_y = cos(lines[i1].first * PI / 180.0) * lines[i2].second - cos(lines[i2].first * PI / 180.0) * lines[i1].second;
		// ������� ��� ��������� ��������, ���� ��� ����������� ���������� > 0
		// ���� ������� ������������ � ���������� �������������, ������ 0
		double x = (det_x / det > 0) ? (det_x / det) : 0.0;
		double y = (det_y / det > 0) ? (det_y / det) : 0.0;
		vertexes.push_back(std::make_pair( x, y));
	}
}

const std::vector<std::pair<double, double>>& HoughTransform::GetVertexes()
{
	return vertexes;
}

void HoughTransform::NormalizeAccumArray()
{
	//���� max >  gray_scale, �������� ��� ���� �� ����������  * gray_scale / max
	int max_th = 0, max_rd = 0;
	int max = HoughTransform::FindAccumMax(max_th, max_rd);
	if (max < gray_scale) return;
	for (int y = 0; y < r_max; y++) {
		for (int x = 0; x < theta_max; x++)
		{
			accum_array[x + theta_max * y] = (unsigned int)((double)accum_array[x + theta_max * y] / max * gray_scale);
		}
	}
}

int HoughTransform::FindAccumMax(int& x, int& y)
{
	unsigned int max = accum_array[0];
	//���������� �������� accum_array ������ ���������
	for (int fy = 0; fy < r_max; fy++) {
		for (int fx = 0; fx < theta_max; fx++)
		{
			if (max < accum_array[fx + theta_max * fy])
			{
				max = accum_array[fx + theta_max * fy];
				x = fx;
				y = fy;
			}
		}
	}
	return max;
}    

//�������� ��� (x,y) � �������� +-peak_area
void HoughTransform::ErasePeak(int x,int y,int peak_area)
{
	// dy � dx �������� �� ������� �������
	for (int dy = ((y - peak_area) > 0 ? (y - peak_area) : 0); dy <= ((y + peak_area) < (r_max - 1) ? (y + peak_area) : r_max - 1); dy++)
		for (int dx = ((x - peak_area) > 0 ? (x - peak_area) : 0); dx <= ((x + peak_area) < (theta_max - 1) ? (x + peak_area) : theta_max - 1); dx++)
		{
			this->accum_array[dy * theta_max + dx] = 0;
		}
}

int HoughTransform::Save(const	string& filename)
{
	NormalizeAccumArray();
	ofstream hough_image_file(filename, ios_base::out | ios::binary | ios_base::trunc);
	if (!hough_image_file.is_open()) return -1; // ���� ���� �� ������
	hough_image_file << "P5\n" << theta_max << " " << r_max << "\n" << gray_scale << "\n";
	//TODO: ��������������, ?	
	for (int y = 0; y < r_max; y++) {
		for (int x = 0; x < theta_max; x++)
		{
			hough_image_file << (unsigned char)accum_array[x + theta_max * y];
		}
	}
	hough_image_file.close();
	/*
	ofstream lines_text_file("lines" + filename + ".txt", ios_base::out | ios_base::trunc);
	if (!lines_text_file.is_open()) return -1; // ���� ���� �� ������
	for (auto i : this->lines)
		lines_text_file << i.first << " " << i.second << endl;
	lines_text_file.close();
	*/
	return 0;
}