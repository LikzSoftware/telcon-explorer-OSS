/*! @file selectionhull.h
 * @author anantonov
 * @date Created on Jun 25, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef SELECTIONHULL_H_
#define SELECTIONHULL_H_

#include <vector>
#include <QPoint>

namespace VCGL {

struct SelectionHull {
	void init(const QPoint& pt);
	void addPoint(const QPoint& pt);
	bool point_belong_rect(QPoint &q, QPoint &a, QPoint &b);    ///< check whether point is inside rectangle
	bool point_belong_hull(QPoint &q); ///< check whether point is inside contour

	std::vector<QPoint> hull; 	///< ONE selection
	QPoint minHull, maxHull;  	///< bounded box of current selection
};

} /* namespace VCGL */

#endif /* SELECTIONHULL_H_ */
