/*! @file regionsearchtest.cpp
 * @author anantonov
 * @date Created on May 17, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */


#include "CppUnitLite/TestHarness.h"
#include "cppunitextras.h"

#include "process/regionsearch.h"
#include "process/link.h"
#include "process/regionconnectivity.h"

#include <vector>
#include <queue>
#include <set>
#include <list>
#include <QPoint>

#include <string.h>
#include <cassert>

namespace Testing {

class TestingRegionSearch: public VCGL::RegionSearch {
public:
	virtual ~TestingRegionSearch() {}

	virtual QPoint findMaximalUnmarkedPoint(
			const std::vector< std::vector<int> >& regionMap,
			const std::vector< std::vector<float> >& tc) override {
		return VCGL::RegionSearch::findMaximalUnmarkedPoint(regionMap, tc);
	}
	virtual bool seed(const std::vector< std::vector<float> >& tc,
			std::vector< std::vector<int> >& regionMap,
			int& nextRegion,
			std::queue<QPoint>& q)  override {
		return VCGL::RegionSearch::seed(tc, regionMap, nextRegion, q);
	}
	virtual void processPointNeighbors(QPoint p,
			std::queue<QPoint>& q,
			std::vector< std::vector<int> >& regionMap,
			QPoint seed,
			VCGL::RSHelper* pHelper = 0)  override {
		VCGL::RegionSearch::processPointNeighbors(p,q,regionMap, seed, pHelper);
	}
	virtual void growRegion(
			std::queue<QPoint>& q,
			std::vector< std::vector<int> >& regionMap,
			VCGL::RSHelper* pHelper = 0) override {
		VCGL::RegionSearch::growRegion(q, regionMap, pHelper);
	}

};

struct QPointOrder {
	bool operator() (const QPoint& a, const QPoint& b) {
		return ( a.x() < b.x() || (a.x() == b.x() && a.y() < b.y()));
	}
};

TEST(filterRegionMapTCThreshold, PrecomputeRegionSearch)
{
	std::vector< std::vector<float> > tc;
	tc.push_back(std::vector<float>({0.2, 0.7, 0.1}));

	std::vector< std::vector<int> > regionMap;
	regionMap.push_back(std::vector<int>(3, -1) );

	VCGL::RegionSearch rs;
	rs.filterRegionMapTCThreshold(tc, 0.5, regionMap);

	CHECK( regionMap[0][0] == 0 );
	CHECK( regionMap[0][1] == -1);
	CHECK( regionMap[0][2] == 0 );
}

TEST(processPointNeighbors, PrecomputeRegionSearch)
{
	std::vector< std::vector<int> > regionMap(3, std::vector<int>(3, -1));
	regionMap[1][1] = 1;

	std::queue<QPoint> q;

	TestingRegionSearch trs;
	trs.processPointNeighbors(QPoint(1,1), q, regionMap, QPoint(1,1));

	CHECK( regionMap[0][1] == 1 );
	CHECK( regionMap[1][0] == 1 );
	CHECK( regionMap[1][1] == 1 );
	CHECK( regionMap[1][2] == 1 );
	CHECK( regionMap[2][1] == 1 );

	CHECK( regionMap[0][0] == 1 );
	CHECK( regionMap[0][2] == 1 );
	CHECK( regionMap[2][0] == 1 );
	CHECK( regionMap[2][2] == 1 );

	std::set<QPoint, QPointOrder> qSet;
	while (!q.empty()) {
		qSet.insert(q.front());
		q.pop();
	}

	CHECK( qSet.find(QPoint(1,1)) == qSet.end());
	CHECK_EQUAL( 8L, (long int)qSet.size() );
}

TEST(processPointNeighborsMarked, PrecomputeRegionSearch)
{
	std::vector< std::vector<int> > regionMap(3, std::vector<int>(3, -1));
	regionMap[1][1] = 1;
	regionMap[2][0] = 2;

	std::queue<QPoint> q;

	TestingRegionSearch trs;
	trs.processPointNeighbors(QPoint(1,1), q, regionMap, QPoint(1,1));

	CHECK( regionMap[0][1] == 1 );
	CHECK( regionMap[1][0] == 1 );
	CHECK( regionMap[1][1] == 1 );
	CHECK( regionMap[1][2] == 1 );
	CHECK( regionMap[2][1] == 1 );

	CHECK( regionMap[0][0] == 1 );
	CHECK( regionMap[0][2] == 1 );
	CHECK( regionMap[2][0] == 2 );
	CHECK( regionMap[2][2] == 1 );

	std::set<QPoint, QPointOrder> qSet;
	while (!q.empty()) {
		qSet.insert(q.front());
		q.pop();
	}

	CHECK( qSet.find(QPoint(1,1)) == qSet.end());
	CHECK_EQUAL( 7L, (long int)qSet.size() );
}

TEST(processPointNeighborsNoThrowSingle, PrecomputeRegionSearch)
{
	std::vector< std::vector<int> > regionMap(1, std::vector<int>(1, 1));
	std::queue<QPoint> q;

	TestingRegionSearch trs;
	try {
		//attention: not testing the helper extension currently
		trs.processPointNeighbors(QPoint(0,0), q, regionMap, QPoint(0,0));
	}
	catch(...) {
		CHECK(!"RegionSearch::processPointNeighbors should now throw");
	}

	CHECK( regionMap[0][0] == 1 );
	CHECK(q.empty());
}

TEST(growRegion, PrecomputeRegionSearch)
{
//	std::vector< std::vector<float> > tc(1);
//	tc[0] = {0.7, 0.8, 0.6, 0.2, 0.9};
//
//	std::vector< std::vector<QPoint> > tcindices(1);
//	tcindices[0] = { QPoint(4,0), QPoint(4,0), QPoint(4,0), QPoint(2,0), QPoint(2,0) };

	std::vector< std::vector<int> > regionMap(1);
	regionMap[0] = { 1, -1, -1, 2, -1 };

	std::queue<QPoint> q;
	q.push(QPoint(0,0));

	TestingRegionSearch trs;
	trs.growRegion(q, regionMap);

	CHECK(q.empty());

	CHECK( regionMap[0][0] == 1 );
	CHECK( regionMap[0][1] == 1 );
	CHECK( regionMap[0][2] == 1 );
	CHECK( regionMap[0][3] == 2 );
	//should be 1 for horizontal looping, -1 without looping
	CHECK( regionMap[0][4] == -1 );
}

TEST(findMaximalUnmarkedPoint, PrecomputeRegionSearch)
{
	std::vector< std::vector<float> > tc(1);
	tc[0] = {0.6, 0.8, 0.7, 0.8, 0.2};

//	std::vector< std::vector<QPoint> > tcindices(1);
//	tcindices[0] = { QPoint(4,0), QPoint(4,0), QPoint(4,0), QPoint(2,0), QPoint(2,0) };

	std::vector< std::vector<int> > regionMap(1);
	regionMap[0] = { -1, 0, -1, -1, -1 };

	TestingRegionSearch trs;
	QPoint pt = trs.findMaximalUnmarkedPoint(regionMap, tc);

	CHECK_EQUAL(QPoint(3,0), pt);
}
 

TEST(seed, PrecomputeRegionSearch) {
	std::vector< std::vector<float> > tc(1);
	tc[0] = {0.6, 0.8, 0.7, 0.2};

	std::vector< std::vector<int> > regionMap(1);
	regionMap[0] = { -1, -1, -1, -1 };

	int nextRegion = 1;
	std::queue<QPoint> q;

	TestingRegionSearch trs;
	trs.seed(tc, regionMap, nextRegion, q);

	//q contains seed point
	LONGS_EQUAL(1L, (long int)q.size());
	QPoint pt = q.front();
	CHECK_EQUAL(QPoint(1,0), pt);

	//seed point is marked with the next region number in regionMap
	LONGS_EQUAL(1L, (long int)regionMap[0][1]);
	LONGS_EQUAL(2L, nextRegion);
}

TEST(findRegions, PrecomputeRegionSearch) {
	std::vector< std::vector<float> > tc(1);
	tc[0] = {0.7, 0.85, 0.2, 0.85, 0.15, 0.8};

	std::vector< std::vector<QPoint> > tcindices(1);
	tcindices[0] = { QPoint(3,0), QPoint(3,0), QPoint(1,0), QPoint(1,0), QPoint(1,0), QPoint(3,0) };

	std::vector< std::vector<int> > regionMap;
	VCGL::RegionConnectivity connectivity;

	VCGL::RegionSearch rs;
	unsigned nRegions = rs.findRegions(tc, tcindices, 0.4, regionMap, connectivity);

	//regionMap
	LONGS_EQUAL(1L, (long int)regionMap.size());
	LONGS_EQUAL(6L, (long int)regionMap[0].size());

	LONGS_EQUAL(1L, regionMap[0][0]);
	LONGS_EQUAL(1L, regionMap[0][1]);
	LONGS_EQUAL(0L, regionMap[0][2]);
	LONGS_EQUAL(2L, regionMap[0][3]);
	LONGS_EQUAL(0L, regionMap[0][4]);
	LONGS_EQUAL(3L, regionMap[0][5]);

	LONGS_EQUAL(4L, (long int)nRegions);

	//connectivity
	VCGL::Link regionLink;
	bool bLinked = connectivity.getLink(1, 2, regionLink);

	CHECK(bLinked);
	CHECK_EQUAL(QPoint(1,0), regionLink.ptA);
	CHECK_EQUAL(QPoint(3,0), regionLink.ptB);
	DOUBLES_EQUAL(0.85, regionLink.w, 0.0001);


	bLinked = connectivity.getLink(3, 2, regionLink);
	CHECK(bLinked);
	CHECK_EQUAL(QPoint(5,0), regionLink.ptA);
	CHECK_EQUAL(QPoint(3,0), regionLink.ptB);
	DOUBLES_EQUAL(0.8, regionLink.w, 0.0001);
}

} //namespace Testing

