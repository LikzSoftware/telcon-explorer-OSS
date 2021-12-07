/*! @file tests-main.cpp
 * @author anantonov
 * @date Created on Apr 18, 2014
 *
 * @brief Startup code for the unit testing harness
 */

#include "CppUnitLite/TestHarness.h"
int main()
{
     TestResult tr;
     TestRegistry::runAllTests(tr);
     return 0;
}


