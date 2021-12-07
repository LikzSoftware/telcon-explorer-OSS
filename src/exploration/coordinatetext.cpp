/*! @file coordinatetext.cpp
 * @author anantonov
 * @date Created on Jul 2, 2014
 *
 * @brief Implementation of a few helper functions to convert coordinates to a user friendly string
 */

#include "coordinatetext.h"

namespace VCGL {

QString CoordinateText::convertPoint(const QPointF point) {
	return QString("(%1, %2)").arg( convertLat(point.y()) , convertLon(point.x()) );
}

QString CoordinateText::convertLon(float lonValue) {
	QString lonStr = "0.00";
	if (lonValue > 0.00001) {
		lonStr = QString::number(lonValue, 'f', 2).append(" E");
	}
	if (lonValue < -0.00001) {
		lonStr = QString::number(-lonValue, 'f', 2).append(" W");
	}
	return lonStr;
}

QString CoordinateText::convertLat(float latValue) {
	QString latStr = "0.00";
	if (latValue > 0.00001) {
		latStr = QString::number(latValue, 'f', 2).append(" N");
	}
	if (latValue < -0.00001) {
		latStr = QString::number(-latValue, 'f', 2).append(" S");
	}
	return latStr;
}


} /* namespace VCGL */
