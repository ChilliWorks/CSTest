//
//  TestRegistry.h
//  CSTest
//  Created by Ian Copland on 15/03/2016.
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

#ifndef _INTEGRATIONTEST_TESTSYSTEM_TESTREGISTRY_H_
#define _INTEGRATIONTEST_TESTSYSTEM_TESTREGISTRY_H_

#include <CSTest.h>

#include <IntegrationTest/TestSystem/TestDesc.h>

#include <vector>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// Contains a list of all registered tests. This is a singleton so that it can
        /// be accessed from multiple translation units during static initialisation.
        ///
        /// This is not thread-safe.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TestRegistry final
        {
        public:
            CS_DECLARE_NOCOPY(TestRegistry);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The singleton instane of the TestRegistry.
            //------------------------------------------------------------------------------
            static TestRegistry& Get() noexcept;
            //------------------------------------------------------------------------------
            /// Adds a new test description to the registry. This is typically handled via
            /// a AutoRegister instance.
            ///
            /// @author Ian Copland
            ///
            /// @param in_desc - The new test description.
            //------------------------------------------------------------------------------
            void RegisterTest(const TestDesc& in_desc) noexcept;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The list of all registered tests.
            //------------------------------------------------------------------------------
            const std::vector<TestDesc>& GetTests() const noexcept;
            
        private:
            //------------------------------------------------------------------------------
            /// Default constructor.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            TestRegistry() = default;
            
            std::vector<TestDesc> m_tests;
        };
    }
}

#endif
