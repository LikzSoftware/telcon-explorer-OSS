/*! @file legendsubview.cpp
 * @author anantonov
 * @date Created on Jun 3, 2014
 *
 * @brief Implementation of the subview that draws the legend
 */

#include "multiplatform/gl.h"
#include "legendsubview.h"
#include "colorizer/icolorizer.h"

#include "textpainter.h"
#include <QString>
#include <QtCore/qnamespace.h>

namespace VCGL {

LegendSubview::LegendSubview(): viewArea(-1,-1,-1,-1), labelWidth(0), labelHeight(0) {}

void
LegendSubview::setArea(const MRect& area) {
	viewArea = area;
}

void
LegendSubview::setLabelsSize(int width, int height) {
	labelWidth = width;
	labelHeight = height;
}

void
LegendSubview::drawLegendBar(const IColorizer* pColorizer, const float pixelRatio) {
	MRect barArea = getBarArea();

	int nLines = barArea.height() * pixelRatio;

	glBegin(GL_LINES);
		for (int i=0; i<nLines; i++) {
			float val = i/((float)nLines-1);
			float r=0, g=0, b=0;

			pColorizer->colorize(val, &r, &g, &b);

			glColor3f(r,g,b);
			glVertex2f(barArea.left+0.5, barArea.bottom+i/pixelRatio+0.5);
			glVertex2f(barArea.right+1.5, barArea.bottom+i/pixelRatio+0.5);
		}
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	//qglColor(Qt::black);
	glLineWidth(1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(barArea.left+0.5, barArea.bottom-0.5);
		glVertex2f(barArea.left+0.5, barArea.bottom+ nLines/pixelRatio + 0.5);

		glVertex2f(barArea.right+1.5, barArea.bottom+ nLines/pixelRatio + 0.5);

		glVertex2f(barArea.right+1.5, barArea.bottom-0.5);
	glEnd();
}

void
LegendSubview::drawLegendLabels(TextPainter* pPainter, const std::vector< std::pair <float, float> >& labels) {
	MRect barArea = getBarArea();

	for (std::pair<float,float> label: labels) {
		pPainter->drawText(
				barArea.right + hMargin + labelWidth,
				barArea.bottom + label.first * (barArea.height()-1),
				Qt::AlignRight | Qt::AlignVCenter,
				QString::number(label.second, 'f', 2));
	}
}

MRect
LegendSubview::getBarArea() {
	int barWidth = viewArea.width() - labelWidth - 2*hMargin;
	int barHeight = viewArea.height() - labelHeight;
	int barLeft = viewArea.left;
	int barBottom = viewArea.bottom + (labelHeight / 2);

	return MRect{
		barLeft,
		barBottom + barHeight - 1,
		barLeft + barWidth - 1,
		barBottom
	};
}

} /* namespace VCGL */
