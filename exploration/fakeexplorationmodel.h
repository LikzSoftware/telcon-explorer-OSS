/*! @file fakeexplorationmodel.h
 * @author anantonov
 * @date Created on Jun 21, 2014
 *
 * @brief Class providing fake implementation of Exploration Model for testing purposes
 */

#ifndef FAKEEXPLORATIONMODEL_H_
#define FAKEEXPLORATIONMODEL_H_

#include "explorationmodel.h"

namespace VCGL {

/// Class providing fake implementation of Exploration Model for testing purposes
class FakeExplorationModel: public ExplorationModel {
public:
	/// Default constructor
	FakeExplorationModel();
	/// Virtual destructor for proper memory deallocation
	virtual ~FakeExplorationModel();

	/// @copydoc ExplorationModel::loadGrid
	virtual void loadGrid(TCStorage& storage) override;
	/// @copydoc ExplorationModel::loadGrid
	virtual void loadGrid(const std::string& fileName) override;
	/// @copydoc ExplorationModel::loadContours
	virtual void loadContours(const std::string& contoursFileName) override;
	/// @copydoc ExplorationModel::loadCorrelations
	virtual void loadCorrelations(const std::string& correlationsFileName) override;
	/// @copydoc ExplorationModel::loadAutocorrelations
	virtual void loadAutocorrelations(const std::string& autocorrFileName) override;
	/// @copydoc ExplorationModel::loadProjection
	virtual void loadProjection(const std::string& projectionFileName) override;

	/// @copydoc ExplorationModel::getCorrelationMapColors
	virtual void getCorrelationMapColors(std::vector< std::vector<float> >& colorData) const  override;
	/// @copydoc ExplorationModel::getTeleconnectivityMapColors
	virtual void getTeleconnectivityMapColors(std::vector< std::vector<float> >& colorData) const  override;
	/// @copydoc ExplorationModel::getProjectionData
	virtual void getProjectionData(std::vector< std::vector<QPointF> >& projectionData) const  override;

private:
	/// synthesize color data for a map
	void getColors(std::vector< std::vector<float> >& colorData) const;

	static const unsigned nx; ///< number of points horizontally (x-dimension, longitudes)
	static const unsigned ny; ///< number of points vertically (y-dimension, latitude)
};

} /* namespace VCGL */

#endif /* FAKEEXPLORATIONMODEL_H_ */
