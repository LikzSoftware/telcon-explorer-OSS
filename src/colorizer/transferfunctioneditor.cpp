/*! @file transferfunctioneditor.cpp
 * @date Created on Apr 29, 2014
 * @author anantonov
 *
 * @brief Implementation of the class encapsulating logic of the transfer editor widget (for testing)
 */

#include "transferfunctioneditor.h"
#include <cassert>

namespace VCGL {

	TransferFunctionEditor::TransferFunctionEditor()
	: currentTFIndex(0) {}

	unsigned TransferFunctionEditor::getTFCount() {
		return tfCollection.size();
	}

	void TransferFunctionEditor::setCollection(const std::vector<VCGL::TransferFunctionObject>& transferFunctionCollection) {
		tfCollection = transferFunctionCollection;
		currentTFIndex = 0;
	}

	unsigned TransferFunctionEditor::getCurrentTFIndex() {
		return currentTFIndex;
	}

	void TransferFunctionEditor::setCurrentTFIndex(unsigned i) {
		assert(getTFCount() > i);
		currentTFIndex = i;
	}

	void TransferFunctionEditor::setCurrentTFObject(const TransferFunctionObject* pTFObject) {
		bool found = false;
		for (unsigned i=0; (!found) && i<getTFCount(); i++) {
			if (tfCollection[i] == (*pTFObject)) {
				found = true;
				setCurrentTFIndex(i);
			}
		}

		if (!found) {
			tfCollection.push_back(*pTFObject);
			setCurrentTFIndex(tfCollection.size()-1);
		}
	}

	TransferFunctionObject* TransferFunctionEditor::getCurrentTF() {
		assert(tfCollection.size() > currentTFIndex);
		return &tfCollection[currentTFIndex];
	}

	void TransferFunctionEditor::setMode(enum IColorizer::ColorizerMode newMode) {
		assert(getTFCount() > 0);
		for (unsigned i=0; i<getTFCount(); i++) {
			tfCollection[i].setMode(newMode);
		}
	}

	const std::string&
	TransferFunctionEditor::getTFName(unsigned index) {
		assert(getTFCount() > index);
		return tfCollection[index].getName();
	}
} /* namespace VCGL */
