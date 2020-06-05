/*! @file explorationwidget.h
 * @author anantonov
 *
 * @brief Definition of the MVC-Controller class for the exploration of teleconnections
 */

#ifndef EXPLORATIONWIDGET_H
#define EXPLORATIONWIDGET_H

#include "multiplatform/QWidget"
#include "ui_explorationwidget.h"
#include <QKeyEvent>
#include <QCloseEvent>

#include "preferences/preferences.h"
#include <vector>
#include "mouseselectionmode.h"

class QListWidgetItem;

class PreferencePane;

namespace VCGL {
	class MapSubview;
	class LegendSubview;
	class ExplorationModel;
	struct TextPainter;
	class TransferFunctionObject;
	struct MapGrid;
	class ProjectionView;
}

/// class representing MVC-Controller for the exploration of teleconnections
class ExplorationWidget : public QWidget
{
    Q_OBJECT

public:
    /*! @brief Default constructor
     *  @attention call to setModel with initialized model is required for the widget's work
     *
     */
    ExplorationWidget(QWidget *parent = 0);
    /// Destructor
    ~ExplorationWidget();

    /// Assign a model (object containing application data and algorithms)
    void setModel(VCGL::ExplorationModel* pModel);

    /// Request the controller to update all of its views
    void updateAllViews();

protected slots:
	/// Get data coordinate reference grid
	void getGrid(VCGL::MapGrid& grid);

	/// Get mouse selection mode
	void getSelectionMode(MouseSelectionMode& msm);

	/*! @brief Get point value for correlation map.
	 *
	 *  (i.e. correlation of the time series at the specified point
	 *  to the time series at the reference point)
	 *
	 * @param[in] point		Point for which the correlation value is requested
	 * @param[out] pValue	Pointer to float receiving the correlation value
	 * @param[out] pbOK		Pointer to bool receiving status of operation
	 */
	void on_mapCorrelation_getPointValue(const QPointF& point, float* pValue, bool* pbOK);

	/// Get teleconnectivity value for the specified point
	void on_mapTeleconnectivity_getPointValue(const QPointF& point, float* pValue, bool* pbOK);

	/// Select specified point as the reference point
	void selectPoint(const QPointF& point);

	/// Select (highlight) region containing the specified point
	void selectRegionAtPoint(const QPointF& point, bool bSelectWholeComponent);
	/// Reset region selection
	void resetRegionSelection();

	/// update correlation map
	void on_mapCorrelation_updateMapRequest(VCGL::MapSubview* pMap);
	/// update text of the correlation map
	void on_mapCorrelation_updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter);
	/// update legend for the correlation map
	void on_mapCorrelation_updateLegendRequest(VCGL::LegendSubview* pLegend);
	/// update text of the legend for the correlation map
	void on_mapCorrelation_updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter);

	/// update teleconnectivity map
	void on_mapTeleconnectivity_updateMapRequest(VCGL::MapSubview* pMap);
	/// update text of the teleconnectivity map
	void on_mapTeleconnectivity_updateMapTextRequest(VCGL::MapSubview* pMap, VCGL::TextPainter* pPainter);
	/// update legend for the teleconnectivity map
	void on_mapTeleconnectivity_updateLegendRequest(VCGL::LegendSubview* pLegend);
	/// update text of the legend for the teleconectivity map
	void on_mapTeleconnectivity_updateLegendTextRequest(VCGL::LegendSubview* pLegend, VCGL::TextPainter* pPainter);

	/// update the projection view
	void on_wProjection_updateProjectionRequest(VCGL::ProjectionView* pProjectionView);
	/// Get projection data
	void on_wProjection_getProjectionDataRequest(std::vector< std::vector<QPointF> >& projectionData);
	/// Set selection based on the projection view
	void on_wProjection_newSelectionInView(const std::vector<std::vector<bool> >& selectionMask, unsigned numSelected);

	/// update the links list
	void updateLinksList();

	//void on_wLinksList_currentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );

	/// process the selection change at the links list
	void on_wLinksList_itemSelectionChanged ();

	/// update the threshold view elements
	void updateThresholdView();
	/// process change of value of the threshold slider
	void on_slThreshold_valueChanged(int value);

	/// accept current preferences of the open PreferencePane (Dialog OK/Apply)
	void acceptPreferencesPane();
	/// reject modified preferences of the opened PreferencePane (Dialog Cancel)
	void rejectPreferencesPane();
	/// open PreferencePane
	void on_btnPreferences_clicked();

	/// open RegionSearchExplorer
	void on_btnRegions_clicked();

	void on_rbtnRefPoint_clicked() {
		selectionMode = MSM_REFERENCE_POINT;
	}
	void on_rbtnRegion_clicked() {
		selectionMode = MSM_REGION;
	}
	void on_rbtnRegionPlus_clicked() {
		selectionMode = MSM_REGION_PLUS;
	}

public slots:
	/// show this widget
	void show();
protected:
	/// process keyboard event
	void keyPressEvent ( QKeyEvent * event ) override;
	/// process close event
	void closeEvent ( QCloseEvent * event ) override;
private:
	/// initialize the viewing elements from preferences
	void reinitFromPreferences();

	/// clean up internal resources
	void cleanup();

	/// get the final (thresholded) version of the transfer function for TC map
	VCGL::TransferFunctionObject getTCTransferFunction();

	/*! create a collection of labels for transfer function
	 *
	 * @param[in] tfo			Transfer function object
	 * @param[in] minValue		Minimum value of the data range (corresponding to tfo's 0)
	 * @param[in] maxValue		Maximum value of the data range (corresponding to tfo's 1)
	 * @param[out] outTFLabels	Resulting collection of labels, consisting of pairs
	 * 							(tfo [0,1] value; data [min, max] value)
	 */
	void fillTFLabels(const VCGL::TransferFunctionObject& tfo,
			const float minValue,
			const float maxValue,
			std::vector< std::pair<float,float> >& outTFLabels);

    Ui::ExplorationWidgetClass ui;	///< Qt widget UI
    VCGL::Preferences preferences;	///< object with all preferences
    VCGL::ExplorationModel* pModel;	///< MVC-model for the aplication
    PreferencePane* pPreferencePane; ///< Dialog for modifying the preferences
	MouseSelectionMode selectionMode; ///< Mode of selecting (refPoint, region, ...)
};

#endif // EXPLORATIONWIDGET_H
