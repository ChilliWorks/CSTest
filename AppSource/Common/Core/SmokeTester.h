//
//  SmokeTester.h
//  CSTest
//  Created by Ian Copland on 28/07/2015.
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

#ifndef _COMMON_CORE_SMOKETESTER_H_
#define _COMMON_CORE_SMOKETESTER_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A system for presenting the user with different smoke test options. This
        /// presents a title and a series of user defined buttons, each of which can have
        /// an action associated with it.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class SmokeTester final : public CSCore::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(SmokeTester);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param in_interfaceId - The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// Presents a series of buttons, each associated with a test.
            ///
            /// @author Ian Copland
            ///
            /// @param in_testSet - The information on the tests which should be presented.
            //------------------------------------------------------------------------------
            void Present(const SmokeTestSet& in_testSet);
            //------------------------------------------------------------------------------
            /// Cleans up all existing buttons, and resets the system back into a reusable
            /// state.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Dismiss();
            
        private:
            friend class CSCore::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static SmokeTesterUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<SmokeTester>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SmokeTester() = default;
            //------------------------------------------------------------------------------
            /// Initialises the Smoke Tester.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Destroys the Smoke Tester.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            CSUI::WidgetSPtr m_ui;
            CSUI::WidgetSPtr m_titleText;
            CSUI::WidgetSPtr m_buttonContainer;
            
            std::vector<CSUI::WidgetSPtr> m_buttons;
            std::vector<CSCore::EventConnectionUPtr> m_connectionContainer;
        };
    }
}

#endif
