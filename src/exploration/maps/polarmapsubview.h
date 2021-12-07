/*! @file polarmapsubview.h
 * @author anantonov
 * @date Created on Jun 4, 2014
 *
 * @brief Subview that visualizes data in the polar map projection
 */

#ifndef POLARMAPSUBVIEW_H_
#define POLARMAPSUBVIEW_H_

#include "mapsubview.h"

namespace VCGL {

class PolarMapSubview: public MapSubview {
public:
	/// Virtual destructor for proper memory deallocation
	virtual ~PolarMapSubview();

	/*! @brief Convert the pixel-based screen coordinates to longitude-latitude (of the closest grid point) (@see MapSubview)
	 *
	 * @param[in] grid			Data reference coordinate grid
	 * @param[in] layoutPos		Pixel position in the layout, to be converted
	 * @param[out] pOutPoint	Longitude-latitude coordinate position (of the closest grid point)
	 * @return 					true if conversion succeeded, false otherwise
	 */
	virtual bool getPointCoordinates(const MapGrid& grid, QPoint layoutPos, QPointF* pOutPoint) const override;

	/*! @brief Draw the color data (@see MapSubview)
	 *
	 * @param pColorizer	Pointer to the colorizer object, which translates data values to colors
	 * @param colorData		2D array of data values to be drawn
	 * @param grid			Data reference coordinate grid
	 * @param selectionMask Selection mask for the points, pass an empty object if no specific selection is made
	 */
	virtual void drawColor(const IColorizer* pColorizer,
				const std::vector< std::vector<float> >& colorData,
				const MapGrid& grid,
				const std::vector< std::vector<bool> >& selectionMask) override;

	/// Draw the grid lines (@see MapSubview)
	virtual void drawGrid(const MapGrid& grid) override;

	/*! @brief Draw the grid labels (@see MapSubview)
	 *
	 * @param pPainter 	A pointer to the text-rendering object
	 * @param grid		Data reference coordinate grid
	 */
	virtual void drawGridLabels(TextPainter* pPainter, const MapGrid& grid) override;

	/*! @brief Draw the land contours (@see MapSubview)
	 *
	 * @param grid		Data reference coordinate grid
	 * @param contours  Vector of vectors as collection of individual contours with points in (lon,lat) coordinate system
	 */
	virtual void drawLandContours(const MapGrid& grid,
			const std::vector< std::vector<QPointF> >& contours) override;
protected:
	/// Get the rectangle (layout positions) for the actual map drawing
	virtual MRect getMapArea() const;

	/// Transform coordinates to the pixel position in the layout (@see MapSubview)
	virtual void transformCoordinates(const MapGrid& grid, const float ptLonF, const float ptLatF, float& pxX, float& pxY) const override;

private:
	/// Get parameters of polar coordinate system (center coordinates, max radius)
	void getPolarParams(float* pCenterX, float* pCenterY, float* pRMax) const;
};

} /* namespace VCGL */

#endif /* POLARMAPSUBVIEW_H_ */
