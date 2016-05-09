//
//  TestSystem.h
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

#ifndef _UNITTEST_TESTSYSTEM_TESTSYSTEM_H_
#define _UNITTEST_TESTSYSTEM_TESTSYSTEM_H_

#include <CSTest.h>
#include <UnitTest/TestSystem/Report.h>

#include <ChilliSource/Core/System.h>

namespace Catch { class Session; }

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A system for performing integration tests using Catch. The tests should be
        /// setup using standard Catch format.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class TestSystem final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(TestSystem);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CS::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// Performs all integration tests and returns a report detailing whether or not
            /// they were successful.
            ///
            /// @author Ian Copland
            ///
            /// @return The test report.
            //------------------------------------------------------------------------------
            Report PerformTests();
            
        private:
            friend class CS::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static TestSystemUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<TestSystem>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            TestSystem() = default;
            //------------------------------------------------------------------------------
            /// Initialises the test system.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            
            static Catch::Session s_session; //there can only be one catch session
        };
    }
}

#endif
