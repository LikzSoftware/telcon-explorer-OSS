/*! @file explorationmodel.cpp
 * @author anantonov
 * @date Created on Jun 4, 2014
 *
 * @brief Implementation stubs for the MVC-Model class for the exploration of teleconnections
 */

#include "explorationmodel.h"

namespace VCGL {

ExplorationModel::ExplorationModel()
: threshold(0.0) {
	// TODO Auto-generated constructor stub

}

ExplorationModel::~ExplorationModel() {
	// TODO Auto-generated destructor stub
}

bool ExplorationModel::getClosestPointCorrelationValue(const QPointF& /*point*/, float* /*pValue*/) const {
	return false;
}

bool ExplorationModel::getClosestPointTeleconnectivityValue(const QPointF& /*point*/, float* /*pValue*/) const {
	return false;
}

void ExplorationModel::getSelectionMask(std::vector< std::vector<bool> >& selectionMask) const {
	selectionMask.clear();
}

void ExplorationModel::getCorrelationMapColors(std::vector< std::vector<float> >& colorData) const {
	colorData.clear();
	if (nlon() && nlat()) {
		colorData.resize(nlat(), std::vector<float>(nlon(), 0.0f));
	}
}

void ExplorationModel::computeStatisticalSignificanceMask(float /*ssLevel*/) {

}

void ExplorationModel::getStatisticalSignificanceMask(std::vector< std::vector<bool> >& ssMask) const {
	ssMask.clear();
}

void ExplorationModel::getTeleconnectivityMapColors(std::vector< std::vector<float> >& colorData) const {
	colorData.clear();
	if (nlon() && nlat()) {
		colorData.resize(nlat(), std::vector<float>(nlon(), 0.0f));
	}
}

void ExplorationModel::getCorrelationMapChainLinks(std::vector<VCGL::AnnotationLinkF>& links) const {
	links.clear();
}

void ExplorationModel::getCorrelationChainProjection(std::vector< QPointF >& chainProjection) const {
	chainProjection.clear();
}

void ExplorationModel::getTeleconnectivityMapLinks(std::vector<VCGL::AnnotationLinkF>& links) const {
	links.clear();
}

void ExplorationModel::getTeleconnectivityLinks(std::vector<VCGL::LinkF>& links) const {
	links.clear();
}

void ExplorationModel::getProjectionData(std::vector< std::vector<QPointF> >& projectionData) const {
	projectionData.clear();
}

void ExplorationModel::selectReferencePoint( const QPointF& /*pointCoordinates*/ ){

}

QPointF ExplorationModel::getReferencePoint() const{
	return QPointF(0.0f, 0.0f);
}

QPointF ExplorationModel::getReferencePointProjection() const {
	return QPointF(0.0f, 0.0f);
}

void ExplorationModel::selectRegionAtPoint(const QPointF& /*point*/, bool /*bSelectWholeComponent*/) {}

void ExplorationModel::resetRegionSelection() {}

void ExplorationModel::setSelectionMask(const std::vector< std::vector<bool> >& /*selectionMask*/, unsigned /*numSelected*/) {

}

bool ExplorationModel::pointsEqual(const QPointF& /*a*/, const QPointF& /*b*/) const {
	return false;
}

} /* namespace VCGL */
