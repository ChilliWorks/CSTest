//
//  Report.cpp
//  CSTest
//  Created by Ian Copland on 15/07/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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
        Report::Report()
            : m_numTestCases(0), m_numAssertions(0)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Report::Report(u32 in_numTestCases, u32 in_numAssertions, const std::vector<TestCase>& in_failedTestCases)
            : m_numTestCases(in_numTestCases), m_numAssertions(in_numAssertions), m_failedTestCases(in_failedTestCases)
        {
            m_numFailedAssertions = 0;
            for (const auto& testCase : m_failedTestCases)
            {
                m_numFailedAssertions += testCase.GetNumFailedAssertions();
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool Report::DidAllTestsPass() const
        {
            return (m_numFailedAssertions == 0);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumTestCases() const
        {
            return m_numTestCases;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumAssertions() const
        {
            return m_numAssertions;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumFailedTestCases() const
        {
            return static_cast<u32>(m_failedTestCases.size());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumFailedAssertions() const
        {
            return m_numFailedAssertions;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<TestCase>& Report::GetFailedTestCases() const
        {
            return m_failedTestCases;
        }
    }
}
