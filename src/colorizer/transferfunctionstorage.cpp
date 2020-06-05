/*! @file transferfunctionstorage.cpp
 * @date Created on Apr 29, 2014
 * @author anantonov
 *
 * @brief Implementation of the methods for loading transfer functions from an external source
 */

#include "transferfunctionstorage.h"

#include <fstream>
#include <sstream>
#include <QString>

namespace VCGL {

void TransferFunctionStorage::LoadFromStream(std::istream& inStream, bool silent, std::vector<TransferFunctionObject>& outTFCollection) {
	std::string str;
	std::istringstream iss;
	std::string strName;
	do {
		if(!getline(inStream,str)) {
			break;
		}

		if(!silent) {
			std::cerr << "Line " << str << std::endl;
		}
		QString qstr = QString::fromStdString(str).simplified(); //remove whitespaces from start and end

		if(qstr.startsWith("#") || qstr.startsWith("//") || qstr.startsWith("\\") || qstr.isEmpty()) {
			if(!silent) {
				std::cerr << "Ignore comment" << std::endl;
			}
			continue;
		} else if ( qstr.startsWith("==") ) {
			if (!silent) {
				std::cerr << "TF name" << std::endl;
			}
			qstr.remove(0,2);
			strName = qstr.toStdString();
			continue;
		}

		iss.clear();
		iss.str(qstr.toStdString());

		short num;
		iss >> num;

		VCGL::TransferFunctionObject tfo;
		tfo.setName(strName);
		strName = "";
		for(short i=0;i<num;++i)
		{
			getline(inStream,str);
			if (!silent) {
				std::cerr << "Reading record " << i << ": " << str << std::endl;
			}
			QString qstr = QString::fromStdString(str).simplified(); //remove whitespaces from start and end
			if (qstr.startsWith("#") || qstr.startsWith("//") || qstr.startsWith("\\") || qstr.isEmpty()) {
				if (!silent) {
					std::cerr << "Ignore comment" << std::endl;
				}
				continue;
			}

			iss.clear();
			iss.str(str);

			VCGL::TFRecord tfr;
			float r=0.0f, g=0.0f, b=0.0f, a=0.0f;
			iss >> tfr.pos >> r >> g >> b >> a;
			tfr.qclr.setRgbF(r,g,b,a);
			tfo.addTFRecord(tfr);
		}

		//TODO: store/read mode from file
		const enum VCGL::IColorizer::ColorizerMode DEFAULT_MODE = VCGL::IColorizer::CONTINUOUS;
		tfo.setMode(DEFAULT_MODE);

		outTFCollection.push_back(tfo);
		if (!silent) {
			std::cerr << "Current size: " << outTFCollection.size() << std::endl;
		}

	} while(true);

	if (!silent) {
		std::cerr << "Final size: " << outTFCollection.size() << std::endl;
	}
}

void TransferFunctionStorage::LoadFromFile(std::string fileName, bool silent, std::vector<TransferFunctionObject>& outTFCollection) {
	std::cerr << "TransferFunctionStorage::LoadFromFile(): reading file " << fileName << std::endl;
    std::ifstream myfile(fileName.c_str());
    if(myfile.is_open()) {
    	LoadFromStream(myfile, silent, outTFCollection);
		myfile.close();
	}
	else {
		std::cerr << "TransferFunctionStorage::LoadFromFile(): cannot open file " << fileName << std::endl;
	}
}

} /* namespace VCGL */
