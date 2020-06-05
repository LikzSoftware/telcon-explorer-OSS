///////////////////////////////////////////////////////////////////////////////
//
// TESTRESULT.H
// 
// A TestResult is a collection of the history of some test runs.  Right now
// it just collects failures.
// 
///////////////////////////////////////////////////////////////////////////////



#ifndef TESTRESULT_H
#define TESTRESULT_H

class Failure;
class SimpleString;

class TestResult
{
public:
					TestResult (); 
	virtual void	testsStarted ();
	virtual void	addRun();
	virtual void	addFailure (const Failure& failure);
	virtual void	testsEnded ();
    SimpleString	nounEnding(long int number);

private:
	long int				runCount;
	long int				failureCount;
};

#endif
