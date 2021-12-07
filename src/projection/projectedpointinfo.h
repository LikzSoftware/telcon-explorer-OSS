/*!	@file projectedpointinfo.h
 *	@author anantonov
 *	@date	Apr 25, 2012 (created)
 *	@brief	Definition of VCGL::FieldType enumeration and VCGL::ProjectedPointInfo structure
 */

#ifndef PROJECTEDPOINTINFO_H_
#define PROJECTEDPOINTINFO_H_

#include "tspoint.h"
#include "typedefs.h"

namespace VCGL {

/*! @brief Structure describing projected point */
struct ProjectedPointInfo {
	LSP::TSPoint pt;	///< Projected coordinate
	strType name;		///< Name of the field

	ProjectedPointInfo();
	double getX() const;
	double getY() const;
};

/*
 * =========================================================================
 * implementation
 * =========================================================================
 */


inline ProjectedPointInfo::ProjectedPointInfo()
: pt(0.0,0.0), name("")
{}

inline double
ProjectedPointInfo::getX() const {
	return pt.getX();
}

inline double
ProjectedPointInfo::getY() const {
	return pt.getY();
}

} // namespace VCGL
#endif // PROJECTEDPOINTINFO_H_
