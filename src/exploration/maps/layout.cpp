/*! @file layout.cpp
 * @author anantonov
 * @date Created on Jun 3, 2014
 *
 * @brief Implementation of the logic for computing layout of MapLayout widget
 */

#include "layout.h"
#include <cassert>

namespace VCGL {
Layout::Layout(): hMargin(0), vMargin(0) {}

void
Layout::setMargins(int horizontalMargin, int verticalMargin) {
	hMargin = horizontalMargin;
	vMargin = verticalMargin;
}

void Layout::computeLayout(int viewWidth, int viewHeight, MapLayoutData* pLayoutData) {
assert(pLayoutData != 0);

const int LEGEND_AREA_WIDTH = 100;

const int viewBottom = 0;
const int viewTop = viewHeight-1;
const int viewLeft = 0;
const int viewRight = viewWidth-1;

const int areasBorderLeft = viewWidth - (LEGEND_AREA_WIDTH + 2*hMargin);

pLayoutData->mapArea = MRect(viewLeft+hMargin,
		viewTop-vMargin,
		areasBorderLeft-1-hMargin,
		viewBottom+vMargin);

pLayoutData->legendArea = MRect(areasBorderLeft+hMargin,
		viewTop-vMargin,
		viewRight-hMargin,
		viewBottom+vMargin);
}

} /* namespace VCGL */
