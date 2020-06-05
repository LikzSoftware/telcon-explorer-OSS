/*! @file pathresolvertest.cpp
 * @author anantonov
 * @date Created on May 15, 2014
 *
 * @brief TODO: Add brief description of the file contents
 */


#include "pathresolver.h"
#include "filesystem.h"

#include "CppUnitLite/TestHarness.h"
#include <vector>
#include <string>

//#include <iostream>

namespace Testing {

class FakeFileSystem: public VCGL::FileSystem {
public:
	std::string cwd;
	std::string home;
	std::vector<std::string> files;

	std::string getCWD() const override {
		return cwd;
	}

	std::string getHomeDir() const override {
		return home;
	}

	bool fileExists(const std::string& path) const override {
		bool bFound = false;
		for (unsigned i=0; !bFound && i<files.size(); i++) {
			if (files[i] == path) {
				bFound = true;
			}
		}
		return bFound;
	}
};

SimpleString StringFrom (const std::string& value)
{
	return SimpleString(value.c_str());
}

TEST(findFileCWDRoot, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/";
	ffs.files.push_back("/fileA.txt");

	VCGL::PathResolver pr(ffs);
	std::string path;
	bool found = pr.find("fileA.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/fileA.txt"),path);
}

TEST(findFileCWD, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/cwd";
	ffs.files.push_back("/cwd/fileB.txt");

	VCGL::PathResolver pr(ffs);
	std::string path;
	bool found = pr.find("fileB.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/cwd/fileB.txt"),path);
}

TEST(findFileHomeDir, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/cwd";
	ffs.home = "/home/user";
	ffs.files.push_back("/home/user/fileC.txt");

	VCGL::PathResolver pr(ffs);
	std::string path;
	bool found = pr.find("fileC.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/home/user/fileC.txt"),path);
}


TEST(findFileMetaDir, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/cwd";
	ffs.home = "/home/user";
	ffs.files.push_back("/app");
	ffs.files.push_back("/app/meta");
	ffs.files.push_back("/app/meta/fileD.txt");

	VCGL::PathResolver pr(ffs);
	pr.setAppDir("/app");

	std::string path;
	bool found = pr.find("fileD.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/app/meta/fileD.txt"),path);
}

TEST(findDependentFileSubdir, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/cwd";
	ffs.files.push_back("/cwd");
	ffs.files.push_back("/cwd/data/file1.txt");
	ffs.files.push_back("/cwd/data/etc/file2.txt");

	VCGL::PathResolver pr(ffs);

	std::string path;
	bool found = pr.findDependency("etc/file2.txt", "data/file1.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/cwd/data/etc/file2.txt"), path);
}

TEST(findDependentFileAbsolutePath, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/cwd";
	ffs.files.push_back("/cwd");
	ffs.files.push_back("/cwd/data/file1.txt");
	ffs.files.push_back("/archives/file2.txt");

	VCGL::PathResolver pr(ffs);

	std::string path;
	bool found = pr.findDependency("/archives/file2.txt", "data/file1.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/archives/file2.txt"), path);
}

TEST(findDependentFileWhenGivenHasAbsolutePath, PathResolver) {
	FakeFileSystem ffs;
	ffs.cwd = "/cwd";
	ffs.files.push_back("/cwd");
	ffs.files.push_back("/cwd/data/file1.txt");
	ffs.files.push_back("/cwd/data/extras/file2.txt");

	VCGL::PathResolver pr(ffs);

	std::string path;
	bool found = pr.findDependency("extras/file2.txt", "/cwd/data/file1.txt", path);
	CHECK(found);
	CHECK_EQUAL(("/cwd/data/extras/file2.txt"), path);
}

//test: relative file names
//implement:
// resolving relative paths
// finding current directory <- main's envp $PWD
// find user's home directory <- from main's envp $HOME
// finding app directory <- from argv[0], realpath+dirname
} // namespace Testing

