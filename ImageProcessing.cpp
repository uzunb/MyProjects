//	BATUHAN UZUN 

#include<iostream>
#include<string>
#include<fstream>
#include<math.h>

using namespace std;

class Matrix
{
public:
	Matrix(int rc, int cc);
	void setColCount(int colCount);
	void setRowCount(int rowCount);
	void setMatx(int i, int j, int var);
	int getColCount();
	int getRowCount();
	int getMatx(int i, int j);

	void displayMatrix();
protected:
	int rowCount;
	int colCount;
	int **Matx;
};
class image:public Matrix
{
public:
	image(int rc = 255, int cc = 255) : Matrix(rc, cc){}

	void sobel();
	void threshold(int limit);

	image operator+(const image &m1);
	image operator*(const image &m1);
	image operator!();
};


Matrix::Matrix(int rc = 10, int cc = 10) {
	rowCount = rc;
	colCount = cc;
	Matx = new int*[rowCount]();
	
	for (int i = 0; i < rowCount; i++) {
		Matx[i] = new int[colCount]();
	}
}
void Matrix::setRowCount(int rowCount)			{ this->rowCount = rowCount;	}
void Matrix::setColCount(int colCount)			{ this->colCount = colCount;	}
void Matrix::setMatx(int i, int j, int var = 0) { this->Matx[i][j] = var;		}
int Matrix::getColCount()						{ return colCount;				}
int Matrix::getRowCount()						{ return rowCount;				}
int Matrix::getMatx(int i, int j)				{ return Matx[i][j];			}

//SHOWS THE MATRIX
void Matrix::displayMatrix() {
	int counter = 0;
	for (int i = 0; i < rowCount; i++) {
		for (int j = 0; j < colCount; j++) {
			cout << this->Matx[i][j];
			counter++;
		}
	}
	cout << "\n\ncounter : " << counter;
}

//OPERATOR OVERLOADING FUNCTIONS
image image::operator+(const image &m1)
{
	image temp(this->rowCount, this->colCount);
	
	for (size_t i = 0; i < this->rowCount; i++)
		for (size_t j = 0; j < colCount; j++)
			temp.Matx[i][j] = this->Matx[i][j] | m1.Matx[i][j];
	return temp;
}	
image image::operator *(const image &m1)
{
	image temp(this->rowCount, this->colCount);

	for (size_t i = 0; i < rowCount; i++)
		for (size_t j = 0; j < colCount; j++)
			temp.Matx[i][j] = this->Matx[i][j] & m1.Matx[i][j];
	return temp;
}
image image::operator!()
{
	image temp(this->rowCount, this->colCount);

	for (size_t i = 0; i < rowCount; i++)
		for (size_t j = 0; j < colCount; j++)
			this->Matx[i][j] == 1 ? temp.Matx[i][j] = 0 : temp.Matx[i][j] = 1;
	return temp;
}
istream &operator >> (istream  &input, image &img) {
	char ch;
	int value, row, col;

	input.get(ch);
	value = ch;
	if (value < 0) value += 256;
	row = value;

	input.get(ch);
	value = ch;
	if (value < 0) value += 256;
	col = value;

	img.setRowCount(col);
	img.setColCount(row);

	row = 0, col = 0;
	while (input.get(ch)) {
		value = ch;
		if (value < 0) value += 256;

		img.setMatx(row, col, value);
		col++;
		if (col == img.getColCount()) { row++;	col = 0; }
	}
	return input;
}
ofstream &operator << (ofstream  &input, image &img)
{
	for (size_t i = 0; i < img.getRowCount(); i++) {
		for (size_t j = 0; j < img.getColCount(); j++)
			img.getMatx(i, j) == 1 ? input << "###" : input << "---";
		input << "\n";
	}
	return input;
}

//SOBEL FILTER AND THRESHOLD FUNCTION
void image::sobel() 
{
	int SobeDikey[3][3] = { {1,0,-1},{2,0,-2},{1,0,-1} };
	int SobeYatay[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };
	int Sx = 0, Sy = 0, S;

	image temp;

	for (size_t i = 1; i < this->getRowCount() - 1; i++)
		for (size_t j = 1; j < this->getColCount() - 1; j++)
		{
			for (size_t a = 0; a < 3; a++)
				for (size_t b = 0; b < 3; b++)
				{
					Sx += this->Matx[i + a - 1][j + b - 1] * SobeDikey[a][b];
					Sy += this->Matx[i + a - 1][j + b - 1] * SobeYatay[a][b];
				}
			temp.Matx[i - 1][j - 1] = sqrt((Sx*Sx) + (Sy*Sy));
			Sx = Sy = 0;
		}

	this->setRowCount(this->getRowCount() - 2);
	this->setColCount(this->getColCount() - 2);

	this->Matx = temp.Matx;
	
 }
void image::threshold(int limit = 100)
{
	for (size_t i = 0; i < this->getRowCount(); i++)
		for (size_t j = 0; j < this->getColCount(); j++)
			this->Matx[i][j] < limit ? this->Matx[i][j] = 0 : this->Matx[i][j] = 1;
}


int main(int argc, char* argv[])
{
  if(argc != 4)
  { cout<<"Missing Arguments..."<<endl;}

	image photo1, photo2, photo3;
	ifstream infile, infile2;
	ofstream outfile;

	infile.open(argv[1], ios::in | ios::binary);
	if (!infile.is_open())	cout << "First file didn't open...";
	infile >> photo1;
	infile.close();

	infile2.open(argv[2], ios::in | ios::binary);
	if (!infile2.is_open())	cout << "Second file didn't open...";
	infile2 >> photo2;
	infile2.close();

	photo2.sobel();
	photo2.threshold();

	photo1.sobel();
	photo1.threshold();

	/*photo3 = photo1 + photo2;
	photo3 = photo1 * photo2;
	photo3 = !photo1;*/

	outfile.open(argv[3], ios::out | ios::binary);
	if (!outfile.is_open())	cout << "Output file didn't open...";
	outfile << photo1;
	outfile.close();
}