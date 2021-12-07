/*! @file transferfunctionstorage.h
 * @date Created on Apr 29, 2014
 * @author anantonov
 *
 * @brief Methods for loading transfer functions from an external source
 */

#ifndef TRANSFERFUNCTIONSTORAGE_H_
#define TRANSFERFUNCTIONSTORAGE_H_

#include <iostream>
#include <string>
#include <vector>
#include "transferfunctionobject.h"

namespace VCGL {

/// Methods for loading transfer functions from an external source
class TransferFunctionStorage {
public:
	/// Load transfer functions from an std::istream
	static void LoadFromStream(std::istream& inStream, bool silent, std::vector<TransferFunctionObject>& outTFCollection);
	/// Load transfer functions from a file
	static void LoadFromFile(std::string fileName, bool silent, std::vector<TransferFunctionObject>& outTFCollection);

private:
	/// Prohibit construction of the class object
	TransferFunctionStorage();
	/// Prohibit copying of the class objects
	TransferFunctionStorage(const TransferFunctionStorage&);
};

} /* namespace VCGL */

#endif /* TRANSFERFUNCTIONSTORAGE_H_ */
