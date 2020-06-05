/*! @file explorationmodelimpl.cpp
 * @author anantonov
 * @date Created on Jun 22, 2014
 *
 * @brief Implementation file for full implementation of ExplorationModel interface
 */

#include "explorationmodelimpl.h"

#include "init/ncfiledatastorage.h"
#include "init/tcstorage.h"
#include "init/read.h"
#include "process/precompute.h"

#include <string>
#include <iostream>
#include <set>
#include <algorithm>

#include "projection/projectedpointinfo.h"
#include "projection/tspoint.h"

#include "process/precompute.h"
#include "process/regionsearch.h"

#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include <math.h>


#include <cassert>

namespace VCGL {

ExplorationModelImpl::ExplorationModelImpl():
		refPtIndices(0,0),
		nRegions(0),
		numSelectedPoints(0) {

	}

ExplorationModelImpl::~ExplorationModelImpl() {
	}

void ExplorationModelImpl::loadGrid(TCStorage& storage) {
	storage.loadGrid(grid.lons, grid.lats);

	_ntime = storage.getNTime();

	if (grid.lats.size()==0 || grid.lons.size()==0) {
		std::cerr << "Problem loading grid"  << std::endl;
	}
}

void ExplorationModelImpl::loadGrid(const std::string& fileName) {
	VCGL::NCFileDataStorage ncf(fileName.c_str());
	grid.lons = ncf.loadLons();
	grid.lats = ncf.loadLats();

	_ntime = ncf.getNTime();

	if (grid.lats.size()==0 || grid.lons.size()==0) {
		std::cerr << "Problem reading file: " << fileName.c_str() << std::endl;
	}
}

void ExplorationModelImpl::loadCorrelations(const std::string& correlationsFileName) {
	assert(nlat() > 0 && nlon() > 0);
	readCorrelationTriangle(correlationsFileName.c_str(), correlations);

	unsigned npoints = nlat()*nlon();
	assert(correlations.size() == npoints);
	assert(correlations[0].size() == npoints);

	computeTeleconnectivity();

	QPoint highestTCindices{0,0};
	float maxTC = 0.0;
	for (unsigned i=0; i<nlat(); i++) {
		for(unsigned j=0; j<nlon(); j++) {
			if (tc[i][j] > maxTC) {
				maxTC = tc[i][j];
				highestTCindices = QPoint{(int)j,(int)i};
			}
		}
	}
	selectReferencePoint( indicesToCoordinates(highestTCindices) );
	//refPtIndices = highestTCindices;
}

void ExplorationModelImpl::loadAutocorrelations(const std::string& autocorrFileName) {
	assert(nlat() > 0 && nlon() > 0);
	readAutocorrelations(autocorrFileName, autocorrelations);

	unsigned npoints = nlat()*nlon();
	assert(autocorrelations.size() == npoints);
}

void ExplorationModelImpl::loadContours(const std::string& contoursFileName) {
	readContours(contoursFileName.c_str(), contours);
	clipContours(grid, contours);
}

void ExplorationModelImpl::loadProjection(const std::string& projectionFileName) {
	assert(nlon()>0 && nlat()>0);
	if (nlon()>0 && nlat()>0) {
		std::vector<VCGL::ProjectedPointInfo> projection;
		loadProjectionLonLat(projectionFileName.c_str(), nlon(), nlat(), projection);

		projectionData.clear();
		projectionData.resize(nlat(), std::vector<QPointF>(nlon(), QPointF(0,0)));
		for (unsigned i=0; i<nlat(); i++) {
			for (unsigned j=0; j<nlon(); j++) {
				int index = i*nlon() + j;
				LSP::TSPoint & pt = projection[index].pt;
				projectionData[i][j] = QPointF{ pt.getX(), pt.getY() };
			}
		}
	}
}

void ExplorationModelImpl::setThreshold(float newValue) {
	ExplorationModel::setThreshold(newValue);
	VCGL::RegionSearch rs;
	nRegions = rs.findRegions(tc, tcindices, threshold, regionMap, rc, (RSHelper*)this);
}

bool ExplorationModelImpl::getClosestPointCorrelationValue(const QPointF& point, float* pValue) const {
	bool bFound = false;
	assert(nlon()>0 && nlat()>0);
	if (nlon()>0 && nlat()>0) {
		QPoint indices = findClosestPointIndices(point);

		*pValue = getCorrelationValue(refPtIndices, indices);
		bFound = true;
	}
	return bFound;
}

bool ExplorationModelImpl::getClosestPointTeleconnectivityValue(const QPointF& point, float* pValue) const {
	bool bFound = false;
	assert(nlon()>0 && nlat()>0);
	assert(nlat() == tc.size() && nlon() == tc[0].size());
	if (nlon()>0 && nlat()>0) {
		QPoint indices = findClosestPointIndices(point);

		*pValue = tc[indices.y()][indices.x()];
		bFound = true;
	}
	return bFound;
}

void ExplorationModelImpl::getSelectionMask(std::vector< std::vector<bool> >& selectionMask) const {
	selectionMask.clear();
	if (numSelectedPoints > 0) {
		assert(nlat()==this->selectionMask.size() && nlon()==this->selectionMask[0].size());

		selectionMask = this->selectionMask;
	}
}

void ExplorationModelImpl::getCorrelationMapColors(std::vector< std::vector<float> >& colorData) const {
	colorData.clear();
	colorData.resize(nlat(), std::vector<float>(nlon(), 0));

	const std::vector<float>& map = correlations[refPtIndices.y()*nlon()+refPtIndices.x()];

	assert(map.size() == nlat()*nlon());

	for (unsigned i=0; i < nlat(); i++) {
		for (unsigned j=0; j< nlon(); j++) {
			//copy & convert range from [-1,1] to [0,1]
			colorData[i][j] =  (map[i*nlon()+j] + 1.0) / 2.0;
		}
	}
}

void ExplorationModelImpl::computeStatisticalSignificanceMask(float ssLevel) {

	assert(nlat() == tc.size() && nlon() == tc[0].size());
	statisticalSignificanceMask.clear();
	statisticalSignificanceMask.resize(nlat(), std::vector<bool>(nlon(), false));

	for (unsigned i=0; i<nlat(); i++) {
		for (unsigned j=0; j<nlon(); j++) {
			int pt = i*nlon()+j;

			float r0 = autocorrelations[pt];

			float delta = r0; //r0 + 0.68 / sqrt( (float)ntime);
			float n_eff = ntime()*(1.0-delta)/(1.0+delta);

			float df = n_eff - 2.0;

			float r = tc[i][j];

			double t=r*sqrt(df/(1.0-r*r));

			statisticalSignificanceMask[i][j] = ttest(ssLevel, t, df, true);
		}
	}
}

void ExplorationModelImpl::getStatisticalSignificanceMask(std::vector< std::vector<bool> >& ssMask) const {
	ssMask.clear();
	ssMask = statisticalSignificanceMask;
}

void ExplorationModelImpl::getTeleconnectivityMapColors(std::vector< std::vector<float> >& colorData) const {
	assert(nlat() == tc.size() && nlon() == tc[0].size());
	colorData = tc;

	//filter the values below threshold
	for (unsigned i=0; i<nlat(); i++) {
		for (unsigned j=0; j<nlon(); j++) {
			if (tc[i][j] < threshold) {
				colorData[i][j] = 0.0f;
			}
		}
	}
}

void ExplorationModelImpl::getCorrelationMapChainLinks(std::vector<VCGL::AnnotationLinkF>& links) const {
	//convert chain to the required format

	//converting the chain to links
	//the first link starts with reference point #0 and connects it to the point #1
	//all the following links start with point #2i and connect it to #2i-1 and to #2i+1

	links.clear();
	if (chosenPoints.size() >= 2) {
		links.push_back(
			VCGL::AnnotationLinkF(
				indicesToCoordinates(chosenPoints[0]),
				indicesToCoordinates(chosenPoints[1]),
				VCGL::AnnotationLinkF::SHOW_ALL));

		for (unsigned j=2; j<chosenPoints.size(); j+=2) {
			links.push_back(
				VCGL::AnnotationLinkF(
					indicesToCoordinates(chosenPoints[j]),
					indicesToCoordinates(chosenPoints[j-1]),
					VCGL::AnnotationLinkF::SHOW_ALL));

			if (j+1<chosenPoints.size()) {
				links.push_back(
					VCGL::AnnotationLinkF(
							indicesToCoordinates(chosenPoints[j]),
							indicesToCoordinates(chosenPoints[j+1]),
							VCGL::AnnotationLinkF::SHOW_ALL));
			}
		}
	}
}

void ExplorationModelImpl::getCorrelationChainProjection(std::vector< QPointF >& chainProjection) const {
	assert(nlat() == projectionData.size() && nlon() == projectionData[0].size());
	chainProjection.clear();
	for (unsigned i=0; i<chosenPoints.size(); i++) {
		const QPointF& projectedPoint = projectionData[ chosenPoints[i].y() ][ chosenPoints[i].x() ];
		chainProjection.push_back( projectedPoint );
	}
}

void ExplorationModelImpl::getTeleconnectivityMapLinks(std::vector<VCGL::AnnotationLinkF>& links) const {
	std::vector<Link> rcLinks;
	rc.getLinks(rcLinks);

	links.clear();
	links.resize(rcLinks.size());

	for (unsigned j=0; j<rcLinks.size(); j++) {
		links[j] = VCGL::AnnotationLinkF(
				indicesToCoordinates(rcLinks[j].ptA),
				indicesToCoordinates(rcLinks[j].ptB),
				(VCGL::AnnotationLinkF::Selection)(VCGL::AnnotationLinkF::SHOW_A | VCGL::AnnotationLinkF::SHOW_LINE));
	}
}

bool weightDescending(const VCGL::LinkF& a, const VCGL::LinkF& b) {
	return a.w > b.w;
}

void ExplorationModelImpl::getTeleconnectivityLinks(std::vector<VCGL::LinkF>& links) const {
	std::vector<Link> rcLinks;
	rc.getLinks(rcLinks);

	links.clear();
	links.resize(rcLinks.size());

	for (unsigned j=0; j<rcLinks.size(); j++) {
		links[j] = VCGL::LinkF(
				indicesToCoordinates(rcLinks[j].ptA),
				indicesToCoordinates(rcLinks[j].ptB),
				rcLinks[j].w);
	}
	std::stable_sort(links.begin(), links.end(), weightDescending);
}

void ExplorationModelImpl::getProjectionData(std::vector< std::vector<QPointF> >& projectionData) const {
	projectionData = this->projectionData;
}

void ExplorationModelImpl::selectReferencePoint( const QPointF& pointCoordinates ) {
	assert(nlon()>0 && nlat()>0);
	if (nlon()>0 && nlat()>0) {
		QPoint indices = findClosestPointIndices(pointCoordinates);

		refPtIndices = indices;

		buildCorrelationChain();
	}
}

QPointF ExplorationModelImpl::getReferencePoint() const {
	return indicesToCoordinates(refPtIndices);
}

QPointF ExplorationModelImpl::getReferencePointProjection() const {
	assert(nlat() == projectionData.size() && nlon() == projectionData[0].size());
	assert(refPtIndices.x()>=0 && refPtIndices.y()>=0);
	assert(nlat() > (unsigned)refPtIndices.y() && nlon() > (unsigned)refPtIndices.x());
	return projectionData[refPtIndices.y()][refPtIndices.x()];
}

void ExplorationModelImpl::selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent) {
	QPoint indices = findClosestPointIndices(point);

	std::vector< std::vector<bool> > newSelectionMask (nlat(), std::vector<bool>(nlon(), false));
	unsigned nSelectedPoints = 0;

	int ptRegionNum = regionMap[indices.y()][indices.x()];

	if (nRegions > 1 && ptRegionNum > 0) {

		//collect connected component
		std::set<int> componentRegions;
		componentRegions.insert(ptRegionNum);

		if (bSelectWholeComponent)
		{
			bool bChanged = true;
			while (bChanged) {
				bChanged = false;

				std::vector<int> currentRegions (componentRegions.begin(), componentRegions.end());
				for (unsigned cr = 0; cr<currentRegions.size(); cr++) {
					for (unsigned r=0; r<nRegions; r++) {
						VCGL::Link link;
						if (rc.getLink( currentRegions[cr], r, link ) ||
								rc.getLink( r, currentRegions[cr], link)){
							if (componentRegions.find(r) == componentRegions.end()) {
								componentRegions.insert(r);
								bChanged = true;
							}
						}
					}
				}
			}
		}

		for (unsigned i=0; i<nlat(); i++) {
			for (unsigned j=0; j<nlon(); j++) {
				if (componentRegions.find(regionMap[i][j]) != componentRegions.end()) {
					newSelectionMask[i][j] = true;
					nSelectedPoints++;
				}
			}
		}

	}

	setSelectionMask(newSelectionMask, nSelectedPoints);
}

void ExplorationModelImpl::resetRegionSelection() {
	std::vector< std::vector<bool> > newSelectionMask (nlat(), std::vector<bool>(nlon(), false));
	unsigned nSelectedPoints = 0;
	setSelectionMask(newSelectionMask, nSelectedPoints);
}

void
ExplorationModelImpl::setSelectionMask(const std::vector< std::vector<bool> >& selectionMask,
		unsigned numSelected) {
	this->numSelectedPoints = numSelected;
	if (numSelected > 0) {
		assert(nlat() == selectionMask.size() && nlon() == selectionMask[0].size());
		this->selectionMask = selectionMask;
	}
}

bool ExplorationModelImpl::pointsEqual(const QPointF& a, const QPointF& b) const {
	QPoint aIndices = findClosestPointIndices(a);
	QPoint bIndices = findClosestPointIndices(b);

	return (aIndices.x() == bIndices.x() && aIndices.y() == bIndices.y());
}

float ExplorationModelImpl::getCorrelationValue( const QPoint& aIndices, const QPoint& bIndices ) const {
	return correlations[aIndices.y()*nlon()+aIndices.x()][bIndices.y()*nlon()+bIndices.x()];
}

bool ExplorationModelImpl::xLooped() const {
	return grid.loopedLon();
}

bool ExplorationModelImpl::isCorrelationSignificant(const QPoint& aIndices, const QPoint& bIndices) const {
	const float ssLevel = 0.995; //TODO: change to user-defined?

	size_t npoints = nlat()*nlon();
	assert (correlations.size() == npoints);
	assert (correlations[0].size() == npoints);
	assert (autocorrelations.size() == npoints);

	int indexA = aIndices.y()*nlon()+aIndices.x();
	float r0 = autocorrelations[indexA];

	float delta = r0; //r0 + 0.68 / sqrt( (float)ntime);
	float n_eff = ntime()*(1.0-delta)/(1.0+delta);
	float df = n_eff - 2.0;
	float r = getCorrelationValue(aIndices, bIndices);

	double t=r*sqrt(df/(1.0-r*r));

	return ttest(ssLevel, t, df, true); // directional or not?
}

bool ExplorationModelImpl::isTeleconnectivitySignificant(const QPoint& indices) const {
	bool bReturn = true;

	if (indices.x() >=0 && indices.y() >= 0 &&
			statisticalSignificanceMask.size() > static_cast<unsigned>(indices.y())
			&& statisticalSignificanceMask[0].size() > static_cast<unsigned>(indices.x())) {
		bReturn = statisticalSignificanceMask[indices.y()][indices.x()];
	}
	return bReturn;
}

QPoint ExplorationModelImpl::findClosestPointIndices(const QPointF& pointCoordinates) const {
	assert(nlon()>0 && nlat()>0);

	float lonDiff = fabs( pointCoordinates.x() - grid.lons[0] );
	int lon = 0;
	for (unsigned i=1; i<nlon(); i++) {
		float diff = fabs( pointCoordinates.x() - grid.lons[i]);
		if (diff < lonDiff) {
			lonDiff = diff;
			lon = i;
		}
	}

	float latDiff = fabs( pointCoordinates.y() - grid.lats[0] );
	int lat = 0;
	for (unsigned i=1; i<nlat(); i++) {
		float diff = fabs( pointCoordinates.y() - grid.lats[i]);
		if (diff < latDiff) {
			latDiff = diff;
			lat = i;
		}
	}
	return QPoint(lon,lat);
}

QPointF ExplorationModelImpl::indicesToCoordinates(const QPoint& indices) const{
	int lat = indices.y();
	int lon = indices.x();
	assert(lon >= 0 && lat >= 0);
	assert(nlon() > (unsigned)lon && nlat() > (unsigned)lat);
	return QPointF(grid.lons[lon], grid.lats[lat]);
}

void ExplorationModelImpl::buildCorrelationChain() {
	//find NCORRELATIONPOINTS links from the reference point to the most negatively correlated
	const int MAXCORRELATIONPOINTS = 100;
	chosenPoints.clear();

	QPoint lastPoint = refPtIndices;
	chosenPoints.push_back( lastPoint );

	int chainLength=0;

	const QPoint indexFirstLink = tcindices[refPtIndices.y()][refPtIndices.x()];
	float lastCorrelationValue = getCorrelationValue(refPtIndices, indexFirstLink);

	while (chainLength < MAXCORRELATIONPOINTS && lastCorrelationValue < getThreshold() ) {

		QPoint minPt = lastPoint;
		float corrValue = 1.0f;

		for (unsigned y=0; y<nlat(); y++) {
			for (unsigned x=0; x<nlon(); x++) {
				QPoint newPt{(int)x, (int)y};
				if (getCorrelationValue(lastPoint,newPt) < corrValue) {

					//check that the point was not yet selected
					bool bPointIsNew = true;
					for (unsigned j=0; j<chosenPoints.size() && bPointIsNew; j++) {
						if (chosenPoints[j] == newPt) {
							bPointIsNew = false;
						}
					}

					if (bPointIsNew) {
						minPt = newPt;
						corrValue = getCorrelationValue(lastPoint,newPt);
					}
				}
			}
		}
		if ( !(minPt == lastPoint) ) {
			lastCorrelationValue = corrValue;
			lastPoint = minPt;
			chosenPoints.push_back(lastPoint);
		}

		chainLength++;
	}
}

void ExplorationModelImpl::computeTeleconnectivity() {
	//find teleconnectivity & tc-indices
	const unsigned npoints = nlat()*nlon();
	tc.clear();
	tcindices.clear();

	tc.resize(nlat(), std::vector<float>(nlon(), 0));
	tcindices.resize(nlat(), std::vector<QPoint>(nlon(), QPoint(-1,-1)));

	for (unsigned i=0; i<npoints; i++) {
		float minCorr = 1.0f;
		int minIndex = i;
		for (unsigned j=0; j<npoints; j++) {
			if (correlations[i][j] < minCorr) {
				minCorr = correlations[i][j];
				minIndex = j;
			}
		}

		int ptlat = i / nlon();
		int ptlon = i % nlon();

		int qlat = minIndex / nlon();
		int qlon = minIndex % nlon();

		tc[ptlat][ptlon] = fabs(minCorr);
		tcindices[ptlat][ptlon] = QPoint(qlon, qlat);
	}
}

bool ExplorationModelImpl::ttest(float ssLevel, double t, float df, bool directional) const {
	bool bSignificant = false;

	t = fabs(t);
	double p = gsl_cdf_tdist_Q(t, df);
	if (!directional) {
		p *= 2.0;
	}

	double z = 1.0-ssLevel;
	if ( p<=z ) {
		bSignificant = true;
	}
	return bSignificant;
}

void sweepContours(const MapGrid& clGrid,
		const std::vector< std::vector<QPointF> >& clContours,
		float deltaLon,
		std::vector< std::vector<QPointF> >& newContours) {

	unsigned currContour = 0;
	unsigned currPoint = 0;

	while (currContour < clContours.size()) {

		std::vector<QPointF> aContour;
		aContour.reserve(clContours[currContour].size());
		while (currPoint < clContours[currContour].size()) {
			QPointF point = clContours[currContour][currPoint];
			currPoint++;

			point = QPointF(point.x()+deltaLon, point.y());

			if (point.x() >= clGrid.lonMin() && point.x() <= clGrid.lonMax()
					&& point.y() >= clGrid.latMin() && point.y() <= clGrid.latMax()) {
				aContour.push_back(point);
			}
			else {
				break;
			}

		}

		if (aContour.size() > 0) {
			newContours.push_back(aContour);
		}
		if (currPoint == clContours[currContour].size()) {
			currContour++;
			currPoint = 0;
		}
	}
}

void ExplorationModelImpl::clipContours(const MapGrid& clGrid,
		std::vector< std::vector<QPointF> >& clContours) {

	std::vector< std::vector<QPointF> > newContours;

	sweepContours(clGrid, clContours, 0.0, newContours);
	sweepContours(clGrid, clContours, 360.0, newContours);
	sweepContours(clGrid, clContours, -360.0, newContours);

	clContours = newContours;

}

} /* namespace VCGL */
