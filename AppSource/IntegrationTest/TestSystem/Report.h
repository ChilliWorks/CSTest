//
//  Report.h
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_REPORT_H_
#define _INTEGRATIONTEST_TESTSYSTEM_REPORT_H_

#include <CSTest.h>

#include <IntegrationTest/TestSystem/TestDesc.h>

#include <vector>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// An immutable container for all information on a set of integration tests.
        ///
        /// This is immutable and therefore thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class Report final
        {
        public:
            //------------------------------------------------------------------------------
            /// An immutable container for information on a failed integration test.
            ///
            /// This is immutable and therefore thread-safe.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            class FailedTest final
            {
            public:
                //------------------------------------------------------------------------------
                /// Constructs a new instance with the given test description and error message.
                ///
                /// @author Ian Copland
                ///
                /// @param in_desc - The test description.
                /// @param in_errorMessage - A message describing why the test failed.
                //------------------------------------------------------------------------------
                FailedTest(const TestDesc& in_desc, const std::string& in_errorMessage) noexcept;
                //------------------------------------------------------------------------------
                /// @author Ian Copland
                ///
                /// @return The test description.
                //------------------------------------------------------------------------------
                const TestDesc& GetDesc() const noexcept;
                //------------------------------------------------------------------------------
                /// @author Ian Copland
                ///
                /// @return A message describing why the test failed.
                //------------------------------------------------------------------------------
                const std::string& GetErrorMessage() const noexcept;
                
            private:
                TestDesc m_desc;
                std::string m_errorMessage;
            };
            //------------------------------------------------------------------------------
            /// Constructs a new instance with the given list of failed tests.
            ///
            /// @author Ian Copland
            ///
            /// @param in_numTests - The total number of tests.
            /// @param in_failedTests - A list of information on the failed tests.
            //------------------------------------------------------------------------------
            Report(u32 in_numTests, const std::vector<FailedTest>& in_failedTests) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of tests.
            //------------------------------------------------------------------------------
            u32 GetNumTests() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of tests which failed.
            //------------------------------------------------------------------------------
            u32 GetNumFailedTests() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of failed tests.
            //------------------------------------------------------------------------------
            const std::vector<FailedTest>& GetFailedTests() const noexcept;
            
        private:
            u32 m_numTests;
            std::vector<FailedTest> m_failedTests;
        };
    }
}

#endif
