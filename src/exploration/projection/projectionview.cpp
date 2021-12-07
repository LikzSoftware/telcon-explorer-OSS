/*! @file projectionview.cpp
 * @author anantonov
 * @date Created on Jun 22, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "projectionview.h"

#include "multiplatform/gl.h"
#include "multiplatform/devicepixelratio.h"
#include "colorizer/icolorizer.h"
#include "colorizer/rgb.h"

#include <iostream>

namespace VCGL {

ProjectionView::ProjectionView()
: viewWidth(0),
  viewHeight(0),
  currentTransform(TransformMatrix2D::Identity()),
  dataInitialized(false),
  dataRangeX(0),
  dataRangeY(0),
  initViewScale(1.0f)
{
	// TODO Auto-generated constructor stub

}

ProjectionView::~ProjectionView() {
	// TODO Auto-generated destructor stub
}

void ProjectionView::drawProjection(
			const std::vector< std::vector<QPointF> >& projectionData,
			const std::vector< std::vector<float> >& colorData,
			const std::vector< std::vector<bool> >& selectionMask,
			const IColorizer* pColorizer,
			float projPointSize) {

	if (dataInitialized == false && projectionData.size() > 0) {
		float minX = 0, maxX = 0, minY = 0, maxY = 0;
		minmax(projectionData, minX, maxX, minY, maxY);
		initTransform(minX, maxX, minY, maxY);
		dataInitialized = true;
	}

	// for visibility:
	// draw not-selected points
	for (unsigned i=0; i<projectionData.size();i++) {
		for (unsigned j=0; j<projectionData[i].size();j++) {
			if (selectionMask.size() > 0 && selectionMask[i][j] == false) {
				float pt_x = 0.0f;
				float pt_y = 0.0f;
				currentTransform.transformPoint(
						projectionData[i][j].x(),
						projectionData[i][j].y(),
						&pt_x,
						&pt_y);

				float r = 0, g = 0, b = 0;
				pColorizer->colorize(colorData[i][j], &r, &g, &b);

				float mult = 0.6;
				glColor3f(r*mult, g*mult, b*mult);

				glPointSize(projPointSize);

				glBegin(GL_POINTS);
					glVertex2f(pt_x, pt_y);
				glEnd();
			}
		}
	}

	//draw selected points
	for (unsigned i=0; i<projectionData.size();i++) {
		for (unsigned j=0; j<projectionData[i].size();j++) {
			if (selectionMask.size() == 0 || selectionMask[i][j] == true) {
				float pt_x = 0.0f;
				float pt_y = 0.0f;
				currentTransform.transformPoint(
						projectionData[i][j].x(),
						projectionData[i][j].y(),
						&pt_x,
						&pt_y);


				float r = 0, g = 0, b = 0;
				pColorizer->colorize(colorData[i][j], &r, &g, &b);
				glColor3f(r,g,b);
				glPointSize(projPointSize);
				glBegin(GL_POINTS);
					glVertex2f(pt_x, pt_y);
				glEnd();
			}
		}
	}

}

void ProjectionView::drawChain(
			const std::vector<QPointF> & chainProjection,
			const RGBF& lineColor,
			const RGBF& evenPointColor,
			const RGBF& oddPointColor,
		float chainLineSize,
		float chainPointSize) {

	glColor3f(lineColor.r, lineColor.g, lineColor.b);
	glLineWidth(chainLineSize);
	glBegin(GL_LINE_STRIP);
		for (unsigned i=0; i<chainProjection.size(); i++) {
			float pt_x = 0.0f;
			float pt_y = 0.0f;
			currentTransform.transformPoint(
					chainProjection[i].x(),
					chainProjection[i].y(),
					&pt_x,
					&pt_y);
			glVertex2f(pt_x, pt_y);
		}
	glEnd();

	for (unsigned i=0; i<chainProjection.size();i++) {
		float pt_x = 0.0f;
		float pt_y = 0.0f;
		currentTransform.transformPoint(
				chainProjection[i].x(),
				chainProjection[i].y(),
				&pt_x,
				&pt_y);

		if (i%2 == 0) {
			glColor3f(evenPointColor.r, evenPointColor.g, evenPointColor.b);
		}
		else {
			glColor3f(oddPointColor.r, oddPointColor.g, oddPointColor.b);
		}

		glPointSize(chainPointSize);

		glBegin(GL_POINTS);
			glVertex2f(pt_x, pt_y);
		glEnd();

	}
}

void ProjectionView::putProjectedPoint(QPointF projectedPoint,
									   const RGBF& color,
									   float pointSize) {
	if (dataInitialized) {
		float pt_x = 0.0f;
		float pt_y = 0.0f;
		currentTransform.transformPoint(
				projectedPoint.x(),
				projectedPoint.y(),
				&pt_x,
				&pt_y);

		glColor3f(color.r, color.g, color.b);

		glPointSize(pointSize);

		glBegin(GL_POINTS);
			glVertex2f(pt_x, pt_y);
		glEnd();
	}
}

void ProjectionView::resizeView(int width, int height) {
	// keep approximately the same view on the projection by adjusting scaling
	if (dataInitialized && viewHeight > 0 && viewWidth > 0) {
		float scaleX = (width-10) / dataRangeX;
		float scaleY = (height-10) / dataRangeY;
		float scale = (scaleX < scaleY) ? scaleX : scaleY;

		currentTransform = TransformMatrix2D::FlipScale(1.0,1.0, scale/initViewScale) *
				currentTransform;
		initViewScale = scale;
	}

	viewWidth = width;
	viewHeight = height;
}

void ProjectionView::zoom(float addScale, QPoint pt) {
	// a bit tricky, because
	//need to scale into the point mouse pointing to
	currentTransform =
			TransformMatrix2D::Translation(pt.x(), pt.y()) *
			TransformMatrix2D::FlipScale(1.0f, 1.0f, addScale) *
			TransformMatrix2D::Translation(-pt.x(), -pt.y()) *
			currentTransform;
	//transformDefault = false;
}

bool
ProjectionView::findClosestPointIndices(
			const std::vector< std::vector<QPointF> >& projectionData,
			QPoint pos,
			QPoint& outIndices) {

	const double sqrDistance = 25.0;
	bool bFound = false;

	unsigned closestXIndex = 0;
	unsigned closestYIndex = 0;
	double currentDistance = viewHeight*viewWidth; //reasonably big value =)

	//std::cerr << "Looking for point close to (" << pos.x() << ',' << pos.y() << ')' << std::endl;

	for (unsigned i=0; i<projectionData.size();i++) {
		for (unsigned j=0; j<projectionData[i].size(); j++) {
			float pt_x = 0.0f;
			float pt_y = 0.0f;;
			currentTransform.transformPoint(
					projectionData[i][j].x(),
					projectionData[i][j].y(),
					&pt_x,
					&pt_y);

			double xDiff = pt_x - pos.x();
			double yDiff = pt_y - pos.y();
			double dist = xDiff*xDiff + yDiff*yDiff;

			if ( dist < sqrDistance ) {
				if (bFound ==  false || dist < currentDistance) {
					closestYIndex = i;
					closestXIndex = j;
					currentDistance = dist;
					bFound = true;
				}
			}
		}
	}

	//std::cerr << "bFound = " << bFound << ", outIndices=(" << closestXIndex << ',' << closestYIndex << ')' << std::endl;

	outIndices = QPoint(closestXIndex, closestYIndex);
	return bFound;

}

QPointF
ProjectionView::dataToModel(const QPointF& projectedPoint) {
	float pt_x = 0.0f;
	float pt_y = 0.0f;
	currentTransform.transformPoint(
			projectedPoint.x(),
			projectedPoint.y(),
			&pt_x,
			&pt_y);
	return QPointF(pt_x, pt_y);
}

//void ProjectionView::init() {
//	double rangeX = maxX - minX;
//	double rangeY = maxY - minY;
//	if (transformDefault && rangeX > 0 && rangeY > 0) {
//		if (viewHeight > 0 && viewWidth > 0) {
//			float oldScale = scale;
//
//			float scaleX = (viewWidth-10) / rangeX;
//			float scaleY = (viewHeight-10) / rangeY;
//			scale = (scaleX < scaleY) ? scaleX : scaleY;
//
//			currentTransform =
//					TransformMatrix2D::FlipScale(1.0f, 1.0f, scale/oldScale) *
//					currentTransform;
//		}
//	}
//}

void
ProjectionView::minmax(const std::vector< std::vector<QPointF> >& projectionData,
		float& minX,
		float& maxX,
		float& minY,
		float& maxY) {

	if (projectionData.size() > 0 && projectionData[0].size() > 0) {

		//find min/max
		minX = projectionData[0][0].x();
		maxX = projectionData[0][0].x();
		minY = projectionData[0][0].y();
		maxY = projectionData[0][0].y();
		for (unsigned i=0; i<projectionData.size(); i++) {
			for (unsigned j=0; j<projectionData[i].size(); j++) {
				const double x = projectionData[i][j].x();
				const double y = projectionData[i][j].y();
				if (minX > x) {
					minX = x;
				}
				if (minY > y) {
					minY = y;
				}
				if (maxX < x) {
					maxX = x;
				}
				if (maxY < y) {
					maxY = y;
				}
			}
		}
	}
}

void
ProjectionView::initTransform(float minX,
		float maxX,
		float minY,
		float maxY) {
	const float flipX = 1.0f;
	const float flipY = 1.0f;


	double rangeX = maxX - minX;
	double rangeY = maxY - minY;

	float trX = - (maxX+minX) / 2.0f;
	float trY = - (maxY+minY) / 2.0f;

	float scale = 1.0f;

	if (viewHeight > 0 && viewWidth > 0) {
		float scaleX = (viewWidth-10) / rangeX;
		float scaleY = (viewHeight-10) / rangeY;
		scale = (scaleX < scaleY) ? scaleX : scaleY;
	}

	dataRangeX = rangeX;
	dataRangeY = rangeY;
	initViewScale = scale;

	currentTransform = TransformMatrix2D::FlipScale(flipX, flipY, scale) *
			TransformMatrix2D::Translation(trX, trY);
}

} /* namespace VCGL */
