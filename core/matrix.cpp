#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>

namespace rt {

Matrix::Matrix(const Float4& r1, const Float4& r2, const Float4& r3, const Float4& r4) {
	 this->r1 = r1;
	 this->r2 = r2;
	this->r3 = r3;
	 this->r4 = r4;

}

Float4& Matrix::operator[](int idx) {
	switch (idx) {
	case 0 :
		return r1;
		break;
	case 1:
		return r2;
		break;
	case 2:
		return r3;
		break;
	case 3:
		return r4;
		break;
	default:
		throw rt::Assert() << "Invalid Index";
		break;
	}
}

Float4 Matrix::operator[](int idx) const {
	switch (idx) {
	case 0:
		return r1;
		break;
	case 1:
		return r2;
		break;
	case 2:
		return r3;
		break;
	case 3:
		return r4;
		break;
	default:
		throw rt::Assert() << "Invalid Index";
		break;
	}
}

Matrix Matrix::operator+(const Matrix& b) const {
	return Matrix(this->r1 + b.r1, this->r2 + b.r2, this->r3 + b.r3, this->r4 + b.r4);
}

Matrix Matrix::operator-(const Matrix& b) const {
	return Matrix(this->r1 - b.r1, this->r2 - b.r2, this->r3 - b.r3, this->r4 - b.r4);
}

Matrix Matrix::transpose() const {
	const Matrix& m = *this;
	Matrix t;

	t = t.identity();
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			t[i][j] = m[j][i];

	t.r1 = t[0];
	t.r2 = t[1];
	t.r3 = t[2];
	t.r4 = t[3];
	return t;
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
	return((this->r1 == b.r1) && (this->r2 == b.r2) &&
		(this->r3 == b.r3) && (this->r4 == b.r4));
}

bool Matrix::operator!=(const Matrix& b) const {
	return(!(*this == b));
}

Matrix product(const Matrix& a, const Matrix& b) {
	Matrix r;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			r[i][j] = a[i][0] * b[0][j] +
			a[i][1] * b[1][j] +
			a[i][2] * b[2][j] +
			a[i][3] * b[3][j];
	return r;
}

Matrix operator*(const Matrix& a, float scalar) {
	return Matrix(a.r1 * scalar, a.r2 * scalar, a.r3 * scalar, a.r4 * scalar);
}

Matrix operator*(float scalar, const Matrix& a) {
	return Matrix(a.r1 * scalar, a.r2 * scalar, a.r3 * scalar, a.r4 * scalar);
}

Float4 Matrix::operator*(const Float4& b) const {
	return Float4(dot(r1, b), dot(r2,b), dot(r3,b), dot(r4,b));
}

Vector Matrix::operator*(const Vector& b) const {
	Float4 pF(b);
	Float4 temp = Float4(dot(r1, pF), dot(r2, pF), dot(r3, pF), dot(r4, pF));
	return Vector(temp.x, temp.y, temp.z);
}

Point Matrix::operator*(const Point& b) const {
	Float4 pF(b);
	Float4 temp = Float4(dot(r1, pF), dot(r2, pF), dot(r3, pF), dot(r4, pF));
	return Point(temp.x, temp.y, temp.z);
}

float Matrix::det() const {
	Matrix result;
	const Matrix& m = *this;

	result[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
	result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
	result[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
	result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

	float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
	return det;
}

Matrix Matrix::zero() {
	Float4 zerF(0.0f, 0.0f, 0.0f, 0.0f);
	return Matrix(zerF,zerF,zerF,zerF);
}

Matrix Matrix::identity() {
	Float4 idenF1(1.0f, 0.0f, 0.0f, 0.0f);
	Float4 idenF2(0.0f, 1.0f, 0.0f, 0.0f);
	Float4 idenF3(0.0f, 0.0f, 1.0f, 0.0f);
	Float4 idenF4(0.0f, 0.0f, 0.0f, 1.0f);
	return Matrix(idenF1, idenF2, idenF3, idenF4);
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
	Float4 b1(e1);
	Float4 b2(e2);
	Float4 b3(e3);
	Float4 b4(0,0,0,1);
	return Matrix(b1, b2, b3, b4 ).transpose();
}

}