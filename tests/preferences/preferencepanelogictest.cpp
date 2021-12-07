/*! @file preferencepanelogictest.cpp
 * @date Created on Apr 28, 2014
 * @author anantonov
 *
 * @brief TODO: add description
 */

#include "CppUnitLite/TestHarness.h"
#include "preferencepanelogic.h"
#include "preferences.h"

#include <vector>
#include "colorizer/transferfunctionobject.h"
#include "colorizer/rgb.h"

#include "fakepreferencepaneview.h"
#include <cmath>

namespace Testing {

bool operator== (const VCGL::Preferences& a, const VCGL::Preferences& b) {
	return (
			a.correlationViewTF == b.correlationViewTF
			&& a.teleconnectivityViewTF == b.teleconnectivityViewTF
			&& a.referencePointColor == b.referencePointColor
			&& a.correlationViewLineColor == b.correlationViewLineColor
			&& a.correlationViewOddPointColor == b.correlationViewOddPointColor
			&& a.correlationViewEvenPointColor == b.correlationViewEvenPointColor
			&& a.teleconnectivityViewLineColor == b.teleconnectivityViewLineColor
			&& a.teleconnectivityViewStartPointColor == b.teleconnectivityViewStartPointColor
			&& a.teleconnectivityViewEndPointColor == b.teleconnectivityViewEndPointColor
			&& fabs(a.significanceThreshold - b.significanceThreshold) < 1e-6f );
}

TEST(Init, PreferencePaneLogic) {
	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	predefinedTFs.push_back(VCGL::TransferFunctionObject::HeatTFO());
	predefinedTFs.push_back(VCGL::TransferFunctionObject::BlueWhiteRedTFO());
	predefinedTFs.push_back(VCGL::TransferFunctionObject::FlameTFO());

	VCGL::Preferences inPreferences;
	inPreferences.correlationViewTF = VCGL::TransferFunctionObject::FlameTFO();
	inPreferences.teleconnectivityViewTF = VCGL::TransferFunctionObject::BlueWhiteRedTFO();
	inPreferences.referencePointColor = { 0.7f, 0.1f, 0.7f };
	inPreferences.correlationViewLineColor = { 0.4f, 0.8f, 0.1f };
	inPreferences.correlationViewOddPointColor = {0.3f, 0.05f, 0.2f};
	inPreferences.correlationViewEvenPointColor = {0.2f, 0.8f, 0.9f};
	inPreferences.teleconnectivityViewLineColor = { 0.95f, 0.1f, 0.15f };
	inPreferences.teleconnectivityViewStartPointColor = {0.17f, 0.15f, 0.2f};
	inPreferences.teleconnectivityViewEndPointColor = {0.8f, 0.05f, 0.54f };
	inPreferences.significanceThreshold = 0.95;

	VCGL::PreferencePaneLogic paneLogic(predefinedTFs);
	paneLogic.init(inPreferences);

	VCGL::Preferences outPreferences;
	paneLogic.getPreferences(&outPreferences);

	CHECK(inPreferences == outPreferences);
}

TEST(ReturnDefaultTF, PreferencePaneLogic)
{
	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	predefinedTFs.push_back(VCGL::TransferFunctionObject::HeatTFO());

	VCGL::PreferencePaneLogic paneLogic(predefinedTFs);
	VCGL::Preferences preferences;

	paneLogic.getPreferences(&preferences);

	CHECK(predefinedTFs[0] == preferences.correlationViewTF);
}

TEST(selectCorrelationViewTF, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	predefinedTFs.push_back(VCGL::TransferFunctionObject::HeatTFO());
	predefinedTFs.push_back(VCGL::TransferFunctionObject::BlueWhiteRedTFO());
	predefinedTFs.push_back(VCGL::TransferFunctionObject::FlameTFO());

	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	view.bUpdateCalled = false;
	paneLogic.selectCorrelationViewTF(1);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);
	CHECK(predefinedTFs[1] == preferences.correlationViewTF);
}


TEST(selectTeleconnectivityViewTransferFunction, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	predefinedTFs.push_back(VCGL::TransferFunctionObject::HeatTFO());
	predefinedTFs.push_back(VCGL::TransferFunctionObject::BlueWhiteRedTFO());
	predefinedTFs.push_back(VCGL::TransferFunctionObject::FlameTFO());

	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	view.bUpdateCalled = false;
	paneLogic.selectTeleconnectivityViewTF(2);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(predefinedTFs[2] == preferences.teleconnectivityViewTF);
}

TEST(setReferencePointColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF green = { 0.0, 1.0, 0.0 };
	view.bUpdateCalled = false;
	paneLogic.setReferencePointColor(green);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(green == preferences.referencePointColor);
}

TEST(setCorrelationViewLineColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF yellow = { 1.0, 1.0, 0.0 };
	view.bUpdateCalled = false;
	paneLogic.setCorrelationViewLineColor(yellow);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(yellow == preferences.correlationViewLineColor);
}

TEST(setCorrelationViewOddPointColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF cyan = { 0.0, 1.0, 1.0 };
	view.bUpdateCalled = false;
	paneLogic.setCorrelationViewOddPointColor(cyan);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(cyan == preferences.correlationViewOddPointColor);
}

TEST(setCorrelationViewEvenPointColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF yellow = { 1.0, 1.0, 0.0 };
	view.bUpdateCalled = false;
	paneLogic.setCorrelationViewEvenPointColor(yellow);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(yellow == preferences.correlationViewEvenPointColor);
}

TEST(setTeleconnectivityViewLineColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF yellow = { 1.0, 1.0, 0.0 };
	view.bUpdateCalled = false;
	paneLogic.setTeleconnectivityViewLineColor(yellow);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(yellow == preferences.teleconnectivityViewLineColor);
}

TEST(setTeleconnectivityViewStartPointColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF cyan = { 0.0, 1.0, 1.0 };
	view.bUpdateCalled = false;
	paneLogic.setTeleconnectivityViewStartPointColor(cyan);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(cyan == preferences.teleconnectivityViewStartPointColor);
}

TEST(setTeleconnectivityViewEndPointColor, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	VCGL::RGBF red = { 1.0, 0.0, 1.0 };
	view.bUpdateCalled = false;
	paneLogic.setTeleconnectivityViewEndPointColor(red);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	CHECK(red == preferences.teleconnectivityViewEndPointColor);
}

TEST(setSignificanceThreshold, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	view.bUpdateCalled = false;
	paneLogic.setSignificanceThreshold(0.777);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	DOUBLES_EQUAL(0.777, preferences.significanceThreshold, 1e-6);
}

TEST(setMapType, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;

	std::vector< VCGL::TransferFunctionObject > predefinedTFs;
	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);

	view.bUpdateCalled = false;
	paneLogic.setMapType(1);
	CHECK(true == view.bUpdateCalled);

	VCGL::Preferences preferences;
	paneLogic.getPreferences(&preferences);

	LONGS_EQUAL(1L, preferences.mapType);
}

TEST(callsUpdateView, PreferencePaneLogic) {
	Testing::FakePreferencePaneView view;
	std::vector< VCGL::TransferFunctionObject > predefinedTFs;

	CHECK(false == view.bUpdateCalled);

	VCGL::PreferencePaneLogic paneLogic(predefinedTFs, &view);
	CHECK(true == view.bUpdateCalled);

}

} // namespace Testing

//Test ideas:

// pass on the current settings as a configuration object
// the transfer functions (which IS TransferFunctionObject) are recognized and set to the respective editors

// ***interesting possible change -> affects TransferFunctionEditorWidget more
// pass on the current transfer function (generic IColorizer, not TransferFunctionObject)
// it is set to the widget for viewing, without editing
