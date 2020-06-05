/*! @file nhtests.cpp
 * @author anantonov
 * @date Created on Nov 23, 2014
 *
 * @brief Tests for the north-hemisphere mode
 */

#include "CppUnitLite/TestHarness.h"
#include "cppunitextras.h"

#include "storage/tcstorage.h"
#include "storage/datastorage.h"
#include "typedefs.h"

namespace Testing {

//TEST(Acceptance, TCStorage) {
//	//TestFileStorage fs;
//	TCStorage storage(fs);
//	storage.useNorthHemisphereOnly(true);
//	storage.loadRawData();
//
//	check lats (0<=lat<=90)
//	check data size equals ntime*(nlev)*nlat*nlon
//
//
//}

class TestingDataStorage: public VCGL::DataStorage {
public:
	virtual std::size_t getNTime() override {
		//TODO: implement
		return 0;
	}
	virtual std::size_t getNLon() override {
		return testLons.size();
	}
	virtual std::size_t getNLat() override {
		return testLats.size();
	}
	virtual std::vector<float> loadLons() override {
		return testLons;
	}

	virtual std::vector<float> loadLats() override {
		return testLats;
	}

	virtual VCGL::vectorFloat3D loadData(std::size_t latStart, std::size_t latCount) override {
		if (latStart == 1 && latCount == 2 && testData.size() == 3) {
			return (VCGL::vectorFloat3D{ testData[1], testData[2] });
		}
		return testData;
	}

	virtual ~TestingDataStorage() {};

	std::vector<float> testLons;
	std::vector<float> testLats;
	VCGL::vectorFloat3D testData;
};


TEST(LoadFullGrid, TCStorage)
{
	TestingDataStorage* td = new TestingDataStorage();
	td->testLons = { 0.0, 90.0, 180.0, 270 };
	td->testLats = { -90.0, 0.0, 90.0 };

	VCGL::TCStorage storage(td);

	std::vector<float> lons;
	std::vector<float> lats;
	storage.loadGrid(lons, lats);

	CHECK_EQUAL( (std::vector<float>{ 0.0, 90.0, 180.0, 270 }), lons);
	CHECK_EQUAL( (std::vector<float>{ -90.0, 0.0, 90.0 }), lats);
}

TEST(LoadFullGridData, TCStorage) {
	TestingDataStorage* td = new TestingDataStorage();
		td->testLons = { 0.0, 90.0, 180.0, 270 };
		td->testLats = { -90.0, 0.0, 90.0 };
		td->testData = {
				{ {1.0}, {2.0}, {3.0}, {4.0} },
				{ {5.0}, {6.0}, {7.0}, {8.0} },
				{ {9.0}, {10.0}, {11.0}, {12.0} }
		};

		VCGL::TCStorage storage(td);

		VCGL::vectorFloat3D data;
		storage.loadData(data);
		CHECK_EQUAL( (VCGL::vectorFloat3D{
							{ {1.0}, {2.0}, {3.0}, {4.0} },
							{ {5.0}, {6.0}, {7.0}, {8.0} },
							{ {9.0}, {10.0}, {11.0}, {12.0} } }), data );


}

TEST(LoadNorthernHemisphereGridBasic, TCStorage)
{
	TestingDataStorage* td = new TestingDataStorage();
	td->testLons = { 0.0, 90.0, 180.0, 270 };
	td->testLats = { -90.0, 0.0, 90.0 };

	VCGL::TCStorage storage(td, true);

	std::vector<float> lons;
	std::vector<float> lats;
	storage.loadGrid(lons, lats);

	CHECK_EQUAL( (std::vector<float>{ 0.0, 90.0, 180.0, 270 }), lons);
	CHECK_EQUAL( (std::vector<float>{ 0.0, 90.0 }), lats);
}

TEST(LoadNorthernHemisphereGridDataBasic, TCStorage) {
	TestingDataStorage* td = new TestingDataStorage();
		td->testLons = { 0.0, 90.0, 180.0, 270 };
		td->testLats = { -90.0, 0.0, 90.0 };
		td->testData = {
				{ {1.0}, {2.0}, {3.0}, {4.0} },
				{ {5.0}, {6.0}, {7.0}, {8.0} },
				{ {9.0}, {10.0}, {11.0}, {12.0} }
		};

		VCGL::TCStorage storage(td, true);

		VCGL::vectorFloat3D data;
		storage.loadData(data);
		CHECK_EQUAL( (VCGL::vectorFloat3D{
					{ {5.0}, {6.0}, {7.0}, {8.0} },
					{ {9.0}, {10.0}, {11.0}, {12.0} } }), data );

}

}
