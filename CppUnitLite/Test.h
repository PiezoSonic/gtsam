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
	Test (const SimpleString& testName, const SimpleString& filename, long lineNumber);
  virtual ~Test() {};

	virtual void	run (TestResult& result) = 0;


	void			setNext(Test *test);
	Test			*getNext () const;
	SimpleString    getName() const {return name_;}
	SimpleString 	getFilename() const {return filename_;}
	long			getLineNumber() const {return lineNumber_;}

protected:

	bool check (long expected, long actual, TestResult& result, const SimpleString& fileName, long lineNumber);
	bool check (const SimpleString& expected, const SimpleString& actual, TestResult& result, const SimpleString& fileName, long lineNumber);

	SimpleString	name_;
	Test			*next_;
	SimpleString 	filename_;
	long 			lineNumber_; /// This is the line line number of the test, rather than the a single check

};


#define TEST(testName, testGroup)\
  class testGroup##testName##Test : public Test \
	{ public: testGroup##testName##Test () : Test (#testName "Test", __FILE__, __LINE__) {} \
            void run (TestResult& result_);} \
    testGroup##testName##Instance; \
	void testGroup##testName##Test::run (TestResult& result_) 



#define CHECK(condition)\
{ if (!(condition)) \
{ result_.addFailure (Failure (name_, __FILE__,__LINE__, #condition)); return; } }

#define THROWS_EXCEPTION(condition)\
{ try { condition; \
		result_.addFailure (Failure (name_, __FILE__,__LINE__, SimpleString("Didn't throw: ") + StringFrom(#condition))); \
		return; } \
  catch (...) {} }

#define CHECK_EXCEPTION(condition, exception_name)\
{ try { condition; \
		result_.addFailure (Failure (name_, __FILE__,__LINE__, SimpleString("Didn't throw: ") + StringFrom(#condition))); \
		return; } \
  catch (exception_name& e) {} \
  catch (...) { \
	result_.addFailure (Failure (name_, __FILE__,__LINE__, SimpleString("Wrong exception: ") + StringFrom(#condition) + StringFrom(", expected: ") + StringFrom(#exception_name))); \
	return; } }

#define CHECK_EQUAL(expected,actual)\
{ if ((expected) == (actual)) return; result_.addFailure(Failure(name_, __FILE__, __LINE__, StringFrom(expected), StringFrom(actual))); }


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
