/*! @file layout.h
 * @author anantonov
 * @date Created on Jun 3, 2014
 *
 * @brief Logic for computing layout of MapLayout widget
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_

#include "mrect.h"

namespace VCGL {

/// Structure describing the layout
struct MapLayoutData {
	MRect mapArea; ///< area for drawing the map (including map labels)
	MRect legendArea; ///< area for drawing the legend (including legend labels)
};

/// Logic for computing layout of MapLayout widget
class Layout {
public:
	/// Default constructor
	Layout();
	/// Set sizes of margins between layout elements and to the borders of the view
	void setMargins(int horizontalMargin, int verticalMargin);
	/// Compute the layout for the given width and height of the view
	void computeLayout(int viewWidth, int viewHeight, MapLayoutData* pLayoutData);
private:
	int hMargin; ///< horizontal margin
	int vMargin; ///< vertical margin
};

} /* namespace VCGL */

#endif /* LAYOUT_H_ */
