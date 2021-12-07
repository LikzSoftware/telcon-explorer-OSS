#ifndef REGIONSEARCHEXPLORER_H
#define REGIONSEARCHEXPLORER_H

#include <QDialog>

#include "exploration/explorationmodel.h"
#include "exploration/maps/mapgrid.h"
#include "colorizer/transferfunctionobject.h"
#include <vector>
#include "process/regionsearch.h"

namespace VCGL {
 class MapSubview;
 struct TextPainter;
 class LegendSubview;
}

namespace Ui {
class RegionSearchExplorer;
}

enum RSEState {
	RSE_Allocated = -1,
	RSE_Prepare = 0,
	RSE_FindSeed,
	RSE_GrowRegion,
	RSE_Finished
};

class RegionSearchExplorer : public QDialog, private VCGL::RegionSearch
{
	Q_OBJECT

public:
	explicit RegionSearchExplorer(QWidget *parent = 0);
	~RegionSearchExplorer();

	///not-owning assignment
	void useModel(VCGL::ExplorationModel* pModel);
	void init();

//signals:
//	/// Request the data reference coordinate grid
//	void getGridRequest(VCGL::MapGrid& grid);

protected slots:
	void on_btnNext_clicked();

	void on_regionMap_getGridRequest(VCGL::MapGrid& grid);

	/// update map
	void on_regionMap_updateMapRequest(VCGL::MapSubview* pMap);
	/// update text of the map
	void on_regionMap_updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter);
	/// update legend for the map
	void on_regionMap_updateLegendRequest(VCGL::LegendSubview* pLegend);
	/// update text of the legend for the map
	void on_regionMap_updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter);



private:
	Ui::RegionSearchExplorer *ui;
	VCGL::MapGrid grid;
	VCGL::TransferFunctionObject* pTFO;
	std::vector< std::vector<int> > regionMap;
	std::vector<VCGL::AnnotationLinkF> links;

	VCGL::ExplorationModel* pModel;	///< MVC-model for the aplication
	std::vector< std::vector<float> > tc; ///< teleconnectivity data

	int nextRegion;
	std::queue<QPoint> pointQueue;
	RSEState state;

};

#endif // REGIONSEARCHEXPLORER_H
