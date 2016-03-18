//
//  TestNavigator.h
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

#ifndef _COMMON_CORE_TESTNAVIGATOR_H_
#define _COMMON_CORE_TESTNAVIGATOR_H_

#include <CSTest.h>
#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        /// A system which provides navigation back to the previous test menu. It also
        /// provides the test with title text.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
       class TestNavigator final : public CSCore::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(TestNavigator);
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
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const noexcept override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the back button is visible. If the button is not
            /// visible, it cannot be pressed.
            //------------------------------------------------------------------------------
            bool IsBackButtonVisible() const noexcept;
            //------------------------------------------------------------------------------
            /// Whether or not the back button is visible. If the button is not visible,
            /// it cannot be pressed.
            ///
            /// @author Ian Copland
            ///
            /// @param in_visible - Whether or not to make the button visible.
            //------------------------------------------------------------------------------
            void SetBackButtonVisible(bool in_visible) noexcept;
            
        private:
            friend class CSCore::State;
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @param in_title - The title.
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static TestNavigatorUPtr Create(const std::string& in_title) noexcept;
            //------------------------------------------------------------------------------
            /// Constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<TestNavigator>().
            ///
            /// @author Ian Copland
            ///
            /// @param in_title - The title.
            //------------------------------------------------------------------------------
            TestNavigator(const std::string& in_title) noexcept;
            //------------------------------------------------------------------------------
            /// Initialises the Test Navigator.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() noexcept override;
            //------------------------------------------------------------------------------
            /// Destroys the Test Navigator.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() noexcept override;
            
            std::string m_title;
            
            CSUI::WidgetSPtr m_ui;
            CSUI::WidgetSPtr m_titleLabel;
            CSUI::WidgetSPtr m_backButton;
            
            CSCore::EventConnectionUPtr m_backPressedConnection;
        };
    }
}

#endif
