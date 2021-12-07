/*! @file legendsubview.h
 * @author anantonov
 * @date Created on Jun 3, 2014
 *
 * @brief Definition of the subview that draws the legend
 */

#ifndef LEGENDSUBVIEW_H_
#define LEGENDSUBVIEW_H_

#include "mrect.h"
#include <vector>

namespace VCGL {
struct IColorizer;
struct TextPainter;

/// Class of the subview that draws the legend
class LegendSubview {
public:
	/// Default constructor
	LegendSubview();
	/// Set corresponding area in the view
	void setArea(const MRect& area);
	/// Hint at the estimated area required for a label
	void setLabelsSize(int width, int height);

	/// Fill the legend color bar
	void drawLegendBar(const IColorizer* pColorizer, const float pixelRatio);

	/*! @brief Render the legend key point labels
	 *
	 * @param pPainter 	A pointer to the text-rendering object
	 * @param labels	Vector of pairs (legend bar position, label value)
	 */
	void drawLegendLabels(TextPainter* pPainter, const std::vector< std::pair <float, float> >& labels);

private:
	const int hMargin = 5;	///< horizontal margin
	/// Get the area for drawing the legend bar
	MRect getBarArea();
	MRect viewArea; ///< complete area for the legend subview
	int labelWidth; ///< hint at the width of the labels
	int labelHeight; ///< hint at the height of the labels
};

} /* namespace VCGL */

#endif /* LEGENDSUBVIEW_H_ */
