//
//  TestCase.h
//  CSTest
//  Created by Ian Copland on 15/03/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _INTEGRATIONTEST_TESTSYSTEM_TESTCASE_H_
#define _INTEGRATIONTEST_TESTSYSTEM_TESTCASE_H_

#include <CSTest.h>

#include <IntegrationTest/TestSystem/AutoRegister.h>
#include <IntegrationTest/TestSystem/Test.h>

//------------------------------------------------------------------------------
/// A macro used to declare a new test case.
///
/// @author Ian Copland
///
/// @param in_name - The name of the test case.
//------------------------------------------------------------------------------
#define CSIT_TESTCASE(in_name) \
    namespace in_name##IntegrationTestCase \
    { \
        const std::string k_testCaseName_ = #in_name; \
    } \
    namespace in_name##IntegrationTestCase
//------------------------------------------------------------------------------
/// A macro used to declare a new test with the given timeout. If the default
/// timeout is desired then the CSIT_TEST() macro should be used instead.
///
/// @author Ian Copland
///
/// @param in_name - The name of the test.
/// @param in_timeoutSeconds - The time before the test fails.
//------------------------------------------------------------------------------
#define CSIT_TEST_TIMEOUT(in_name, in_timeoutSeconds) \
    void in_name##IntegrationTest(const CSTest::IntegrationTest::TestSPtr& in_thisTest_) noexcept; \
    namespace \
    { \
        CSTest::IntegrationTest::AutoRegister in_name##IntegrationTest##AutoReg(CSTest::IntegrationTest::TestDesc(k_testCaseName_, #in_name, in_name##IntegrationTest, in_timeoutSeconds)); \
    } \
    void in_name##IntegrationTest(const CSTest::IntegrationTest::TestSPtr& in_thisTest_) noexcept
//------------------------------------------------------------------------------
/// A macro used to declare a new test with the default timeout.
///
/// @author Ian Copland
///
/// @param in_name - The name of the test.
//------------------------------------------------------------------------------
#define CSIT_TEST(in_name) CSIT_TEST_TIMEOUT(in_name, CSTest::IntegrationTest::TestDesc::k_defaultTimeoutSeconds)
//------------------------------------------------------------------------------
/// A macro used to flag a test as passed. This can be called on any thread.
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_PASS() in_thisTest_->Pass();
//------------------------------------------------------------------------------
/// A macro used to flag a test as failed, providing an error message detailing
/// what went wrong. This can be called on any thread.
///
/// @author Ian Copland
///
/// @param in_message - The error message detailing why it failed.
//------------------------------------------------------------------------------
#define CSIT_FAIL(in_message) in_thisTest_->Fail(in_message);
//------------------------------------------------------------------------------
/// An assertion macro which will call Fail() if the given condition is not true.
///
/// @author Ian Copland
///
/// @param in_condition - The condition to check.
/// @param in_failureMessage - The message to pass to Fail() if the condition is
/// not true.
//------------------------------------------------------------------------------
#define CSIT_ASSERT(in_condition, in_failureMessage) in_thisTest_->Assert(in_condition, in_failureMessage);

#endif
