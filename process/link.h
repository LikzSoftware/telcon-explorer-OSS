/*! @file link.h
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief Structure describing a link between two points
 */

#ifndef LINK_H_
#define LINK_H_

#include <QPoint>

namespace VCGL {

///Structure representing a link between two points (via indices)
struct Link {
	///point A (starting point for unidirectional links)
	QPoint ptA;

	///point B (ending point for unidirectional links)
	QPoint ptB;

	///weight of the link
	float w;

	Link(): ptA(-1,-1), ptB(-1,-1), w(0) {}
	Link(QPoint pointA, QPoint pointB, float weight): ptA(pointA), ptB(pointB), w(weight) {}
};

///Structure representing a link between two points (via coordinates)
struct LinkF {
	///point A (starting point for unidirectional links)
	QPointF ptA;

	///point B (ending point for unidirectional links)
	QPointF ptB;

	///weight of the link
	float w;

	LinkF(): ptA(-1,-1), ptB(-1,-1), w(0) {}
	LinkF(QPointF pointA, QPointF pointB, float weight): ptA(pointA), ptB(pointB), w(weight) {}
};

} /* namespace VCGL */

#endif /* LINK_H_ */
