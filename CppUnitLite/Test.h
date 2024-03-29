///////////////////////////////////////////////////////////////////////////////
//
// TEST.H
// 
// This file contains the Test class along with the macros which make effective
// in the harness.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TEST_H
#define TEST_H


#include <cmath>
#include "SimpleString.h"

class TestResult;



class Test
{
public:
	Test (const SimpleString& testName);
	virtual ~Test() {}

	virtual void	run (TestResult& result) = 0;


	void			setNext(Test *test);
	Test			*getNext () const;

protected:
	SimpleString	name_;
	Test			*next_;

};


#define TEST(testName, testGroup)\
  class testGroup##testName##Test : public Test \
	{ public: testGroup##testName##Test () : Test (#testName "Test") {} \
            void run (TestResult& result_); } \
    testGroup##testName##Instance; \
	void testGroup##testName##Test::run (TestResult& result_) 



#define CHECK(condition)\
{ if (!(condition)) \
{ result_.addFailure (Failure (name_, __FILE__,__LINE__, #condition)); return; } }


#define CHECK_EQUAL(expected,actual)\
{ if (!((expected) == (actual))) {result_.addFailure(Failure(name_, __FILE__, __LINE__, StringFrom(expected), StringFrom(actual))); return; }}

#define LONGS_EQUAL(expected,actual)\
{ long actualTemp = actual; \
  long expectedTemp = expected; \
  if ((expectedTemp) != (actualTemp)) \
{ result_.addFailure (Failure (name_, __FILE__, __LINE__, StringFrom(expectedTemp), \
StringFrom(actualTemp))); return; } }



#define DOUBLES_EQUAL(expected,actual,threshold)\
{ double actualTemp = actual; \
  double expectedTemp = expected; \
  if (fabs ((expectedTemp)-(actualTemp)) > threshold) \
{ result_.addFailure (Failure (name_, __FILE__, __LINE__, \
StringFrom((double)expectedTemp), StringFrom((double)actualTemp))); return; } }



#define FAIL(text) \
{ result_.addFailure (Failure (name_, __FILE__, __LINE__,(text))); return; }



#endif
