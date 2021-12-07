/*!
* @file transferfunctioneditorwidget.h
* @author Vladimir Molchanov (original code)
* @author Anatoliy Antonov (refactoring)
*
* @brief Definition of the class of the widget for editing transfer functions
*/

#ifndef TRANSFERFUNCTIONEDITORWIDGET_H
#define TRANSFERFUNCTIONEDITORWIDGET_H

#include <vector>
#include <iostream>
#include <cstdio>
#include <cmath>

#include <QtOpenGL/QGLWidget>
#include "multiplatform/QApplication"
#include <QtGui/QtGui>
#include "transferfunctionobject.h"

#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QColorDialog>
	#include <QFileDialog>
	#include <QMenu>
	#include <QAction>
#endif // QT_VERSION >= 0x050000

namespace VCGL {
	struct IColorizer;
	class TransferFunctionEditor;
}


/// Rendering and manipulating a transfer function. Only colors are handled (transparency is 1).
class TransferFunctionEditorWidget : public QGLWidget
{
    Q_OBJECT

public:
    /// Construct with logic object
    TransferFunctionEditorWidget(VCGL::TransferFunctionEditor& editor, QWidget *parent = 0);
    /// Destructor
    ~TransferFunctionEditorWidget();

    /// Initialize the widget
    void initialize();

    /// Set range minimum and maximum for the legend
    void setColorDataRange(float colorDataMin = 0.0f, float colorDataMax = 1.0);

    ///	Reset node positions
    void recompute_positions();

protected:
signals:
	/// Signal that the transfer function has been modified
    void signal_slider_moved();

protected:
	/// Initialize GL engine
    void initializeGL() override;
    /// Draw widget contents
    void paintGL() override;
	/// Reinit GL engine to fit the current size of the widget
    void resizeGL(int width, int height) override;

/// @name Mouse events handlers
//@{
    void mousePressEvent(QMouseEvent *event) override; ///< Called when the user presses a mouse button
    /*! @brief Called when the user moves the mouse
	 *
	 * @attention setMouseTracking(true) in the constructor allows to receive the event when no mouse buttons are pressed
	 */
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override; ///< Called when the user releases a mouse button
//@}

private slots:
	/// Show the context menu
    void slot_showContextMenu(const QPoint& pos);
    /// Save the current transfer function visualization
    void slot_screenshot();
    /// Provide a temporary color for previewing the edit
    void slot_previewColorForCurrentNode(const QColor& newColor);
    /*! @brief Notification that the color modification is complete
     * @param result Either QDialog::Accepted or QDialog::Rejected
     */
    void slot_colorDialogFinished(int result);

private:
    QColor getQColor(const float dataValue) const;    ///< get color for the specified value from [0,1] (0==range min, 1==range max)
    void paintBackground(); ///< visualize the transfer function in the widget
    void remove_node(const unsigned int i); ///< remove the specified node from the selected transfer function

    int windowHeight;   ///< height of widget
    int windowWidth;    ///< width of widget
    int borderoff;      ///< defines size of the border

    bool bShowLine;     ///< show line with nodes

    bool bFocusLinear;                   ///< scaling when zooming
    double focusMin, focusMax;           ///< zooming limits in values
    int positionFocusMin, positionFocusMax; ///< zooming limits in pixels

    std::vector<int> nodesPositions;     ///< positions of nodes
    int selectedNode;                    ///< selected point
    short mouseButton;                   ///< pressed mouse button flag

	QPoint lastPos;     ///< recorded mouse position
	bool bPointClicked; ///< if a node is clicked

	int currentColoredNode; 			///< index of a node for which color is modified
	QColor oldColor; ///< initial color of the node prior to opening the color dialog
	QColorDialog * pColorDialog; 			///< Pointer to the color dialog

	VCGL::TransferFunctionEditor& tfEditor;	///< object encapsulating the main logic of the widget
	float rangeMin;							///< minimum of the range (for the legend)
	float rangeMax;							///< maximum of the range (for the legend)
};

#endif // TRANSFERFUNCTIONEDITORWIDGET_H
