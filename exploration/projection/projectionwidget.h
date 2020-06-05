/*! @file projectionwidget.h
 * @author anantonov
 * @date Created on Jun 22, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef PROJECTIONWIDGET_H_
#define PROJECTIONWIDGET_H_

#include <qgl.h>
#include "multiplatform/declareqmouseevent.h"
#include <vector>
#include "selectionhull.h"

namespace VCGL {
class ProjectionView;
struct MapGrid;

class ProjectionWidget: public QGLWidget {
	Q_OBJECT
public:
	ProjectionWidget(QWidget* parent);
	virtual ~ProjectionWidget();
	void render();

signals:
	void getGridRequest(VCGL::MapGrid& grid);
	void getProjectionDataRequest(std::vector< std::vector<QPointF> >& projectionData);

	void updateProjectionRequest(VCGL::ProjectionView* pProjectionView);
	void getPointValue(const QPointF& point, float* pValue, bool* pbOK);
	void selectPoint(const QPointF& point);
	void selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent);
	void newSelectionInView(const std::vector<std::vector<bool> >& selectionMask, unsigned numSelected);
protected:
	void initializeGL() override;
	void resizeGL( int width, int height ) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
private:
	void setupViewport( int width, int height );
	QPoint screenToModel(QPoint pos);
	bool findClosestPointCoordinates(QPoint modelPos, QPointF& outCoordinates);
	void drawSelectionHull();

	ProjectionView* pProjectionView;

	bool selectingRegion;
	SelectionHull hull;
};

} /* namespace VCGL */

#endif /* PROJECTIONWIDGET_H_ */
