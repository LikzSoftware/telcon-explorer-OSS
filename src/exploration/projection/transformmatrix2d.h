/*!	@file transformmatrix2d.h
 *	@author anantonov
 *	@date	Jan 28, 2013 (created)
 *	@brief	TODO <file description>
 */

#ifndef TRANSFORMMATRIX2D_H_
#define TRANSFORMMATRIX2D_H_

#include <iostream>

namespace VCGL {

class TransformMatrix2D {
public:
	TransformMatrix2D(
			float a00, float a01, float a02,
			float a10, float a11, float a12,
			float a20, float a21, float a22);

	TransformMatrix2D(const TransformMatrix2D& other);

	/*! Transform a point
	 *
	 * @param px0,py0 initial coordinates of the point
	 * @param ppx,ppy pointers to float receiving result of the transformation
	 */
	void transformPoint(const float px0, const float py0, float* ppx, float *ppy);

	static TransformMatrix2D Identity();
	static TransformMatrix2D Rotation(float rad);
	static TransformMatrix2D Translation(float tx, float ty);
	static TransformMatrix2D FlipScale(float flipX, float flipY, float scale);

	const TransformMatrix2D& operator= (const TransformMatrix2D& other);
	friend TransformMatrix2D operator* (const TransformMatrix2D& a, const TransformMatrix2D& b);
	friend std::ostream& operator<< (std::ostream& out, const TransformMatrix2D& m);
private:
	TransformMatrix2D();
	float matrix[3][3];
};

TransformMatrix2D operator* (const TransformMatrix2D& a, const TransformMatrix2D& b);
std::ostream& operator<< (std::ostream& out, const TransformMatrix2D& m);

}


#endif // TRANSFORMMATRIX2D_H_
