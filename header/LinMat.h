#ifndef LinMat_TORBEN_H
#define LinMat_TORBEN_H

class LinMat {
public:
	LinMat::LinMat(int r, int c);
	int index(int i, int j) const;
	double get(int index) const;
	void set(int index, double val);
	void setRow(int row, double vals[]);
	int LinMat::getCols() const;
	int LinMat::getRows() const;
private:
	int cols;
	int rows;
	double* mat;//Format: [a11,a12,a13,a21,a22,a23,a31,a32,a33 ...] for 3 cols
};

#endif