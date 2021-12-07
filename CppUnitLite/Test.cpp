

#include "Test.h"
#include "TestRegistry.h"
#include "TestResult.h"
#include "Failure.h"


Test::Test (const SimpleString& testName) 
	: name_ (testName) 
{
	TestRegistry::addTest (this);
}

Test *Test::getNext() const
{
	return next_;
}

void Test::setNext(Test *test)
{	
	next_ = test;
}

