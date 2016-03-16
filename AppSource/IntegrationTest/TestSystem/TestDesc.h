//
//  TestDesc.h
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_TESTDESC_H_
#define _INTEGRATIONTEST_TESTSYSTEM_TESTDESC_H_

#include <CSTest.h>

#include <vector>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// TODO
        ///
        /// This is immutable and therefore thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TestDesc final
        {
        public:
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            using TestDelegate = std::function<void(Test*) noexcept>;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            TestDesc(const std::string& in_testCaseName, const std::string& in_testName, const TestDelegate& in_testDelegate) noexcept;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            const std::string& GetTestCaseName() const noexcept;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            const std::string& GetTestName() const noexcept;
            //------------------------------------------------------------------------------
            /// TODO
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            const TestDelegate& GetTestDelegate() const noexcept;
            
        private:
            std::string m_testCaseName;
            std::string m_testName;
            TestDelegate m_testDelegate;
        };
    }
}

#endif
