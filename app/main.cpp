
#include <iostream>
#include <cstring>
#include <cassert>

#include "init/startup.h"
#include "init/pathresolver.h"
#include "init/filesystem.h"
#include <getopt.h>

//TODO: make binary flags of this state
enum RunState {
	PRECOMPUTE_ONLY,
	PRECOMPUTE_AND_SHOW,
	SHOW_ONLY,
	UI_TEST,
	REGIONS_EXPLORER,
	ERROR
};

void showUsage() {
	std::cerr << "Usage: NCCorrelation [-N | --northOnly] [-P | --precomputeOnly] [-v variableName] [-l level] fileName.nc" << std::endl;
}

int main(int argc, char *argv[])
{
	VCGL::FileSystem fs;
	VCGL::PathResolver::setAppDir( fs.getAppDir(argv[0]) );

	for (int i=0; i<argc; i++) {
		std::cerr << i << ": " << argv[i] << std::endl;
	}

	// parse arguments (TODO: see getopt)
	// precompute/test/show(default) flags, with/without lags, variable name, variable level, other parameters

	char* varName = 0;
	char* levelValue = 0;
	char* thresholdValue = 0;
	char* fileName = 0;
	bool northOnly = false; // work only with northern hemisphere

	enum RunState state = SHOW_ONLY;
	int returnValue = 0;

	// define the mode of execution (precompute or show, with ot without lags, or test, or combination)

	int c = 0;
	do {
		int optionIndex = 0;
		// ATTENTION: change PCONLY_INDEX accordingly to the options array
		static struct option longOptions[] = {
				{"northOnly", no_argument, 0, 'N'},
				{"precompute", no_argument, 0, 'P'},
				{"precomputeOnly", no_argument, 0, 0},
				{"var", required_argument, 0, 'v'},
				{"level", required_argument, 0, 'l'},
				{0, 0, 0, 0}
		};
		const int PCONLY_INDEX = 2;
		assert(0 == strcmp(longOptions[2].name, "precomputeOnly" ));

		c = getopt_long(argc, argv, "NPv:l:t:ur", longOptions, &optionIndex);
		switch (c) {
		case -1:
			std::cerr << "end of options list" << std::endl;
			break;
		case 0:

			std::cerr << "option " << longOptions[optionIndex].name;
			if (optarg) {
				std::cerr << ", argument = " << optarg;
			}
			std::cerr << std::endl;

			if (optionIndex == PCONLY_INDEX) {
				state = PRECOMPUTE_ONLY;
			}
			break;
		case 'N':
			std::cerr << "option northOnly" << std::endl;
			northOnly = true;
			break;
		case 'P':
			std::cerr << "option precompute" << std::endl;
			if (state == SHOW_ONLY) {
				state = PRECOMPUTE_AND_SHOW;
			}
			else {
				std::cerr << "ignoring precompute option because of previous options" << std::endl;
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
			state = UI_TEST;
			break;
		case 'r':
			state = REGIONS_EXPLORER;
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
	// ***if requested - test (pass on parameters)
	// by default - show

	if (state == PRECOMPUTE_AND_SHOW || state == PRECOMPUTE_ONLY) {
		//std::cerr << "running precompute..."  << std::endl;
		returnValue = VCGL::Startup::runPrecompute(fileName, varName, levelValue, northOnly);
	}

	if (state == UI_TEST) {
		//pass all arguments
		returnValue = VCGL::Startup::runUITest(argc-optind, argv+optind);
	}

	if (state == REGIONS_EXPLORER) {
		returnValue = VCGL::Startup::runRegionExplorer(fileName, varName, levelValue);
	}

	if (0 == returnValue && (state == PRECOMPUTE_AND_SHOW || state == SHOW_ONLY) ) {
		//std::cerr << "running show..."  << std::endl;
		returnValue = VCGL::Startup::runShow(fileName, varName, levelValue, northOnly);
	}

	return returnValue;

}
