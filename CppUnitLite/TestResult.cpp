
#include "TestResult.h"
#include "Failure.h"
#include "SimpleString.h"

#include <stdio.h>


TestResult::TestResult ()
	: runCount(0), failureCount (0)
{
}


void TestResult::testsStarted () 
{
}

void TestResult::addRun() {
	runCount++;
}

void TestResult::addFailure (const Failure& failure) 
{
	fprintf (stdout, "In %s:\n%s:%ld: Failure: \"%s\"\n",
		failure.testName.asCharString(),
		failure.fileName.asCharString(),
		failure.lineNumber,
		failure.message.asCharString());
		
	failureCount++;
}


void TestResult::testsEnded () 
{
	if (failureCount > 0)
		fprintf (stdout,
				"%ld test%s ran, %ld failure%s\n",
				runCount,
				nounEnding(runCount).asCharString(),
				failureCount,
				nounEnding(failureCount).asCharString());
	else
		fprintf (stdout, "%ld tests OK\n", runCount);
}

SimpleString TestResult::nounEnding(long int number) {
	if (number == 1) return "";
	return "s";
}
