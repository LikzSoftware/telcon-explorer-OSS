/*! @file transferfunctionobjecttest.cpp
 * @date Created on Apr 24, 2014
 * @author anantonov
 *
 * @brief Unit tests for transfer function
 */

#include "CppUnitLite/TestHarness.h"
#include "colorizer/transferfunctionobject.h"

namespace Testing {

TEST(ContinuousBasic, TransferFunctionObject)
{
    VCGL::TransferFunctionObject tfo;

    CHECK (tfo.getMode() == VCGL::IColorizer::CONTINUOUS);

    VCGL::TFRecord startRec(0.0f, 0.0f, 0.0f, 0.0f);
    VCGL::TFRecord endRec(1.0f, 1.0f, 1.0f, 1.0f);
    tfo.addTFRecord(startRec);
    tfo.addTFRecord(endRec);

    float r = 0.0f, g = 0.0f, b=0.0f;

    tfo.colorize(0.5f, &r, &g, &b);

    DOUBLES_EQUAL(0.50f, r, 0.001f);
    DOUBLES_EQUAL(0.50f, g, 0.001f);
    DOUBLES_EQUAL(0.50f, b, 0.001f);
}

TEST(BandedMidpoint, TransferFunctionObject)
{
    VCGL::TransferFunctionObject tfo;
    tfo.setMode(VCGL::IColorizer::BANDED_MIDPOINT);

    VCGL::TFRecord startRec(0.0f, 0.0f, 0.0f, 0.0f);
    VCGL::TFRecord endRec(1.0f, 1.0f, 1.0f, 1.0f);
    tfo.addTFRecord(startRec);
    tfo.addTFRecord(endRec);

    float r = 0.0f, g = 0.0f, b=0.0f;

    tfo.colorize(0.25f, &r, &g, &b);

    DOUBLES_EQUAL(0.0f, r, 0.001f);
    DOUBLES_EQUAL(0.0f, g, 0.001f);
    DOUBLES_EQUAL(0.0f, b, 0.001f);

    r = 0.0f; g = 0.0f; b=0.0f;
    tfo.colorize(0.75f, &r, &g, &b);

   DOUBLES_EQUAL(1.0f, r, 0.001f);
   DOUBLES_EQUAL(1.0f, g, 0.001f);
   DOUBLES_EQUAL(1.0f, b, 0.001f);
}

TEST(BandedSides, TransferFunctionObject)
{
    VCGL::TransferFunctionObject tfo;
    tfo.setMode(VCGL::IColorizer::BANDED_SIDES);

    VCGL::TFRecord startRec(0.0f, 0.0f, 0.0f, 0.0f);
    VCGL::TFRecord midRec(0.5f, 0.3f, 0.3f, 0.3f);
    VCGL::TFRecord endRec(1.0f, 1.0f, 1.0f, 1.0f);
    tfo.addTFRecord(startRec);
    tfo.addTFRecord(midRec);
    tfo.addTFRecord(endRec);

    float r = 0.0f, g = 0.0f, b=0.0f;

    tfo.colorize(0.25f, &r, &g, &b);

    DOUBLES_EQUAL(0.0f, r, 0.001f);
    DOUBLES_EQUAL(0.0f, g, 0.001f);
    DOUBLES_EQUAL(0.0f, b, 0.001f);

    r = 0.0f; g = 0.0f; b=0.0f;
    tfo.colorize(0.75f, &r, &g, &b);

   DOUBLES_EQUAL(0.3f, r, 0.001f);
   DOUBLES_EQUAL(0.3f, g, 0.001f);
   DOUBLES_EQUAL(0.3f, b, 0.001f);
}

TEST(Equals, TransferFunctionObject)
{
    VCGL::TFRecord startRec(0.0f, 0.0f, 0.0f, 0.0f);
    VCGL::TFRecord endRec(1.0f, 1.0f, 1.0f, 1.0f);

    VCGL::TransferFunctionObject tfo1;
    tfo1.addTFRecord(startRec);
	tfo1.addTFRecord(endRec);
	tfo1.setName("greyscale");

	VCGL::TransferFunctionObject tfo2;
	tfo2.addTFRecord(startRec);
	tfo2.addTFRecord(endRec);
	tfo2.setName("greyscale");

	CHECK(tfo1 == tfo2);
}

} //namespace Testing
