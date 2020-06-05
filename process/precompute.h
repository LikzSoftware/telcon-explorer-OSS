/*!	@file precompute.h
 *	@author anantonov
 *	@date	Sep 11, 2013 (created)
 *	@brief	TODO <file description>
 */

#ifndef PRECOMPUTE_H_
#define PRECOMPUTE_H_

#include <vector>
#include <string>

#include "typedefs.h"
#include "projection/projectedpointinfo.h"

namespace VCGL {
	struct ProjectedPointInfo;
	class DistanceMatrix;
}


/** @brief Compute correlation matrix of all pairs of time series
 *
 * @param data 3D data array, indices LAT, LON, TIME
 * @param correlationMatrix output - symmetric square matrix with correlations
 */
void computeCorrelations(
		const std::vector< std::vector< std::vector<float> > >& data,
		std::vector< std::vector<float> >& correlationMatrix,
		std::vector< std::vector<bool> >& validityMask);

void storeCorrelationsTriangle(const std::vector< std::vector<float> >& correlationMatrix, const char* corrFileNameOUT);
void readCorrelationTriangle(const char* fileName, std::vector< std::vector<float> > & correlationMatrix);

void computeAutocorrelations(const std::vector< std::vector< std::vector<float> > >& data,
				std::vector<float> & autocorrelations,
				std::vector< std::vector<bool> >& validityMask);
void storeAutocorrelations(const std::vector<float>& autocorrelations, const std::string& autocorrFileNameOUT);
void readAutocorrelations(const std::string& fileName, std::vector<float>& autocorrelations);

void
projectCorrelationMatrix(const std::vector< std::vector<float> >& correlations,
		int nx, int ny, std::vector<VCGL::ProjectedPointInfo>& output);
void projectDMAT(
		const std::vector<VCGL::strType>& ids,
		const VCGL::DistanceMatrix& dmat,
		std::vector<VCGL::ProjectedPointInfo>& output);
void storeProjectionResults(const char* fileName, std::vector<VCGL::ProjectedPointInfo>& results);
void loadProjectionLonLat(const char* fnProjection, int nlon, int nlat, std::vector<VCGL::ProjectedPointInfo>& projection);
#endif // PRECOMPUTE_H_
