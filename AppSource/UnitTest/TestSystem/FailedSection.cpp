//
//  FailedSection.cpp
//  CSTest
//  Created by Ian Copland on 18/03/2016.
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

#include <UnitTest/TestSystem/FailedSection.h>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        FailedSection::FailedSection(const std::string& in_name, u32 in_numAssertions, const std::vector<FailedAssertion>& in_failedAssertions) noexcept
            : m_name(in_name), m_numAssertions(in_numAssertions), m_failedAssertions(in_failedAssertions)
        {
            CS_ASSERT(in_failedAssertions.size() > 0, "Failed section cannot contain zero failed assertions.");
            CS_ASSERT(in_numAssertions >= in_failedAssertions.size(), "There cannot be more failed assertions than there are assertions.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::string& FailedSection::GetName() const noexcept
        {
            return m_name;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 FailedSection::GetNumAssertions() const noexcept
        {
            return m_numAssertions;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        u32 FailedSection::GetNumFailedAssertions() const noexcept
        {
            return u32(m_failedAssertions.size());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        const std::vector<FailedAssertion>& FailedSection::GetFailedAssertions() const noexcept
        {
            return m_failedAssertions;
        }
    }
}
