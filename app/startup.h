/*!	@file startup.h
 *	@author anantonov
 *	@date	Feb 13, 2014 (created)
 *	@brief	Routines encompassing various modes of running the application
 */

#ifndef STARTUP_H_
#define STARTUP_H_

namespace VCGL {

class Startup {
public:
	static int runPrecompute(char* fileName,
			char* variableName,
			char* levelValue = 0,
			bool northOnly = false );

	static int runShow(char* fileName,
			char* variableName,
			char* levelValue = 0,
			bool northOnly = false );

	static int runRegionExplorer(char* fileName,
			char* variableName,
			char* levelValue);


	static int runUITest( int argCount, char** argValues );

private:
	Startup();
};



}


#endif // STARTUP_H_
