/*! @file preferencestorage.cpp
 * @author anantonov
 * @date Created on Jun 30, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "preferences.h"
#include "preferencestorage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "storage/cfgfileparser.h"
#include <cassert>


namespace {

	void storeTFOPreference(std::ostream& out, const std::string& preferenceName, const VCGL::TransferFunctionObject& tfo) {
		out << preferenceName << "=";
		out << tfo.getName() << ':';

		switch (tfo.getMode()) {
		case VCGL::IColorizer::CONTINUOUS:
			//print nothing, this is the default
			break;
		case VCGL::IColorizer::BANDED_MIDPOINT:
			out << "BM";
			break;
		case VCGL::IColorizer::BANDED_SIDES:
			out << "BS";
			break;
		default:
			std::cerr << "Warning: storing preferences: unknown transfer function mode, resetting to default" << std::endl;
			break;
		}
		out << ':';

		for (unsigned i=0; i< tfo.getRecordsCount(); i++) {
			const VCGL::TFRecord& tfr = tfo.getTFRecord(i);
			out << tfr.pos << '*' << '(' <<
					tfr.qclr.redF() << ';' <<
					tfr.qclr.greenF() << ';' <<
					tfr.qclr.blueF() << ')';
			if (i < tfo.getRecordsCount()-1) {
				out << ':';
			}
		}
		out << std::endl;
	}

	void storeColorPreference(std::ostream& out, const std::string& preferenceName, const VCGL::RGBF& color) {
		out << preferenceName << "=";
		out << '(' <<
			color.r << ';' <<
			color.g << ';' <<
			color.b << ')';
		out << std::endl;
	}
}

namespace VCGL {
void PreferenceStorage::load(const std::string& fileName, Preferences* pPreferences) {
	assert(pPreferences != 0);
	Preferences::GetDefaults(pPreferences);

	CfgFileParser parser;
	if (parser.readConfig(fileName.c_str())) {
		const unsigned bufSize = 1000;
		char buffer[bufSize];

		if (parser.getAttr("correlationViewTF", 's', bufSize, buffer )) {
			parseTFO(buffer, bufSize, &pPreferences->correlationViewTF);
		}

		if (parser.getAttr("teleconectivityViewTF", 's', bufSize, buffer )) {
			parseTFO(buffer, bufSize, &pPreferences->teleconnectivityViewTF);
		}

		if (parser.getAttr("referencePointColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->referencePointColor);
		}

		if (parser.getAttr("correlationViewLineColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->correlationViewLineColor);
		}
		if (parser.getAttr("correlationViewOddPointColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->correlationViewOddPointColor);
		}
		if (parser.getAttr("correlationViewEvenPointColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->correlationViewEvenPointColor);
		}

		if (parser.getAttr("teleconnectivityViewLineColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->teleconnectivityViewLineColor);
		}
		if (parser.getAttr("teleconnectivityViewStartPointColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->teleconnectivityViewStartPointColor);
		}
		if (parser.getAttr("teleconnectivityViewEndPointColor", 's', bufSize, buffer )) {
			parseColor(buffer, &pPreferences->teleconnectivityViewEndPointColor);
		}

		parser.getAttr("significanceThreshold", 'f', sizeof(pPreferences->significanceThreshold), &pPreferences->significanceThreshold);
		parser.getAttr("mapType", 'i', sizeof(pPreferences->mapType), &pPreferences->mapType);

		parser.getAttr("corrMapRefPointSize", 'f', sizeof(pPreferences->corrMapRefPointSize), &pPreferences->corrMapRefPointSize);
		parser.getAttr("corrMapPointSize", 'f', sizeof(pPreferences->corrMapPointSize), &pPreferences->corrMapPointSize);
		parser.getAttr("corrMapLineSize", 'f', sizeof(pPreferences->corrMapLineSize), &pPreferences->corrMapLineSize);
		parser.getAttr("tcMapRefPointSize", 'f', sizeof(pPreferences->tcMapRefPointSize), &pPreferences->tcMapRefPointSize);
		parser.getAttr("tcMapPointSize", 'f', sizeof(pPreferences->tcMapPointSize), &pPreferences->tcMapPointSize);
		parser.getAttr("tcMapLineSize", 'f', sizeof(pPreferences->tcMapLineSize), &pPreferences->tcMapLineSize);


		parser.getAttr("projPointSize", 'f', sizeof(pPreferences->projPointSize), &pPreferences->projPointSize);
		parser.getAttr("projRefPointSize", 'f', sizeof(pPreferences->projRefPointSize), &pPreferences->projRefPointSize);
		parser.getAttr("projChainPointSize", 'f', sizeof(pPreferences->projChainPointSize), &pPreferences->projChainPointSize);
		parser.getAttr("projChainLineSize", 'f', sizeof(pPreferences->projChainLineSize), &pPreferences->projChainLineSize);
	}
}

void PreferenceStorage::store(const std::string& fileName, const Preferences* pPreferences) {
	std::ofstream out(fileName);
	if (out.good()) {
		storeTFOPreference(out, "correlationViewTF", pPreferences->correlationViewTF );
		storeTFOPreference(out, "teleconectivityViewTF",  pPreferences->teleconnectivityViewTF);

		storeColorPreference(out, "referencePointColor",  pPreferences->referencePointColor);

		storeColorPreference(out, "correlationViewLineColor",  pPreferences->correlationViewLineColor);
		storeColorPreference(out, "correlationViewOddPointColor",  pPreferences->correlationViewOddPointColor);
		storeColorPreference(out, "correlationViewEvenPointColor",  pPreferences->correlationViewEvenPointColor);
		storeColorPreference(out, "teleconnectivityViewLineColor",  pPreferences->teleconnectivityViewLineColor);
		storeColorPreference(out, "teleconnectivityViewStartPointColor",  pPreferences->teleconnectivityViewStartPointColor);
		storeColorPreference(out, "teleconnectivityViewEndPointColor",  pPreferences->teleconnectivityViewEndPointColor);

		out << "significanceThreshold" << '=' << pPreferences->significanceThreshold << std::endl;
		out << "mapType" << '=' << pPreferences->mapType << std::endl;

		out << "corrMapRefPointSize" << '=' << pPreferences->corrMapRefPointSize << std::endl;
		out << "corrMapPointSize" << '=' << pPreferences->corrMapPointSize << std::endl;
		out << "corrMapLineSize" << '=' << pPreferences->corrMapLineSize << std::endl;
		out << "tcMapRefPointSize" << '=' << pPreferences->tcMapRefPointSize << std::endl;
		out << "tcMapPointSize" << '=' << pPreferences->tcMapPointSize << std::endl;
		out << "tcMapLineSize" << '=' << pPreferences->tcMapLineSize << std::endl;

		out << "projPointSize" << '=' << pPreferences->projPointSize << std::endl;
		out << "projRefPointSize" << '=' << pPreferences->projRefPointSize << std::endl;
		out << "projChainPointSize" << '=' << pPreferences->projChainPointSize << std::endl;
		out << "projChainLineSize" << '=' << pPreferences->projChainLineSize << std::endl;

	}
}

void PreferenceStorage::parseColor(char* buffer, VCGL::RGBF* pColor) {
		int len = strlen(buffer);
		if (len < 7 || buffer[0] != '(') {
			std::cerr << "incorrect data for PreferenceStorage::parseColor: " << buffer << std::endl;
			return;
		}

//		std::cerr << "parseColor: \"" <<  buffer << '\"' << std::endl;

		//check: starts with '(' ends with ')'
				//check: contains two ';'
				//replace all special symbols with spaces

		{ //check color
			int countLB = 0;
			int countRB = 0;
			int countSC = 0;
			for (int i=0; i<len; i++) {
				switch(buffer[i]) {
				case '(':
					countLB++;
					buffer[i]=' ';
				break;
				case ')':
					countRB++;
					buffer[i]=' ';
					break;
				case ';':
					countSC++;
					buffer[i]=' ';
					break;
				default: break;
				}
			}
			assert(countLB == 1 && countRB == 1 && countSC == 2);
		}

		std::string s(buffer);
		std::stringstream in(s);
		float r=0, g=0, b=0;
		in >> r >> g >> b;
//		std::cerr << "\tparsed as: " << '(' << r << ';' << g << ';' << b << ')' << std::endl;

		*pColor = {r,g,b};
	}

void PreferenceStorage::parseTFO(char* buffer, unsigned bufSize, VCGL::TransferFunctionObject* pTFObject) {
//	std::cerr << "parseTFO: \"" <<  buffer << '\"' << std::endl;

	*pTFObject = TransferFunctionObject();

	const char tlDelimiter = ':';

	char* newBuf = new char[bufSize];
	memccpy(newBuf, buffer, 1, bufSize);

	std::string s(buffer);
	size_t topLevelStartPos = 0;
	size_t topLevelEndPos = 0;

	topLevelEndPos = s.find(tlDelimiter);

	std::string tfoName;
	if (topLevelEndPos != topLevelStartPos) {
		tfoName = s.substr(topLevelStartPos, topLevelEndPos-topLevelStartPos);
	}
//	std::cerr << "TFO name: \"" << tfoName << "\"" << std::endl;
	pTFObject->setName(tfoName);
	topLevelStartPos = topLevelEndPos + 1;


	std::string tfoMode;
	topLevelEndPos = s.find(tlDelimiter, topLevelStartPos);

	VCGL::IColorizer::ColorizerMode mode = VCGL::IColorizer::CONTINUOUS;
	if (topLevelEndPos != topLevelStartPos) {
		tfoMode = s.substr(topLevelStartPos, topLevelEndPos-topLevelStartPos);

		if (tfoMode == "BM") {
			mode = VCGL::IColorizer::BANDED_MIDPOINT;
		}
		if (tfoMode == "BS") {
			mode = VCGL::IColorizer::BANDED_SIDES;
		}

		pTFObject->setMode(mode);
	}

	topLevelStartPos = topLevelEndPos + 1;


	while (topLevelStartPos < s.length() && topLevelStartPos != std::string::npos) {
		topLevelEndPos = s.find(tlDelimiter, topLevelStartPos);

		std::string tlToken;
		if (topLevelEndPos != std::string::npos) {
			tlToken = s.substr(topLevelStartPos, topLevelEndPos-topLevelStartPos);
			topLevelStartPos = topLevelEndPos + 1;
		}
		else {
			tlToken = s.substr(topLevelStartPos);
			topLevelStartPos = std::string::npos;
		}

		{//process token
			const char posDelimiter = '*';
			size_t endPos = tlToken.find(posDelimiter);

			if (endPos != std::string::npos) {
				std::string posStr = tlToken.substr(0, endPos);
				std::string colorStr = tlToken.substr(endPos+1);

//				std::cerr << "\tseparated as: position =\""
//						<< posStr << "\", color = \""
//						<< colorStr << "\"" << std::endl;

				float position = 0; //= std::stof(posStr);
				std::stringstream in(posStr);
				in >> position;
//				std::cerr << "\tposition parsed as: " << position << std::endl;
				RGBF color;

				unsigned len = colorStr.size();
				char colorChars[len+1];
				colorStr.copy(colorChars, len);
				colorChars[len] = 0;

				parseColor(colorChars, &color);

				pTFObject->addTFRecord(TFRecord(position, color.r, color.g, color.b));
			}
		}
	}
//	std::cerr << std::endl;
//	storeTFOPreference(std::cerr, "parsedTFO", *pTFObject);
//	std::cerr << std::endl;
}

} /* namespace VCGL */
