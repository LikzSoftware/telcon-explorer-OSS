/*! @file regionsearch.cpp
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "regionsearch.h"
#include "regionconnectivity.h"
#include <cassert>

namespace {

bool checkLocalMaximum(const std::vector< std::vector<float> >& tc,
					   int lat, int lon, int nlat, int nlon) {
	bool bMax = true;
	const float val = tc[lat][lon];

	for (int i = -1; i<= 1; i++) {
		for (int j=-1; j<=1; j++) {
			if (i!=0 || j!=0) {
				int ii = lat+i;
				int jj = lon+j;
				if ( ii >= 0 && ii <= nlat-1 && jj >=0 && jj <= nlon-1) {
					if (tc[ii][jj] > val) {
						bMax = false;
					}
				}
			}
		}
	}

	return bMax;
}

}

namespace VCGL {

unsigned
RegionSearch::findRegions(const std::vector< std::vector<float> >& tc,
		const std::vector< std::vector<QPoint> >& tcindices,
		const float threshold,
		std::vector< std::vector<int> >& regionMap,
		VCGL::RegionConnectivity& connectivity,
		VCGL::RSHelper* pHelper) {

	assert(tc.size() > 0 && tc[0].size() > 0);

	const int nlat = tc.size();
	const int nlon = tc[0].size();

	assert(tcindices.size() == (unsigned)nlat && tcindices[0].size()==(unsigned)nlon);

	regionMap.clear();
	const std::vector<int> rmapRow(nlon,-1);
	regionMap.resize(nlat, rmapRow);
	connectivity = VCGL::RegionConnectivity();

	filterRegionMapTCThreshold(tc, threshold, regionMap, pHelper);
	int nextRegion = 1;

	std::queue<QPoint> pointQueue;
	bool bSeedFound = false;

	do {
		bSeedFound = seed(tc,regionMap, nextRegion, pointQueue);
		if (!pointQueue.empty()) {
			growRegion(pointQueue, regionMap, pHelper);
		}
	} while( bSeedFound );


	for (int i=0; i<nlat; i++) {
		for (int j=0; j<nlon; j++) {
			if (checkLocalMaximum(tc, i, j, nlat, nlon)) {
				QPoint ptB = tcindices[i][j];

				int regionA = regionMap[i][j];
				int regionB = regionMap[ptB.y()][ptB.x()];

				//check that link starts and ends above threshold
				if (regionA >0 && regionB >0 && regionA != regionB) {
					connectivity.suggestLink(
						regionA,
						regionB,
						VCGL::Link{ QPoint{j,i}, ptB, tc[i][j] });
				}
			}
		}
	}

	return nextRegion;
}

///sets region number for all filtered points to 0
void
RegionSearch::filterRegionMapTCThreshold(const std::vector< std::vector<float> >& tc,
		const float threshold,
		std::vector< std::vector<int> >& regionMap,
		VCGL::RSHelper* pHelper) {
	assert(tc.size() == regionMap.size());
	assert(tc[0].size() == regionMap[0].size());
	unsigned nlat = tc.size();
	unsigned nlon = tc[0].size();

	for (unsigned i=0; i<nlat; i++) {
		for (unsigned j=0; j<nlon; j++) {
			if (tc[i][j] < threshold || (pHelper != 0 && !pHelper->isTeleconnectivitySignificant( {(int)j,(int)i} ))) {
				regionMap[i][j] = 0;
			}
		}
	}
}

QPoint RegionSearch::findMaximalUnmarkedPoint(const std::vector< std::vector<int> >& regionMap,
		const std::vector< std::vector<float> >& tc) {
	assert(regionMap.size() > 0 && regionMap[0].size() > 0);

	const int nlat = regionMap.size();
	const int nlon = regionMap[0].size();

	assert(tc.size() == (unsigned)nlat && tc[0].size() == (unsigned)nlon);
	float maxTC = -1.0;
	QPoint maxPt = {-1,-1};

	for (int i=0; i<nlat; i++) {
		for (int j=0; j<nlon; j++) {
			if (regionMap[i][j]<0 && tc[i][j] > maxTC) {
				if (checkLocalMaximum(tc, i, j, nlat, nlon)) {
					maxTC = tc[i][j];
					maxPt = {j, i};
				}
			}
		}
	}
	return maxPt;
}

bool
RegionSearch::seed(const std::vector< std::vector<float> >& tc,
			std::vector< std::vector<int> >& regionMap,
			int& nextRegion,
			std::queue<QPoint>& q)  {
	bool bSeedFound = false;

	QPoint pt = findMaximalUnmarkedPoint(regionMap, tc);
	if (pt.x()>=0 && pt.y()>=0) {
		regionMap[pt.y()][pt.x()] = nextRegion++;
		q.push(pt);
		bSeedFound = true;
	}
	return bSeedFound;
}

void
RegionSearch::processPointNeighbors(QPoint pt,
		std::queue<QPoint>& q,
		std::vector< std::vector<int> >& regionMap,
		QPoint seed,
		VCGL::RSHelper* pHelper){
	const int nlat = regionMap.size();
	const int nlon = regionMap[0].size();

	//remember about QPoint(lon,lat) while array[lat][lon]!
	int ptlon = pt.x();
	int ptlat = pt.y();
	int ptRegion = regionMap[ptlat][ptlon];
	assert(ptRegion >= 0);

	for (int i=-1; i<2; i++) {
		for (int j=-1;j<2;j++) {
			int neilat = ptlat+i;
			int neilon = ptlon+j;
			if (pHelper != 0 && pHelper->xLooped()) {
				neilon = (neilon+nlon)%nlon;
			}

			if (neilat >= 0 && neilat <= nlat-1 && neilon >= 0 && neilon <= nlon-1) {
				QPoint neighbor = QPoint(neilon,neilat);
				if (regionMap[neilat][neilon] < 0 &&
						(pHelper==0
						 ||
						 (pHelper->getCorrelationValue(seed, neighbor)>=0.0
						  && pHelper->isCorrelationSignificant(seed, {neilon,neilat}))
						 )
					) {
					regionMap[neilat][neilon] = ptRegion;
					q.push(QPoint(neilon,neilat));
				}
			}
		}
	}
}

void
RegionSearch::growRegion(
		std::queue<QPoint>& q,
		std::vector< std::vector<int> >& regionMap,
		VCGL::RSHelper* pHelper)
{
	assert(!q.empty());
	QPoint seed = q.front(); //first point in the queue as the method is called

	while(!q.empty()) {
		QPoint pt = q.front();
		q.pop();
		processPointNeighbors(pt, q, regionMap, seed, pHelper);
	}
}

} // namespace VCGL

//void findRegions() {
//	int nlon; int nlat;
//	std::vector< std::vector<float> > tc;
//	float threshold;
//
//	int npoints = nlon*nlat;
//	int nextOdd = 1;
//	int nextEven = 2;
//
//
//	std::vector< std::vector<int> > regionMap (nlat, std::vector<int>(nlon, -1) );
//
//}

/* (1)
 * init regionMap with -1, nextOdd 1, nextEven 2
 * threshold-filter regionMap -> set to 0 those points which are below threshold
 *
 * loopA: while non-marked pixels exist (do..while)
 *
 * find unmarked pixel m with maximal teleconnectivity
 * put to the link list: ptA = m, ptB = m, prev = 0, tc = 0
 *
 * loopB: while links list not empty
 * take link
 * if link.ptB non-marked:
 * 		mark
 * 		add link to connectivity matrix
 * 		add ptB to the queue q
 * 	else
 * 		check if the link in connectivity matrix has smaller teleconnectivity value
 * 		if so, replace with the current one
 *
 * 	loopC: while queue q not empty
 * 		take a pixel p
 * 		add its link to the links list
 *		for each neighbor
 *			if neighbor is not marked
 *				mark it as p
 *				add it to the queue q
 *	{end loopC}
 *	{end loopB}
 *	{end loopA}
 */
