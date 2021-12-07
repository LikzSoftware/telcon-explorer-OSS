/*! @file mapsubview.h
 * @author anantonov
 * @date Created on Jun 1, 2014
 *
 * @brief Definition of abstract base class for map subviews
 */

#ifndef MAPSUBVIEW_H_
#define MAPSUBVIEW_H_

#include "mrect.h"
#include <vector>
#include <QPoint>
#include "annotationlink.h"

namespace VCGL {
struct IColorizer;
struct MapGrid;
struct TextPainter;
struct RGBF;

/// Abstract base class for map subviews
class MapSubview {
public:
	/// Virtual destructor for proper memory deallocation
	virtual ~MapSubview();

	/// Defautl constructor
	MapSubview();

	/// Set area for the subview (boundaries, in pixel coordinates)
	void setArea(const MRect& area);

	/// Get the area for the subview
	MRect getArea() const;

	/// Hint at the size of the labels (area occupied by each of them)
	void setLabelsSize(int width, int height);

	/*! @brief Convert the pixel-based screen coordinates to longitude-latitude (of the closest grid point)
	 *
	 * @param[in] grid			Data reference coordinate grid
	 * @param[in] layoutPos		Pixel position in the layout, to be converted
	 * @param[out] pOutPoint	Longitude-latitude coordinate position (of the closest grid point)
	 * @return 					true if conversion succeeded, false otherwise
	 */
	virtual bool getPointCoordinates(const MapGrid& grid, QPoint layoutPos, QPointF* pOutPoint) const = 0;

	/*! @brief Draw the color data
	 *
	 * @param pColorizer	Pointer to the colorizer object, which translates data values to colors
	 * @param colorData		2D array of data values to be drawn
	 * @param grid			Data reference coordinate grid
	 * @param selectionMask Selection mask for the points, pass an empty object if no specific selection is made
	 */
	virtual void drawColor(const IColorizer* pColorizer,
			const std::vector< std::vector<float> >& colorData,
			const MapGrid& grid,
			const std::vector< std::vector<bool> >& selectionMask) = 0;

	/// Draw the grid lines
	virtual void drawGrid(const MapGrid& grid) = 0;

	/*! @brief Draw the grid labels
	 *
	 * @param pPainter 	A pointer to the text-rendering object
	 * @param grid		Data reference coordinate grid
	 */
	virtual void drawGridLabels(TextPainter* pPainter, const MapGrid& grid) = 0;

	/*! @brief Draw the land contours
	 *
	 * @param grid		Data reference coordinate grid
	 * @param contours  Vector of vectors as collection of individual contours with points in (lon,lat) coordinate system
	 */
	virtual void drawLandContours(const MapGrid& grid, const std::vector< std::vector<QPointF> >& contours) = 0;

	virtual void putPoint(const MapGrid& grid,
						  QPointF ptCoordinates,
						  const RGBF& color,
						  float pointSize);

	virtual void drawLinks(const MapGrid& grid,
			const std::vector<VCGL::AnnotationLinkF>& links,
			const RGBF& lineColor,
			const RGBF& ptAColor,
			const RGBF& ptBColor,
			float pointSize,
			float lineSize);
protected:
	/// Access label width
	int getLabelWidth() const;
	/// Access label height
	int getLabelHeight() const;

	/*! @brief Place an OpenGL vertex corresponding to the given coordinates
	 *
	 * @param grid		Data reference coordinate grid
	 * @param ptLonF	Longitude of the point
	 * @param ptLatF	Latitude of the point
	 * @param shift		Correction for rasterization of primitives, true for lines and false for big areas
	 */
	void putVertex(const MapGrid& grid, const float ptLonF, const float ptLatF, bool shift = false);

	/// Transform coordinates to the pixel position in the layout
	virtual void transformCoordinates(const MapGrid& grid, const float ptLonF, const float ptLatF, float& pxX, float& pxY) const = 0;
private:
	MRect viewArea; ///< area for the subview
	int labelWidth;	///< width of labels
	int labelHeight; ///< height of labels
};

} /* namespace VCGL */

#endif /* MAPSUBVIEW_H_ */
