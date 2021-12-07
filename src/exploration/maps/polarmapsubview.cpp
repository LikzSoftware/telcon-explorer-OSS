/*! @file polarmapsubview.cpp
 * @author anantonov
 * @date Created on Jun 4, 2014
 *
 * @brief Subview that visualizes data in the polar map projection
 */

#include "multiplatform/gl.h"
#include "polarmapsubview.h"
#include "colorizer/icolorizer.h"
#include "mapgrid.h"
#include <cmath>

#include <QtCore/qnamespace.h>
#include <QString>
#include "textpainter.h"
#include "exploration/coordinatetext.h"

#include <QPoint>
#include <cassert>

namespace VCGL {

PolarMapSubview::~PolarMapSubview() {}

bool PolarMapSubview::getPointCoordinates(const MapGrid& grid,
		QPoint layoutPos,
		QPointF* pOutPoint) const {

	bool bResult = false;

	if (grid.nlon()>0 && grid.nlat()>0) {
		const MRect mapArea = getMapArea();
		if (layoutPos.x() >= mapArea.left && layoutPos.x() <= mapArea.right &&
				layoutPos.y() >= mapArea.bottom && layoutPos.y() <= mapArea.top) {

			float centerX = 0.0, centerY = 0.0, rmax = 0.0;
			getPolarParams(&centerX, &centerY, &rmax);

			float pX = (layoutPos.x()-centerX);
			float pY = -(layoutPos.y()-centerY);
			float r = sqrt( pX*pX + pY*pY );

			if (r <= rmax) {
				float cphi = pY/r;

				float latMin = grid.latMin();
				float latCenter = 90.0;

				float phi = acos(cphi);
				if (pX < 0) {
					phi = M_PI*2.0-phi ;
				}

				float latPos = r/rmax;
				float lonValueEst = phi * 180.0 * M_1_PI;
				float latValueEst = latCenter - latPos * (latCenter-latMin);

				if (lonValueEst >= grid.lonMin() && lonValueEst <= grid.lonMax()
						&& latValueEst >= grid.latMin() && latValueEst <= grid.latMax()) {
					*pOutPoint = grid.nearestGridPoint( QPointF{lonValueEst, latValueEst} );
					bResult = true;
				}
			}
		}
	}

	return bResult;
}

void
PolarMapSubview::drawColor(const IColorizer* pColorizer,
		const std::vector< std::vector<float> >& colorData,
		const MapGrid& grid,
		const std::vector< std::vector<bool> >& selectionMask) {
	assert(colorData.size()>0 && colorData[0].size()>0);
	const unsigned nlat = colorData.size();
	const unsigned nlon = colorData[0].size();
	assert(selectionMask.size() == 0 || (selectionMask.size()==nlat && selectionMask[0].size()==nlon) );

	unsigned xSteps = grid.loopedLon() ? nlon : nlon-1;

	glBegin(GL_QUADS);
		for (unsigned i=0; i<nlat-1; i++) {
			for (unsigned j=0; j<xSteps; j++) {

				float r=0, g=0, b=0;

				pColorizer->colorize(colorData[i][j], &r, &g, &b);

				float mult = 1.0;
				if (selectionMask.size() > 0 && selectionMask[i][j] == false) {
					mult = 0.6;
				}
				glColor3f(r*mult, g*mult, b*mult);


				putVertex(grid, grid.lons[j], grid.lats[i+1]);
				putVertex(grid, grid.lons[j], grid.lats[i]);
				putVertex(grid, grid.lons[(j+1)%nlon], grid.lats[i]);
				putVertex(grid, grid.lons[(j+1)%nlon], grid.lats[i+1]);
			}
		}
	glEnd();


}

void PolarMapSubview::drawGrid(const MapGrid& grid) {
	glColor3f(0.0f,0.0f,0.0f);

	glPushAttrib(GL_ENABLE_BIT);
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);

	glLineWidth(1.0f);

	const float lonStep = 30.0f;
	int nMeridians = (int) (360.0f / lonStep + 0.5)+1;

	const float latStep = 30.0f;
	int nParallels = (int) (180.0f / latStep + 0.5)+1; // including poles

	glBegin(GL_LINES);
		const float gridLatCenter = 90.0;
		const float gridLatEnd = grid.latMin();

		for (int i=0; i<nMeridians; i++) {
			float lon = i*lonStep;
			if ( (lon - grid.lons[0])/grid.dx() >= 0 &&
				 (lon - grid.lons[grid.nlon()-1])/grid.dx() <= 0) {
					putVertex(grid, lon, gridLatCenter, true);
					putVertex(grid, lon, gridLatEnd, true);
			}
		}

		unsigned xSteps = grid.loopedLon() ? grid.nlon() : grid.nlon()-1;

		//not drawing poles
		for (int i=1; i<nParallels-1; i++) {
			float lat = 90 - i*latStep;
			if (lat >= grid.latMin()) {
				for (unsigned j=0; j<xSteps; j++) {
					putVertex(grid, grid.lons[j], lat, true);
					putVertex(grid, grid.lons[(j+1)%grid.nlon()], lat, true);
				}
			}
		}
	glEnd();

	glPopAttrib();
}

void PolarMapSubview::drawGridLabels(TextPainter* pPainter, const MapGrid& grid) {

	const float lonStep = 30.0f;
	int nMeridians = (int) (360.0f / lonStep + 0.5); // not including 360

	float centerX = 0.0, centerY = 0.0, rmax = 0.0;
	getPolarParams(&centerX, &centerY, &rmax);

	float rLabel = sqrt( (getLabelWidth()/2)*(getLabelWidth()/2) + (getLabelHeight()/2)*(getLabelHeight()/2) );

	for (int i=0; i<nMeridians; i++) {
		float lonValue = i*lonStep;

		float r = rmax + rLabel;
		float phi = lonValue * M_PI/180.0;
		float pxX = centerX + r * sin( phi );
		float pxY = centerY - r * cos( phi );


		QString lonStr = CoordinateText::convertLon(lonValue);
		pPainter->drawText(pxX, pxY, Qt::AlignHCenter | Qt::AlignVCenter, lonStr);
	}
}

void PolarMapSubview::drawLandContours(const MapGrid& grid, const std::vector< std::vector<QPointF> >& contours) {
	//TODO: change!!! doing very approximate, dirty rendering now!
		glColor3f(0,0,0);
		glLineWidth(1.0f);

		for (unsigned j=0; j<contours.size(); j++) {
			glBegin(GL_LINE_STRIP);
				for (unsigned k=0; k< contours[j].size(); k++) {
					QPointF pt = contours[j][k];
					putVertex(grid, pt.x(), pt.y(), true);
				}

			glEnd();
		}
}

MRect PolarMapSubview::getMapArea() const {
	MRect viewArea = getArea();
	MRect mapArea = viewArea;

	if (getLabelWidth() > 0 && getLabelHeight() > 0 ) {
		const float centerX = (viewArea.left+viewArea.right)/2.0;
		const float centerY = (viewArea.top+viewArea.bottom)/2.0;

		float sideY = viewArea.height() - getLabelHeight();
		float sideX = viewArea.width() - getLabelWidth();
		float side = (sideY < sideX ? sideY : sideX);

		float rLabel = sqrt( (getLabelWidth()/2)*(getLabelWidth()/2) + (getLabelHeight()/2)*(getLabelHeight()/2) );

		float rmax = side / 2.0 - rLabel;


		mapArea = MRect { (int)(centerX - rmax),
			(int)(centerY + rmax),
			(int)(centerX + rmax),
			(int)(centerY - rmax) };
	}
	return mapArea;
}

void PolarMapSubview::transformCoordinates(const MapGrid& grid, const float ptLonF, const float ptLatF, float& pxX, float& pxY) const {
	float centerX = 0.0, centerY = 0.0, rmax = 0.0;
	getPolarParams(&centerX, &centerY, &rmax);

	float r = rmax * ((90.0-ptLatF)/ (90.0-grid.latMin()) );
	float phi = ptLonF * M_PI/180.0;
	pxX = centerX + r * sin( phi );
	pxY = centerY - r * cos( phi );

}

void PolarMapSubview::getPolarParams(float* pCenterX, float* pCenterY, float* pRMax) const {
	const MRect mapArea = getMapArea();
	*pCenterX = (mapArea.left+mapArea.right)/2.0;
	*pCenterY = (mapArea.top+mapArea.bottom)/2.0;
	*pRMax = ( mapArea.width() < mapArea.height() ? mapArea.width()-1 : mapArea.height()-1 ) / 2.0;

}

} /* namespace VCGL */
