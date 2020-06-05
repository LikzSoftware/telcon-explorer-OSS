/*! @file projectionwidget.cpp
 * @author anantonov
 * @date Created on Jun 22, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "projectionwidget.h"
#include "projectionview.h"

#include <QtGui>
#include <QtOpenGL>
#include <QMouseEvent>
#include <QToolTip>

#include "colorizer/icolorizer.h"
#include "exploration/maps/mapgrid.h"
#include "exploration/coordinatetext.h"

#include "multiplatform/devicepixelratio.h"


#include <cassert>

namespace VCGL {

ProjectionWidget::ProjectionWidget(QWidget* parent)
: QGLWidget(parent), pProjectionView(0), selectingRegion(false) {
	setMouseTracking(true); // allows to show tooltip when no mouse button pressed

	pProjectionView = new ProjectionView();
}

ProjectionWidget::~ProjectionWidget() {
	if (pProjectionView != 0) {
		delete pProjectionView;
		pProjectionView = 0;
	}
}

void ProjectionWidget::render() {
	update();
}

void
ProjectionWidget::initializeGL() {
}

void
ProjectionWidget::resizeGL( int width, int height ) {
	setupViewport(width, height);

}

void
ProjectionWidget::paintGL() {
	makeCurrent();
	qglClearColor(Qt::white);
	glDisable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupViewport(width(), height());

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();                           // Reset The Modelview Matrix

	emit updateProjectionRequest(pProjectionView);

	if (selectingRegion) {
		drawSelectionHull();
	}
}

void
ProjectionWidget::mousePressEvent(QMouseEvent* event) {
		switch(event->button()) {
		case Qt::LeftButton:
		{
			{ // begin region selection
				selectingRegion = true;
				QPoint currPos = screenToModel(event->pos());
				hull.init(currPos);
			} // end region selection
		}
		break;

//		case Qt::RightButton:
//			changingTransform = true;
//			prevPos = event->pos();
//			break;

		default:
			break;// do nothing
		}
//	}
}
void
ProjectionWidget::mouseReleaseEvent(QMouseEvent *event) {
//	switch(event->button()) {
//		case Qt::RightButton:
//			changingTransform = false;
//			break;
//		default:
//			break;
//	}

	if (event->button() == Qt::RightButton) {
		{
		// reset region selection
			std::vector<std::vector<QPointF>> projectionData;
			emit getProjectionDataRequest(projectionData);

			if (projectionData.size() > 0) {
				unsigned ny = projectionData.size();
				unsigned nx = projectionData[0].size();

				std::vector< std::vector<bool> > selectionMask;
				selectionMask.resize(ny, std::vector<bool>(nx, false));
				unsigned numSelected = 0;

				emit newSelectionInView(selectionMask, numSelected);
			}
		} // end region selection
	}

	if (event->button() == Qt::LeftButton)
	{
		selectingRegion = false;
		QPoint currPos = screenToModel(event->pos());

		if (hull.hull.size() == 1) { //single click
			{ //select a point
				QPointF point;
				if (findClosestPointCoordinates(currPos, point)) {
					switch (event->modifiers()) {
						case Qt::ShiftModifier: {
							emit selectRegionAtPoint(point, false);
						}
						break;

						case Qt::ControlModifier: {
							emit selectRegionAtPoint(point, true);
						}
						break;

						default: {
							emit selectPoint(point);
						}
						break;
					}
				}
			} //end select a point
		}
		else {
			// begin region selection
			std::vector<std::vector<QPointF>> projectionData;
			emit getProjectionDataRequest(projectionData);

			if (projectionData.size() > 0) {
				unsigned ny = projectionData.size();
				unsigned nx = projectionData[0].size();

				std::vector< std::vector<bool> > selectionMask;
				selectionMask.resize(ny, std::vector<bool>(nx, false));
				unsigned numSelected = 0;

				for(unsigned i=0;i<ny;i++) {
					for (unsigned j=0; j<nx; j++) {
						QPointF qqf = pProjectionView->dataToModel(projectionData[i][j]);
						QPoint qq{(int)qqf.x(), (int)qqf.y()};

						if(hull.point_belong_hull(qq)) {
							selectionMask[i][j] = true;
							numSelected++;
						}
					}
				}

				emit newSelectionInView(selectionMask, numSelected);
			}
		} // end region selection
	}
}
void
ProjectionWidget::mouseMoveEvent(QMouseEvent *event)
  {
	QPoint currPos = screenToModel(event->pos());
	if (selectingRegion)
	{ // begin region selection
		hull.addPoint(currPos);
		render();
	} // end region selection
	else {
		if (event->button() == Qt::NoButton) {
			QPointF coordinates;
			if (findClosestPointCoordinates(currPos, coordinates)) {
				bool bOK = false;
				float value = 0.0;
				emit getPointValue(coordinates, &value, &bOK);

				 QString message;
				 message = CoordinateText::convertPoint(coordinates);
				 if (bOK) {
					 message.append( QString(", value=%1").arg(QString::number(value, 'f', 2)) );
				 }
				QToolTip::showText( mapToGlobal(event->pos()), message );
			}
		}
	}

//	QPoint mousePosScreen = event->pos();
//	if (changingTransform) {
//		if (QApplication::keyboardModifiers() & Qt::CTRL) {
//			//std::cout << "ctrl is pressed " << std::endl;
//			QPoint prevPosModel = screenToModel(prevPos);
//			QPoint currPosModel = screenToModel(mousePosScreen);
//			prevPos = mousePosScreen;
//
//			float oldAngle = atan2(prevPosModel.y(), prevPosModel.x());
//			float newAngle = atan2(currPosModel.y(), currPosModel.x());
//			currentTransform =
//					TransformMatrix2D::Rotation(newAngle-oldAngle) *
//					currentTransform;
//			transformDefault = false;
//			updateGL();
//		}
//		else {
//			float trAddScreenX = mousePosScreen.x() - prevPos.x();
//			float trAddScreenY = mousePosScreen.y() - prevPos.y();
//			prevPos = mousePosScreen;
//
//			currentTransform =
//					TransformMatrix2D::Translation(trAddScreenX, trAddScreenY) *
//					currentTransform;
//			transformDefault = false;
//			updateGL();
//		}
//	}
//	else {
//		showTooltip(mousePosScreen);
//	}
}

void
ProjectionWidget::wheelEvent(QWheelEvent *event) {

	QPoint mousePos = event->pos();
	QPoint pt = screenToModel(mousePos);

	{ // begin zoom
		int ticks = event->delta()/120;
		float scaleFactor = 1.1;
		float addScale = 1.0f;
		if (ticks < 0) {
			scaleFactor = 1/1.1;
			ticks = -ticks;
		}
		for (int i=0; i< ticks; i++) {
			addScale *= scaleFactor;
		}

		pProjectionView->zoom(addScale, pt);
	} // end zoom
	render();
}

void ProjectionWidget::setupViewport(int width, int height) {
	float halfWidth = width/2.0f;
	float halfHeight = height/2.0f;

	pProjectionView->resizeView(width, height);

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width * retinaScale, height * retinaScale);
	//glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);                // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix

	glOrtho(-halfWidth, halfWidth, halfHeight, -halfHeight, 0, 1);


	glMatrixMode(GL_MODELVIEW);                 // Select The Modelview Matrix
}

QPoint
ProjectionWidget::screenToModel(QPoint pos) {
	return QPoint(pos.x() - width()/2, pos.y() - height()/2 );
}

bool
ProjectionWidget::findClosestPointCoordinates(QPoint modelPos, QPointF& outCoordinates) {
	bool bFound = false;
	QPoint ptIndices(-1,-1);
	std::vector<std::vector<QPointF>> projectionData;
	emit getProjectionDataRequest(projectionData);
	if (pProjectionView->findClosestPointIndices(projectionData, modelPos, ptIndices)) {
		MapGrid grid;
		emit getGridRequest(grid);
		outCoordinates = QPointF( grid.lons[ptIndices.x()], grid.lats[ptIndices.y()] );
		bFound = true;
	}
	return bFound;
}

void
ProjectionWidget::drawSelectionHull() {
	QColor lineColor = Qt::blue;
	qglColor(lineColor);

	glBegin(GL_LINE_STRIP);
		const std::vector<QPoint>& selHull = hull.hull;
		assert (selHull.size()>0);

		for (unsigned i=0; i< selHull.size(); i++) {
			glVertex2i(selHull[i].x(), selHull[i].y());
		}
		glVertex2i(selHull[0].x(), selHull[0].y());
	glEnd();
}

} /* namespace VCGL */
