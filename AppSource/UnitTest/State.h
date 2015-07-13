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

#ifndef _UNITTEST_STATE_H_
#define _UNITTEST_STATE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/State.h>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A state which runs a number of unit tests using Catch. The tests performed
        /// here are not strictly unit tests as they are implemented at a higher level
        /// than a "unit" and implement no stubbing or mocking. Automated integration
        /// tests would probably be a more accurate description. They are referred to as
        /// Unit Tests because they are implemented using a Unit Testing API and to
        /// separate them from the visual integration tests.
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
            /// A life-cycle event called when the State is first added to the state stack,
            /// immediately after creation of state systems.
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
            //------------------------------------------------------------------------------
            /// The life-cycle event called when the State is about to be destroyed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
        };
    }
}

#endif
