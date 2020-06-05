/*!	@file transferfunctionobject.cpp
 *	@author anantonov
 *	@date	Feb 19, 2014 (created)
 *	@brief	Implementation of the transfer function
 */


#include "transferfunctionobject.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace VCGL {

TransferFunctionObject
TransferFunctionObject::HeatTFO() {
	TransferFunctionObject tfo;
	tfo.addTFRecord( TFRecord(0.0f, 0.0f, 0.0f, 0.0f) );
	tfo.addTFRecord( TFRecord(0.4f, 1.0f, 0.0f, 0.0f) );
	tfo.addTFRecord( TFRecord(0.75f, 1.0f, 1.0f, 0.0f) );
	tfo.addTFRecord( TFRecord(1.0f, 1.0f, 1.0f, 1.0f) );
	return tfo;
}

TransferFunctionObject
TransferFunctionObject::BlueWhiteRedTFO() {
	TransferFunctionObject tfo;
	tfo.addTFRecord( TFRecord(0.0f, 0.0f, 0.0f, 1.0f) );
	tfo.addTFRecord( TFRecord(0.5f, 1.0f, 1.0f, 1.0f) );
	tfo.addTFRecord( TFRecord(1.0f, 1.0f, 0.0f, 0.0f) );
	return tfo;
}

TransferFunctionObject
TransferFunctionObject::FlameTFO() {
	TransferFunctionObject tfo;
	tfo.addTFRecord( TFRecord(0.0f, 0.0f, 0.0f, 0.0f) );
	tfo.addTFRecord( TFRecord(0.2f, 0.294117647f, 0.0f, 0.509803922f) );
	tfo.addTFRecord( TFRecord(0.4f, 1.0f, 0.0f, 0.0f) );
	tfo.addTFRecord( TFRecord(0.6f, 1.0f, 0.647058824f, 0.0f) );
	tfo.addTFRecord( TFRecord(0.8f, 1.0f, 1.0f, 0.0f) );
	tfo.addTFRecord( TFRecord(1.0f, 1.0f, 1.0f, 1.0f) );
	return tfo;
}

TransferFunctionObject::TransferFunctionObject(): transferFunction(), mode(VCGL::IColorizer::CONTINUOUS), name("") {}

TransferFunctionObject::TransferFunctionObject(const TransferFunctionObject& other)
: transferFunction(other.transferFunction), mode(other.mode), name(other.name) {}

const TransferFunctionObject& TransferFunctionObject::operator= (const TransferFunctionObject& other) {
	if (this != (&other)) {
		transferFunction = other.transferFunction;
		mode = other.mode;
		name = other.name;
	}

	return *this;
}


void
TransferFunctionObject::colorize(float dataValue, float* pr, float* pg, float* pb) const {
	*pr = 0;
	*pg = 0;
	*pb = 0;

	if(dataValue<=transferFunction[0].pos) {
		const TFRecord& tfr = transferFunction[0];
		*pr = tfr.qclr.redF();
		*pb = tfr.qclr.blueF();
		*pg = tfr.qclr.greenF();
	}
	else if(dataValue>=transferFunction.back().pos) {
		const TFRecord& tfr = transferFunction.back();
		*pr = tfr.qclr.redF();
		*pb = tfr.qclr.blueF();
		*pg = tfr.qclr.greenF();
	}
	else {
		assert ( transferFunction.size() >= 2 ); //interpolation below won't work if we have less than 2 samples
		unsigned ind = 1;
		while (ind < transferFunction.size()-1 && dataValue > transferFunction[ind].pos) {
			ind++;
		}

		const TFRecord& tfrLeft = transferFunction[ind-1];
		const TFRecord& tfrRight = transferFunction[ind];

		double tmp = (dataValue - tfrLeft.pos) / (tfrRight.pos - tfrLeft.pos);

		switch (mode) {
			case VCGL::IColorizer::CONTINUOUS:
				*pr = tfrLeft.qclr.redF() * (1.0f-tmp) + tfrRight.qclr.redF() * tmp;
				*pg = tfrLeft.qclr.greenF() * (1.0f-tmp) + tfrRight.qclr.greenF() * tmp;
				*pb = tfrLeft.qclr.blueF() * (1.0f-tmp) + tfrRight.qclr.blueF() * tmp;
				//TODO: add alpha channel return
				// *pa = tfrLeft.qclr.alphaF() * (1.0f-tmp) + tfrRight.qclr.alphaF() * tmp);
				break;
			case VCGL::IColorizer::BANDED_MIDPOINT:
			{
				QColor color;
				if (tmp < 0.5) {
					color = tfrLeft.qclr;
				} else {
					color = tfrRight.qclr;
				}
				*pr = color.redF();
				*pg = color.greenF();
				*pb = color.blueF();
				// *pa = color.alphaF();
			}
			break;
			case VCGL::IColorizer::BANDED_SIDES:
			{
				QColor color = tfrLeft.qclr;
				*pr = color.redF();
				*pg = color.greenF();
				*pb = color.blueF();
			}
			break;


			default:
				std::cerr << "TransferFunctionObject::colorize Unknown mode value: " << (int)mode << std::endl;
				assert(0 || !"TransferFunctionObject::colorize Unknown mode value!");
			break;
		}
	}
}

const TFRecord& TransferFunctionObject::getTFRecord(unsigned i) const {
	return transferFunction[i];
}

void TransferFunctionObject::addTFRecord(const TFRecord& tfr) {
	transferFunction.push_back(tfr);
}

void TransferFunctionObject::insertTFRecord(unsigned index, const TFRecord& tfr) {
	assert (index <= transferFunction.size());
	transferFunction.insert(transferFunction.begin() + index, tfr);
}

void TransferFunctionObject::removeTFRecord(unsigned index) {
	assert (index < transferFunction.size());
	transferFunction.erase(transferFunction.begin() + index);
}

unsigned TransferFunctionObject::getRecordsCount() const {
	return transferFunction.size();
}

void TransferFunctionObject::getRecordColor(unsigned i, float* pr, float* pg, float* pb) const {
	assert (i < transferFunction.size());
	const TFRecord& tfr = transferFunction[i];
	*pr = tfr.qclr.redF();
	*pb = tfr.qclr.blueF();
	*pg = tfr.qclr.greenF();
}

void TransferFunctionObject::setRecordColor(unsigned i, float r, float g, float b) {
	assert (i < transferFunction.size());
	transferFunction[i].qclr.setRgbF(r,g,b,1.0);
}

float TransferFunctionObject::getRecordPos(unsigned i) const {
	assert (i < transferFunction.size());
	return transferFunction[i].pos;
}

void TransferFunctionObject::setRecordPos(unsigned i, float newPos) {
	assert (i < transferFunction.size());
	transferFunction[i].pos = newPos;
}

enum VCGL::IColorizer::ColorizerMode TransferFunctionObject::getMode() const {
	return mode;
}
void TransferFunctionObject::setMode(enum VCGL::IColorizer::ColorizerMode newMode) {
	mode = newMode;
}

const std::string&
TransferFunctionObject::getName() const {
	return name;
}

void
TransferFunctionObject::setName(const std::string& newName) {
	name = newName;
}

bool TransferFunctionObject::operator==(const VCGL::TransferFunctionObject& other) const {
	bool bResult = true;
	if (bResult && mode != other.mode) { bResult = false; }
	if (bResult && name != other.name) { bResult = false; }
	if (bResult && transferFunction.size() != other.transferFunction.size()) { bResult = false; }
	for (unsigned i = 0; bResult && i<transferFunction.size(); i++) {
		if (fabs(transferFunction[i].pos - other.transferFunction[i].pos)>0.0001 ||
				transferFunction[i].qclr != other.transferFunction[i].qclr) {
			bResult = false;
		}
	}
	return bResult;
}

} // namespace VCGL
