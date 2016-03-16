//
//  TestCase.h
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

#ifndef _UNITTEST_TESTSYSTEM_TESTCASE_H_
#define _UNITTEST_TESTSYSTEM_TESTCASE_H_

#include <CSTest.h>

#include <UnitTest/TestSystem/FailedAssertion.h>

#include <vector>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A container for information on a single test case.
        ///
        /// This is immutable after construction.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TestCase final
        {
        public:
            //------------------------------------------------------------------------------
            /// Construtor. Creates a blank test case.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            TestCase();
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_name - The name of the test case.
            /// @param in_numAssertions - The total number of assertions in the test case.
            /// @param in_failedAssertions - A list containing data on each of the failed
            /// assertions.
            //------------------------------------------------------------------------------
            TestCase(const std::string& in_name, u32 in_numAssertions, const std::vector<FailedAssertion>& in_failedAssertions);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the test case.
            //------------------------------------------------------------------------------
            const std::string& GetName() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of assertions in the test case.
            //------------------------------------------------------------------------------
            u32 GetNumAssertions() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The number of assertions which failed.
            //------------------------------------------------------------------------------
            u32 GetNumFailedAssertions() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A list containing data on each of the failed assertions.
            //------------------------------------------------------------------------------
            const std::vector<FailedAssertion>& GetFailedAssertions() const;
            
        private:
            std::string m_name;
            u32 m_numAssertions;
            std::vector<FailedAssertion> m_failedAssertions;
        };
    }
}

#endif
