/*! @file explorationwidget.cpp
 * @author anantonov
 *
 * @brief Implementation of the MVC-Controller class for the exploration of teleconnections
 */

#include "explorationwidget.h"

#include "preferences/preferencepane.h"
#include "preferences/preferences.h"
#include "preferences/preferencestorage.h"
#include "exploration/maps/mapsubview.h"
#include "exploration/maps/legendsubview.h"
#include "exploration/projection/projectionview.h"
#include "exploration/regions/regionsearchexplorer.h"
#include "explorationmodel.h"
#include "coordinatetext.h"

#include "colorizer/transferfunctionobject.h"
#include "exploration/maps/maptype.h"

#include "multiplatform/devicepixelratio.h"


#include <QListWidgetItem>
#include <iostream>

#include <fstream>
#include <cassert>

ExplorationWidget::ExplorationWidget(QWidget *parent)
	: QWidget(parent), pModel(0), pPreferencePane(0), selectionMode(MSM_REFERENCE_POINT)
{
	ui.setupUi(this);

	ui.rbtnRefPoint->setChecked( selectionMode == MSM_REFERENCE_POINT );
	ui.rbtnRegion->setChecked( selectionMode == MSM_REGION );
	ui.rbtnRegionPlus->setChecked( selectionMode == MSM_REGION_PLUS );
	QList<int> sizes;
	sizes << 300 << 300;
	ui.splitter->setSizes(sizes);

	connect(ui.mapCorrelation, SIGNAL(getSelectionModeRequest(MouseSelectionMode&)), this, SLOT(getSelectionMode(MouseSelectionMode&)));
	connect(ui.mapTeleconnectivity, SIGNAL(getSelectionModeRequest(MouseSelectionMode&)), this, SLOT(getSelectionMode(MouseSelectionMode&)));

	connect(ui.mapCorrelation, SIGNAL(getGridRequest(VCGL::MapGrid&)), this, SLOT(getGrid(VCGL::MapGrid&)));
	connect(ui.mapTeleconnectivity, SIGNAL(getGridRequest(VCGL::MapGrid&)), this, SLOT(getGrid(VCGL::MapGrid&)));
	connect(ui.wProjection, SIGNAL(getGridRequest(VCGL::MapGrid&)), this, SLOT(getGrid(VCGL::MapGrid&)));

	connect(ui.mapCorrelation, SIGNAL(selectPoint(const QPointF&)), this, SLOT(selectPoint(const QPointF&)));
	connect(ui.mapTeleconnectivity, SIGNAL(selectPoint(const QPointF&)), this, SLOT(selectPoint(const QPointF&)));
	connect(ui.wProjection, SIGNAL(selectPoint(const QPointF&)), this, SLOT(selectPoint(const QPointF&)));

	connect(ui.mapCorrelation,
			SIGNAL(selectRegionAtPoint(const QPointF&, bool)),
			this,
			SLOT(selectRegionAtPoint(const QPointF&, bool)));
	connect(ui.mapTeleconnectivity,
			SIGNAL(selectRegionAtPoint(const QPointF&, bool)),
			this,
			SLOT(selectRegionAtPoint(const QPointF&, bool)));
	connect(ui.wProjection,
			SIGNAL(selectRegionAtPoint(const QPointF&, bool)),
			this,
			SLOT(selectRegionAtPoint(const QPointF&, bool)));

	connect(ui.mapCorrelation,
			SIGNAL(resetRegionSelection()),
			this,
			SLOT(resetRegionSelection()));
	connect(ui.mapTeleconnectivity,
				SIGNAL(resetRegionSelection()),
				this,
				SLOT(resetRegionSelection()));

	connect(ui.wProjection,
			SIGNAL(getPointValue(const QPointF&, float*, bool*)),
			this,
			SLOT(on_mapCorrelation_getPointValue(const QPointF&, float*, bool*)));

	std::ifstream in("preferences.txt");
	if (in.good()) {
		in.close();
		VCGL::PreferenceStorage::load("preferences.txt", &preferences);
	}
	else {
		VCGL::Preferences::GetDefaults(&preferences);
	}
	reinitFromPreferences();
}

ExplorationWidget::~ExplorationWidget()
{
	if (pPreferencePane != 0) {
		delete pPreferencePane;
		pPreferencePane = 0;
	}
	cleanup();
}

void
ExplorationWidget::setModel(VCGL::ExplorationModel* pModel) {

	cleanup();
	this->pModel = pModel;
	reinitFromPreferences();
}

void ExplorationWidget::updateAllViews() {
	ui.mapCorrelation->render();
	ui.mapTeleconnectivity->render();
	ui.wProjection->render();
	updateLinksList();
	updateThresholdView();
}

void ExplorationWidget::getGrid(VCGL::MapGrid& grid) {
	if (pModel != 0) {
		grid = pModel->getGrid();
	}
}

void ExplorationWidget::getSelectionMode(MouseSelectionMode &msm) {
	msm = selectionMode;
}

void ExplorationWidget::on_mapCorrelation_getPointValue(const QPointF& point, float* pValue, bool* pbOK) {
	if (pModel != 0) {
		*pbOK = pModel->getClosestPointCorrelationValue(point, pValue);
	}
}

void ExplorationWidget::on_mapTeleconnectivity_getPointValue(const QPointF& point, float* pValue, bool* pbOK) {
	if (pModel != 0) {
		*pbOK = pModel->getClosestPointTeleconnectivityValue(point, pValue);
	}
}

void ExplorationWidget::selectPoint(const QPointF& point) {
	if (pModel != 0) {
		pModel->selectReferencePoint(point);
		updateAllViews();
	}
}

void ExplorationWidget::selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent) {
	if (pModel != 0) {
		pModel->selectRegionAtPoint(point, bSelectWholeComponent);
		updateAllViews();
	}
}

void ExplorationWidget::resetRegionSelection() {
	if (pModel != 0) {
		pModel->resetRegionSelection();
		updateAllViews();
	}
}

void ExplorationWidget::on_mapCorrelation_updateMapRequest(VCGL::MapSubview* pMap) {
	if (pModel != 0) {
		std::vector< std::vector<float> > colorData;
		std::vector< std::vector<bool> > selectionMask;
		VCGL::MapGrid grid;
		getGrid(grid);

		pModel->getSelectionMask(selectionMask);
		pModel->getCorrelationMapColors(colorData);

		pMap->drawColor(&preferences.correlationViewTF, colorData, grid, selectionMask);
		pMap->drawLandContours(grid, pModel->getContours());

		pMap->drawGrid(grid);

		const float pxRatio = devicePixelRatio();

		std::vector<VCGL::AnnotationLinkF> links;
				pModel->getCorrelationMapChainLinks(links);
				pMap->drawLinks(grid,
						links,
						preferences.correlationViewLineColor,
						preferences.correlationViewEvenPointColor,
						preferences.correlationViewOddPointColor,
						preferences.corrMapPointSize * pxRatio,
						preferences.corrMapLineSize * pxRatio);


		pMap->putPoint(grid,
					   pModel->getReferencePoint(),
					   preferences.referencePointColor,
					   preferences.corrMapRefPointSize * pxRatio);
	}
}

void
ExplorationWidget::on_mapCorrelation_updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter) {
	VCGL::MapGrid grid;
	getGrid(grid);

	pMap->drawGridLabels(pPainter, grid);
}

void ExplorationWidget::on_mapCorrelation_updateLegendRequest(VCGL::LegendSubview* pLegend) {
	pLegend->drawLegendBar(&preferences.correlationViewTF, devicePixelRatio());
}

void ExplorationWidget::on_mapCorrelation_updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter) {
	const float corrMax = 1.0;
	const float corrMin = -1.0;
	std::vector< std::pair<float,float> > labels;
	fillTFLabels(preferences.correlationViewTF, corrMin, corrMax, labels);
	pLegend->drawLegendLabels(pPainter, labels);
}

void ExplorationWidget::on_mapTeleconnectivity_updateMapRequest(VCGL::MapSubview* pMap) {
	if (pModel != 0) {
		std::vector< std::vector<float> > colorData;
		std::vector< std::vector<bool> > ssMask;
		std::vector< std::vector<bool> > selectionMask;
		VCGL::MapGrid grid;
		getGrid(grid);

		float threshold = pModel->getThreshold();

		pModel->getSelectionMask(selectionMask);

		pModel->getTeleconnectivityMapColors(colorData);
		pModel->getStatisticalSignificanceMask(ssMask);
		if (selectionMask.size() < ssMask.size()) {
			assert(selectionMask.size() == 0);
			std::vector<bool> row(ssMask[0].size(), true);
			selectionMask.resize(ssMask.size(), row);
		}

		for (unsigned i=0; i< ssMask.size(); i++) {
			for (unsigned j=0; j< ssMask[0].size(); j++) {
				if (!ssMask[i][j] && threshold < colorData[i][j]) {
					colorData[i][j] = 0.0;
					selectionMask[i][j] = false;
				}
			}
		}

		pMap->drawColor(&preferences.teleconnectivityViewTF, colorData, grid, selectionMask);

		pMap->drawLandContours(grid, pModel->getContours());

		pMap->drawGrid(grid);

		const float pxRatio = devicePixelRatio();

		std::vector<VCGL::AnnotationLinkF> links;
		pModel->getTeleconnectivityMapLinks(links);
		pMap->drawLinks(grid,
				links,
				preferences.teleconnectivityViewLineColor,
				preferences.teleconnectivityViewStartPointColor,
				preferences.teleconnectivityViewEndPointColor,
				preferences.tcMapPointSize * pxRatio,
				preferences.tcMapLineSize * pxRatio);


		pMap->putPoint(grid,
				pModel->getReferencePoint(),
				preferences.referencePointColor,
				preferences.tcMapRefPointSize * pxRatio);
	}
}

void
ExplorationWidget::on_mapTeleconnectivity_updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter) {
	VCGL::MapGrid grid;
	getGrid(grid);

	pMap->drawGridLabels(pPainter, grid);
}

void ExplorationWidget::on_mapTeleconnectivity_updateLegendRequest(VCGL::LegendSubview* pLegend) {
	VCGL::TransferFunctionObject tcTransferFunction(getTCTransferFunction());
	pLegend->drawLegendBar(&tcTransferFunction, devicePixelRatio());
}

void ExplorationWidget::on_mapTeleconnectivity_updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter) {
	const float tcMax = 1.0;
	const float tcMin = 0.0;
	std::vector< std::pair<float,float> > labels;
	fillTFLabels(getTCTransferFunction(), tcMin, tcMax, labels);
	pLegend->drawLegendLabels(pPainter, labels);
}

void ExplorationWidget::on_wProjection_updateProjectionRequest(VCGL::ProjectionView* pProjectionView) {
	if (pModel != 0) {
		std::vector< std::vector<float> > colorData;
		std::vector< std::vector<bool> > selectionMask;
		std::vector< std::vector<QPointF> > projectionData;

		pModel->getCorrelationMapColors(colorData);
		pModel->getSelectionMask(selectionMask);
		pModel->getProjectionData(projectionData);

		const float pxRatio = devicePixelRatio();

		if (projectionData.size() > 0) {
			pProjectionView->drawProjection(projectionData,
					colorData,
					selectionMask,
					&preferences.correlationViewTF,
					preferences.projPointSize * pxRatio);

			std::vector< QPointF > chainProjection;
			pModel->getCorrelationChainProjection(chainProjection);
			pProjectionView->drawChain(chainProjection,
					preferences.correlationViewLineColor,
					preferences.correlationViewEvenPointColor,
					preferences.correlationViewOddPointColor,
					preferences.projChainLineSize * pxRatio,
					preferences.projChainPointSize * pxRatio);

			pProjectionView->putProjectedPoint(pModel->getReferencePointProjection(),
					preferences.referencePointColor,
					preferences.projRefPointSize * pxRatio);
		}
	}
}

void ExplorationWidget::on_wProjection_getProjectionDataRequest(
		std::vector< std::vector<QPointF> >& projectionData) {
	if (pModel != 0) {
		pModel->getProjectionData(projectionData);
	}
}

void ExplorationWidget::on_wProjection_newSelectionInView(const std::vector<std::vector<bool> >& selectionMask, unsigned numSelected) {
	if (pModel != 0) {
		pModel->setSelectionMask(selectionMask, numSelected);
		updateAllViews();
	}
}

void ExplorationWidget::updateLinksList() {
	if (pModel != 0) {
		std::vector<VCGL::LinkF> links;
		pModel->getTeleconnectivityLinks(links);

		bool bChanged = false;
		if (ui.wLinksList->count() != (int)links.size()) {
			bChanged = true;
		}
		else {
			for (unsigned i=0; i<links.size(); i++) {
				QListWidgetItem* item = ui.wLinksList->item(i);
				QPointF pt = item->data(Qt::UserRole).toPointF();

				if (!pModel->pointsEqual(links[i].ptA, pt)) {
					bChanged = true;
				}
			}
		}

		if (bChanged) {
			ui.wLinksList->clear();


			for (unsigned i=0; i<links.size(); i++) {

				QString line = QString("%1: %2 -> %3").arg(
						QString::number( links[i].w, 'f', 3),
						VCGL::CoordinateText::convertPoint(links[i].ptA),
						VCGL::CoordinateText::convertPoint(links[i].ptB));
				QListWidgetItem* item = new QListWidgetItem(line, ui.wLinksList);
				item->setData(Qt::UserRole, QVariant(links[i].ptA));
			}
		}

		//search for reference point
		QPointF refPt = pModel->getReferencePoint();
		ui.wLinksList->setCurrentRow(-1);
		for (int i=0; i<ui.wLinksList->count(); i++) {
			QListWidgetItem* item = ui.wLinksList->item(i);
			QPointF pt = item->data(Qt::UserRole).toPointF();

			if (pModel->pointsEqual(refPt, pt)) {
				ui.wLinksList->setCurrentRow(i);
				break;
			}
		}

		ui.wLinksList->update();
	}
}

void ExplorationWidget::on_wLinksList_itemSelectionChanged () {
	if (pModel != 0) {
		QListWidgetItem* item = ui.wLinksList->currentItem();
		if (item != 0) {
			QPointF refPt = pModel->getReferencePoint();

			QPointF pt = item->data(Qt::UserRole).toPointF();

			if (!pModel->pointsEqual(refPt, pt)) {
				pModel->selectReferencePoint(pt);
				updateAllViews();
			}
		}
	}
}

void ExplorationWidget::updateThresholdView() {
	if (pModel != 0) {
		float threshold = pModel->getThreshold();
		ui.slThreshold->setSliderPosition( (int)(100.0*threshold + 0.5) );
		ui.edThreshold->setText( QString::number(threshold, 'f', 2) );
	}
}

void ExplorationWidget::on_slThreshold_valueChanged(int value) {
	if (pModel != 0) {
		float newThreshold = value/100.0;
		pModel->setThreshold(newThreshold);
		updateAllViews();
	}
}

void
ExplorationWidget::acceptPreferencesPane() {
	if (pPreferencePane != 0) {
		pPreferencePane->getPreferences(&preferences);
		reinitFromPreferences();
		updateAllViews();
	}
}

void
ExplorationWidget::rejectPreferencesPane() {
	if (pPreferencePane != 0) {
		pPreferencePane->setAttribute(Qt::WA_DeleteOnClose, true);
		pPreferencePane->close();
		pPreferencePane = 0;
	}
}

void
ExplorationWidget::on_btnPreferences_clicked() {
	if (pPreferencePane == 0) {
		pPreferencePane = new PreferencePane();
		connect(pPreferencePane, SIGNAL(dialogAccepted(void)), this, SLOT(acceptPreferencesPane(void)));
		connect(pPreferencePane, SIGNAL(dialogRejected(void)), this, SLOT(rejectPreferencesPane(void)));

		pPreferencePane->init(&preferences);
	}

	pPreferencePane->show();
}

void
ExplorationWidget::on_btnRegions_clicked() {
	RegionSearchExplorer* pRegions = new RegionSearchExplorer(this);
	pRegions->setAttribute(Qt::WA_DeleteOnClose, true);
	pRegions->useModel(this->pModel);
	pRegions->init();
	pRegions->show();
}

void ExplorationWidget::show() {
	((QGLWidget*)this)->show();
	updateAllViews();
}

void ExplorationWidget::keyPressEvent ( QKeyEvent * event ) {
	switch (event->key()) {
	case Qt::Key_P:
		std::cerr << "selecting polar map" << std::endl;
		preferences.mapType = (int)VCGL::POLAR;
		reinitFromPreferences();
		updateAllViews();
		break;
	case Qt::Key_R:
		std::cerr << "selecting equirectangular map" << std::endl;
		preferences.mapType = (int)VCGL::EQUIRECTANGULAR;
		reinitFromPreferences();
		updateAllViews();
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}

void ExplorationWidget::closeEvent ( QCloseEvent * event ) {
	VCGL::PreferenceStorage::store("preferences.txt", &preferences);
	event->accept();
}

void ExplorationWidget::reinitFromPreferences() {
	if (pModel != 0) {
		pModel->computeStatisticalSignificanceMask(preferences.significanceThreshold);

		//force model to recompute teleconnectivity regions :(
		pModel->setThreshold( pModel->getThreshold() );
	}

	switch ((VCGL::MapType)preferences.mapType) {
		case VCGL::EQUIRECTANGULAR:
			ui.mapCorrelation->selectRectilinearMap();
			ui.mapTeleconnectivity->selectRectilinearMap();
			break;
		case VCGL::POLAR:
			ui.mapCorrelation->selectPolarMap();
			ui.mapTeleconnectivity->selectPolarMap();
			break;
		default:
			std::cerr << "ExplorationWidget::reinitFromPreferences unknown map type: " << preferences.mapType << std::endl;
			break;
		}
}

void ExplorationWidget::cleanup() {
	if (pModel != 0) {
		delete pModel;
		pModel = 0;
	}
}

VCGL::TransferFunctionObject
ExplorationWidget::getTCTransferFunction() {
	assert(preferences.teleconnectivityViewTF.getRecordsCount() > 0);
	assert(pModel != 0);

	float threshold = pModel->getThreshold();
	VCGL::TransferFunctionObject& tfo = preferences.teleconnectivityViewTF;
	VCGL::TransferFunctionObject tcTransferFunction;
	tcTransferFunction.setMode( tfo.getMode() );

	tcTransferFunction.addTFRecord( tfo.getTFRecord(0)  );

	unsigned idx = 1;
	while( idx < tfo.getRecordsCount() && tfo.getRecordPos(idx) < threshold ) {
		idx++;
	}

	float r=0, g=0, b=0;
	tfo.getRecordColor(0, &r, &g, &b);
	VCGL::TFRecord thrL(threshold, r, g, b);
	tcTransferFunction.addTFRecord(thrL);

	tfo.colorize(threshold, &r, &g, &b);
	VCGL::TFRecord thrR(threshold, r, g, b);
	tcTransferFunction.addTFRecord(thrR);

	while (idx < tfo.getRecordsCount()) {
		tcTransferFunction.addTFRecord( tfo.getTFRecord(idx) );
		idx++;
	}
	return tcTransferFunction;
}

void
ExplorationWidget::fillTFLabels(const VCGL::TransferFunctionObject& tfo,
			const float minValue,
			const float maxValue,
			std::vector< std::pair<float,float> >& outTFLabels) {

	//get control points from TF object
	//convert control points to function values
	for (unsigned i=0; i<tfo.getRecordsCount(); i++) {
		float controlPointPos = tfo.getRecordPos(i);
		outTFLabels.emplace_back( std::pair<float,float> {
			controlPointPos,
			minValue + controlPointPos*(maxValue-minValue) } );
	}
}
