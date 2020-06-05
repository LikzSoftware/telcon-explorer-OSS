/*!	@file annotationlink.h
 *	@author anantonov
 *	@date	Mar 17, 2014 (original version)
 *	@date	Jun 17, 2014 (modified to QPointF)
 *	@brief	Definition of structure used to annotate maps with links (points and lines)
 */

#ifndef ANNOTATIONLINK_H_
#define ANNOTATIONLINK_H_

#include <QPoint>

namespace VCGL {

///Structure for annotating maps with links (points and lines)
struct AnnotationLinkF {

	///Enumeration for indicating what to draw
	enum Selection {
		SHOW_NONE = 0,
		SHOW_LINE = 0x01,
		SHOW_A = 0x02,
		SHOW_B = 0x04,
		SHOW_POINTS = SHOW_A | SHOW_B,
		SHOW_ALL = SHOW_LINE | SHOW_POINTS
	};

	QPointF ptA;	///< "start" point for link
	QPointF ptB;	///< "end" point for link
	Selection sel;	///< indication of what to draw


	/// Default constructor
	AnnotationLinkF();

	/// Constructor with initialization, by default draw both points and the line
	AnnotationLinkF(const QPointF& a, const QPointF& b, Selection s = SHOW_ALL);

	bool showA() const;		///< check if point A will be shown
	bool showB() const;		///< check if point B will be shown
	bool showLine() const;	///< check if the line will be shown
};

inline
AnnotationLinkF::AnnotationLinkF(): ptA(), ptB(), sel(SHOW_NONE) {}

inline
AnnotationLinkF::AnnotationLinkF(const QPointF& a, const QPointF& b, Selection s)
: ptA(a), ptB(b), sel(s) {}

inline bool
AnnotationLinkF::showA() const { return ((int)(sel & SHOW_A) != 0); }

inline bool
AnnotationLinkF::showB() const { return ((int)(sel & SHOW_B) != 0); }

inline bool
AnnotationLinkF::showLine() const { return ((int)(sel & SHOW_LINE) != 0); }


} // namespace VCGL
#endif // ANNOTATIONLINK_H_
