/*!
* @file transferfunctionwidget.cpp
* @author Vladimir Molchanov (original code)
* @author Anatoliy Antonov (refactoring, commenting)
*
* @brief Implementation of the widget for editing a collection of transfer functions
*/

#include "transferfunctionwidget.h"
#include "transferfunctioneditorwidget.h"
#include "transferfunctioneditor.h"

#include "icolorizer.h"

#include <iostream>
#include <cassert>

using namespace std;

TransferFunctionWidget::TransferFunctionWidget(QWidget *parent) :
    QWidget(parent), pTFEditorWidget(0), pEditor(0), bEditorOwnership(true) {
    ui.setupUi(this);
}

TransferFunctionWidget::~TransferFunctionWidget() {
	releaseEditor();
}

void TransferFunctionWidget::initialize(const std::vector<VCGL::TransferFunctionObject>& transferFunctionCollection)
{
	attachEditor(new VCGL::TransferFunctionEditor(), true);
	pEditor->setCollection(transferFunctionCollection);
	initWithEditor();
}

void TransferFunctionWidget::initialize(VCGL::TransferFunctionEditor* pTransferFunctionEditor, bool bTakeOwnership) {
	attachEditor(pTransferFunctionEditor, bTakeOwnership);
	initWithEditor();
}

void
TransferFunctionWidget::setColorDataRange(float colorDataMin, float colorDataMax) {
	pTFEditorWidget->setColorDataRange(colorDataMin, colorDataMax);
}

QColor TransferFunctionWidget::getColor(const float dataValue) const
{
	QColor qclr;
	float r=0, g=0, b=0; //TODO: add alpha channel?
	pEditor->getCurrentTF()->colorize(dataValue, &r, &g, &b);
	qclr.setRgbF(r, g, b, 1.0);
	return (qclr);
}

const VCGL::IColorizer* TransferFunctionWidget::getColorizer() const {
	return pEditor->getCurrentTF();
}

VCGL::IColorizer* TransferFunctionWidget::cloneColorizer() const {
	return new VCGL::TransferFunctionObject(*(pEditor->getCurrentTF()));
}

unsigned TransferFunctionWidget::getTFCount() {
	return pEditor->getTFCount();
}
void TransferFunctionWidget::setCurrentTF(unsigned tfIndex) {
	assert (tfIndex < getTFCount());
	ui.comboBox_colorScheme->setCurrentIndex(tfIndex);
	//pTFEditorWidget->setSelectedScheme(tfIndex);
}

void TransferFunctionWidget::addIcons()
{
	unsigned currentTFIndex = pEditor->getCurrentTFIndex();

    ui.comboBox_colorScheme->clear();
    const int IMAGE_WIDTH = 50;
    const int IMAGE_HEIGHT = 40;

    for(unsigned i=0; i<getTFCount(); i++) {
        QImage img(IMAGE_WIDTH,IMAGE_HEIGHT,QImage::Format_RGB32);

        for(int j=0; j<IMAGE_WIDTH; j++) {
            pEditor->setCurrentTFIndex(i);
            QColor qcolor = getColor((float) j / IMAGE_WIDTH);
            for(int k=0; k<IMAGE_HEIGHT; k++) {
                img.setPixel(j,k,qcolor.rgba());
            }
        }
        ui.comboBox_colorScheme->addItem(QIcon(QPixmap::fromImage(img)),QString( pEditor->getTFName(i).c_str() ));
    }

   pEditor->setCurrentTFIndex(currentTFIndex);
    ui.comboBox_colorScheme->setCurrentIndex(currentTFIndex);
}


void TransferFunctionWidget::on_comboBox_colorScheme_currentIndexChanged(int index)
{
	pEditor->setCurrentTFIndex(index);
    pTFEditorWidget->recompute_positions();
    pTFEditorWidget->update();
    emit signal_tf_updated();
}

void TransferFunctionWidget::on_rbtn_BandedMidpoint_toggled(bool checked) {
	if (checked) {
		pEditor->setMode(VCGL::IColorizer::BANDED_MIDPOINT);
		pTFEditorWidget->update();
	}
}

void TransferFunctionWidget::on_rbtn_BandedSides_toggled(bool checked) {
	if (checked) {
		pEditor->setMode(VCGL::IColorizer::BANDED_SIDES);
		pTFEditorWidget->update();
	}
}

void TransferFunctionWidget::on_rbtn_Continuous_toggled(bool checked) {
	if (checked) {
		pEditor->setMode(VCGL::IColorizer::CONTINUOUS);
		pTFEditorWidget->update();
	}
}

void TransferFunctionWidget::attachEditor(VCGL::TransferFunctionEditor* pTransferFunctionEditor, bool bTakeOwnership) {
	releaseEditor();
	pEditor = pTransferFunctionEditor;
	bEditorOwnership = bTakeOwnership;
}

void TransferFunctionWidget::releaseEditor() {
	if (bEditorOwnership && pEditor != 0) {
		delete pEditor;
		pEditor = 0;
	}
	pEditor = 0;
}

void TransferFunctionWidget::initWithEditor() {
	pTFEditorWidget = new TransferFunctionEditorWidget(*pEditor, this);
	ui.gridLayout->addWidget( pTFEditorWidget );
	pTFEditorWidget->setFocusPolicy(Qt::StrongFocus);
	connect(pTFEditorWidget,SIGNAL(signal_slider_moved()),this,SIGNAL(signal_tf_updated()));

	pTFEditorWidget->initialize();
	addIcons();

	assert(pEditor->getTFCount()>0);
	switch(pEditor->getCurrentTF()->getMode()) {
		case VCGL::IColorizer::BANDED_SIDES:
			ui.rbtn_Continuous->setChecked(false);
			ui.rbtn_BandedMidpoint->setChecked(false);
			ui.rbtn_BandedSides->setChecked(true);
			break;

		case VCGL::IColorizer::BANDED_MIDPOINT:
			ui.rbtn_Continuous->setChecked(false);
			ui.rbtn_BandedMidpoint->setChecked(true);
			ui.rbtn_BandedSides->setChecked(false);
			break;

		case VCGL::IColorizer::CONTINUOUS:
		default:
			ui.rbtn_Continuous->setChecked(true);
			ui.rbtn_BandedMidpoint->setChecked(false);
			ui.rbtn_BandedSides->setChecked(false);
			break;
	}
}
