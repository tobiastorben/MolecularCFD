#include "../header/LinMat.h"

LinMat::LinMat(int r, int c) {
	rows = r;
	cols = c;
	mat = new double[rows*cols];
}int LinMat::index(int i, int j) const  {
	return i*cols + j ;
}
double LinMat::get(int index) const {
	return mat[index];
}
void LinMat::set(int index, double val){
	 mat[index] = val;
}
void LinMat::setRow(int row, double vals[]) {
	for (int i = 0; i < cols; i++) {
		mat[row + i] = vals[i];
	}
}

int LinMat::getCols() const  { return cols; }
int LinMat::getRows() const { return rows; }
