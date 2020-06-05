/*! @file fakepreferencepaneview.h
 * @date Created on Apr 30, 2014
 * @author anantonov
 *
 * @brief TODO: add description
 */

#ifndef FAKEPREFERENCEPANEVIEW_H_
#define FAKEPREFERENCEPANEVIEW_H_

#include "preferences/preferencepanelogic.h"
#include "preferences/preferences.h"

namespace Testing {
	class FakePreferencePaneView: public VCGL::PreferencePaneView {
	public:
		FakePreferencePaneView(): bUpdateCalled(false) {}

		virtual void updateView(const VCGL::Preferences& /*preferences*/) override {
			bUpdateCalled = true;
		}

		virtual ~FakePreferencePaneView() {}

		bool bUpdateCalled;
	};

} //namespace Testing

#endif /* FAKEPREFERENCEPANEVIEW_H_ */
