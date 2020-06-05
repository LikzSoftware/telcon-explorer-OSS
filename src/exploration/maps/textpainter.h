/*! @file textpainter.h
 * @author anantonov
 * @date Created on Jun 11, 2014
 *
 * @brief Interface for text rendering
 */

#ifndef TEXTPAINTER_H_
#define TEXTPAINTER_H_

class QString;

namespace VCGL {

/// Interface for text rendering
struct TextPainter {
	/*! @brief Render the text string
	 *
	 * @param x		Horizontal coordinate for the anchor point
	 * @param y 	Vertical coordinate for the anchor point
	 * @param flags Flags for the rendering (@see QPainter::drawText)
	 * @param text	Text string to be rendered
	 */
	virtual void drawText(int x, int y, int flags, const QString& text) = 0;

	/// Virtual destructor for proper memory deallocation
	virtual ~TextPainter() {}
};

} /* namespace VCGL */

#endif /* TEXTPAINTER_H_ */
