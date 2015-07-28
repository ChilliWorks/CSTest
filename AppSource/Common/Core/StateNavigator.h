//
//  StateNavigator.h
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

#ifndef _COMMON_CORE_STATENAVIGATOR_H_
#define _COMMON_CORE_STATENAVIGATOR_H_

#include <CSTest.h>
#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A system which provides navigation between the different test states. This
        /// will present an on screen next button which can be used to navigate to the
        /// state after this one. The next state type is specified by the system template
        /// parameter.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        template <typename TNextState> class StateNavigator final : public CSCore::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(StateNavigator);
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
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the next button is visible. If the button is not
            /// visible, it cannot be pressed.
            //------------------------------------------------------------------------------
            bool IsNextButtonVisible() const;
            //------------------------------------------------------------------------------
            /// Whether or not the next button is visible. If the button is not visible,
            /// it cannot be pressed.
            ///
            /// @author Ian Copland
            ///
            /// @param in_visibile - Whether or not to make the button visible.
            //------------------------------------------------------------------------------
            void SetNextButtonVisible(bool in_visibile);
            
        private:
            friend class CSCore::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static std::unique_ptr<StateNavigator<TNextState>> Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<StateNavigator>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            StateNavigator() = default;
            //------------------------------------------------------------------------------
            /// Initialises the test navigator.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Destroys the test navigator.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            CSUI::WidgetSPtr m_ui;
            CSUI::WidgetSPtr m_nextButton;
            
            CSCore::EventConnectionUPtr m_nextPressedConnection;
        };
    }
}

#include <Common/Core/StateNavigatorImpl.h>

#endif
