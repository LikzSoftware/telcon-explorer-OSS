/*! @file preferences.cpp
 * @date Created on Apr 30, 2014
 * @author anantonov
 *
 * @brief TODO: add description
 */

#include "preferences.h"

namespace VCGL {
	void Preferences::GetDefaults(Preferences* pPreferences) {
		pPreferences->correlationViewTF = TransferFunctionObject::BlueWhiteRedTFO();
		pPreferences->teleconnectivityViewTF = TransferFunctionObject::FlameTFO();
		pPreferences->referencePointColor = { 0.0f, 0.9f, 0.0f };
		pPreferences->correlationViewLineColor = { 0.0f, 0.0f, 0.0f };
		pPreferences->correlationViewOddPointColor = { 0.0, 1.0, 1.0 };
		pPreferences->correlationViewEvenPointColor = { 1.0f, 1.0f, 0.0f };
		pPreferences->teleconnectivityViewLineColor = { 0.0f, 0.7f, 0.0f };
		pPreferences->teleconnectivityViewStartPointColor = { 0.0f, 17.0f/255.0f, 199.0f/255.0f };
		pPreferences->teleconnectivityViewEndPointColor = { 0.0f, 0.5f, 0.0f };

		pPreferences->significanceThreshold = 0.99;
		pPreferences->mapType = 0;

		pPreferences->corrMapRefPointSize = 8.0;
		pPreferences->corrMapPointSize = 4.0;
		pPreferences->corrMapLineSize = 1.0;

		pPreferences->tcMapRefPointSize = 10.0;
		pPreferences->tcMapPointSize = 8.0;
		pPreferences->tcMapLineSize = 4.0;

		pPreferences->projPointSize = 5.0;
		pPreferences->projRefPointSize = 6.0;
		pPreferences->projChainPointSize = 3.0;
		pPreferences->projChainLineSize = 1.0;
	}
}



