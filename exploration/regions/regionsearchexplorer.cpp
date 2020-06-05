#include "regionsearchexplorer.h"
#include "ui_regionsearchexplorer.h"

#include "exploration/maps/mapsubview.h"
#include "exploration/maps/textpainter.h"
#include "exploration/maps/legendsubview.h"
#include <cassert>

#include "process/regionsearch.h"
#include "exploration/explorationmodelimpl.h"
#include "colorizer/rgb.h"
#include "multiplatform/devicepixelratio.h"

using VCGL::TFRecord;

class RSE_TransferFunctionObject: public VCGL::TransferFunctionObject {
public:
virtual void colorize(float val, float* pr, float *pg, float *pb) const override {
		/*
-1 1.0 1.0 1.0
0 0.85 0.85 0.85
1 0.553 0.827 0.78
2 1.0 1.0 0.702
3 0.745 0.729 0.855
4 0.984 0.502 0.447
5 0.503 0.694 0.827
6 0.992 0.706 0.384
7 0.702 0.871 0.411
8 0.988 0.804 0.898
9 0.737 0.503 0.741
10 0.8 0.922 0.772
11 1.0 0.929 0.435
*/
		if (val < -0.5) {
			*pr = 0; *pg = 0; *pb = 0;
		}
		else if (val < 0.5) {
			*pr = 1; *pg = 1; *pb = 1;
		}
		else {
			*pr = 1; *pg = 0; *pb = 0;
		}
	}
virtual ~RSE_TransferFunctionObject() {}
};

namespace {
	const unsigned num_classes = 12;
}

RegionSearchExplorer::RegionSearchExplorer(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::RegionSearchExplorer),
	pTFO(0),
	pModel(0),
	nextRegion(0),
	state(RSE_Allocated)
{
	ui->setupUi(this);
	ui->regionMap->selectRectilinearMap();

	//ideally, this one needs an other type of transfer function,
	//the one which accepts integer number values for classes
	//and returns colors, but that is against the current
	//semantics of IColorizer interface (float in [0,1])
	pTFO = new VCGL::TransferFunctionObject();

	pTFO->setMode(VCGL::IColorizer::BANDED_SIDES);

	float i = 0;
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 1.0, 1.0, 1.0) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.85, 0.85, 0.85) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.553, 0.827, 0.78) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 1.0, 1.0, 0.702) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.984, 0.502, 0.447) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.503, 0.694, 0.827) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.992, 0.706, 0.384) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.702, 0.871, 0.411) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.988, 0.804, 0.898) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.737, 0.503, 0.741) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 0.8, 0.922, 0.772) );
	pTFO->addTFRecord( TFRecord( (i++)/num_classes , 1.0, 0.929, 0.435) );
	pTFO->addTFRecord( TFRecord( 1.0 , 1.0, 0.929, 0.435) );
}

RegionSearchExplorer::~RegionSearchExplorer()
{
	delete ui;
//	delete pModel; //TODO: don't delete unless unique owner -> switch to shared_ptr
	delete pTFO;
}

void
RegionSearchExplorer::useModel(VCGL::ExplorationModel* pModel) {
	this->pModel = pModel;
}


void
RegionSearchExplorer::init() {
	assert(pModel != 0);
	grid = pModel->getGrid();

	regionMap.clear();
	const std::vector<int> rmapRow(grid.nlon(),-1);
	regionMap.resize(grid.nlat(), rmapRow);
	links.clear();

	// tc === tc colors
	pModel->getTeleconnectivityMapColors(tc);
	state = RSE_Prepare;
}

void
RegionSearchExplorer::on_btnNext_clicked() {
	assert(pModel != 0);

	VCGL::RSHelper* pHelper = 0;
	VCGL::ExplorationModelImpl* pModelImpl = dynamic_cast<VCGL::ExplorationModelImpl*>(pModel);
	pHelper = dynamic_cast<VCGL::RSHelper*>(pModelImpl);

	switch (state) {

case RSE_Prepare: {
		// 1) filter the below threshold
		float threshold = pModel->getThreshold();


		filterRegionMapTCThreshold(tc, threshold, regionMap, pHelper);

		// 1a) init the iterations
		nextRegion = 1;
		pointQueue = std::queue<QPoint>();

		state = RSE_FindSeed;

	}
	break;
///////////
case RSE_FindSeed: {

		// 2) find a seed
		bool bSeedFound = false;
		bSeedFound = seed(tc,regionMap, nextRegion, pointQueue);

		// 2a) seed not found -> stop
		//     seed found -> show its link
		if (bSeedFound) {
			state = RSE_GrowRegion;
			// show the link

			VCGL::ExplorationModelImpl* pModelImpl = dynamic_cast<VCGL::ExplorationModelImpl*>(pModel);
			if (pModelImpl != 0) {
				VCGL::AnnotationLinkF link = pModelImpl->tcLinkForPointIndices(pointQueue.front());
				link.sel = static_cast<VCGL::AnnotationLinkF::Selection>
						(VCGL::AnnotationLinkF::SHOW_LINE | VCGL::AnnotationLinkF::SHOW_A);
				links.push_back( link );
			}
		}
		else {
			state = RSE_Finished;
		}
	}
	break;

case RSE_GrowRegion: {
		// 3) grow the region (until the queue is empty)
		if (!pointQueue.empty()) {
			growRegion(pointQueue, regionMap, pHelper);
			state = RSE_FindSeed;
		}
	}
	break;
case RSE_Finished: break;
	default:
		std::cerr << "RegionSearchExplorer::on_btnNext_clicked(): unknown state" << std::endl;
		break;
	}

if (state == RSE_Finished) {
	// show message box "finished";
	ui->btnNext->setEnabled(false);
}


// 4) find other interconnected regions
//	for (int i=0; i<nlat; i++) {
//		for (int j=0; j<nlon; j++) {

//			QPoint ptB = tcindices[i][j];

//			int regionA = regionMap[i][j];
//			int regionB = regionMap[ptB.y()][ptB.x()];

//			//check that link starts and ends above threshold
//			if (regionA >0 && regionB >0 && regionA != regionB) {
//				connectivity.suggestLink(
//					regionA,
//					regionB,
//					VCGL::Link{ QPoint{j,i}, ptB, tc[i][j] });
//			}
//		}
//	}

///////////

	ui->regionMap->update();
}

void
RegionSearchExplorer::on_regionMap_getGridRequest(VCGL::MapGrid& grid) {
	grid = this->grid;
}


void RegionSearchExplorer::on_regionMap_updateMapRequest(VCGL::MapSubview* pMap) {

	std::vector< std::vector<float> > colorData;
	std::vector< std::vector<bool> > selectionMask;
	const int ny = grid.nlat();
	const int nx = grid.nlon();

	colorData.clear();
	colorData.resize(ny, std::vector<float>(nx, 0));
	for (unsigned i=0; i < colorData.size(); i++) {
		for (unsigned j=0; j< colorData[i].size(); j++) {
			colorData[i][j] = (regionMap[i][j]+1.1)/num_classes;
		}
	}

	pMap->drawColor(pTFO, colorData, grid, selectionMask);

	pMap->drawLandContours(grid, pModel->getContours());

	const float regionMapPointSize = 16.0;
	const float regionMapLineSize = 8.0;

	VCGL::RGBF linksColor = { 19.0/255, 129.0/255, 21.0/255 };
				pMap->drawLinks(grid,
						links,
						linksColor,
						linksColor,
						linksColor,
						regionMapPointSize,
						regionMapLineSize);

//		pMap->putPoint(grid, pModel->getReferencePoint(), preferences.referencePointColor);

	pMap->drawGrid(grid);
}

void
RegionSearchExplorer::on_regionMap_updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter) {
	pMap->drawGridLabels(pPainter, grid);
}

void RegionSearchExplorer::on_regionMap_updateLegendRequest(VCGL::LegendSubview* pLegend) {
	pLegend->drawLegendBar(static_cast<VCGL::IColorizer*>(pTFO), devicePixelRatio());
}

void RegionSearchExplorer::on_regionMap_updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter) {
	std::vector< std::pair<float,float> > labels;
//	fillTFLabels(preferences.correlationViewTF, corrMin, corrMax, labels);

	for (unsigned i=0; i<num_classes; i++) {
		labels.push_back( std::pair<float,float>( (i+0.5)*1.0/(num_classes) , ((int)i)-1 ) );
	}
	pLegend->drawLegendLabels(pPainter, labels);
}
