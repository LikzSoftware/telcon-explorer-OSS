/*!	@file startup.cpp
 *	@author anantonov
 *	@date	Feb 13, 2014 (created)
 *	@brief	TODO <file description>
 */

#include "startup.h"

#include "init/read.h"
#include "ncfiledatastorage.h"
#include "tcstorage.h"

#include "init/pathresolver.h"
#include "init/filesystem.h"

#include "process/precompute.h"

#include <sstream>

#include <vector>
#include <iostream>

#include <string>

#include <time.h>

#include "exploration/maps/maplayoutview.h"
#include "exploration/explorationwidget.h"
#include "exploration/explorationmodel.h"
#include "exploration/explorationmodelimpl.h"
#include "exploration/fakeexplorationmodel.h"

#include "colorizer/icolorizer.h"

#include "process/link.h"
#include "process/regionconnectivity.h"

#include <QtGui>
#include <QApplication>

#include <cassert>

#include "preferences/preferences.h"
#include "preferences/preferencepane.h"
#include "exploration/regions/regionsearchexplorer.h"

void generateFilenames_var_level(
		const std::string& fileName,
		const std::string& varName,
		const std::string& level,
		bool northOnly,
		std::string& fnCorrelation,
		std::string& fnAutocorr,
		std::string& fnProjection) {
	std::string fnRoot = VCGL::stringExtractFilenameNoExt(fileName);

	std::stringstream basestr;
	basestr << fnRoot.c_str() << '_' << varName.c_str();
	if (level.size() > 0) {
		basestr << '_' << level.c_str();
	}
	if (northOnly) {
		basestr << "_nh";
	}

	fnCorrelation = basestr.str() + "_correlation.txt";
	fnAutocorr = basestr.str() + "_autocorr.txt";
	fnProjection = basestr.str() + "_projection.txt";
}

void precompute_var_level(const char * dataFN,
		const char* varName,
		const char* level = 0,
		bool northOnly = false) {
	std::string fileName(dataFN);
	std::string varNameStr(varName);
	std::string levelStr;
	int levelValue = -1;
	if (level != 0) {
		sscanf(level, "%d", &levelValue);
		levelStr = level;
	}

	std::string fnCorrelation;
	std::string fnAutocorr;
	std::string fnProjection;
	generateFilenames_var_level(fileName,
			varNameStr,
			levelStr,
			northOnly,
			fnCorrelation,
			fnAutocorr,
			fnProjection);

	std::cout << "Correlation file name: " << fnCorrelation.c_str() << std::endl;
	std::cout << "Projection file name: " << fnProjection.c_str() << std::endl;

	//load data
	std::cout << "Loading data from file: " << fileName.c_str() << "..." << std::endl;

	VCGL::NCFileDataStorage* pncf = new VCGL::NCFileDataStorage(fileName.c_str());
	pncf->initVariable(varNameStr.c_str(), levelValue);
	VCGL::TCStorage storage(pncf, northOnly); // takes ownership of pncf pointer

	std::vector< std::vector< std::vector<float> > > data; //= ncf.loadData(0, ncf.getNLat());
	storage.loadData(data);


	int nlat = 0, nlon = 0, ntime = 0;

	nlat = data.size();
	if (nlat) { nlon = data[0].size(); }
	if (nlat && nlon) { ntime = data[0][0].size(); }

	std::cout << "Read data of size: nlat = " << nlat << ", nlon = " << nlon << ", ntime = " << ntime << std::endl;

	//TODO: correct validity if necessary
		std::vector< std::vector<bool> > validityMask;
		const std::vector<bool> vmpar_init(nlon, true);
	   validityMask.resize(nlat, vmpar_init);

	//compute correlations
	std::cout << "Computing correlations..." << std::endl;
	clock_t start_corr = clock();
	std::vector< std::vector<float> > correlationMatrix;
	computeCorrelations(data, correlationMatrix, validityMask);
	clock_t end_corr = clock();
	float seconds_corr = ((float)(end_corr - start_corr)) / CLOCKS_PER_SEC;
	std::cout << "...completed in " << seconds_corr << " seconds" << std::endl;

	//store correlations
	std::cout << "Storing correlations to file: " << fnCorrelation.c_str() << "..." << std::endl;
	storeCorrelationsTriangle(correlationMatrix, fnCorrelation.c_str());
	std::cout << "...stored." << std::endl;


	//compute autocorrelations
	std::cout << "Computing autocorrelations..." << std::endl;
	clock_t start_acorr = clock();
	std::vector<float> autocorrelations;
	computeAutocorrelations(data, autocorrelations, validityMask);
	clock_t end_acorr = clock();
	float seconds_acorr = ((float)(end_acorr - start_acorr)) / CLOCKS_PER_SEC;
	std::cout << "...completed in " << seconds_acorr << " seconds" << std::endl;

	//store autocorrelations
	std::cout << "Storing autocorrelations to file: " << fnAutocorr << "..." << std::endl;
	storeAutocorrelations(autocorrelations, fnAutocorr);
	std::cout << "...stored." << std::endl;


	//compute projection
	std::cout << "Computing projection..." << std::endl;
	clock_t start_proj = clock();

	std::vector<VCGL::ProjectedPointInfo> projectionResults;
	projectCorrelationMatrix(correlationMatrix, nlon, nlat, projectionResults);

	clock_t end_proj = clock();
	float seconds_proj = ((float)(end_proj - start_proj)) / CLOCKS_PER_SEC;
	std::cout << "...completed in " << seconds_proj << " seconds" << std::endl;

	//store projection
	std::cout << "Storing projection to file: " << fnProjection.c_str() << "..." << std::endl;
	storeProjectionResults(fnProjection.c_str(), projectionResults);
	std::cout << "...stored." << std::endl;

}

namespace VCGL {

int Startup::runPrecompute(char* fileName, char* variableName, char* levelValue, bool northOnly) {
	precompute_var_level(fileName, variableName, levelValue, northOnly);
	return 0;
}

int Startup::runShow(char* fileName, char* variableName, char* levelValue, bool northOnly) {
	int retVal = 0;

	int argcFake = 0;
	char* argvFake[] = {};
	QApplication a(argcFake, argvFake);

	std::string strFN(fileName);
	std::string strVar(variableName);
	std::string strLVL;
	std::string fnCorrelation;
	std::string fnAutocorr;
	std::string fnProjection;

	int lvlValue = -1;
	if (levelValue != 0) {
		strLVL = levelValue;
		sscanf(levelValue, "%d", &lvlValue);
	}

	generateFilenames_var_level(strFN,
			strVar,
			strLVL,
			northOnly,
			fnCorrelation,
			fnAutocorr,
			fnProjection);

	FileSystem fs;
	PathResolver pr(fs);

	pr.find(std::string(strFN), strFN);
	pr.findDependency(std::string(fnCorrelation), strFN, fnCorrelation);
	pr.findDependency(std::string(fnAutocorr), strFN, fnAutocorr);
	pr.findDependency(std::string(fnProjection), strFN, fnProjection);

	std::cout << "Data file: " << strFN.c_str() << std::endl;
	std::cout << "Correlation file name: " << fnCorrelation.c_str() << std::endl;
	std::cout << "Autocorrelation file name: " << fnAutocorr << std::endl;
	std::cout << "Projection file name: " << fnProjection.c_str() << std::endl;

	std::string pathContours;
	pr.find("new_land_contours.dat", pathContours);
	std::cerr << "Using land contours file " << pathContours << std::endl;

	{ // development version

		std::cout << "Loading data..." << std::endl;
		clock_t start_load = clock();

		ExplorationModel* pem = new ExplorationModelImpl();

		VCGL::NCFileDataStorage* pncf = new VCGL::NCFileDataStorage(strFN.c_str());
		pncf->initVariable(strVar.c_str(), lvlValue);
		VCGL::TCStorage storage(pncf, northOnly); // takes ownership of pncf pointer

		pem->loadGrid(storage);
		//pem->loadGrid(strFN);
		pem->loadCorrelations(fnCorrelation);
		pem->loadAutocorrelations(fnAutocorr);
		pem->loadContours(pathContours);
		pem->loadProjection(fnProjection);

		pem->computeStatisticalSignificanceMask(0.99); // prepare statistical significance
		pem->setThreshold(0.0); // perform region search


		ExplorationWidget ew;
		ew.move(200,200);
		ew.setModel(pem);
		ew.show();

		clock_t end_load = clock();
		float seconds_load = ((float)(end_load - start_load)) / CLOCKS_PER_SEC;
		std::cout << "Loading completed in " << seconds_load << " seconds" << std::endl;

		retVal = a.exec();
	}

	return retVal;
}

int Startup::runRegionExplorer(char* fileName, char* variableName, char* levelValue) {
	const bool northOnly = false;
	int retVal = 0;

	int argcFake = 0;
	char* argvFake[] = {};
	QApplication a(argcFake, argvFake);

	std::string strFN(fileName);
	std::string strVar(variableName);
	std::string strLVL;
	std::string fnCorrelation;
	std::string fnAutocorr;
	std::string fnProjection;

	int lvlValue = -1;
	if (levelValue != 0) {
		strLVL = levelValue;
		sscanf(levelValue, "%d", &lvlValue);
	}

	generateFilenames_var_level(strFN,
			strVar,
			strLVL,
			northOnly,
			fnCorrelation,
			fnAutocorr,
			fnProjection);

	FileSystem fs;
	PathResolver pr(fs);

	pr.find(std::string(strFN), strFN);
	pr.findDependency(std::string(fnCorrelation), strFN, fnCorrelation);
	pr.findDependency(std::string(fnAutocorr), strFN, fnAutocorr);

	std::cout << "Data file: " << strFN.c_str() << std::endl;
	std::cout << "Correlation file name: " << fnCorrelation.c_str() << std::endl;
	std::cout << "Autocorrelation file name: " << fnAutocorr << std::endl;

	std::string pathContours;
	pr.find("new_land_contours.dat", pathContours);
	std::cerr << "Using land contours file " << pathContours << std::endl;

	{ // development version
		std::shared_ptr<ExplorationModel> pem = std::make_shared<ExplorationModelImpl>();

		VCGL::NCFileDataStorage* pncf = new VCGL::NCFileDataStorage(strFN.c_str());
		pncf->initVariable(strVar.c_str(), lvlValue);
		VCGL::TCStorage storage(pncf, northOnly); // takes ownership of pncf pointer

		pem->loadGrid(storage);
		//pem->loadGrid(strFN);
		pem->loadCorrelations(fnCorrelation);
		pem->loadAutocorrelations(fnAutocorr);
		pem->loadContours(pathContours);
		//pem->loadProjection(fnProjection);

		pem->computeStatisticalSignificanceMask(0.99); // prepare statistical significance
		pem->setThreshold(0.0); // perform region search

		RegionSearchExplorer rse;
		rse.useModel(pem.get());
		rse.init();
		rse.show();

		retVal = a.exec();
		pem.reset();
	}

	return retVal;
}

int Startup::runUITest( int argCount, char** argValues ) {

	std::cout << "UI test arguments: " << std::endl;
	for (int i=0; i<argCount; i++) {
		std::cout << i << ": " << argValues[i] << std::endl;
	}

	int retVal = 0;

	if (!argCount) {
		int argcFake = 0;
		char* argvFake[] = {};
		QApplication a(argcFake, argvFake);

		std::vector< std::vector<QPointF> > contours;
		FileSystem fs;
		PathResolver pr(fs);
	//	std::string pathContours;
	//	pr.find("land_contours.dat", pathContours);
	//	std::cerr << "Using land contours file " << pathContours << std::endl;
	//	readContours(pathContours.c_str(), contours);
		ExplorationModel* pem = new FakeExplorationModel();
		pem->loadGrid("");
		pem->loadCorrelations("");
		pem->loadAutocorrelations("");
		pem->loadContours("");
		pem->loadProjection("");

		ExplorationWidget ew;
		ew.move(200,200);
		//ML.resize(400,200);
		ew.setModel(pem);
		ew.show();

		retVal = a.exec();
	}

	if (argCount == 1 && argValues[0][0] == 'P') {
		int argcFake = 0;
		char* argvFake[] = {};
		QApplication a(argcFake, argvFake);

		Preferences prefs;
		Preferences::GetDefaults(&prefs);

		PreferencePane pane;
		pane.init(&prefs);
		pane.show();
		retVal = a.exec();
	}

	if (argCount == 1 && argValues[0][0] == 'R') {
		int argcFake = 0;
		char* argvFake[] = {};
		QApplication a(argcFake, argvFake);

//		std::vector< std::vector<QPointF> > contours;
//		FileSystem fs;
//		PathResolver pr(fs);
		std::shared_ptr<ExplorationModel> pem = std::make_shared<FakeExplorationModel>();
		pem->loadGrid("");
		pem->loadCorrelations("");
		pem->loadAutocorrelations("");
		pem->loadContours("");
		pem->loadProjection("");

		RegionSearchExplorer rse;
		rse.useModel(pem.get());
		rse.init();
		rse.show();


		retVal = a.exec();
		pem.reset();
	}

	return retVal;
}

} // namespace VCGL
