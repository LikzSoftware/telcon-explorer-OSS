/*! @file equirectangularmapsubview.cpp
 * @author anantonov
 * @date Created on Jun 10, 2014
 *
 * @brief Subview that visualizes data in the equirectangular map projection
 */

#include "multiplatform/gl.h"
#include "mapsubview.h"
#include "colorizer/icolorizer.h"
#include "mapgrid.h"
#include "equirectangularmapsubview.h"

#include "exploration/coordinatetext.h"

#include <QtCore/qnamespace.h>
#include <QString>
#include "textpainter.h"

#include <cassert>
#include <QPoint>
#include <cmath>

namespace VCGL {

EquirectangularMapSubview::~EquirectangularMapSubview() {}

bool EquirectangularMapSubview::getPointCoordinates(const MapGrid& grid,
		QPoint layoutPos,
		QPointF* pOutPoint) const {
	bool bResult = false;

	if (grid.nlon()>0 && grid.nlat()>0) {
		const MRect mapArea = getMapArea();
		if (layoutPos.x() >= mapArea.left && layoutPos.x() <= mapArea.right &&
				layoutPos.y() >= mapArea.bottom && layoutPos.y() <= mapArea.top) {

			float lonLeft = grid.lonMin() - fabs(grid.dx()/2);
			float lonRight = grid.lonMax() + fabs(grid.dx()/2);
			float latBottom = grid.latMin() - fabs(grid.dy()/2);
			float latTop = grid.latMax() + fabs(grid.dy()/2);

			float lonPos = (layoutPos.x() - mapArea.left) / ((float)(mapArea.width()-1));
			float latPos = (layoutPos.y() - mapArea.bottom) / ((float)(mapArea.height()-1));
			float lonValueEst = lonLeft + lonPos * (lonRight-lonLeft);
			float latValueEst = latBottom + latPos * (latTop-latBottom);

			*pOutPoint = grid.nearestGridPoint( QPointF{lonValueEst, latValueEst} );
			bResult = true;
		}
	}
	return bResult;
}


void
EquirectangularMapSubview::drawColor(const IColorizer* pColorizer,
		const std::vector< std::vector<float> >& colorData,
		const MapGrid& grid,
		const std::vector< std::vector<bool> >& selectionMask) {
	assert(colorData.size()>0 && colorData[0].size()>0);
	const unsigned nlat = colorData.size();
	const unsigned nlon = colorData[0].size();
	assert(selectionMask.size() == 0 || (selectionMask.size()==nlat && selectionMask[0].size()==nlon) );

//	const MRect mapArea = getMapArea();

//	float cellWidth = mapArea.width()/((float)nlon);
//	float cellHeight = mapArea.height()/((float)nlat);

	glBegin(GL_QUADS);
		for (unsigned i=0; i<nlat; i++) {
			//float ccBottom = mapArea.top-(i+1)*cellHeight + 1;

			for (unsigned j=0; j<nlon; j++) {
				//float ccLeft = mapArea.left+j*cellWidth;

				float r=0, g=0, b=0;

				pColorizer->colorize(colorData[i][j], &r, &g, &b);

				float mult = 1.0;
				if (selectionMask.size() > 0 && selectionMask[i][j] == false) {
					mult = 0.6;
				}
				glColor3f(r*mult, g*mult, b*mult);

//				glVertex2f(ccLeft, ccBottom);
//				glVertex2f(ccLeft, ccBottom+cellHeight);
//				glVertex2f(ccLeft+cellWidth, ccBottom+cellHeight);
//				glVertex2f(ccLeft+cellWidth, ccBottom);

//				putVertex(grid, grid.lons[j], grid.lats[i+1]);
//				putVertex(grid, grid.lons[j], grid.lats[i]);
//				putVertex(grid, grid.lons[(j+1)%nlon], grid.lats[i]);
//				putVertex(grid, grid.lons[(j+1)%nlon], grid.lats[i+1]);

				putVertex(grid, grid.lons[j]-fabs(grid.dx()/2), grid.lats[i]-fabs(grid.dy()/2));
				putVertex(grid, grid.lons[j]-fabs(grid.dx()/2), grid.lats[i]+fabs(grid.dy()/2));
				putVertex(grid, grid.lons[j]+fabs(grid.dx()/2), grid.lats[i]+fabs(grid.dy()/2));
				putVertex(grid, grid.lons[j]+fabs(grid.dx()/2), grid.lats[i]-fabs(grid.dy()/2));
			}
		}
	glEnd();
}

void
EquirectangularMapSubview::drawGrid(const MapGrid& grid) {
	const MRect area = getMapArea();

	glColor3f(0.0f,0.0f,0.0f);

	glPushAttrib(GL_ENABLE_BIT);
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);

	glLineWidth(1.0f);

	float colorCellWidth = area.width()/((float)grid.nlon());
	float colorCellHeight = area.height()/((float)grid.nlat());

	float gridWidth = colorCellWidth * (grid.nlon()-1);
	float gridHeight = colorCellHeight * (grid.nlat()-1);

	const int minLineDistanceX = 100; //pixels
	int maxLinesX = (int)( (grid.nlon()-1) * colorCellWidth/minLineDistanceX)  + 1;
	int nVerticalLines = maxLinesX;
	float gridCellWidth = gridWidth / (nVerticalLines-1);

	const int minLineDistanceY = 50; //pixels
	int maxLinesY = (int)( (grid.nlat()-1) * colorCellHeight/minLineDistanceY) + 1;
	int nHorizontalLines = maxLinesY;
	float gridCellHeight = gridHeight / (float)(nHorizontalLines-1);


	glBegin(GL_LINES);
		if (nVerticalLines > 1) {
			for (int i=0; i<nVerticalLines; i++) {
				float gridX = i*gridCellWidth;
				float kX = (int)( gridX * 2 / colorCellWidth + 0.5) / 2.0 ; // round to the closest half-cell

				const float lineX = area.left + (kX+0.5)*colorCellWidth; // start mid-cell
				glVertex2f(lineX+0.5, area.bottom+0.5);
				glVertex2f(lineX+0.5, area.top+1.5);
			}
		}

		if (nHorizontalLines > 1) {
			for (int i=0; i<nHorizontalLines; i++) {
				float gridY = i*gridCellHeight;
				float kY = (int)( gridY * 2 / colorCellHeight + 0.5) / 2.0; // round to the closest half-cell

				const float lineY = area.top-(kY+0.5)*colorCellHeight; //start mid-cell
				glVertex2f(area.left+0.5, lineY+0.5);
				glVertex2f(area.right+1.5, lineY+0.5);
			}
		}
	glEnd();

	glPopAttrib();
}

void EquirectangularMapSubview::drawGridLabels(TextPainter* pPainter, const MapGrid& grid) {
	//TODO: refactor, extract common part with drawGrid
	const MRect area = getMapArea();

	float colorCellWidth = area.width()/((float)grid.nlon());
	float colorCellHeight = area.height()/((float)grid.nlat());

	float gridWidth = colorCellWidth * (grid.nlon()-1);
	float gridHeight = colorCellHeight * (grid.nlat()-1);

	const int minLineDistanceX = 100; //pixels
	int maxLinesX = (int)( (grid.nlon()-1) * colorCellWidth/minLineDistanceX)  + 1;
	int nVerticalLines = maxLinesX;
	float gridCellWidth = gridWidth / (nVerticalLines-1);

	const int minLineDistanceY = 50; //pixels
	int maxLinesY = (int)( (grid.nlat()-1) * colorCellHeight/minLineDistanceY) + 1;
	int nHorizontalLines = maxLinesY;
	float gridCellHeight = gridHeight / (float)(nHorizontalLines-1);


	if (nVerticalLines > 1) {
		for (int i=0; i<nVerticalLines; i++) {
			float gridX = i*gridCellWidth;
			float kX = (int)( gridX * 2 / colorCellWidth + 0.5) / 2.0 ; // round to the closest half-cell

			const float lineX = area.left + (kX+0.5)*colorCellWidth; // start mid-cell


			int ikX = (int)kX; // either line or half cell before
			float lonValue = grid.lons[ikX];
			if (ikX < ((int)grid.nlon()-1) ) {
				float alpha = kX - ikX;
				lonValue = (1.0-alpha)*grid.lons[ikX] + alpha*grid.lons[ikX+1];
			}

			QString lonStr = CoordinateText::convertLon(lonValue);

			pPainter->drawText(lineX,
					area.bottom,
					Qt::AlignHCenter | Qt::AlignTop,
					lonStr);
		}
	}

	if (nHorizontalLines > 1) {
		bool reversedLats = false;
		if (grid.nlat() > 0 && grid.latMin() == grid.lats[0]) {
			reversedLats = true;
		}
		for (int i=0; i<nHorizontalLines; i++) {
			float gridY = i*gridCellHeight;
			float kY = (int)( gridY * 2 / colorCellHeight + 0.5) / 2.0; // round to the closest half-cell

			const float lineY = area.top-(kY+0.5)*colorCellHeight; //start mid-cell


			int ikY = (int)kY; // either line or half cell before
			float latValue = grid.lats[ reversedLats ? grid.nlat()-1 - ikY : ikY ];
			if (ikY < ((int)grid.nlat()-1) ) {
				float alpha = kY - ikY;
				latValue = (1.0-alpha)*grid.lats[ reversedLats ? (grid.nlat()-1 - ikY) : ikY ]
								+ alpha*grid.lats[reversedLats ? (grid.nlat()-1 -(ikY+1)) : (ikY+1)];
			}

			QString latStr = CoordinateText::convertLat(latValue);

			pPainter->drawText(area.left - hMargin,
					lineY,
					Qt::AlignRight | Qt::AlignVCenter,
					latStr);
		}
	}
}

void EquirectangularMapSubview::drawLandContours(const MapGrid& grid,
		const std::vector< std::vector<QPointF> >& contours) {
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0f); // should be dependent on device pixel ratio and/or purpose (screen/print)

	for (unsigned j=0; j<contours.size(); j++) {
		glBegin(GL_LINE_STRIP);
			for (unsigned k=0; k<contours[j].size(); k++) {
				QPointF pt = contours[j][k];
				putVertex(grid, pt.x(), pt.y(), true);
			}
		glEnd();
	}
}

MRect EquirectangularMapSubview::getMapArea() const {
	MRect viewArea = getArea();
	MRect mapArea = viewArea;
	if (getLabelWidth() > 0 && getLabelHeight() > 0 ) {
		mapArea.left += getLabelWidth() + hMargin;
		mapArea.right -= (getLabelWidth()/2);
		mapArea.bottom += getLabelHeight();
		mapArea.top -= getLabelHeight()/2;
	}
	return mapArea;
}

void EquirectangularMapSubview::transformCoordinates(const MapGrid& grid,
		const float ptLonF,
		const float ptLatF,
		float& pxX,
		float& pxY) const {
	const MRect area = getMapArea();

//	//try 1: assume that the grid is 360 x 180 = [0,360)x[-90,90]
//	const float lonMin = 0.0;
//	const float lonMax = 360.0;
//	const float latMin = -90.0;
//	const float latMax = 90.0;

	const float left = grid.lonMin() - fabs(grid.dx()/2.0);
	const float right = grid.lonMax() + fabs(grid.dx()/2.0);
	const float top = grid.latMax() + fabs(grid.dy()/2.0);
	const float bottom = grid.latMin() - fabs(grid.dy()/2.0);


	float xFactor = (ptLonF-left)/(right - left);
	float yFactor = (ptLatF-bottom)/(top - bottom);

	pxX = area.left + xFactor*area.width();
	pxY = area.bottom + yFactor*area.height();
}

} /* namespace VCGL */
