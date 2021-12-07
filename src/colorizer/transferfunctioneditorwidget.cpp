/*!
* @file transferfunctioneditorwidget.cpp
* @author Vladimir Molchanov (original code)
* @author Anatoliy Antonov (refactoring, commenting)
*
* @brief Implementation of the widget for editing transfer functions
*/

#include "transferfunctioneditorwidget.h"
#include "transferfunctioneditor.h"
#include "texture.h"
#include "multiplatform/devicepixelratio.h"

#include <QToolTip>

#include <cassert>
#include <cmath>

#define NO_BUTTON 0
#define RIGHT_BUTTON 1
#define LEFT_BUTTON 2

using namespace std;

TransferFunctionEditorWidget::TransferFunctionEditorWidget(VCGL::TransferFunctionEditor& editor, QWidget* parent)
:QGLWidget(parent),
 windowHeight(0),
 windowWidth(0),
 borderoff(0),
 bShowLine(false),
 bFocusLinear(false),
 focusMin(0),
 focusMax(0),
 positionFocusMin(0),
 positionFocusMax(0),
 selectedNode(-1),
 mouseButton(NO_BUTTON),
 bPointClicked(false),
 currentColoredNode(-1),
 pColorDialog(0),
 tfEditor(editor),
 rangeMin(0.0),
 rangeMax(1.0){

	setMouseTracking(true); // allows to show tooltip when no mouse button pressed
}

TransferFunctionEditorWidget::~TransferFunctionEditorWidget() {
	if (pColorDialog != 0) {
		pColorDialog->setAttribute(Qt::WA_DeleteOnClose);

		// either closes and deletes directly,
		// or rejects closing and will be deleted when closed
		pColorDialog->close();
		pColorDialog = 0;
	}
}

void
TransferFunctionEditorWidget::initialize() {
    mouseButton = NO_BUTTON;

    bPointClicked = false;
    selectedNode = -1;
    borderoff = 0;

    windowWidth = width();
    windowHeight = height();

    bFocusLinear = true;
    focusMin = 0.0;
    focusMax = 100.0;
    positionFocusMin = borderoff;
    positionFocusMax = windowHeight-borderoff;

    bShowLine = true;

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    recompute_positions();
    update();
}

void
TransferFunctionEditorWidget::setColorDataRange(float colorDataMin, float colorDataMax) {
	rangeMin = colorDataMin;
	rangeMax = colorDataMax;
}


QColor
TransferFunctionEditorWidget::getQColor(const float dataValue) const {
	QColor qclr;
	float r=0, g=0, b=0; //TODO: add alpha channel?
	tfEditor.getCurrentTF()->colorize(dataValue, &r, &g, &b);
	qclr.setRgbF(r, g, b, 1.0);
	return (qclr);
}

void
TransferFunctionEditorWidget::initializeGL() {
    glClearColor(0.75,0.75,0.75,0.75);
}


void
TransferFunctionEditorWidget::paintGL() {

	glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
	
    if ( tfEditor.getTFCount() == 0 ) {
    	return;
    }

    if (tfEditor.getCurrentTF()->getRecordsCount() == 0) {
    	return;
    }

	const qreal retinaScale = devicePixelRatio();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width()*retinaScale,0,height()*retinaScale,-1,1);


//    //Background
	paintBackground();


	if(bShowLine)
	{
		const qreal retinaScale = devicePixelRatio();
		float middle = windowWidth / 2 * retinaScale;

		glPointSize(5);
		float r=0, g=0, b=0;
		tfEditor.getCurrentTF()->getRecordColor(0, &r, &g, &b);
		glColor3f(1.0-r, 1.0-g, 1.0-b);

		size_t numNodes = nodesPositions.size();
		if (numNodes > 0) {
			glBegin(GL_LINES);
				glVertex2f(middle, nodesPositions[0]*retinaScale);
				glVertex2f(middle, nodesPositions[numNodes-1]*retinaScale);
			glEnd();
		}

		glBegin(GL_POINTS);
			for(unsigned int i=0;i<numNodes;i++) {
				glVertex2f(middle, nodesPositions[i]*retinaScale);
			}
		glEnd();

		glPointSize(10);
		glColor3f(1.0,1.0,1.0);
		if(selectedNode!=-1) {
			glBegin(GL_POINTS);
				glVertex2f(middle, nodesPositions[selectedNode]*retinaScale);
			glEnd();
		}
	}

    glPopMatrix();
	glFlush();
}

void
TransferFunctionEditorWidget::resizeGL(int w, int h) {
    if(w==0 || h==0) {
    	return;
    }

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, w*retinaScale, h*retinaScale);


	windowWidth = width();
	windowHeight = height();

    recompute_positions();
}


void
TransferFunctionEditorWidget::mousePressEvent(QMouseEvent *event) {
    const float ACCEPTED_ERROR = 5;
    lastPos = event->pos();
    int nowpos = windowHeight - lastPos.y();

    if(event->button()==Qt::LeftButton && bShowLine) {
		mouseButton = LEFT_BUTTON;
		bool bFoundPoint = false;
		unsigned ptIndex = 0;

		for(unsigned i=0;i<nodesPositions.size();i++) {
			if(abs(nodesPositions[i] - nowpos) < ACCEPTED_ERROR && abs(lastPos.x() - windowWidth/2) < ACCEPTED_ERROR) {
				bFoundPoint = true;
				ptIndex = i;
			}
       }
       if (bFoundPoint) {
    	   //warning: selectedNode is reset when the user releases the mouse button
    	   //and this happens while QColorDialog is still executing - after that, selectedNode equals -1!!!
			selectedNode = ptIndex;
            std::cout << "Selected node:" << selectedNode << std::endl;

			bPointClicked = true;
			if(QApplication::keyboardModifiers() == Qt::ShiftModifier) {
				currentColoredNode = ptIndex;

				float r=0, g=0, b=0;
				tfEditor.getCurrentTF()->getRecordColor(ptIndex, &r, &g, &b);

              	oldColor.setRgbF(r,g,b,1.0);
              	if (pColorDialog == 0) {
              		pColorDialog = new QColorDialog(this);

            		connect(pColorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(slot_previewColorForCurrentNode(const QColor&)));
					connect(pColorDialog, SIGNAL(finished(int)), this, SLOT(slot_colorDialogFinished(int)));
              	}
              	pColorDialog->setCurrentColor(oldColor);
              	pColorDialog->show();
			}
       }
    }
   else if(event->button()==Qt::RightButton) {
		mouseButton = RIGHT_BUTTON;

		if(QApplication::keyboardModifiers() == Qt::ShiftModifier) {
			slot_showContextMenu(lastPos);
		}
		else {
			if(bShowLine) {
				bool bFoundNode = false;
				unsigned foundNodeIndex = 0;
				for(unsigned int i=1;i<nodesPositions.size()-1;i++) //end-nodes can not be removed
				if(abs(nodesPositions[i] - nowpos) < ACCEPTED_ERROR && abs(lastPos.x() - windowWidth/2) < ACCEPTED_ERROR) {
					bFoundNode = true;
					foundNodeIndex = i;
				}
				if (bFoundNode) {
					remove_node(foundNodeIndex);
				}
				else {
					int ind = -1;
					while ( (ind<((signed)nodesPositions.size()-1) ) && nodesPositions[ind+1]<nowpos) {
						ind++;
					}

					VCGL::TFRecord rec;
					rec.pos = static_cast<float>(nowpos - borderoff) / (windowHeight-2*borderoff);
					rec.qclr = getQColor(rec.pos);

					tfEditor.getCurrentTF()->insertTFRecord(ind+1, rec);
					recompute_positions();

					bPointClicked = true;
					selectedNode = ind+1;
					update();
					emit signal_slider_moved();
				}
			}
        }
    }
}


void
TransferFunctionEditorWidget::mouseMoveEvent(QMouseEvent *event) {
    int nowpos = windowHeight - event->pos().y();

    if (mouseButton == LEFT_BUTTON) {
        if(bPointClicked && selectedNode>0 && selectedNode<(signed)nodesPositions.size()-1) { //if a point was selected
            bool ok = true;
            if(!bFocusLinear) { //crossing zoom boundaries is not allowed
                ok = (bool) (abs(nowpos-positionFocusMin)>1 && abs(nowpos-positionFocusMax)>1 &&
                            (nowpos - positionFocusMin)*(nodesPositions[selectedNode] - positionFocusMin)>0 &&
                            (nowpos - positionFocusMax)*(nodesPositions[selectedNode] - positionFocusMax)>0);
            }

            if(ok && nowpos>nodesPositions[selectedNode-1] && nowpos<nodesPositions[selectedNode+1]) {

                nodesPositions[selectedNode] = nowpos;

                double newTFRPos = 0;
                if(bFocusLinear) {
                    newTFRPos = (double)(nodesPositions[selectedNode] - borderoff)/(windowHeight-4*borderoff);
                }
                else {
                    if(nodesPositions[selectedNode]<positionFocusMin) {
                    	newTFRPos = focusMin/100.0 - (double)(positionFocusMin-nodesPositions[selectedNode])/(positionFocusMin-borderoff)*focusMin/100.0;
                    }
                    else if(nodesPositions[selectedNode]>positionFocusMax) {
                    	newTFRPos = focusMax/100.0 + (double)(nodesPositions[selectedNode]-positionFocusMax)/(windowHeight-3*borderoff - positionFocusMax)*(1.0 - focusMax/100.0);
                    }
                    else {
                    	newTFRPos = focusMin/100.0 + (double)(nodesPositions[selectedNode]-positionFocusMin)/(positionFocusMax-positionFocusMin)*(focusMax-focusMin)/100.0;
                    }
                }
                tfEditor.getCurrentTF()->setRecordPos(selectedNode, newTFRPos);
            }
        }

        update();
        emit signal_slider_moved();
    }

    if (bFocusLinear && nowpos > borderoff && nowpos < (windowHeight-borderoff)) {
		const float sliderPos = static_cast<double>(nowpos - borderoff)/(windowHeight-4*borderoff);
		float value = sliderPos*(rangeMax-rangeMin) + rangeMin;
		QString message;
		message = QString::number(value, 'f', 3);
			/*message = QString("(%1,%2): value = %3").arg(
					QString::number( lon ),
					QString::number( lat ),
					QString::number( colorData[lat*nlon+lon] ));*/
		QToolTip::showText( mapToGlobal(event->pos()), message );
    }
}


void
TransferFunctionEditorWidget::mouseReleaseEvent(QMouseEvent * /*event*/) {
    mouseButton = NO_BUTTON;
    selectedNode = -1;
    bPointClicked = false;
}


void
TransferFunctionEditorWidget::remove_node(const unsigned int i) {
    if(nodesPositions.size()<3) {
    	return; //at least two nodes remain
    }

    for(unsigned int j=i; j<nodesPositions.size()-1; j++) {
        nodesPositions[j] = nodesPositions[j+1];
    }
    nodesPositions.pop_back();

    tfEditor.getCurrentTF()->removeTFRecord(i);
    update();
    emit signal_slider_moved();
}


void
TransferFunctionEditorWidget::recompute_positions()
{
	assert(tfEditor.getTFCount()>0);

    nodesPositions.resize(tfEditor.getCurrentTF()->getRecordsCount(),0);
    if(bFocusLinear) {
        for(unsigned short i=0; i<nodesPositions.size(); i++) {
            nodesPositions[i] = borderoff + (windowHeight-2*borderoff)*tfEditor.getCurrentTF()->getRecordPos(i);
        }
    }
}


void
TransferFunctionEditorWidget::slot_screenshot() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"),
                                                    QDir::currentPath() + "/data/transfer_function",
                                                    tr("Images (*.png *.xpm *.jpg *.bmp)"));
    if(fileName.isEmpty()) {
    	return;
    }
    QImage image = grabFrameBuffer(false);
    image.save(fileName,"PNG");
}


void
TransferFunctionEditorWidget::slot_showContextMenu(const QPoint& pos) {
    // for most widgets
    QPoint globalPos = mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    QAction actionScreenshot("Screenshot",this);
    myMenu.addAction(&actionScreenshot);
    QAction actionShowLine("Show line",this);
    actionShowLine.setCheckable(true);
    actionShowLine.setChecked(bShowLine);
    myMenu.addAction(&actionShowLine);
    QAction actionZoomOut("Zoom out",this);
    if(!bFocusLinear) {
    	myMenu.addAction(&actionZoomOut);
    }

    QAction* selectedItem = myMenu.exec(globalPos);
    if(selectedItem) {
        if(selectedItem == &actionScreenshot) {
        	slot_screenshot();
        }
        else if(selectedItem == &actionShowLine) {
        	bShowLine = !bShowLine;
        	update();
        }
        else if(selectedItem == &actionZoomOut) {
        	bFocusLinear = true;
        	update();
        }
    }
    else {
    	return;
    }
}

void
TransferFunctionEditorWidget::slot_previewColorForCurrentNode(const QColor& newColor) {
	if (currentColoredNode >=0 && currentColoredNode < (int)(tfEditor.getCurrentTF()->getRecordsCount()) ) {
		tfEditor.getCurrentTF()->setRecordColor(currentColoredNode, newColor.redF(), newColor.greenF(), newColor.blueF());
		update();
		emit signal_slider_moved();
	}
}


void TransferFunctionEditorWidget::slot_colorDialogFinished(int result) {
	if (currentColoredNode >=0 && currentColoredNode < (int)(tfEditor.getCurrentTF()->getRecordsCount()) ) {
		QColor color = Qt::black;

		switch (result) {
		case QDialog::Accepted:
			color = pColorDialog->currentColor();
			break;
		case QDialog::Rejected:
			color = oldColor;
			break;
		default:
			//do nothing
			break;
		}

		if (result == QDialog::Accepted || result == QDialog::Rejected) {
			tfEditor.getCurrentTF()->setRecordColor(currentColoredNode, color.redF(), color.greenF(), color.blueF());
			update();
			emit signal_slider_moved();
		}
	}

	currentColoredNode = -1;
}

void TransferFunctionEditorWidget::paintBackground() {

	const qreal retinaScale = devicePixelRatio();
	int width = 1;
	int height = (windowHeight-2*borderoff) * retinaScale;

	VCGL::Texture tex;
	if (width > 0 && height > 0) {
		tex.allocateMemory(width, height);

		for (int j=0; j<height; j++) {
			float r = 0.0f, g = 0.0f, b = 0.0f;
			tfEditor.getCurrentTF()->colorize(j/(float)height, &r, &g, &b);
			for (int i=0; i<width; i++) {
				tex.setPixel(i,j,r,g,b);
			}
		}
	}

	GLboolean enabled = glIsEnabled(GL_TEXTURE_2D);
	if (!enabled) {
		glEnable(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, tex.getWidth(), tex.getHeight(),
				  0, GL_RGB, GL_FLOAT, tex.getBits());

	glColor3f(1.0f, 1.0f, 1.0f);

	int top = (windowHeight-borderoff) * retinaScale;
	int left = borderoff * retinaScale;
	int right = (windowWidth-borderoff) * retinaScale;
	int bottom = borderoff * retinaScale;

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(left, bottom);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(right, bottom);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(right, top);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(left, top);
	glEnd();

	if (!enabled) {
		glDisable(GL_TEXTURE_2D);
	}

}
