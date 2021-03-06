//
//  TestDesc.cpp
//  CSTest
//  Created by Ian Copland on 16/03/2016.
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

#include <IntegrationTest/TestSystem/TestDesc.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        TestDesc::TestDesc(const std::string& in_testCaseName, const std::string& in_testName, const TestDelegate& in_testDelegate, f32 in_timeoutSeconds) noexcept
            : m_testCaseName(in_testCaseName), m_testName(in_testName), m_testDelegate(in_testDelegate), m_timeoutSeconds(in_timeoutSeconds)
        {
            CS_ASSERT(!m_testCaseName.empty(), "The test case must have a name.");
            CS_ASSERT(!m_testName.empty(), "The test must have a name.");
            CS_ASSERT(m_testDelegate, "A valid test delegate must be supplied.");
            CS_ASSERT(m_timeoutSeconds > 0.0f, "The timeout must be more than 0 seconds.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& TestDesc::GetTestCaseName() const noexcept
        {
            return m_testCaseName;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& TestDesc::GetTestName() const noexcept
        {
            return m_testName;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const TestDesc::TestDelegate& TestDesc::GetTestDelegate() const noexcept
        {
            return m_testDelegate;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 TestDesc::GetTimeoutSeconds() const noexcept
        {
            return m_timeoutSeconds;
        }
    }
}
