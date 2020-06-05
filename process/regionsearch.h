/*! @file regionsearch.h
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef REGIONSEARCH_H_
#define REGIONSEARCH_H_

#include <vector>

#include <queue>
#include <QPoint>
#include <list>
#include "link.h"

namespace VCGL {
class RegionConnectivity;

struct RSHelper {
	virtual ~RSHelper() {}
	virtual bool xLooped() const = 0;
	virtual float getCorrelationValue(const QPoint& aIndices, const QPoint& bIndices) const  = 0;
	virtual bool isTeleconnectivitySignificant(const QPoint& indices) const = 0;
	virtual bool isCorrelationSignificant(const QPoint& aIndices, const QPoint& bIndices) const = 0;
};

class RegionSearch {
public:
	virtual ~RegionSearch() {}
	virtual unsigned findRegions(const std::vector< std::vector<float> >& tc,
				const std::vector< std::vector<QPoint> >& tcindices,
				const float threshold,
				std::vector< std::vector<int> >& regionMap,
				VCGL::RegionConnectivity& connectivity,
				VCGL::RSHelper* pHelper = 0);

	virtual void filterRegionMapTCThreshold(
			const std::vector< std::vector<float> >& tc,
			const float threshold,
			std::vector< std::vector<int> >& regionMap,
			VCGL::RSHelper* pHelper = 0);
protected:
	virtual QPoint findMaximalUnmarkedPoint(
			const std::vector< std::vector<int> >& regionMap,
			const std::vector< std::vector<float> >& tc);

	virtual bool seed(const std::vector< std::vector<float> >& tc,
				std::vector< std::vector<int> >& regionMap,
				int& nextRegion,
				std::queue<QPoint>& q);

	virtual void processPointNeighbors(QPoint p,
			std::queue<QPoint>& q,
			std::vector< std::vector<int> >& regionMap,
			QPoint seed,
			VCGL::RSHelper* pHelper = 0);
	virtual void growRegion(
			std::queue<QPoint>& q,
			std::vector< std::vector<int> >& regionMap,
			VCGL::RSHelper* pHelper = 0);

};

} // namespace VCGL

#endif /* REGIONSEARCH_H_ */
