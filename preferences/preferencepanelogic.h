/*! @file preferencepanelogic.h
 * @date Created on Apr 28, 2014
 * @author anantonov
 *
 * @brief TODO: add description
 */

#ifndef PREFERENCEPANELOGIC_H_
#define PREFERENCEPANELOGIC_H_

#include <vector>
#include "colorizer/transferfunctionobject.h"
#include "preferences.h"

namespace VCGL {

class TransferFunctionEditor;

struct PreferencePaneView {
	virtual void updateView(const Preferences& preferences) = 0;
	virtual ~PreferencePaneView() {}
};

class PreferencePaneLogic {
public:
	PreferencePaneLogic(const std::vector< VCGL::TransferFunctionObject >& predefinedTransferFunctions,
			PreferencePaneView* pPreferencePaneView = 0);

	PreferencePaneLogic(TransferFunctionEditor* pCorrelationTFEditor = 0,
				TransferFunctionEditor* pTeleconnectivityTFEditor = 0,
				PreferencePaneView* pPreferencePaneView = 0);

	void init(const VCGL::Preferences& inPreferences);

	void getPreferences(VCGL::Preferences* pPreferences);

	void selectCorrelationViewTF(unsigned i);
	void selectTeleconnectivityViewTF(unsigned i);
	void setReferencePointColor(const RGBF& color);
	void setCorrelationViewLineColor(const RGBF& color);
	void setCorrelationViewOddPointColor(const RGBF& color);
	void setCorrelationViewEvenPointColor(const RGBF& color);
	void setTeleconnectivityViewLineColor(const RGBF& color);
	void setTeleconnectivityViewStartPointColor(const RGBF& color);
	void setTeleconnectivityViewEndPointColor(const RGBF& color);
	void setSignificanceThreshold(float threshold);
	void setMapType(int type);

private:
	void updateView();

	Preferences currentPreferences;
	PreferencePaneView* pView;
	TransferFunctionEditor* pCorrelationTFEditor;
	TransferFunctionEditor* pTeleconnectivityTFEditor;
};

} /* namespace VCGL */

#endif /* PREFERENCEPANELOGIC_H_ */
