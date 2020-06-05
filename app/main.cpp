
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cassert>

#include "startup.h"
#include "storage/pathresolver.h"
#include "storage/filesystem.h"
#include <getopt.h>

enum RunState: uint8_t {
	UNKNOWN = 0,
	DEFAULT = 0x01,			// SHOW by default
	PRECOMPUTE = 0x02,		// 0b00000010
	REGION_EXPLORER = 0x20,	// 0b00100000
	UI_TEST = 0x40,			// 0b01000000
	ERROR = 0x80			// 0b10000000
};

void showUsage() {
	std::cerr << "Usage: telcon-explorer [action] [flags] -v variableName -l level fileName.nc" << std::endl;
	std::cerr << "By default (when no action is specified), ";
	std::cerr << "application assumes the precompute is performed and loads the main window." << std::endl;
	std::cerr << "Flags:" << std::endl;
	std::cerr << "\t-N (--northOnly) use only northern hemisphere portion of the data file (unstable)" << std::endl;
	std::cerr << "Actions (cannot be combined):" << std::endl;
	std::cerr << "\t-P               precompute" << std::endl;
	std::cerr << "\t-u               load region explorer" << std::endl;
	std::cerr << "\t-r               load ui test" << std::endl;
}

int main(int argc, char *argv[])
{
	VCGL::FileSystem fs;
	VCGL::PathResolver::setAppDir( fs.getAppDir(argv[0]) );

	for (int i=0; i<argc; i++) {
		std::cerr << i << ": " << argv[i] << std::endl;
	}

	// parse arguments (see getopt)
	// precompute/test/show(default) flags, with/without lags, variable name, variable level, other parameters

	char* varName = 0;
	char* levelValue = 0;
	char* fileName = 0;
	bool northOnly = false; // work only with northern hemisphere

	enum RunState state = DEFAULT;
	int returnValue = 0;

	// define the mode of execution (precompute or show, with ot without lags, or test, or combination)

	int c = 0;
	do {
		int optionIndex = 0;
		static struct option longOptions[] = {
				{"northOnly", no_argument, 0, 'N'},
				{"precompute", no_argument, 0, 'P'},
				{"var", required_argument, 0, 'v'},
				{"level", required_argument, 0, 'l'},
				{"help", no_argument, 0, 'h'},
				{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "NPv:l:t:ur", longOptions, &optionIndex);
		switch (c) {
		case 'h':
			showUsage();
			return 1;
		case -1:
			std::cerr << "end of options list" << std::endl;
			break;
		case 0:

			std::cerr << "option " << longOptions[optionIndex].name;
			if (optarg) {
				std::cerr << ", argument = " << optarg;
			}
			std::cerr << std::endl;

		case 'N':
			std::cerr << "option northOnly" << std::endl;
			northOnly = true;
			break;
		case 'P':
			std::cerr << "option precompute" << std::endl;
			// accept the action only when in default state, otherwise fail
			if (state == DEFAULT) {
				state = PRECOMPUTE;
			}
			else {
				std::cerr << "ERROR: actions cannot be combined" << std::endl;
				state = ERROR;
			}
			break;
		case 'v':
			std::cerr << "variable name is " << optarg << std::endl;
			varName = optarg;
			break;
		case 'l':
			std::cerr << "level is " << optarg << std::endl;
			levelValue = optarg;
			break;
		case 'u':
			std::cerr << "option UI test" << std::endl;
			// accept the action only when in default state, otherwise fail
			if (state == DEFAULT) {
				state = UI_TEST;
			}
			else {
				std::cerr << "ERROR: actions cannot be combined" << std::endl;
				state = ERROR;
			}
			break;
		case 'r':
			std::cerr << "option region explorer" << std::endl;
			// accept the action only when in default state, otherwise fail
			if (state == DEFAULT) {
				state = REGION_EXPLORER;
			}
			else {
				std::cerr << "ERROR: actions cannot be combined" << std::endl;
				state = ERROR;
			}
			break;
		case '?':
			std::cerr << "unrecognized option" << std::endl;
			break;
		default:
			std::cerr << "?? getopt returned character code " << static_cast<int>(c)
						<< "= " << static_cast<char>(c) << " ??" << std::endl;
			break;
		}
	} while (c != -1);

	if (optind + 1 > argc && state != UI_TEST) {
		std::cerr << "Missing fileName.nc" << std::endl;
		showUsage();
		return 1;
	}

	//take the last option as the file name
	fileName = argv[argc-1];


	// if requested - precompute
	if (state & PRECOMPUTE) {
		//std::cerr << "running precompute..."  << std::endl;
		returnValue = VCGL::Startup::runPrecompute(fileName, varName, levelValue, northOnly);
	}

	// if requested - test (pass on parameters)
	if (state & UI_TEST) {
		//pass all arguments
		returnValue = VCGL::Startup::runUITest(argc-optind, argv+optind);
	}

	// if requested - open region explorer
	if (state & REGION_EXPLORER) {
		returnValue = VCGL::Startup::runRegionExplorer(fileName, varName, levelValue);
	}

	// by default, load the main UI
	if ((state == DEFAULT) && (0 == returnValue)) {
		//std::cerr << "running show..."  << std::endl;
		returnValue = VCGL::Startup::runShow(fileName, varName, levelValue, northOnly);
	}

	return returnValue;

}
