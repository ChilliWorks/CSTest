//
//  Report.cpp
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

#include <IntegrationTest/TestSystem/Report.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Report::FailedTest::FailedTest(const TestDesc& in_desc, const std::string& in_errorMessage) noexcept
            : m_desc(in_desc), m_errorMessage(in_errorMessage)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const TestDesc& Report::FailedTest::GetDesc() const noexcept
        {
            return m_desc;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& Report::FailedTest::GetErrorMessage() const noexcept
        {
            return m_errorMessage;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Report::Report(u32 in_numTests, const std::vector<FailedTest>& in_failedTests) noexcept
            : m_numTests(in_numTests), m_failedTests(in_failedTests)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumTests() const noexcept
        {
            return m_numTests;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumFailedTests() const noexcept
        {
            return u32(m_failedTests.size());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<Report::FailedTest>& Report::GetFailedTests() const noexcept
        {
            return m_failedTests;
        }
    }
}
