/*! @file transferfunctioneditortest.cpp
 * @date Created on Apr 29, 2014
 * @author anantonov
 *
 * @brief Tests for the logic of the transfer editor widget
 */

#include "colorizer/transferfunctioneditor.h"

#include "CppUnitLite/TestHarness.h"
#include "colorizer/transferfunctioneditorwidget.h"
#include "colorizer/transferfunctionobject.h"

namespace Testing {

TEST(getTFCount, TransferFunctionEditor)
{
	VCGL::TransferFunctionEditor editor;
	LONGS_EQUAL(0, editor.getTFCount());

    std::vector<VCGL::TransferFunctionObject> tfCollection(2);
    editor.setCollection(tfCollection);

    LONGS_EQUAL(2, editor.getTFCount());
}

TEST(currentTF, TransferFunctionEditor)
{
	VCGL::TransferFunctionEditor editor;
	std::vector<VCGL::TransferFunctionObject> tfCollection;

	VCGL::TransferFunctionObject tfo1;
	tfo1.setName("tfo1");
	tfCollection.push_back(tfo1);

	VCGL::TransferFunctionObject tfo2;
	tfo2.setName("tfo2");
	tfCollection.push_back(tfo2);

    editor.setCollection(tfCollection);


    editor.setCurrentTFIndex(0);
    VCGL::TransferFunctionObject* pCurrent0 = editor.getCurrentTF();
    CHECK( pCurrent0 != 0 && tfo1 == (*pCurrent0) );

    editor.setCurrentTFIndex(1);
    VCGL::TransferFunctionObject* pCurrent1 = editor.getCurrentTF();
	CHECK( pCurrent1 != 0 && tfo2 == (*pCurrent1) );

}

TEST(currentTFIndex, TransferFunctionEditor)
{
	VCGL::TransferFunctionEditor editor;
	std::vector<VCGL::TransferFunctionObject> tfCollection;

	VCGL::TransferFunctionObject tfo1;
	tfo1.setName("tfo1");
	tfCollection.push_back(tfo1);

	VCGL::TransferFunctionObject tfo2;
	tfo2.setName("tfo2");
	tfCollection.push_back(tfo2);

    editor.setCollection(tfCollection);

    editor.setCurrentTFIndex(0);
    CHECK( 0 == editor.getCurrentTFIndex() );

    editor.setCurrentTFIndex(1);
    CHECK( 1 == editor.getCurrentTFIndex() );

}

TEST(currentTFObject, TransferFunctionEditor)
{
	VCGL::TransferFunctionEditor editor;
		std::vector<VCGL::TransferFunctionObject> tfCollection;

		VCGL::TransferFunctionObject tfo1;
		tfo1.setName("tfo1");
		tfCollection.push_back(tfo1);

		VCGL::TransferFunctionObject tfo2;
		tfo2.setName("tfo2");
		tfCollection.push_back(tfo2);

	    editor.setCollection(tfCollection);
	    LONGS_EQUAL(2, editor.getTFCount());

	    editor.setCurrentTFIndex(0);
	    VCGL::TransferFunctionObject* pCurrentTF0 = editor.getCurrentTF();
	    CHECK( pCurrentTF0 != 0 && !(tfo2 == (*pCurrentTF0)) );

	    editor.setCurrentTFObject(&tfo2);

	    LONGS_EQUAL(2, editor.getTFCount());
	    VCGL::TransferFunctionObject* pCurrentTF1 = editor.getCurrentTF();
		CHECK( pCurrentTF1 != 0 && tfo2 == (*pCurrentTF1) );

		VCGL::TransferFunctionObject tfo3;
		tfo3.setName("tfo3");
		editor.setCurrentTFObject(&tfo3);

		LONGS_EQUAL(3, editor.getTFCount());
		VCGL::TransferFunctionObject* pCurrentTF2 = editor.getCurrentTF();
		CHECK( pCurrentTF2 != 0 && tfo3 == (*pCurrentTF2) );
}

} // namespace Testing
