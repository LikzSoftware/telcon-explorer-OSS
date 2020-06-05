/*! @file datastorage.h
 * @author anantonov
 * @date Created on Nov 26, 2014
 *
 * @brief Generic interface for classes providing data storage functions
 */

#ifndef DATASTORAGE_H_
#define DATASTORAGE_H_

#include "typedefs.h"

namespace VCGL {

struct DataStorage {
	virtual std::size_t getNTime() = 0;
	virtual std::size_t getNLon() = 0;
	virtual std::size_t getNLat() = 0;
	virtual std::vector<float> loadLons() = 0;
	virtual std::vector<float> loadLats() = 0;

	/*! Read variable data
	 * @return  Data array with indices (LAT, LON, TIME).
	 */
	virtual vectorFloat3D loadData(std::size_t latStart, std::size_t latCount) = 0;

	virtual ~DataStorage() {};
};

} // namespace VCGL



#endif /* DATASTORAGE_H_ */
