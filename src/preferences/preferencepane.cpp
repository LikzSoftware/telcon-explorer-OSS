#include "preferencepane.h"
#include "colorizer/transferfunctionstorage.h"
#include "colorizer/transferfunctioneditor.h"

#include <QColorDialog>

#include "storage/pathresolver.h"
#include "storage/filesystem.h"
#include <string>

#include "exploration/maps/maptype.h"

namespace {
	const float SIGN_OFF = 0.0;
	const float SIGNIFICANT = 0.95;
	const float HIGHLY_SIGNIFICANT = 0.99;
}

PreferencePane::PreferencePane(QWidget *parent)
    : QDialog(parent), pLogic(0), pCorrelationTFEditor(0), pTeleconnectivityTFEditor(0)
{
	ui.setupUi(this);
	ui.rbtnSSOff->setText("off");
	ui.rbtnSignificant->setText( QString("significant (%1)").arg(QString::number(SIGNIFICANT))  );
	ui.rbtnHighlySignificant->setText( QString("highly significant (%1)").arg(QString::number(HIGHLY_SIGNIFICANT)) );
}

PreferencePane::~PreferencePane() {
	if (pLogic != 0) {
		delete pLogic;
		pLogic = 0;
	}
	if (pCorrelationTFEditor != 0) {
		delete pCorrelationTFEditor;
		pCorrelationTFEditor = 0;
	}
	if (pTeleconnectivityTFEditor != 0) {
		delete pTeleconnectivityTFEditor;
		pTeleconnectivityTFEditor = 0;
	}
}

void PreferencePane::getPreferences(VCGL::Preferences* pPreferences) {
	pLogic->getPreferences(pPreferences);
}

void PreferencePane::init(const VCGL::Preferences* pPreferences) {
	VCGL::FileSystem fs;
	VCGL::PathResolver pr(fs);
	std::string colorSchemesFN;
	pr.find("color_schemes.txt", colorSchemesFN);
	std::cerr << "Using color schemes file " << colorSchemesFN << std::endl;


	std::vector<VCGL::TransferFunctionObject> transferFunctionCollection;
	VCGL::TransferFunctionStorage::LoadFromFile(colorSchemesFN.c_str(), true, transferFunctionCollection);

	pCorrelationTFEditor = new VCGL::TransferFunctionEditor();
	pTeleconnectivityTFEditor = new VCGL::TransferFunctionEditor();

	pCorrelationTFEditor->setCollection(transferFunctionCollection);
	pTeleconnectivityTFEditor->setCollection(transferFunctionCollection);

	pCorrelationTFEditor->setCurrentTFObject(&pPreferences->correlationViewTF);
	pTeleconnectivityTFEditor->setCurrentTFObject(&pPreferences->teleconnectivityViewTF);

	pLogic = new VCGL::PreferencePaneLogic(pCorrelationTFEditor, pTeleconnectivityTFEditor, this);
	pLogic->init(*pPreferences);

	ui.wTFCorrelations->initialize(pCorrelationTFEditor);
	ui.wTFTeleconnectivity->initialize(pTeleconnectivityTFEditor);

	ui.wTFCorrelations->setColorDataRange(-1.0, 1.0);
	ui.wTFTeleconnectivity->setColorDataRange(0.0, 1.0);
}

void PreferencePane::updateView(const VCGL::Preferences& preferences) {
	setColorToButton(preferences.teleconnectivityViewStartPointColor, ui.btnTCStartPointsColor);
	setColorToButton(preferences.teleconnectivityViewEndPointColor, ui.btnTCEndPointsColor);
	setColorToButton(preferences.teleconnectivityViewLineColor, ui.btnTCLinesColor);
	setColorToButton(preferences.referencePointColor, ui.btnRefPointColor);
	setColorToButton(preferences.correlationViewOddPointColor, ui.btnCorrOddPointsColor);
	setColorToButton(preferences.correlationViewEvenPointColor, ui.btnCorrEvenPointsColor);
	setColorToButton(preferences.correlationViewLineColor, ui.btnCorrLinesColor);

	const float epsilon = 1e-6f;
	if (preferences.significanceThreshold >= HIGHLY_SIGNIFICANT-epsilon) {
		ui.rbtnHighlySignificant->setChecked(true);
	}
	else if (preferences.significanceThreshold >= SIGNIFICANT-epsilon) {
		ui.rbtnSignificant->setChecked(true);
	}
	else {
		ui.rbtnSSOff->setChecked(true);
	}

	switch ((VCGL::MapType)preferences.mapType) {
	case VCGL::EQUIRECTANGULAR:
		ui.rbtnEquirectangular->setChecked(true);
		break;
	case VCGL::POLAR:
		ui.rbtnPolar->setChecked(true);
		break;
	default:
		std::cerr << "PreferencePane::updateView: unknown map type: " << preferences.mapType << std::endl;
		break;
	}

}

void PreferencePane::on_btnOK_clicked() {
	on_btnApply_clicked();
	close();
}

void
PreferencePane::on_btnCancel_clicked() {
	emit dialogRejected();
	close();
}

void
PreferencePane::on_btnApply_clicked() {
	emit dialogAccepted();
}

void PreferencePane::on_btnTCStartPointsColor_clicked() {
	pLogic->setTeleconnectivityViewStartPointColor( getNewColor(ui.btnTCStartPointsColor) );
}

void PreferencePane::on_btnTCEndPointsColor_clicked() {
	pLogic->setTeleconnectivityViewEndPointColor( getNewColor(ui.btnTCEndPointsColor) );
}

void PreferencePane::on_btnTCLinesColor_clicked() {
	pLogic->setTeleconnectivityViewLineColor( getNewColor(ui.btnTCLinesColor) );
}

void PreferencePane::on_btnCorrOddPointsColor_clicked() {
	pLogic->setCorrelationViewOddPointColor( getNewColor(ui.btnCorrOddPointsColor) );
}

void PreferencePane::on_btnCorrEvenPointsColor_clicked() {
	pLogic->setCorrelationViewEvenPointColor( getNewColor(ui.btnCorrEvenPointsColor) );
}

void PreferencePane::on_btnCorrLinesColor_clicked() {
	pLogic->setCorrelationViewLineColor( getNewColor(ui.btnCorrLinesColor) );
}

void PreferencePane::on_btnRefPointColor_clicked() {
	pLogic->setReferencePointColor( getNewColor(ui.btnRefPointColor) );
}

void PreferencePane::on_rbtnSSOff_clicked(bool checked) {
	if (checked) {
		pLogic->setSignificanceThreshold(SIGN_OFF);
	}
}

void PreferencePane::on_rbtnSignificant_clicked(bool checked) {
	if (checked) {
		pLogic->setSignificanceThreshold(SIGNIFICANT);
	}

}

void PreferencePane::on_rbtnHighlySignificant_clicked(bool checked) {
	if (checked) {
		pLogic->setSignificanceThreshold(HIGHLY_SIGNIFICANT);
	}
}

void PreferencePane::on_rbtnEquirectangular_clicked(bool checked) {
	if (checked) {
		pLogic->setMapType((int)VCGL::EQUIRECTANGULAR);
	}
}
void PreferencePane::on_rbtnPolar_clicked(bool checked) {
	if (checked) {
		pLogic->setMapType((int)VCGL::POLAR);
	}
}


void PreferencePane::setColorToButton(const VCGL::RGBF& color, QAbstractButton* pButton) {
	QPixmap pixmap(16, 16);
	pixmap.fill(QColor::fromRgbF(color.r, color.g, color.b));
	pButton->setIcon(pixmap);
}

QColor PreferencePane::getButtonColor(const QAbstractButton* pButton) {
	QIcon icon = pButton->icon();
	QPixmap pixmap = icon.pixmap(1,1);
	QImage image = pixmap.toImage();
	return (image.pixel(0,0));
}

VCGL::RGBF PreferencePane::getNewColor(const QAbstractButton* pButton) {
	QColor newColor = QColorDialog::getColor(getButtonColor(pButton), this);
	VCGL::RGBF rgb = { (float)newColor.redF(), (float)newColor.greenF(), (float)newColor.blueF() };
	return rgb;
}
