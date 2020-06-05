/*! @file maplayouttest.cpp
 * @author anantonov
 * @date Created on Jun 3, 2014
 *
 * @brief Tests for the layout logic of MapLayout
 */

#include "CppUnitLite/TestHarness.h"

#include "cppunitextras.h"
#include <QRect>
#include "exploration/maps/mrect.h"
#include "exploration/maps/layout.h"

namespace Testing {


TEST(QRectFromQPoint, MapLayout)
{
	QRect rect { QPoint{1,5}, QPoint {7, 2} };
	LONGS_EQUAL(1, rect.left());
	LONGS_EQUAL(5, rect.top());
	LONGS_EQUAL(7, rect.right());
	LONGS_EQUAL(2, rect.bottom());

	LONGS_EQUAL(7, rect.width());

	//warning
	CHECK(4 != rect.height());
	LONGS_EQUAL(-2, rect.height());
}

TEST(QRectFromInts, MapLayout)
{
	QRect rect { 1, 5, 7, 4 };
	LONGS_EQUAL(1, rect.left());
	LONGS_EQUAL(5, rect.top());
	LONGS_EQUAL(7, rect.right());

	//warning!
	CHECK(2 != rect.bottom())
	LONGS_EQUAL(8, rect.bottom());
}

TEST(MRectFromInts, MapLayout) {
	VCGL::MRect rect { 1, 5, 7, 2 };

	LONGS_EQUAL(1, rect.left);
	LONGS_EQUAL(5, rect.top);
	LONGS_EQUAL(7, rect.right);
	LONGS_EQUAL(2, rect.bottom)

	LONGS_EQUAL(7, rect.width());
	LONGS_EQUAL(4, rect.height());
}

TEST(BasicLayout, Layout) {
	VCGL::Layout layout;
	VCGL::MapLayoutData data;
	layout.computeLayout(400,200, &data);

	CHECK_EQUAL( VCGL::MRect(0,199,299,0), data.mapArea );
	CHECK_EQUAL( VCGL::MRect(300,199,399,0), data.legendArea );
}

TEST(MarginsLayout, Layout) {
	VCGL::Layout layout;
	layout.setMargins(10, 5);

	VCGL::MapLayoutData data;
	layout.computeLayout(400,200, &data);

	CHECK_EQUAL( VCGL::MRect(10,194,269,5), data.mapArea );

	CHECK_EQUAL( VCGL::MRect(290,194,389,5), data.legendArea );
}

}
