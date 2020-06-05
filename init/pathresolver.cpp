/*! @file pathresolver.cpp
 * @author anantonov
 * @date Created on May 15, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#include "pathresolver.h"
#include <cassert>

#include "filesystem.h"
//#include <iostream>

#include <libgen.h>
#include <limits.h>
#include <cstring>



namespace VCGL {

std::string PathResolver::appDirName;

PathResolver::PathResolver(const FileSystem& fileSystem)
: fs(fileSystem) {}

bool PathResolver::find(const std::string& inFileName, std::string& outPath) {
	outPath = "";

	bool bFound = false;
	std::string testPath;

	bFound = checkDir(fs.getCWD(), inFileName, testPath);

	if (!bFound) {
		bFound = checkDir(fs.getHomeDir(), inFileName, testPath);
	}

	if (!bFound) {
		bFound = checkDir(getMetaDir(), inFileName, testPath);
	}

	if (bFound) {
		outPath = testPath;
	}

	return bFound;
}

bool PathResolver::findDependency(const std::string& dependencyName, const std::string& givenFileName, std::string& outPath) {
	if (dependencyName.c_str()[0] != '/') {
		std::string givenFileDirStr;
		char* givenFileNameCopy = strdup(givenFileName.c_str());
		char* givenFileDir = dirname(givenFileNameCopy);

		if (givenFileDir[0] != '/') {
			givenFileDirStr = fs.getCWD() + '/' + std::string(givenFileDir);
		}
		else {
			givenFileDirStr = givenFileDir;
		}
		free(givenFileNameCopy);

		outPath = givenFileDirStr + '/' + dependencyName;
	}
	else {
		outPath = dependencyName;
	}
	return fs.fileExists(outPath);
}

void
PathResolver::setAppDir(const std::string& dirName) {
	assert(dirName[dirName.length()-1] != '/');
	appDirName = dirName;
}

bool PathResolver::checkDir(const std::string& dirName, const std::string& inFileName,
		std::string& testPath) {
	assert(dirName.length() == 1 || dirName[dirName.length() - 1] != '/');
	if (dirName[dirName.length() - 1] != '/') {
		testPath = dirName + '/' + inFileName;
	}
	else {
		testPath = dirName + inFileName;
	}
	return fs.fileExists(testPath);
}

std::string PathResolver::getMetaDir() const {
	return (appDirName + "/meta");
}

} /* namespace VCGL */
