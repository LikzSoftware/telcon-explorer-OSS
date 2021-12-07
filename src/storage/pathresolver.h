/*! @file pathresolver.h
 * @author anantonov
 * @date Created on May 15, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */

#ifndef PATHRESOLVER_H_
#define PATHRESOLVER_H_

#include <string>

namespace VCGL {

class FileSystem;

class PathResolver {
public:
	PathResolver(const FileSystem& fileSystem);
	bool find(const std::string& inFileName, std::string& outPath);
	bool findDependency(const std::string& dependencyName, const std::string& givenFileName, std::string& outPath);
	static void setAppDir(const std::string& dirName);
private:
	bool checkDir(const std::string& dirName, const std::string& inFileName,
			std::string& testPath);
	std::string getMetaDir() const;

	const FileSystem& fs;
	static std::string appDirName;
};

} /* namespace VCGL */

#endif /* PATHRESOLVER_H_ */
