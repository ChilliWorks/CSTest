//
//  FailedTestCase.h
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

#ifndef _UNITTEST_TESTSYSTEM_FAILEDTESTCASE_H_
#define _UNITTEST_TESTSYSTEM_FAILEDTESTCASE_H_

#include <CSTest.h>

#include <UnitTest/TestSystem/FailedSection.h>

#include <vector>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A container for information on a single failed test case.
        ///
        /// This is immutable after construction.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class FailedTestCase final
        {
        public:
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_name - The name of the test case.
            /// @param in_numSections - The total number of sections in the test case.
            /// @param in_numAssertions - The total number of assertions in the test case.
            /// @param in_failedSections - A list containing data on each failed section.
            //------------------------------------------------------------------------------
            FailedTestCase(const std::string& in_name, u32 in_numSections, u32 in_numAssertions, const std::vector<FailedSection>& in_failedSections) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the test case.
            //------------------------------------------------------------------------------
            const std::string& GetName() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of test sections in the test case.
            //------------------------------------------------------------------------------
            u32 GetNumSections() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of test sections with at least one failed assertion.
            //------------------------------------------------------------------------------
            u32 GetNumFailedSections() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of assertions in the test case.
            //------------------------------------------------------------------------------
            u32 GetNumAssertions() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of assertions which failed.
            //------------------------------------------------------------------------------
            u32 GetNumFailedAssertions() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A list containing data on each of the failed sections in the test
            /// case.
            //------------------------------------------------------------------------------
            const std::vector<FailedSection>& GetFailedSections() const noexcept;
            
        private:
            std::string m_name;
            u32 m_numSections;
            u32 m_numAssertions;
            u32 m_numFailedAssertions = 0;
            std::vector<FailedSection> m_failedSections;
        };
    }
}

#endif
