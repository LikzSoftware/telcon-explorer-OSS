/*!	@file icolorizer.cpp
 *	@author Anatoliy Antonov
 *	@date	Feb 11, 2014 (created)
 *	@brief	Implementation of the factory method for objects providing IColorizer
 */


#include "icolorizer.h"

#include "transferfunctionobject.h"

#include <cassert>
#include <iostream>

namespace VCGL {

IColorizer* IColorizer::createColorizer(enum ColorizerType type) {
	IColorizer* pColorizer = 0;
	TransferFunctionObject* tfo = 0;

	switch (type) {
	case BLUE_WHITE_RED:
		tfo = new TransferFunctionObject(TransferFunctionObject::BlueWhiteRedTFO());
		pColorizer = static_cast<IColorizer*>(tfo);
		break;

	case HEAT:
		tfo = new TransferFunctionObject(TransferFunctionObject::HeatTFO());
		pColorizer = static_cast<IColorizer*>(tfo);
		break;

	case FLAME:
		tfo = new TransferFunctionObject(TransferFunctionObject::FlameTFO());
		pColorizer = static_cast<IColorizer*>(tfo);
		break;

	default:
		std::cerr << "IColorizer::createColorizer(): Unknown colorizer type: " << (int)type << std::endl;
		break;
	}

	return pColorizer;
}

} //namespace VCGL
