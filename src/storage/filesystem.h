/*! @file filesystem.h
 * @author anantonov
 * @date Created on May 15, 2014
 *
 * @brief Naive reusable implementation of basic file system queries
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>

namespace VCGL {

class FileSystem {
public:
	virtual std::string getAppDir( const std::string& appCallString );
	virtual std::string getCWD() const;
	virtual std::string getHomeDir() const;
	virtual bool fileExists(const std::string& path) const;
	virtual ~FileSystem();
};

} /* namespace VCGL */

#endif /* FILESYSTEM_H_ */
