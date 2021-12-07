/*! @file regionconnectivitytest.cpp
 * @author anantonov
 * @date Created on May 18, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */


#include "CppUnitLite/TestHarness.h"
#include "cppunitextras.h"

#include "regionconnectivity.h"
#include "link.h"
//#include "regionsearch.h"
//#include <vector>
//#include <queue>
//#include <set>
//#include <list>
//#include <QPoint>
//
//#include <string.h>

namespace Testing {

class TestingRegionConnectivity: public VCGL::RegionConnectivity {
public:
	unsigned getNumLinks() {
		return links.size();
	}
};

//test get nonexistent link
TEST(getLinkNonexistent, RegionConnectivity)
{
	VCGL::RegionConnectivity rc;
	VCGL::Link ln;
	bool bResult = rc.getLink(0,1, ln);

	CHECK(bResult == false);
}


TEST(suggestLinkAddsNewLink, RegionConnectivity)
{
	TestingRegionConnectivity trc;

	LONGS_EQUAL(0L, (long int)trc.getNumLinks());
	trc.suggestLink(0,1,VCGL::Link(QPoint(1,2), QPoint(8,3), 0.75));

	LONGS_EQUAL(1L, (long int)trc.getNumLinks());
}

TEST(getLinkSuggestedBefore, RegionConnectivity)
{
	VCGL::RegionConnectivity rc;
	rc.suggestLink(0,1,VCGL::Link(QPoint(1,2), QPoint(8,3), 0.75));

	VCGL::Link ln;
	bool bResult = rc.getLink(0,1, ln);

	CHECK(bResult);
	CHECK_EQUAL(QPoint(1,2), ln.ptA);
	CHECK_EQUAL(QPoint(8,3), ln.ptB);
	DOUBLES_EQUAL(0.75, ln.w, 0.0001);
}

TEST(suggestLinkWorseLink, RegionConnectivity)
{
	VCGL::RegionConnectivity rc;
	rc.suggestLink(0,1,VCGL::Link(QPoint(1,2), QPoint(8,3), 0.75));

	rc.suggestLink(0,1,VCGL::Link(QPoint(0,1), QPoint(10,2), 0.45));

	VCGL::Link ln;
	bool bResult = rc.getLink(0,1, ln);

	CHECK(bResult);
	CHECK_EQUAL(QPoint(1,2), ln.ptA);
	CHECK_EQUAL(QPoint(8,3), ln.ptB);
	DOUBLES_EQUAL(0.75, ln.w, 0.0001);
}

TEST(suggestLinkBetterLink, RegionConnectivity)
{
	VCGL::RegionConnectivity rc;
	rc.suggestLink(0,1,VCGL::Link(QPoint(1,2), QPoint(8,3), 0.75));

	rc.suggestLink(0,1,VCGL::Link(QPoint(3,4), QPoint(10,1), 0.8));

	VCGL::Link ln;
	bool bResult = rc.getLink(0,1, ln);

	CHECK(bResult);
	CHECK_EQUAL(QPoint(3,4), ln.ptA);
	CHECK_EQUAL(QPoint(10,1), ln.ptB);
	DOUBLES_EQUAL(0.8, ln.w, 0.0001);
}

} // namespace Testing
