/*!	@file ilspdata.h
 *	@author anantonov
 *	@date	Apr 9, 2012 (created)
 *	@brief	Interface for LSP multifield data objects
 */

#ifndef ITSDATA_H_
#define ITSDATA_H_

namespace LSP {

typedef double distanceType;

struct ILSPData {
	virtual distanceType distance(const ILSPData& other) const = 0;
	virtual ~ILSPData() {}
};

inline distanceType distance(ILSPData* const pData, ILSPData* const pOtherData) {
	return pData->distance(*pOtherData);
}

}


#endif // ITSDATA_H_
