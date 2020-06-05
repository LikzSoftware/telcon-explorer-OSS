/*! @file projectionview.h
 * @author anantonov
 * @date Created on Jun 22, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef PROJECTIONVIEW_H_
#define PROJECTIONVIEW_H_

#include <vector>
#include <QPoint>
#include "transformmatrix2d.h"

namespace VCGL {
struct IColorizer;
struct RGBF;

class ProjectionView {
public:
	ProjectionView();
	virtual ~ProjectionView();

	virtual void drawProjection(
			const std::vector< std::vector<QPointF> >& projectionData,
			const std::vector< std::vector<float> >& colorData,
			const std::vector< std::vector<bool> >& selectionMask,
			const IColorizer* pColorizer,
			float projPointSize);
	virtual void drawChain(
			const std::vector<QPointF> & chainProjection,
			const RGBF& lineColor,
			const RGBF& evenPointColor,
			const RGBF& oddPointColor,
			float chainLineSize,
			float chainPointSize);
	virtual void putProjectedPoint(QPointF projectedPoint,
								   const RGBF& color,
								   float pointSize);

	void resizeView(int width, int height);

	void zoom(float addScale, QPoint pt);

	bool findClosestPointIndices(
			const std::vector< std::vector<QPointF> >& projectionData,
			QPoint currPos,
			QPoint& ptIndices);

	QPointF dataToModel(const QPointF& projectedPoint);
private:
	void minmax(const std::vector< std::vector<QPointF> >& projectionData,
			float& minX,
			float& maxX,
			float& minY,
			float& maxY);
	void initTransform(float minX,
			float maxX,
			float minY,
			float maxY);

	int viewWidth; int viewHeight;
	TransformMatrix2D currentTransform;

	bool dataInitialized;

	float dataRangeX;
	float dataRangeY;
	float initViewScale;

};

} /* namespace VCGL */

#endif /* PROJECTIONVIEW_H_ */
