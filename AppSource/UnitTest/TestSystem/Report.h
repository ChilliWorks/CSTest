//
//  Report.h
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

#ifndef _UNITTEST_TESTSYSTEM_REPORT_H_
#define _UNITTEST_TESTSYSTEM_REPORT_H_

#include <CSTest.h>
#include <UnitTest/TestSystem/FailedTestCase.h>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// Provides information on the overall progress of a test run through the
        /// TestSystem.
        ///
        /// This is immutable after construction.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class Report final
        {
        public:
            //------------------------------------------------------------------------------
            /// Construtor. Creates a blank report.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            Report() = default;
            //------------------------------------------------------------------------------
            /// Construtor. Takes all input data for the Report.
            ///
            /// @author Ian Copland
            ///
            /// @param in_numTestCases - The total number of test cases in the test run.
            /// @param in_numSections - The total number of sections in the test run.
            /// @param in_numAssertions - The total number of assertions in the test run.
            /// @param in_failedTestCases - The list of failed test cases.
            //------------------------------------------------------------------------------
            Report(u32 in_numTestCases, u32 in_numSections, u32 in_numAssertions, const std::vector<FailedTestCase>& in_failedTestCases) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of test cases that were run.
            //------------------------------------------------------------------------------
            u32 GetNumTestCases() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of test cases which failed.
            //------------------------------------------------------------------------------
            u32 GetNumFailedTestCases() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of sections that were run.
            //------------------------------------------------------------------------------
            u32 GetNumSections() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of sections which failed.
            //------------------------------------------------------------------------------
            u32 GetNumFailedSections() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of assertions that were run.
            //------------------------------------------------------------------------------
            u32 GetNumAssertions() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of failed assertions accross all tests cases.
            //------------------------------------------------------------------------------
            u32 GetNumFailedAssertions() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of failed assertions.
            //------------------------------------------------------------------------------
            const std::vector<FailedTestCase>& GetFailedTestCases() const noexcept;
            
        private:
            u32 m_numTestCases = 0;
            u32 m_numSections = 0;
            u32 m_numAssertions = 0;
            u32 m_numFailedSections = 0;
            u32 m_numFailedAssertions = 0;
            std::vector<FailedTestCase> m_failedTestCases;
        };
    }
}

#endif
