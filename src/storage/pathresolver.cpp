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

namespace {
	inline bool isRelativePath(const char* str) {
		return (str[0] != '/');
	}
}

std::string PathResolver::appDirName;

PathResolver::PathResolver(const FileSystem& fileSystem)
: fs(fileSystem) {}

bool PathResolver::find(const std::string& inFileName, std::string& outPath) {
	/* for a relative path to a [config] file, have to check:
	 * 1) current directory
	 * 2) subdirectory meta of current directory
	 * 3) user preferences directory 
	 * 4) global data directory
	 */

	outPath = "";
	bool bFound = false;

	if (isRelativePath(inFileName.c_str())) {
		// (1) check the current directory
		bFound = checkDir(fs.getCWD(), inFileName, outPath);
	
		if (!bFound) {
			// (2) check the meta subdirectory
			bFound = checkDir(fs.getCWD()+"/meta", inFileName, outPath);
		}
	
		if (!bFound) {
			// (3) user preferences directory
			// TODO: change from $HOME to $HOME/.telcon-explorer
			bFound = checkDir(fs.getHomeDir(), inFileName, outPath);
		}
	
		if (!bFound) {
			// (4) global data directory
			bFound = checkDir(getGlobalDataDir(), inFileName, outPath);
		}
	}
	else {
		// file is given by an absolute path, take as is
		bFound = checkDir("/", inFileName, outPath);
	}

	return bFound;
}

bool PathResolver::findDependency(const std::string& dependencyName, const std::string& givenFileName, std::string& outPath) {
	bool bFound = false;
	if (isRelativePath(dependencyName.c_str())) {
		// 1) check dependency relative to the current directory
		bFound = checkDir(fs.getCWD(), dependencyName, outPath);

		if (!bFound) {
			// 2) check dependency relative to the directory of the given file
			std::string givenFileDirStr;
			char* givenFileNameCopy = strdup(givenFileName.c_str());
			char* givenFileDir = dirname(givenFileNameCopy);
	
			if (isRelativePath(givenFileDir)) {
				// append to CWD
				givenFileDirStr = fs.getCWD() + '/' + std::string(givenFileDir);
			}
			else {
				// given file name is an absolute path, use as is
				givenFileDirStr = givenFileDir;
			}
			free(givenFileNameCopy);
	
			bFound = checkDir(givenFileDirStr, dependencyName, outPath);
		}
	}
	else {
		// dependency is given by an absolute path, take as is
		bFound = checkDir("/", dependencyName, outPath);
	}

	return bFound;
}

void
PathResolver::setAppDir(const std::string& dirName) {
	assert(dirName[dirName.length()-1] != '/');
	appDirName = dirName;
}

bool PathResolver::checkDir(const std::string& dirName, const std::string& inFileName,
		std::string& testPath) {
//	std::cout << "Searching for file '" << inFileName << "' in '" << dirName << "': " << std::endl;
	assert(dirName.length() == 1 || dirName[dirName.length() - 1] != '/');
	assert(!inFileName.empty());

	if (dirName[dirName.length() - 1] != '/') {
		testPath = dirName + '/' + inFileName;
	}
	else {
		if (isRelativePath(inFileName.c_str())) {
			testPath = dirName + inFileName;
		}
		else {
			assert(dirName == "/");
			testPath = inFileName;
		}
	}
	bool bExists = fs.fileExists(testPath);
//	std::cout << "Testing '" << testPath << "': " << (bExists ? "file exists" : "no such file") << std::endl;
	if (!bExists) {
		testPath.clear();
	}
	return bExists;
}

std::string PathResolver::getGlobalDataDir() const {
#ifdef DATADIR
	// return data dir if it exists
	// otherwise fall back to the meta subdirectory of the app dir
	return (fs.fileExists(DATADIR) ? DATADIR : appDirName+"/meta");
#else
	return (appDirName + "/meta");
#endif
}

} /* namespace VCGL */
