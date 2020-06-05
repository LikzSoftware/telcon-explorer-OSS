/*!	@file maplayout.cpp
 *	@author anantonov
 *	@date	Nov 2, 2013 (created)
 *	@brief	Implementation of the widget for managing a map layout
 */

//#include <GL/gl.h>
//#include <GL/glu.h>

#include "maplayoutview.h"
#include "layout.h"
#include "mapsubview.h"
#include "equirectangularmapsubview.h"
#include "polarmapsubview.h"
#include "legendsubview.h"
#include "mapgrid.h"
#include "exploration/coordinatetext.h"

#include "multiplatform/devicepixelratio.h"

#include <QtGui>
#include <QtOpenGL>
#include <QMouseEvent>
#include <QToolTip>
#include <iostream>

#include <cassert>

namespace VCGL {

MapLayoutView::MapLayoutView(QWidget* parent)
: QGLWidget(parent), pMap(0), pLegend(0), pCurrentPainter(0) {
	pMap = new VCGL::PolarMapSubview();
	pLegend = new LegendSubview();

	setAutoFillBackground(false);
	setMinimumSize(200, 200);
	setMouseTracking(true); // allows to show tooltip when no mouse button pressed
}

MapLayoutView::~MapLayoutView() {
	if (pMap != 0) {
		delete pMap;
		pMap = 0;
	}
	if (pLegend != 0) {
		delete pLegend;
		pLegend = 0;
	}
}

void MapLayoutView::render() { update(); }

void MapLayoutView::selectRectilinearMap() {
	if (pMap != 0) {
		delete pMap;
		pMap = 0;
	}
	pMap = new VCGL::EquirectangularMapSubview();
	//pMap->setArea(layoutData.mapArea);
	const qreal retinaScale = devicePixelRatio();
	resizeGL(width()*retinaScale, height()*retinaScale);
}

void MapLayoutView::selectPolarMap() {
	if (pMap != 0) {
		delete pMap;
		pMap = 0;
	}
	pMap = new VCGL::PolarMapSubview();
	//pMap->setArea(layoutData.mapArea);
	const qreal retinaScale = devicePixelRatio();
	resizeGL(width()*retinaScale, height()*retinaScale);
}

void MapLayoutView::initializeGL() {

}

void MapLayoutView::resizeGL( int width, int height ) {
	setupViewport(width, height);

	Layout layout;
	layout.setMargins(10,5);
	const qreal retinaScale = devicePixelRatio();
	layout.computeLayout(width/retinaScale, height/retinaScale, &layoutData);
	pMap->setArea(layoutData.mapArea);
	pLegend->setArea(layoutData.legendArea);

	QFontMetrics metrics = QFontMetrics(font());

	QString sampleLabelMap = "289.75 Z";
	QRect rectMap = metrics.boundingRect(0, 0, 0, 0, Qt::AlignLeft | Qt::AlignTop, sampleLabelMap);
	pMap->setLabelsSize(rectMap.width(), rectMap.height());

	QString sampleLabelLegend = "-0.69";
	QRect rectLegend = metrics.boundingRect(0, 0, 0, 0, Qt::AlignLeft | Qt::AlignTop, sampleLabelLegend);
	pLegend->setLabelsSize(rectLegend.width(), rectLegend.height());
}

void MapLayoutView::paintEvent(QPaintEvent* /*event*/) {
	 makeCurrent();
	 glMatrixMode(GL_MODELVIEW);
	 glPushMatrix();

	 qglClearColor(Qt::white);

	glDisable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupViewport(width(), height());

	 glClear(GL_COLOR_BUFFER_BIT);
	 glLoadIdentity();

	rasterize(layoutData.mapArea, Qt::white);
	rasterize(layoutData.legendArea, Qt::white);

	emit updateMapRequest(pMap);
	emit updateLegendRequest(pLegend);

	 glMatrixMode(GL_MODELVIEW);
	 glPopMatrix();

	 assert(pCurrentPainter == 0);
	 pCurrentPainter = new QPainter(this);

	 emit updateMapTextRequest(pMap, this);
	 emit updateLegendTextRequest(pLegend, this);

	 pCurrentPainter->end();
	 delete pCurrentPainter;
	 pCurrentPainter = 0;
}

void MapLayoutView::rasterize(const MRect& area, QColor color) {
	qglColor(Qt::black);
	glLineWidth(1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(area.left-0.5, area.bottom-0.5);
		glVertex2f(area.left-0.5, area.top+1.5);

		glVertex2f(area.right+1.5, area.top+1.5);

		glVertex2f(area.right+1.5, area.bottom-0.5);
	glEnd();

	qglColor(color);

	glBegin(GL_QUADS);
		glVertex2f(area.left, area.bottom);
		glVertex2f(area.left, area.top+1);
		glVertex2f(area.right+1, area.top+1);
		glVertex2f(area.right+1, area.bottom);
	glEnd();
}

void MapLayoutView::mousePressEvent(QMouseEvent *event)
 {
	QPoint layoutPos = screenToLayout(event->pos());
	 if (layoutPos.x() >= layoutData.mapArea.left && layoutPos.x() <= layoutData.mapArea.right &&
			 layoutPos.y() >= layoutData.mapArea.bottom && layoutPos.y() <= layoutData.mapArea.top) {

		 QPointF point;
		 MapGrid grid;
		 emit getGridRequest(grid);

		 if (pMap->getPointCoordinates(grid, layoutPos, &point)) {
			 if (event->button() == Qt::LeftButton) {
				 MouseSelectionMode msm = MSM_REFERENCE_POINT;
				 emit getSelectionModeRequest(msm);

				 switch (msm) {
				 case MSM_REGION:
					 emit selectPoint(point);
					 emit selectRegionAtPoint(point, false);
					 break;
				 case MSM_REGION_PLUS:
					 emit selectPoint(point);
					 emit selectRegionAtPoint(point, true);
					 break;
				 case MSM_REFERENCE_POINT:
					 emit selectPoint(point);
					 break;
				 default:
					 std::cerr << "MapLayoutView: unknown mouse selection mode" << std::endl;
					 emit selectPoint(point);
					 break;
				 }

//				switch (event->modifiers()) {
//					case Qt::ShiftModifier: {
//						emit selectRegionAtPoint(point, false);
//					}
//					break;

//					case Qt::ControlModifier: {
//						emit selectRegionAtPoint(point, true);
//					}
//					break;

//					default: {
//						emit selectPoint(point);
//					}
//					break;
//				}
			 }
			 if (event->button() == Qt::RightButton) {
				 emit resetRegionSelection();
			 }
		 }
	 }
 }

 void MapLayoutView::mouseMoveEvent(QMouseEvent *event)
 {
	 QPoint layoutPos = screenToLayout(event->pos());

	 if (layoutPos.x() >= layoutData.mapArea.left && layoutPos.x() <= layoutData.mapArea.right &&
			 layoutPos.y() >= layoutData.mapArea.bottom && layoutPos.y() <= layoutData.mapArea.top) {

		 QPointF point;
		 MapGrid grid;
		 emit getGridRequest(grid);

		 if (pMap->getPointCoordinates(grid, layoutPos, &point)) {
			 bool bOK = false;
			 float value = 0.0;
			 emit getPointValue(point, &value, &bOK);

			 QString message;
			 message = CoordinateText::convertPoint(point);
			 if (bOK) {
				 message.append( QString(", value=%1").arg(QString::number(value, 'f', 2)) );
			 }
			QToolTip::showText( mapToGlobal(event->pos()), message );
		 }
		 else {
			 QToolTip::hideText();
		 }
	 }
 }

void MapLayoutView::setupViewport(int width, int height) {
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width * retinaScale, height * retinaScale);
	//glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// caution: origin in the left-bottom corner of the widget
	glOrtho(0, width, 0, height, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}

QPoint MapLayoutView::screenToLayout(QPoint screenPos) {
	return QPoint(screenPos.x(), height()-1-screenPos.y());
}

void MapLayoutView::drawText(int x, int y, int flags, const QString& text) {
	if (pCurrentPainter != 0) {
		QFontMetrics metrics = QFontMetrics(font());
		QRect rect = metrics.boundingRect(x, (height()-1)-y, 0, 0, flags, text);
		pCurrentPainter->setRenderHint(QPainter::TextAntialiasing);
		pCurrentPainter->setPen(Qt::black);

		pCurrentPainter->drawText(rect,flags, text);
	}
}

} // namespace VCGL
