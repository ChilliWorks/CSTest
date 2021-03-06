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

#include <UnitTest/TestSystem/Report.h>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        Report::Report(u32 in_numTestCases, u32 in_numSections, u32 in_numAssertions, const std::vector<FailedTestCase>& in_failedTestCases) noexcept
            : m_numTestCases(in_numTestCases), m_numSections(in_numSections), m_numAssertions(in_numAssertions), m_failedTestCases(in_failedTestCases)
        {
            m_numFailedAssertions = 0;
            for (const auto& testCase : m_failedTestCases)
            {
                m_numFailedAssertions += testCase.GetNumFailedAssertions();
                m_numFailedSections += testCase.GetNumFailedSections();
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumTestCases() const noexcept
        {
            return m_numTestCases;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumFailedTestCases() const noexcept
        {
            return static_cast<u32>(m_failedTestCases.size());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumSections() const noexcept
        {
            return m_numSections;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumFailedSections() const noexcept
        {
            return m_numFailedSections;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumAssertions() const noexcept
        {
            return m_numAssertions;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 Report::GetNumFailedAssertions() const noexcept
        {
            return m_numFailedAssertions;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<FailedTestCase>& Report::GetFailedTestCases() const noexcept
        {
            return m_failedTestCases;
        }
    }
}
