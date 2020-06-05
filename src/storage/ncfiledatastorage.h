/*! @file ncfiledatastorage.h
 * @author anantonov
 * @date Created on Nov 26, 2014
 *
 * @brief Definition of the class for reading data stored in NetCDF files
 */

#ifndef NCFILEDATASTORAGE_H_
#define NCFILEDATASTORAGE_H_

#include "datastorage.h"
#include <vector>

namespace VCGL {

class NCFileDataStorage: public DataStorage {
public:
	NCFileDataStorage(const char* filename);
	virtual ~NCFileDataStorage();

	int initVariable(const char* varname,
			int level);
	void releaseVariable();

	virtual std::size_t getNTime() override;
	virtual std::size_t getNLon() override;
	virtual std::size_t getNLat() override;
	virtual std::vector<float> loadLons() override;
	virtual std::vector<float> loadLats() override;
	virtual vectorFloat3D loadData(std::size_t latStart, std::size_t latCount) override;

private:
	bool read4DVar(std::vector<float>& var_in, std::size_t latStart, std::size_t latCount);
	bool read3DVar(std::vector<float>& var_in, std::size_t latStart, std::size_t latCount);

	int ncid;

	size_t nlat;
	size_t nlon;
	size_t ntime;

	int varID;
	int var_numdim;
	size_t iLev;

	int lat_varid;
	int lon_varid;
};

} /* namespace VCGL */

#endif /* NCFILEDATASTORAGE_H_ */
