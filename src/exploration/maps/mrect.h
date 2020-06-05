/*! @file mrect.h
 * @author anantonov
 * @date Created on Jun 3, 2014
 *
 * @brief Map rectangle, defining pixelwise rectangle where left < right, bottom < top
 */

#ifndef MRECT_H_
#define MRECT_H_

namespace VCGL {

/// Structure defining pixelwise rectangle where left < right, bottom < top
struct MRect {
	int left;
	int top;
	int right;
	int bottom;

	/// width of the rectangle in pixels
	int width() const { return right-left+1; }

	/// height of the rectangle in pixels
	int height() const { return top-bottom+1; }

	/// Default constructor (a pixel)
	MRect(): left(0), top(0), right(0), bottom(0) {}
	/// Constructor with initialization
	MRect(int l, int t, int r, int b):
		left(l), top(t), right(r), bottom(b) {}
};

/// Comparison operator
inline bool operator==(const MRect& a, const MRect& b) {
	return (a.left == b.left && a.top == b.top && a.right == b.right && a.bottom == b.bottom);
}

}



#endif /* MRECT_H_ */
