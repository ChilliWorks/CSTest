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
/// TODO
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_TESTCASE(in_name) \
    namespace in_name##IntegrationTestCase \
    { \
        const std::string k_testCaseName_ = #in_name; \
    } \
    namespace in_name##IntegrationTestCase
//------------------------------------------------------------------------------
/// TODO
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_TEST_TIMEOUT(in_name, in_timeoutSeconds) \
    void in_name##IntegrationTest(const CSTest::IntegrationTest::TestSPtr& in_thisTest_); \
    namespace \
    { \
        CSTest::IntegrationTest::AutoRegister in_name##IntegrationTest##AutoReg(CSTest::IntegrationTest::TestDesc(k_testCaseName_, #in_name, in_name##IntegrationTest, in_timeoutSeconds)); \
    } \
    void in_name##IntegrationTest(const CSTest::IntegrationTest::TestSPtr& in_thisTest_)
//------------------------------------------------------------------------------
/// TODO
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_TEST(in_name) CSIT_TEST_TIMEOUT(in_name, CSTest::IntegrationTest::TestDesc::k_defaultTimeoutSeconds)
//------------------------------------------------------------------------------
/// TODO
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_PASS() in_thisTest_->Pass();
//------------------------------------------------------------------------------
/// TODO
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_FAIL(in_message) in_thisTest_->Fail(in_message);
//------------------------------------------------------------------------------
/// TODO
///
/// @author Ian Copland
//------------------------------------------------------------------------------
#define CSIT_ASSERT(in_condition, in_failureMessage) in_thisTest_->Assert(in_condition, in_failureMessage);

#endif
