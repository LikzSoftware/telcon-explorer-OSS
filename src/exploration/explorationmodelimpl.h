/*! @file explorationmodelimpl.h
 * @author anantonov
 * @date Created on Jun 22, 2014
 *
 * @brief Class fully implementing the ExplorationModel interface
 */

#ifndef EXPLORATIONMODELIMPL_H_
#define EXPLORATIONMODELIMPL_H_

#include "explorationmodel.h"
#include "process/regionsearch.h"

namespace VCGL {

///Class providing full implementation of the ExplorationModel interface
class ExplorationModelImpl: public ExplorationModel, public RSHelper {
public:
	/// Default constructor
	ExplorationModelImpl();
	/// Destructor for proper memory allocation
	virtual ~ExplorationModelImpl();

	/// @copydoc ExplorationModel::loadGrid
	virtual void loadGrid(TCStorage& storage) override;
	/// @copydoc ExplorationModel::loadGrid
	virtual void loadGrid(const std::string& fileName) override;
	/// @copydoc ExplorationModel::loadContours
	virtual void loadContours(const std::string& contoursFileName) override;
	/// @copydoc ExplorationModel::loadCorrelations
	virtual void loadCorrelations(const std::string& correlationsFileName) override;
	/// @copydoc ExplorationModel::loadAutocorrelations
	virtual void loadAutocorrelations(const std::string& autocorrFileName) override;
	/// @copydoc ExplorationModel::loadProjection
	virtual void loadProjection(const std::string& projectionFileName) override;

	/// @copydoc ExplorationModel::setThreshold
	virtual void setThreshold(float newValue) override;

	/// @copydoc ExplorationModel::getClosestPointCorrelationValue
	virtual bool getClosestPointCorrelationValue(const QPointF& point, float* pValue) const override;
	/// @copydoc ExplorationModel::getClosestPointTeleconnectivityValue
	virtual bool getClosestPointTeleconnectivityValue(const QPointF& point, float* pValue) const override;

	/// @copydoc ExplorationModel::getSelectionMask
	virtual void getSelectionMask(std::vector< std::vector<bool> >& selectionMask) const override;
	/// @copydoc ExplorationModel::getCorrelationMapColors
	virtual void getCorrelationMapColors(std::vector< std::vector<float> >& colorData) const override;

	/// @copydoc ExplorationModel::computeStatisticalSignificanceMask
	virtual void computeStatisticalSignificanceMask(float ssLevel) override;
	/// @copydoc ExplorationModel::getStatisticalSignificanceMask
	virtual void getStatisticalSignificanceMask(std::vector< std::vector<bool> >& ssMask) const override;
	/// @copydoc ExplorationModel::getTeleconnectivityMapColors
	virtual void getTeleconnectivityMapColors(std::vector< std::vector<float> >& colorData) const override;

	/// @copydoc ExplorationModel::getCorrelationMapChainLinks
	virtual void getCorrelationMapChainLinks(std::vector<VCGL::AnnotationLinkF>& links) const override;
	/// @copydoc ExplorationModel::getCorrelationChainProjection
	virtual void getCorrelationChainProjection(std::vector< QPointF >& chainProjection) const override;
	/// @copydoc ExplorationModel::getTeleconnectivityMapLinks
	virtual void getTeleconnectivityMapLinks(std::vector<VCGL::AnnotationLinkF>& links) const override;
	/// @copydoc ExplorationModel::getTeleconnectivityLinks
	virtual void getTeleconnectivityLinks(std::vector<VCGL::LinkF>& links) const override;
	/// @copydoc ExplorationModel::getProjectionData
	virtual void getProjectionData(std::vector< std::vector<QPointF> >& projectionData) const override;

	/// @copydoc ExplorationModel::selectReferencePoint
	virtual void selectReferencePoint( const QPointF& pointCoordinates ) override;
	/// @copydoc ExplorationModel::getReferencePoint
	virtual QPointF getReferencePoint() const override;
	/// @copydoc ExplorationModel::getReferencePointProjection
	virtual QPointF getReferencePointProjection() const override;

	/// @copydoc ExplorationModel::selectRegionAtPoint
	virtual void selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent) override;
	/// @copydoc ExplorationModel::resetRegionSelection
	virtual void resetRegionSelection() override;
	/// @copydoc ExplorationModel::setSelectionMask
	virtual void setSelectionMask(const std::vector< std::vector<bool> >& selectionMask, unsigned numSelected) override;

	/// @copydoc ExplorationModel::pointsEqual
	virtual bool pointsEqual(const QPointF& a, const QPointF& b) const override;

	/// @copydoc RSHelper::getCorrelationValue
	virtual float getCorrelationValue( const QPoint& aIndices, const QPoint& bIndices ) const override;
	/// @copydoc RSHelper::xLooped
	virtual bool xLooped() const override;
	/// @copydoc RSHelper::isCorrelationSignificant
	virtual bool isCorrelationSignificant(const QPoint& aIndices, const QPoint& bIndices) const override;
	/// @copydoc RSHelper::isTeleconnectivitySignificant
	virtual bool isTeleconnectivitySignificant(const QPoint& indices) const override;

public:
	VCGL::AnnotationLinkF tcLinkForPointIndices(QPoint pt)
	{
		QPoint ptB = tcindices[pt.y()][pt.x()];
		QPointF ptAF = indicesToCoordinates(pt);
		QPointF ptBF = indicesToCoordinates(ptB);
		return VCGL::AnnotationLinkF(ptAF, ptBF, VCGL::AnnotationLinkF::SHOW_LINE );
	}

private:
	/// Find indices in longitude and latitude arrays for the point given as (lon,lat)-pair
	QPoint findClosestPointIndices(const QPointF& pointCoordinates) const;

	/// Return a point corresponding to the specified indices in lon/lat arrays
	QPointF indicesToCoordinates(const QPoint& indices) const;

	/// Build the correlation chain (@see ExplorationModel::getCorrelationMapChainLinks)
	void buildCorrelationChain();

	/*! @brief Compute teleconnectivity for points.
	 *
	 * For each point, its teleconnectivity is the absolute value of the most negative correlation
	 * with another point. For the exploration purposes, it is required to know the point to which
	 * the point of interest exposes that amount of teleconnectivity.
	 *
	 */
	void computeTeleconnectivity();

	/*! @brief Perform the Student t-test of statistical significance
	 *
	 * @param ssLevel		Level of significance (here e.g. 0.9, 0.95, 0.99)
	 * @param t				t-value
	 * @param df			number of degrees of freedom
	 * @param directional	one or two-tailed test.
	 * 						true for one-tailed/directional test,
	 * 						false for two-tailed test
	 * @return true if this t-value is statistically significant at the
	 * 			given ssLevel
	 */
	bool ttest(float ssLevel, double t, float df, bool directional) const;

	/*! @brief Clip contours to the given map area
	 *
	 * @param[in] clGrid		Data reference coordinate grid which defines the analyzed map area
	 * @param[in,out] clContours	Collection of contours, on input containing all possible contours,
	 * 								on output containing the clipped contours
	 */
	void clipContours(const MapGrid& clGrid, std::vector< std::vector<QPointF> >& clContours);

	QPoint refPtIndices;	///< (iLon,iLat) - indices of the reference point cell in the longitude/latitude arrays of the grid

	/*!
	 * A matrix containing all pairwise correlations between points.
	 * The size of matrix is npoints x npoints, where npoints = nlat*nlon,
	 * and each individual point is assigned an id = iLat*nlon + iLon.
	 * (
	 * 		npoints - number of points,
	 * 		nlat - number of latitudes in the grid,
	 * 		nlon - number of longitudes in the grid,
	 * 		iLat - index of point's latitude in the grid,
	 * 		iLon - index of point's longitude in the grid,
	 * 		id   - point's identifier
	 * 	)
	 */
	std::vector< std::vector<float> > correlations;

	/*!
	 *  A vector of correlations of all points to themselves with a lag 1
	 *  (indexed by point identifier: id = iLat*nlon  + iLon)
	 */
	std::vector< float > autocorrelations;

	/// Points which are chosen for the correlation chain (@see ExplorationModel::getCorrelationMapChainLinks)
	std::vector<QPoint> chosenPoints;

	/*!
	 * Teleconnectivity values: for each point of the map,
	 * the absolute value of the most negative correlation
	 * that the time series at this point reaches with any other.
	 * (Indexed like map, first is latitude index, second is longitude index,
	 * i.e. tc[iLat][iLon])
	 */
	std::vector< std::vector<float> > tc;

	/*! Teleconnectivity points: for each point of the map,
	 * the indices of a point with which this point reaches
	 * its teleconnectivity value
	 * (Indexed like map, first is latitude index, second is longitude index,
	 * i.e. tcindices[iLat][iLon])
	 */
	std::vector< std::vector<QPoint> > tcindices;

	/// number of regions found in the teleconnectivity map
	unsigned nRegions;

	/*!
	 * Region assignments: for each point of the map,
	 * the id of the region it was assigned to.
	 * (Indexed like map, first is latitude index, second is longitude index,
	 * i.e. regionMap [iLat][iLon])
	 *
	 * Currently, valid region numbers are positive,
	 * with 0 reserved for a thresholded area
	 * (teleconnectivity below threshold)
	 */
	std::vector<std::vector<int> > regionMap;

	/// Region connectivity: provides links between regions
	VCGL::RegionConnectivity rc;

	/*!
	 * Projection result: for each point of the map,
	 * its coordinates in the projected space
	 * (Indexed like map, first is latitude index, second is longitude index,
	 * i.e. projectionData[iLat][iLon])
	 */
	std::vector< std::vector<QPointF> > projectionData;

	/*!
	 * Selection mask: for each point of the map,
	 * true if the point is currently selected, false otherwise.
	 * Could be empty, ignored if numSelectedPoints <= 0
	 * (Indexed like map, first is latitude index, second is longitude index,
	 * i.e. selectionMask[iLat][iLon])
	 */
	std::vector< std::vector<bool> > selectionMask;

	/*!
	 * Statistical significance mask: for each point of the map,
	 * true if the point is statistically significant at the previously
	 * specified level (here: 0.9, 0.95, 0.99 etc),
	 * false otherwise.
	 * (Indexed like map, first is latitude index, second is longitude index,
	 * i.e. statisticalSignificanceMask[iLat][iLon])
	 */
	std::vector< std::vector<bool> > statisticalSignificanceMask;

	/*!
	 * Number of currently selected points (corresponds to number of true values
	 * in selectionMask when numSelectedPoints > 0)
	 */
	unsigned numSelectedPoints;
};

} /* namespace VCGL */

#endif /* EXPLORATIONMODELIMPL_H_ */
