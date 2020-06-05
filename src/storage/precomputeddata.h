/*!	@file precomputeddata.h
 *	@author anantonov
 *	@date	Mar 27, 2017 (created)
 *	@brief	TODO <file description>
 */

#ifndef PRECOMPUTEDDATA_H_
#define PRECOMPUTEDDATA_H_

#include <vector>
#include <string>

#include "typedefs.h"
#include "projection/projectedpointinfo.h"

namespace VCGL {
	struct ProjectedPointInfo;
	class DistanceMatrix;
}

void storeCorrelationsTriangle(const std::vector< std::vector<float> >& correlationMatrix, const std::string& corrFileNameOUT, bool binary = true);
void readCorrelationTriangle(const std::string& fileName, std::vector< std::vector<float> > & correlationMatrix, bool binary = true);

void storeAutocorrelations(const std::vector<float>& autocorrelations, const std::string& autocorrFileNameOUT, bool binary=true);
void readAutocorrelations(const std::string& fileName, std::vector<float>& autocorrelations, bool binary=true);

void storeProjectionResults(const std::string& fileName, std::vector<VCGL::ProjectedPointInfo>& results, bool binary=true);
void loadProjectionLonLat(const std::string& fnProjection, int nlon, int nlat, std::vector<VCGL::ProjectedPointInfo>& projection, bool binary=true);

#endif // PRECOMPUTEDDATA_H_
