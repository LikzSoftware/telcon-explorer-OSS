/*! @file precomputeddatatest.cpp
 * @author anantonov
 * @date Created on Mar 25, 2017
 *
 * @brief Tests for the precomputed data write and read
 */

#include "CppUnitLite/TestHarness.h"
#include "cppunitextras.h"
#include "typedefs.h"

#include "projection/projectedpointinfo.h"
#include "storage/precomputeddata.h"

namespace Testing {

// TODO: setup / teardown to create a temporary folder and cleanup

TEST(AutocorrelationsWriteReadText, PrecomputedData)
{
	const std::vector<float> autocorrelations = { 0.3, 0.2, 0.7 };
	const std::string autocorrFileName = "test-autocorr.txt";
	storeAutocorrelations(autocorrelations, autocorrFileName, false);

	std::vector<float> autocorrelationsIn;
	readAutocorrelations(autocorrFileName, autocorrelationsIn, false);
	CHECK_EQUAL(autocorrelations, autocorrelationsIn);
}

TEST(AutocorrelationsWriteReadBinary, PrecomputedData)
{
	const std::vector<float> autocorrelations = { 0.3, 0.2, 0.7 };
	const std::string autocorrFileName = "test-autocorr.bin";
	storeAutocorrelations(autocorrelations, autocorrFileName, true);

	std::vector<float> autocorrelationsIn;
	readAutocorrelations(autocorrFileName, autocorrelationsIn, true);
	CHECK_EQUAL(autocorrelations, autocorrelationsIn);
}

TEST(CorrelationsTriangleWriteReadText, PrecomputedData)
{
	const std::vector< std::vector<float> > correlations =
		{ {1.0, 0.3, 0.7},
		  {0.3, 1.0, 0.6},
		  {0.7, 0.6, 1.0} };
	const std::string corrFileName = "test-corr.txt";
	storeCorrelationsTriangle(correlations, corrFileName, false);

	std::vector< std::vector<float> > correlationsIn;
	readCorrelationTriangle(corrFileName, correlationsIn, false);
	CHECK_EQUAL(correlations, correlationsIn);
}

TEST(CorrelationsTriangleWriteReadBinary, PrecomputedData)
{
	const std::vector< std::vector<float> > correlations =
		{ {1.0, 0.3, 0.7},
		  {0.3, 1.0, 0.6},
		  {0.7, 0.6, 1.0} };
	const std::string corrFileName = "test-corr.bin";
	storeCorrelationsTriangle(correlations, corrFileName, true);

	std::vector< std::vector<float> > correlationsIn;
	readCorrelationTriangle(corrFileName, correlationsIn, true);
	CHECK_EQUAL(correlations, correlationsIn);
}

TEST(ProjectionResultsWriteReadText, PrecomputedData)
{
	const int nlon = 2;
	const int nlat = 1;
	std::vector<VCGL::ProjectedPointInfo> projection(nlon*nlat);
	projection[0].pt = LSP::TSPoint(13.0, 3.0);
	projection[0].name = "(0,0)";
	projection[1].pt = LSP::TSPoint(5.0, 2.0);
	projection[1].name = "(1,0)";

	const std::string projFileName = "test-proj.txt";
	storeProjectionResults(projFileName, projection, false);

	std::vector<VCGL::ProjectedPointInfo> projectionIn;
	loadProjectionLonLat(projFileName, nlon, nlat, projectionIn, false);
	CHECK_EQUAL(projection, projectionIn);
}

TEST(ProjectionResultsWriteReadBinary, PrecomputedData)
{
	const int nlon = 2;
	const int nlat = 1;
	std::vector<VCGL::ProjectedPointInfo> projection(nlon*nlat);
	projection[0].pt = LSP::TSPoint(13.0, 3.0);
	projection[0].name = "(0,0)";
	projection[1].pt = LSP::TSPoint(5.0, 2.0);
	projection[1].name = "(1,0)";

	const std::string projFileName = "test-proj.bin";
	storeProjectionResults(projFileName, projection, true);

	std::vector<VCGL::ProjectedPointInfo> projectionIn;
	loadProjectionLonLat(projFileName, nlon, nlat, projectionIn, true);
	CHECK_EQUAL(projection, projectionIn);
}

} // namespace Testing
