/*! @file transferfunctioneditor.h
 * @date Created on Apr 29, 2014
 * @author anantonov
 *
 * @brief Definition of the class encapsulating logic of the transfer editor widget (for testing)
 */

#ifndef TRANSFERFUNCTIONEDITOR_H_
#define TRANSFERFUNCTIONEDITOR_H_

#include <vector>
#include "transferfunctionobject.h"
#include "icolorizer.h"

namespace VCGL {

///Class encapsulating logic of the transfer editor widget (for testing)
class TransferFunctionEditor {
public:
	/// Default constructor
	TransferFunctionEditor();

	/// Return number of transfer functions in the inner collection
	unsigned getTFCount();

	/// Set an inner collection of transfer functions
	void setCollection(const std::vector<VCGL::TransferFunctionObject>& transferFunctionCollection);


	/// Return the index of the currently selected transfer function
	unsigned getCurrentTFIndex();

	/// Select the transfer function at the specified index
	void setCurrentTFIndex(unsigned i);

	/*! @brief select the transfer function that is equal to the provided object
	 * @attention If no object in the inner collection equals the provided object,
	 * 				then the object is added to the collection and selected
	 *
	 * @param pTFObject A transfer function to set
	 */
	void setCurrentTFObject(const TransferFunctionObject* pTFObject);

	/// Get a pointer to the current transfer function object
	TransferFunctionObject* getCurrentTF();

	/// Set the mode for transfer functions (@see VCGL::IColorizer::ColorizerMode)
	void setMode(enum IColorizer::ColorizerMode newMode);

	/// Get the name of the transfer function at the specified index
	const std::string& getTFName(unsigned index);
private:
	/// Prohibit copying editor logic
	TransferFunctionEditor(const TransferFunctionEditor& other);
	/// Prohibit assigning editor logic
	const TransferFunctionEditor& operator== (const TransferFunctionEditor& other);

	std::vector<VCGL::TransferFunctionObject> tfCollection; ///< inner collection of transfer functions
	unsigned currentTFIndex; ///< index of the currently selected transfer function
};

} /* namespace VCGL */

#endif /* TRANSFERFUNCTIONEDITOR_H_ */
