/*! @file mapgrid.h
 * @author anantonov
 * @date Created on Jun 4, 2014
 *
 * @brief Definition of the structure describing the reference coordinate grid
 */

#ifndef MAPGRID_H_
#define MAPGRID_H_

#include <vector>
#include <QPointF>

namespace VCGL {

/*! @brief Structure describing the reference coordinate grid.
 *
 *  It is assumed that the grid coordinates monotonously increase
 *  or decrease in each direction, and the difference in coordinates
 *  between neighboring cells is always constant
 */
struct MapGrid {
	MapGrid() {}
	~MapGrid() {}
	MapGrid(const MapGrid& other): lons(other.lons), lats(other.lats) {	}
	const MapGrid& operator=(const MapGrid& other) {
		if (this != &other) {
			lons = other.lons;
			lats = other.lats;
		}
		return (*this);
	}

	std::vector<float> lons; ///< collection of longitudes
	std::vector<float> lats; ///< collection of latitudes

	/// number of points in longitude dimension
	unsigned nlon() const { return lons.size(); }
	/// number of points in latitude dimension
	unsigned nlat() const { return lats.size(); }

	/// difference between coordinates in the x (longitude) dimension
	float dx() const;
	/// maximal longitude (left or right border)
	float lonMax() const;
	/// minimal longitude (left or right border)
	float lonMin() const;
	/// range of longitudes
	float lonRange() const;
	/// flag indicating whether the map loops in the longitude dimension
	bool loopedLon() const;

	/// difference between coordinates in the y (latitude) dimension
	float dy() const;
	/// maximal latitude (top or bottom)
	float latMax() const;
	/// minimal latitude (top or bottom)
	float latMin() const;

	/// get the nearest grid point for the specified point
	QPointF nearestGridPoint(const QPointF& coordinates) const;
};

} //namespace VCGL
#endif /* MAPGRID_H_ */
