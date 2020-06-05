/*! @file explorationmodel.h
 * @author anantonov
 * @date Created on Jun 4, 2014
 *
 * @brief Definition of the MVC-Model class for the exploration of teleconnections
 */

#ifndef EXPLORATIONMODEL_H_
#define EXPLORATIONMODEL_H_

#include <string>
#include <vector>
#include "maps/mapgrid.h"
#include "maps/annotationlink.h"
#include "process/link.h"
#include "process/regionconnectivity.h"
#include <QPointF>

namespace VCGL {
class Model;
class TCStorage;

/// Abstract base class representing MVC-Model for the exploration of teleconnections
class ExplorationModel {
public:
	/// Default constructor
	ExplorationModel();
	/// Virtual destructor for proper memory deallocation
	virtual ~ExplorationModel();

	/// Load data coordinate reference grid from the specified file
	virtual void loadGrid(TCStorage& storage) = 0;

	/// Load data coordinate reference grid from the specified file
	virtual void loadGrid(const std::string& fileName) = 0;

	/// Load land contours data from the specified file
	virtual void loadContours(const std::string& contoursFileName) = 0;

	/// Load precomputed correlations data from the specified file
	virtual void loadCorrelations(const std::string& correlationsFileName) = 0;

	/// Load precomputed correlations data from the specified file
	virtual void loadAutocorrelations(const std::string& autocorrFileName) = 0;

	/// Load precomputed projection data from the specified file
	virtual void loadProjection(const std::string& projectionFileName) = 0;

	/// Get number of points in the longitude dimension
	virtual size_t nlon() const { return grid.lons.size(); }

	/// Get number of points in the latitude dimension
	virtual size_t nlat() const { return grid.lats.size(); }

	/// Get number of time steps
	virtual size_t ntime() const { return _ntime; }

	/// Get the data coordinate reference grid
	virtual const MapGrid& getGrid() const { return grid; }

	/*! @brief Get the land contours.
	 *
	 *  Outer collection represents different contours,
	 *  inner collection represents points of a contour as (lon, lat)-pairs
	 */
	virtual const std::vector< std::vector<QPointF> >& getContours() const { return contours; }

	/// Get current threshold
	virtual float getThreshold() const { return threshold; }

	/// Set new threshold value
	virtual void setThreshold(float newValue) { threshold = newValue; }

	/*! @brief Get correlation value for the point closest to the specified one
	 *
	 * @param[in] point  Coordinates of a point as a (lon, lat)-pair
	 * @param[out] pValue Pointer to a float receiving the requested correlation value
	 * @return true if the operation was successful, false otherwise
	 */
	virtual bool getClosestPointCorrelationValue(const QPointF& point, float* pValue) const;

	/*! @brief Get teleconnectivity value for the point closest to the specified one
	 *
	 * @param[in] point  Coordinates of a point as a (lon, lat)-pair
	 * @param[out] pValue Pointer to a float receiving the requested teleconnectivity value
	 * @return true if the operation was successful, false otherwise
	 */
	virtual bool getClosestPointTeleconnectivityValue(const QPointF& point, float* pValue) const;

	/*! @brief Get current selection mask.
	 *
	 *	@attention Two-dimensional vector has [iLat][iLon]-indices.
	 *	If no specific selection set, an empty vector is returned.
	 *
	 * @param selectionMask Reference to a vector receiving selection mask.
	 */
	virtual void getSelectionMask(std::vector< std::vector<bool> >& selectionMask) const;

	/*! @brief Get color data for the correlation map.
	 *
	 *	@attention Two-dimensional vector has [iLat][iLon]-indices.
	 *	Color data is in range [0,1]
	 *
	 * @param colorData Reference to a vector receiving color data.
	 */
	virtual void getCorrelationMapColors(std::vector< std::vector<float> >& colorData) const;

	/// Compute the statistical significance mask at a given level (here: e.g. 0.9, 0.95, 0.99)
	virtual void computeStatisticalSignificanceMask(float ssLevel);

	/*! @brief Get statistical significance mask
	 *
	 * @attention Two-dimensional vector has [iLat][iLon]-indices.
	 * True if the teleconnectivity value is statistically significant at the
	 * previously specified level, false otherwise.
	 *
	 * @param ssMask Reference to a vector receiving color data.
	 */
	virtual void getStatisticalSignificanceMask(std::vector< std::vector<bool> >& ssMask) const;

	/*! @brief Get color data for the teleconnectivity map.
	 *
	 *	@attention Two-dimensional vector has [iLat][iLon]-indices.
	 *	Color data is in range [0,1]
	 *
	 * @param colorData Reference to a vector receiving color data.
	 */
	virtual void getTeleconnectivityMapColors(std::vector< std::vector<float> >& colorData) const;

	/*! @brief Get correlation chain as a collection of links for the map view
	 *
	 * Correlation chain is a sequence of points that is built starting
	 * from the reference point and iteratively taking the most negatively
	 * correlated point to the last, not included any of already taken points
	 *
	 * @param links Reference to a vector receiving the correlation chain links
	 */
	virtual void getCorrelationMapChainLinks(std::vector<VCGL::AnnotationLinkF>& links) const;

	/*! @brief Get projected version of correlation chain for the projection view
	 *
	 * Correlation chain is a sequence of points that is built starting
	 * from the reference point and iteratively taking the most negatively
	 * correlated point to the last, not included any of already taken points
	 *
	 * @param chainProjection Reference to a vector receiving the projected correlation chain
	 */
	virtual void getCorrelationChainProjection(std::vector< QPointF >& chainProjection) const;

	/*! @brief Get collection of the teleconnectivity links for the map view
	 *
	 * Teleconnectivity links represent the strongest one-directional links between regions
	 * in the teleconnectivity map. (Regions are defined by a separate algorithm, which
	 * depends the teleconnectivity threshold into account as well)
	 *
	 * @param links Reference to a vector receiving a collection
	 */
	virtual void getTeleconnectivityMapLinks(std::vector<VCGL::AnnotationLinkF>& links) const;

	/*! @brief Get collection of the teleconnectivity links (for the list view)
	 *
	 * Teleconnectivity links represent the strongest one-directional links between regions
	 * in the teleconnectivity map. (Regions are defined by a separate algorithm, which
	 * depends the teleconnectivity threshold into account as well)
	 *
	 * @param links Reference to a vector receiving a collection
	 */
	virtual void getTeleconnectivityLinks(std::vector<VCGL::LinkF>& links) const;

	/*! @brief Get the projection data
	 *
	 * @attention Two-dimensional vector has [iLat][iLon]-indices.
	 *
	 * Projection presents an overview on the correlations between all time series.
	 * The distance measure used at the moment of this comment writing (used since rev. 272,
	 * @see DistanceMatrix::fromCorrelationMatrixArray), favors points corresponding to
	 * highly positively correlated time series to be placed close to each other, while
	 * points for less correlated and highly negatively correlated time series
	 * are to be placed far apart.
	 *
	 *
	 * @param projectionData Reference to a vector receiving projection data.
	 */
	virtual void getProjectionData(std::vector< std::vector<QPointF> >& projectionData) const;

	/*! @brief Select reference point
	 *
	 * Reference point is the point, correlations with which are visualized as color data in
	 * the correlation map view. It is also the starting point for building the correlation chain
	 *
	 * @param pointCoordinates (lon,lat)-pair of coordinates to be selected as a new reference point
	 */
	virtual void selectReferencePoint( const QPointF& pointCoordinates );

	/// Get the reference point as a (lon,lat)-pair
	virtual QPointF getReferencePoint() const;
	/// Get the reference point in projected space
	virtual QPointF getReferencePointProjection() const;

	/*! @brief Select region around the point
	 *
	 * @param point	(lon,lat)-pair of a point inside the region to be selected
	 * @param bSelectWholeComponent true for inclusion of connected regions into selection,
	 * 								false otherwise
	 */
	virtual void selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent);

	/// Reset the region selection
	virtual void resetRegionSelection();

	/*! @brief Set the new selection mask
	 *
	 * @param selectionMask Two-dimensional vector containing mask of point selection
	 * 						(for each point: true if the point is selected, false otherwise).
	 * 						If no points are selected, selectionMask can be an empty vector.
	 *
	 * @param numSelected	Performance hint. SelectionMask can be empty when numSelected is zero.
	 */
	virtual void setSelectionMask(const std::vector< std::vector<bool> >& selectionMask, unsigned numSelected);

	///Compare two (lon,lat) points to determine whether they belond to the same grid cell
	virtual bool pointsEqual(const QPointF& a, const QPointF& b) const;

protected:
	MapGrid grid;		///< Data reference coordinate grid
	size_t _ntime;		///< Number of time steps
	std::vector< std::vector<QPointF> > contours; ///< Land contours as two-dimensional collection of (lon,lat)-pairs

	float threshold; ///< Teleconnection threshold used in visualization, as well as in determining regions
};

} /* namespace VCGL */

#endif /* EXPLORATIONMODEL_H_ */
