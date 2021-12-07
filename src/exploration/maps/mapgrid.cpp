/*! @file mapgrid.cpp
 * @author anantonov
 * @date Created on Jul 2, 2014
 *
 * @brief Implementation of the reference coordinate grid helper functions
 */


#include "mapgrid.h"
#include <cassert>
#include <cmath>

namespace VCGL {

float MapGrid::dx() const {
	assert(nlon() > 1);
	return lons[1]-lons[0];
}

float MapGrid::lonMax() const {
	unsigned nlon = lons.size();
	assert(nlon > 0);

	return (lons[0] > lons[nlon-1] ? lons[0] : lons[nlon-1]);
}

float MapGrid::lonMin() const {
	unsigned nlon = lons.size();
	assert(nlon > 0);

	return (lons[0] < lons[nlon-1] ? lons[0] : lons[nlon-1]);
}

float MapGrid::lonRange() const {
	assert(nlon() > 0);

	return fabs(dx()*(nlon()-1));
}

bool MapGrid::loopedLon() const {
	unsigned nlon = lons.size();
	assert(nlon > 2);

	float lonFullRange = 360.0;
	float dx = lons[1]-lons[0]; // change in a grid step

	float regionBorderLeft = lons[0] - dx/2.0;
	float regionBorderRight = lons[nlon-1] + dx/2.0;

	//ideally, the borders have exactly lonFullRange distance (360.0)
	//but we'll allow for a small gap/overlap of 1/3 of a cell
	return ( fabs( fabs(regionBorderRight - regionBorderLeft) - lonFullRange) < dx/3 );
}

float MapGrid::dy() const {
	assert(nlat() > 1);

	return lats[1]-lats[0];
}

float MapGrid::latMax() const {
	unsigned nlat = lats.size();
	assert(nlat > 0);

	return (lats[0] > lats[nlat-1] ? lats[0] : lats[nlat-1]);
}

float MapGrid::latMin() const {
	unsigned nlat = lats.size();
	assert(nlat > 0);

	return (lats[0] < lats[nlat-1] ? lats[0] : lats[nlat-1]);
}

QPointF MapGrid::nearestGridPoint(const QPointF& coordinates) const {
	assert(nlon()>0 && nlat()>0);

	float lonDiff = fabs( coordinates.x() - lons[0] );
	float lonF = lons[0];
	for (unsigned i=1; i<nlon(); i++) {
		float diff = fabs( coordinates.x() - lons[i]);
		if (diff < lonDiff) {
			lonDiff = diff;
			lonF = lons[i];
		}
	}

	float latDiff = fabs( coordinates.y() - lats[0] );
	float latF = lats[0];
	for (unsigned i=1; i<nlat(); i++) {
		float diff = fabs( coordinates.y() - lats[i]);
		if (diff < latDiff) {
			latDiff = diff;
			latF = lats[i];
		}
	}
	return QPointF(lonF,latF);
}

} //namespace VCGL
