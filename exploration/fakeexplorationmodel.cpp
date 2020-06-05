/*! @file fakeexplorationmodel.cpp
 * @author anantonov
 * @date Created on Jun 21, 2014
 *
 * @brief Fake implementation of Exploration Model for testing purposes
 */

#include "fakeexplorationmodel.h"

#include <cmath>

namespace VCGL {

const unsigned FakeExplorationModel::nx = 30;
const unsigned FakeExplorationModel::ny = 20;

FakeExplorationModel::FakeExplorationModel() {
	// TODO Auto-generated constructor stub

}

FakeExplorationModel::~FakeExplorationModel() {
	// TODO Auto-generated destructor stub
}

void FakeExplorationModel::loadGrid(TCStorage&) {
	loadGrid("");
}

void FakeExplorationModel::loadGrid(const std::string& /*fileName*/){
	const float lonMin = 0;
	const float lonMax = 275;
	const float latMin = -65;
	const float latMax = 65;

	grid.lons.clear();
	grid.lons.resize(nx);
	for (unsigned j=0; j<nx; j++) {
		grid.lons[j] = lonMin + j*( (lonMax-lonMin) / nx);
	}

	grid.lats.clear();
	grid.lats.resize(ny);
	for (unsigned i=0; i<ny; i++) {
		grid.lats[i] = latMax - i*( (latMax-latMin) /(ny-1));
	}
}

void FakeExplorationModel::loadContours(const std::string& /*contoursFileName*/){ }

void FakeExplorationModel::loadCorrelations(const std::string& /*correlationsFileName*/){ }

void FakeExplorationModel::loadAutocorrelations(const std::string& /*autocorrFileName*/){ }

void FakeExplorationModel::loadProjection(const std::string& /*projectionFileName*/) { }

void FakeExplorationModel::getCorrelationMapColors(std::vector< std::vector<float> >& colorData) const {
	getColors(colorData);
}
void FakeExplorationModel::getTeleconnectivityMapColors(std::vector< std::vector<float> >& colorData) const {
	getColors(colorData);
}

void FakeExplorationModel::getProjectionData(std::vector< std::vector<QPointF> >& projectionData) const {
	projectionData.clear();
	projectionData.resize(ny, std::vector<QPointF>(nx, QPointF(0,0)));

	for (unsigned i=0; i < projectionData.size(); i++) {
		for (unsigned j=0; j< projectionData[i].size(); j++) {
			const float rmax = 100.0;
			float dx = 1.0 / nx;
			float dy = 1.0 / ny;
			float r = rmax * j * dx;
			float phi = i * 2*M_PI*dy - cos(M_PI/2*(2*j*dx - 1)) * 2*M_PI*dy;

			float x = r * cos(phi);
			float y = r * sin(phi);

			projectionData[i][j] = QPointF(x,y);
		}
	}
}

void FakeExplorationModel::getColors(std::vector< std::vector<float> >& colorData) const {
	colorData.clear();
	colorData.resize(ny, std::vector<float>(nx, 0));
	for (unsigned i=0; i < colorData.size(); i++) {
		for (unsigned j=0; j< colorData[i].size(); j++) {
			colorData[i][j] = (i+j)/((float)nx+ny-2);
		}
	}
}

} /* namespace VCGL */
