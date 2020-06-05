/*! @file tcstorage.h
 * @author anantonov
 * @date Created on Nov 24, 2014
 *
 * @brief Definition of the object responsible for the application data storage
 */

#ifndef TCSTORAGE_H_
#define TCSTORAGE_H_

#include <vector>
#include "datastorage.h"

namespace VCGL {

//struct DataStorage;

class TCStorage {
public:
	TCStorage(DataStorage* pDataStorage, bool bNorthHemisphereOnly = false) {
		this->pDataStorage = pDataStorage;
		this->bNorthHemisphere = bNorthHemisphereOnly;
	}
	~TCStorage() {
		if (pDataStorage != 0) {
			delete pDataStorage;
		}
	}

	size_t getNTime() {
		unsigned result = 0;
		if (pDataStorage) {
			result = pDataStorage->getNTime();
		}
		return result;
	}

	void loadGrid(std::vector<float>& lons, std::vector<float>& lats) {
		lons.clear();
		lats.clear();

		if (pDataStorage) {
			lons = pDataStorage->loadLons();

			if (!bNorthHemisphere) {
				lats = pDataStorage->loadLats();
			}
			else {
				for (auto lat: pDataStorage->loadLats()) {
					if (lat >= 0.0) {
						lats.push_back(lat);
					}
				}
			}
		}
	}

	void loadData(vectorFloat3D& data) {
		data.clear();
		if (pDataStorage) {
			std::size_t start = 0;
			std::size_t count = pDataStorage->getNLat();

			if (bNorthHemisphere) {
				std::vector<float> lats = pDataStorage->loadLats();
				start = 0;
				while (start<lats.size() && lats[start] < 0) {
					start++;
				}
				std::size_t end = start;
				while (end<lats.size() && lats[end] >= 0) {
					end++;
				}
				count = end-start;
			}
			data = pDataStorage->loadData(start, count);
		}
	}
private:
	DataStorage* pDataStorage;
	bool bNorthHemisphere;

};

} /* namespace VCGL */

#endif /* TCSTORAGE_H_ */
