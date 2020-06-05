/*! @file filesystem.cpp
 * @author anantonov
 * @date Created on May 15, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "filesystem.h"

#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string.h>


namespace VCGL {

std::string
FileSystem::getAppDir( const std::string& appCallString ) {
	std::string result("");
	if (NULL != strchr(appCallString.c_str(), '/')) {
		char path[PATH_MAX];
		memset(path, 0, PATH_MAX);
		if (realpath(appCallString.c_str(), path)) {
			result = dirname(path);
		}
	}
	return result;
}

std::string FileSystem::getCWD() const {
	char path[PATH_MAX];
	memset(path, 0, PATH_MAX);
	(void)getcwd(path, PATH_MAX);
	return std::string(path);
}

std::string FileSystem::getHomeDir() const {
	std::string result("");
	char* value = getenv("HOME");
	if (value) {
		result = value;
	}
	return result;
}

bool FileSystem::fileExists(const std::string& path) const {
	struct stat buffer;
	return ( stat(path.c_str(), &buffer) == 0);
}

FileSystem::~FileSystem() {

}

} /* namespace VCGL */
