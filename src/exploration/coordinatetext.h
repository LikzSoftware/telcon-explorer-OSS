/*! @file coordinatetext.h
 * @author anantonov
 * @date Created on Jul 2, 2014
 *
 * @brief A few helper functions to convert coordinates to a user friendly string
 */

#ifndef COORDINATETEXT_H_
#define COORDINATETEXT_H_

#include <QString>
#include <QPoint>

namespace VCGL {

/// A few helper functions to convert coordinates to a user friendly string
class CoordinateText {
public:
	/// Convert point to text string
	static QString convertPoint(const QPointF point);
	/// Convert longitude to text string
	static QString convertLon(float lon);
	/// Convert latitude to text string
	static QString convertLat(float lat);
};

} /* namespace VCGL */

#endif /* COORDINATETEXT_H_ */
