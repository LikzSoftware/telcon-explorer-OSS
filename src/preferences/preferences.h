/*! @file preferences.h
 * @date Created on Apr 28, 2014
 * @author anantonov
 *
 * @brief TODO: add description
 */

#ifndef PREFERENCES_H_
#define PREFERENCES_H_

#include "colorizer/transferfunctionobject.h"
#include "colorizer/rgb.h"

namespace VCGL {

struct Preferences {
	TransferFunctionObject correlationViewTF;
	TransferFunctionObject teleconnectivityViewTF;
	RGBF referencePointColor;
	RGBF correlationViewLineColor;
	RGBF correlationViewOddPointColor;
	RGBF correlationViewEvenPointColor;
	RGBF teleconnectivityViewLineColor;
	RGBF teleconnectivityViewStartPointColor;
	RGBF teleconnectivityViewEndPointColor;
	float significanceThreshold;
	int mapType;
	float corrMapRefPointSize;
	float corrMapPointSize;
	float corrMapLineSize;
	float tcMapRefPointSize;
	float tcMapPointSize;
	float tcMapLineSize;
	float projPointSize;
	float projRefPointSize;
	float projChainPointSize;
	float projChainLineSize;

	static void GetDefaults(Preferences* pPreferences);
};

} /* namespace VCGL */

#endif /* PREFERENCES_H_ */
