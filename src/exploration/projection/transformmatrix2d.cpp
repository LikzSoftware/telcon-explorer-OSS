/*!	@file transformmatrix2d.cpp
 *	@author anantonov
 *	@date	Jan 28, 2013 (created)
 *	@brief	TODO <file description>
 */

#include "transformmatrix2d.h"
#include <cstring>
#include <cmath>

namespace VCGL {

TransformMatrix2D::TransformMatrix2D(
		float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22) {
	matrix[0][0] = a00;
	matrix[0][1] = a01;
	matrix[0][2] = a02;
	matrix[1][0] = a10;
	matrix[1][1] = a11;
	matrix[1][2] = a12;
	matrix[2][0] = a20;
	matrix[2][1] = a21;
	matrix[2][2] = a22;
}

TransformMatrix2D::TransformMatrix2D(const TransformMatrix2D& other) {
	memcpy(matrix, other.matrix, sizeof(matrix));
}

TransformMatrix2D::TransformMatrix2D() {
	memset(matrix, 0, sizeof(matrix));
}

void
TransformMatrix2D::transformPoint(const float px0, const float py0, float* ppx, float *ppy) {
	*ppx = matrix[0][0]*px0 + matrix[0][1]*py0 + matrix[0][2];
	*ppy = matrix[1][0]*px0 + matrix[1][1]*py0 + matrix[1][2];
}

TransformMatrix2D
TransformMatrix2D::Identity() {
	return TransformMatrix2D(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
}

TransformMatrix2D
TransformMatrix2D::Rotation(float rad) {
	float cosa = cos(rad);
	float sina = sin(rad);
	return TransformMatrix2D(
			cosa, -sina, 0.0f,
			sina,  cosa, 0.0f,
			0.0f,  0.0f, 1.0f);
}
TransformMatrix2D
TransformMatrix2D::Translation(float tx, float ty) {
	return TransformMatrix2D(
				1.0f, 0.0f, tx,
				0.0f, 1.0f, ty,
				0.0f, 0.0f, 1.0f);
}
TransformMatrix2D
TransformMatrix2D::FlipScale(float flipX, float flipY, float scale) {
	return TransformMatrix2D(
				flipX*scale,        0.0f, 0.0f,
				0.0f,        flipY*scale, 0.0f,
				0.0f,               0.0f, 1.0f);
}

const TransformMatrix2D&
TransformMatrix2D::operator= (const TransformMatrix2D& other) {
	memcpy(matrix, other.matrix, sizeof(matrix));
	return *this;
}

TransformMatrix2D operator* (const TransformMatrix2D& a, const TransformMatrix2D& b) {
	TransformMatrix2D res;
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			for (int k=0; k<3; k++) {
				res.matrix[i][j] += a.matrix[i][k]*b.matrix[k][j];
			}
		}
	}
	return res;
}

std::ostream& operator<< (std::ostream& out, const TransformMatrix2D& m) {
	out << '[';
	for (int i=0; i<3; i++) {
		for(int j=0; j<3; j++) {
			out << m.matrix[i][j];

			if (i<2 || j<2) {
				out << ',';
			}
			else {
				out << ']';
			}
		}
		out << std::endl;
	}
	return out;
}


} //namespace VCGL


