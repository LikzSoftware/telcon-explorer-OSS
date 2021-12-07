/*! @file preferencestorage.h
 * @author anantonov
 * @date Created on Jun 30, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef PREFERENCESTORAGE_H_
#define PREFERENCESTORAGE_H_

#include <string>

namespace VCGL {
struct Preferences;
class TransferFunctionObject;

class PreferenceStorage {
public:
	static void load(const std::string& fileName, Preferences* pPreferences);
	static void store(const std::string& fileName, const Preferences* pPreferences);
private:
	static void parseColor(char* buffer, VCGL::RGBF* pColor);
	static void parseTFO(char* buffer, unsigned bufSize, VCGL::TransferFunctionObject* pTFObject);
};

} /* namespace VCGL */

#endif /* PREFERENCESTORAGE_H_ */
