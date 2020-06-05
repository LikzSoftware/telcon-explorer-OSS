/*! @file mapsubviewtest.cpp
 * @author anantonov
 * @date Created on Nov 19, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "CppUnitLite/TestHarness.h"
#include "cppunitextras.h"

//#include <QRect>
#include "exploration/maps/mrect.h"
#include "exploration/maps/mapgrid.h"
#include "exploration/maps/polarmapsubview.h"

namespace Testing {

class TestingPolarMap: public VCGL::PolarMapSubview {
public:
	virtual VCGL::MRect getMapArea() const override {
		return {0, 50, 50, 0}; // rectangle with pixels 0..50 x 0..50, i.e. with size 51x51 px
	}
	virtual void transformCoordinates(
			const VCGL::MapGrid& grid,
			const float ptLonF,
			const float ptLatF,
			float& pxX,
			float& pxY) const override {

		PolarMapSubview::transformCoordinates(grid, ptLonF, ptLatF, pxX, pxY);

	}
	virtual ~TestingPolarMap() {}
};


TEST(TransformsEquatorFullGrid, PolarMap)
{
	TestingPolarMap tpm;

	VCGL::MapGrid grid;
	grid.lons = { 0.0, 90.0, 180.0, 270 };
	grid.lats = { -90.0, 0.0, 90.0 };

	float x1 = 0;
	float y1 = 0;
	tpm.transformCoordinates(grid, 0, 0, x1, y1);
	DOUBLES_EQUAL(25.0, x1, 0.001);
	DOUBLES_EQUAL(12.5, y1, 0.001);

	float x2 = 0;
	float y2 = 0;
	tpm.transformCoordinates(grid, 90, 0, x2, y2);
	DOUBLES_EQUAL(37.5, x2, 0.001);
	DOUBLES_EQUAL(25.0, y2, 0.001);

	float x3 = 0;
	float y3 = 0;
	tpm.transformCoordinates(grid, 180, 0, x3, y3);
	DOUBLES_EQUAL(25.0, x3, 0.001);
	DOUBLES_EQUAL(37.5, y3, 0.001);

	float x4 = 0;
	float y4 = 0;
	tpm.transformCoordinates(grid, 270, 0, x4, y4);
	DOUBLES_EQUAL(12.5, x4, 0.001);
	DOUBLES_EQUAL(25.0, y4, 0.001);
}

TEST(TransformsEquatorNorthHemisphereGrid, PolarMap)
{
	TestingPolarMap tpm;

	VCGL::MapGrid grid;
	grid.lons = { 0.0, 90.0, 180.0, 270.0 };
	grid.lats = { 0.0, 45.0, 90.0 };

	float x1 = 0;
	float y1 = 0;
	tpm.transformCoordinates(grid, 0, 0, x1, y1);
	DOUBLES_EQUAL(25.0, x1, 0.001);
	DOUBLES_EQUAL(0.0, y1, 0.001);

	float x2 = 0;
	float y2 = 0;
	tpm.transformCoordinates(grid, 90, 0, x2, y2);
	DOUBLES_EQUAL(50.0, x2, 0.001);
	DOUBLES_EQUAL(25.0, y2, 0.001);

	float x3 = 0;
	float y3 = 0;
	tpm.transformCoordinates(grid, 180, 0, x3, y3);
	DOUBLES_EQUAL(25.0, x3, 0.001);
	DOUBLES_EQUAL(50.0, y3, 0.001);

	float x4 = 0;
	float y4 = 0;
	tpm.transformCoordinates(grid, 270, 0, x4, y4);
	DOUBLES_EQUAL(0.0, x4, 0.001);
	DOUBLES_EQUAL(25.0, y4, 0.001);
}

}
