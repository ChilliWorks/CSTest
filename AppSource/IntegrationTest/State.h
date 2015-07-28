//
//  State.h
//  CSTest
//  Created by Ian Copland on 13/07/2015.
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

#ifndef _INTEGRATIONTEST_STATE_H_
#define _INTEGRATIONTEST_STATE_H_

#include <CSTest.h>

#include <ChilliSource/Core/State.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// A state which runs a number of low level integration tests using Catch.
        /// All Tests for features which can be automated are included here. Other
        /// features such as input and graphics are smoked tested in other states.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class State : public CSCore::State
        {
            //------------------------------------------------------------------------------
            /// The life-cycle event for creating all state systems.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void CreateSystems() override;
            //------------------------------------------------------------------------------
            /// Initialises the state.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// The life-cycle event when is called every frame that the state is active.
            ///
            /// @author Ian Copland
            ///
            /// @param in_deltaTime - The time passed since the last frame.
            //------------------------------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            
            TestSystem* m_testSystem = nullptr;
            ReportPresenter* m_reportPresenter = nullptr;
            bool m_testsPerformed = false;
            f32 m_timer = 0.0f;
        };
    }
}

#endif
