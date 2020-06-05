/*! @file preferencepanelogic.cpp
 * @date Created on Apr 28, 2014
 * @author anantonov
 *
 * @brief TODO: add description
 */

#include "preferences/preferencepanelogic.h"
#include "colorizer/transferfunctioneditor.h"

#include <cassert>

namespace VCGL {

PreferencePaneLogic::PreferencePaneLogic(const std::vector< VCGL::TransferFunctionObject >& predefinedTransferFunctions,
		PreferencePaneView* pPreferencePaneView)
: pView(pPreferencePaneView), pCorrelationTFEditor(0), pTeleconnectivityTFEditor(0)
{
	Preferences::GetDefaults(&currentPreferences);

	if (predefinedTransferFunctions.size()>0) {
		pCorrelationTFEditor = new TransferFunctionEditor();
		pCorrelationTFEditor->setCollection(predefinedTransferFunctions);
		pCorrelationTFEditor->setCurrentTFIndex(0);

		pTeleconnectivityTFEditor = new TransferFunctionEditor();
		pTeleconnectivityTFEditor->setCollection(predefinedTransferFunctions);
		pTeleconnectivityTFEditor->setCurrentTFIndex(0);

		currentPreferences.correlationViewTF = (*(pCorrelationTFEditor->getCurrentTF()));
		currentPreferences.teleconnectivityViewTF = (*(pTeleconnectivityTFEditor->getCurrentTF()));
	}
	updateView();
}

PreferencePaneLogic::PreferencePaneLogic(TransferFunctionEditor* pCorrelationTFEditor,
				TransferFunctionEditor* pTeleconnectivityTFEditor,
				PreferencePaneView* pPreferencePaneView) {
	Preferences::GetDefaults(&currentPreferences);

	this->pCorrelationTFEditor = pCorrelationTFEditor;
	this->pTeleconnectivityTFEditor = pTeleconnectivityTFEditor;
	this->pView = pPreferencePaneView;

	if (pCorrelationTFEditor != 0) {
		currentPreferences.correlationViewTF = (*(pCorrelationTFEditor->getCurrentTF()));
	}

	if (pTeleconnectivityTFEditor != 0) {
		currentPreferences.teleconnectivityViewTF = (*(pTeleconnectivityTFEditor->getCurrentTF()));
	}
	updateView();
}

void
PreferencePaneLogic::init(const VCGL::Preferences& inPreferences) {
	this->currentPreferences = inPreferences;
	if (pCorrelationTFEditor != 0) {
		this->pCorrelationTFEditor->setCurrentTFObject(&inPreferences.correlationViewTF);
	}
	if (pTeleconnectivityTFEditor != 0) {
		this->pTeleconnectivityTFEditor->setCurrentTFObject(&inPreferences.teleconnectivityViewTF);
	}
	updateView();
}

void
PreferencePaneLogic::getPreferences(VCGL::Preferences* pPreferences) {
	assert(pPreferences != 0);
	//assert(pCorrelationTFEditor != 0);
	//assert(pTeleconnectivityTFEditor != 0);

	if (pPreferences != 0) {
		*pPreferences = currentPreferences;
		if (pCorrelationTFEditor != 0) {
			pPreferences->correlationViewTF = (*(pCorrelationTFEditor->getCurrentTF()));
		}
		if (pTeleconnectivityTFEditor != 0) {
			pPreferences->teleconnectivityViewTF = (*(pTeleconnectivityTFEditor->getCurrentTF()));
		}
	}
}

void PreferencePaneLogic::selectCorrelationViewTF(unsigned i) {
	assert(pCorrelationTFEditor != 0);
	pCorrelationTFEditor->setCurrentTFIndex(i);
	updateView();
}

void PreferencePaneLogic::selectTeleconnectivityViewTF(unsigned i) {
	assert(pTeleconnectivityTFEditor != 0);
	pTeleconnectivityTFEditor->setCurrentTFIndex(i);
	updateView();
}

void PreferencePaneLogic::setReferencePointColor(const RGBF& color) {
	currentPreferences.referencePointColor = color;
	updateView();
}

void PreferencePaneLogic::setCorrelationViewLineColor(const RGBF& color) {
	currentPreferences.correlationViewLineColor = color;
	updateView();
}

void PreferencePaneLogic::setCorrelationViewOddPointColor(const RGBF& color) {
	currentPreferences.correlationViewOddPointColor = color;
	updateView();
}

void PreferencePaneLogic::setCorrelationViewEvenPointColor(const RGBF& color) {
	currentPreferences.correlationViewEvenPointColor = color;
	updateView();
}

void PreferencePaneLogic::setTeleconnectivityViewLineColor(const RGBF& color) {
	currentPreferences.teleconnectivityViewLineColor = color;
	updateView();
}

void PreferencePaneLogic::setTeleconnectivityViewStartPointColor(const RGBF& color) {
	currentPreferences.teleconnectivityViewStartPointColor = color;
	updateView();
}

void PreferencePaneLogic::setTeleconnectivityViewEndPointColor(const RGBF& color) {
	currentPreferences.teleconnectivityViewEndPointColor = color;
	updateView();
}

void PreferencePaneLogic::setSignificanceThreshold(float threshold) {
	currentPreferences.significanceThreshold = threshold;
	updateView();
}

void PreferencePaneLogic::setMapType(int type) {
	currentPreferences.mapType = type;
	updateView();
}

void PreferencePaneLogic::updateView() {
	if (pView != 0) {
		Preferences prefs;
		getPreferences(&prefs);
		pView->updateView(prefs);
	}
}



} /* namespace VCGL */
