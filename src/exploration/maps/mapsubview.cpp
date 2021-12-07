/*! @file mapsubview.cpp
 * @author anantonov
 * @date Created on Jun 1, 2014
 *
 * @brief Implementation of abstract base class for map subviews
 */

#include "mapsubview.h"
#include "colorizer/rgb.h"
#include "multiplatform/gl.h"

#include <cassert>

namespace VCGL {

MapSubview::~MapSubview() {}

MapSubview::MapSubview()
:viewArea(-1,-1,-1,-1), labelWidth(0), labelHeight(0) {}

void
MapSubview::setArea(const MRect& area) {
	viewArea = area;
}

MRect
MapSubview::getArea() const {
	return viewArea;
}

void MapSubview::setLabelsSize(int width, int height) {
	labelWidth = width;
	labelHeight = height;
}

void MapSubview::putPoint(const MapGrid& grid,
						  QPointF ptCoordinates,
						  const RGBF& color,
						  float pointSize) {
	glColor3f(color.r, color.g, color.b);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
		putVertex(grid, ptCoordinates.x(), ptCoordinates.y());
	glEnd();
}

void MapSubview::drawLinks(const MapGrid& grid,
		const std::vector<VCGL::AnnotationLinkF>& links,
		const RGBF& lineColor,
		const RGBF& ptAColor,
		const RGBF& ptBColor,
		float pointSize,
		float lineSize) {
	//draw lines
	glColor3f(lineColor.r, lineColor.g, lineColor.b);
	glLineWidth(lineSize); // should be dependent on device pixel ratio and/or purpose (screen/print)
	glBegin(GL_LINES);
		for (unsigned i=0; i<links.size(); i++) {
			if (links[i].sel & AnnotationLinkF::SHOW_LINE) {
				putVertex(grid, links[i].ptA.x(), links[i].ptA.y(), true);
				putVertex(grid, links[i].ptB.x(), links[i].ptB.y(), true);
			}
		}
	glEnd();

	//draw points B
	glColor3f(ptBColor.r, ptBColor.g, ptBColor.b);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
		for (unsigned i=0; i<links.size(); i++) {
			if (links[i].sel & AnnotationLinkF::SHOW_B) {
				putVertex(grid, links[i].ptB.x(), links[i].ptB.y());
			}
		}
	glEnd();

	//draw points A
	glColor3f(ptAColor.r, ptAColor.g, ptAColor.b);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
		for (unsigned i=0; i<links.size(); i++) {
			if (links[i].sel & AnnotationLinkF::SHOW_A) {
				putVertex(grid, links[i].ptA.x(), links[i].ptA.y());
			}
		}
	glEnd();
}

int
MapSubview::getLabelWidth() const {
	return labelWidth;
}

int
MapSubview::getLabelHeight() const {
	return labelHeight;
}

void MapSubview::putVertex(const MapGrid& grid, const float ptLonF, const float ptLatF, bool shift) {
	float x = -1.0;
	float y = -1.0;

	transformCoordinates(grid, ptLonF, ptLatF, x, y);
	if (shift) {
		x += 0.5;
		y += 0.5;
	}
	glVertex2f(x,y);
}

} /* namespace VCGL */
