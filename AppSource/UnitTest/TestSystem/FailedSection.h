//
//  FailedSection.h
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

#ifndef _UNITTEST_TESTSYSTEM_FAILEDSECTION_H_
#define _UNITTEST_TESTSYSTEM_FAILEDSECTION_H_

#include <CSTest.h>

#include <UnitTest/TestSystem/FailedAssertion.h>

#include <vector>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A container for information on a single failed test section.
        ///
        /// This is immutable after construction.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class FailedSection final
        {
        public:
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_name - The name of the section.
            /// @param in_numAssertions - The total number of assertions in the section.
            /// @param in_failedAssertions - A list containing data on each of the failed
            /// assertions.
            //------------------------------------------------------------------------------
            FailedSection(const std::string& in_name, u32 in_numAssertions, const std::vector<FailedAssertion>& in_failedAssertions) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The name of the section.
            //------------------------------------------------------------------------------
            const std::string& GetName() const noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The total number of assertions in the section.
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
            /// @return A list containing data on each of the failed assertions.
            //------------------------------------------------------------------------------
            const std::vector<FailedAssertion>& GetFailedAssertions() const noexcept;
            
        private:
            std::string m_name;
            u32 m_numAssertions;
            std::vector<FailedAssertion> m_failedAssertions;
        };
    }
}

#endif
