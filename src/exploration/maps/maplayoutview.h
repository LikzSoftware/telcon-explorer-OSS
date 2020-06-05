/*!	@file maplayout.h
 *	@author anantonov
 *	@date	Nov 2, 2013 (created)
 *	@brief	Widget for managing a map layout combining the data with the legend
 */

#ifndef MAPLAYOUT_H_
#define MAPLAYOUT_H_

#include <QGLWidget>

#include "mrect.h"
#include "layout.h"
#include "textpainter.h"
#include "exploration/mouseselectionmode.h"

class QPaintEvent;
class QPainter;

namespace VCGL {
	class MapSubview;
	class LegendSubview;
	struct MapGrid;

/// Widget for managing a map layout combining the data with the legend
class MapLayoutView: public QGLWidget, public TextPainter {
	  Q_OBJECT

public:
	/// Default constructor
	MapLayoutView(QWidget *parent = 0);
	/// Destructor
	~MapLayoutView();

	/// Request redraw of the view
	void render();

	/// Select rectilinear map projection for the map data visualization
	void selectRectilinearMap();

	/// Select polar map projection for the map data visualization
	void selectPolarMap();

signals:
	/// Request the data reference coordinate grid
	void getGridRequest(VCGL::MapGrid& grid);

	/// Request the mouse selection mode
	void getSelectionModeRequest(MouseSelectionMode& msm);

	/// Request the data value at the given coordinate
	void getPointValue(const QPointF& point, float* pValue, bool* pbOK);

	/// Request selection of the point at the given coordinates
	void selectPoint(const QPointF& point);

	/*! @brief Request selection of a region starting from the given point
	 *
	 * @param point					Point inside a region to select
	 * @param bSelectWholeComponent	true to select all connected component,
	 * 								false to select only the enclosing region
	 */
	void selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent);

	/// Request reset of the region selection
	void resetRegionSelection();

	/// Signal that the map needs to be updated (drawn at the signal)
	void updateMapRequest(VCGL::MapSubview* pMap);
	/// Signal that the legends needs to be updated  (drawn at the signal)
	void updateLegendRequest(VCGL::LegendSubview* pLegend);

	/// Signal that the map text needs to be updated (rendered at the signal)
	void updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter);

	/// Signal that the legend text needs to be updated (rendered at the signal)
	void updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter);
protected:
	/// Initialize GL engine
	void initializeGL() override;
	/// Reinit GL engine to fit the current size of the widget
	void resizeGL( int width, int height ) override;
	/// Draw widget contents
	void paintEvent(QPaintEvent *event) override;
	/// Fill the specified area with the given color
	void rasterize(const MRect& area, QColor color);


	void mousePressEvent(QMouseEvent *event) override; ///< Called when the user presses a mouse button
	/*! @brief Called when the user moves the mouse
	 *
	 * @attention setMouseTracking(true) in the constructor allows to receive the event when no mouse buttons are pressed
	 */
	void mouseMoveEvent(QMouseEvent *event) override;
private:
	/// Initialize GL engine viewport parameters
	void setupViewport(int width, int height);

	/*! @brief Render the text string (@see TextPainter)
	 *
	 * @param x		Horizontal coordinate for the anchor point
	 * @param y 	Vertical coordinate for the anchor point
	 * @param flags Flags for the rendering (@see QPainter::drawText)
	 * @param text	Text string to be rendered
	 */
	void drawText(int x, int y, int flags, const QString& text) override;

	/// Convert a point from the "screen" coordinates (e.g. mouse) to the layout pixel coordinates
	QPoint screenToLayout(QPoint screenPos);

	MapSubview* pMap;		///< map subview pointer
	LegendSubview* pLegend;	///< legend subview pointer

	MapLayoutData layoutData;	///< description of the widget layout
	QPainter* pCurrentPainter;	///< QPainter pointer for this widget (mainly for rendering text)
};

} //namespace VCGL

#endif // MAPLAYOUT_H_
