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

#ifndef _KEYBOARD_KEYBOARDPRESENTER_H_
#define _KEYBOARD_KEYBOARDPRESENTER_H_

#include <CSTest.h>

#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/System.h>

namespace CSTest
{
    namespace Keyboard
    {
        /// A system which displays keypress information on-screen.
        ///
        class KeyboardPresenter final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(KeyboardPresenter);

            /// Allows querying of whether or not the component implements the
            /// interface associated with the given interface Id.
            ///
            /// @param interfaceId 
            ///     The interface Id.
            ///
            /// @return Whether the object implements the given interface.
            ///
            bool IsA(CS::InterfaceIDType interfaceId) const noexcept override;
            
        private:
            friend class CS::State;

            /// Creates a new instance of the system.
            ///
            /// @return The new instance.
            ///
            static KeyboardPresenterUPtr Create() noexcept;
  
            /// Constructor
            ///
            KeyboardPresenter() = default;
            
            /// Initialises all of the UI on which the gesture information will be presented.
            ///
            void InitUI() noexcept;
            
            /// Called when the presenter is first created. This will set up the
            /// information which should be presented.
            ///
            void OnInit() noexcept override;

            /// Called during init; adds event receiver for key presses.
            ///
            void AddKeyPressHandler() noexcept;

            /// Called during init; adds event receiver for key releases.
            ///
            void AddKeyReleaseHandler() noexcept;

            /// Displays held keys on screen.
            ///
            void DisplayKeys() noexcept;
            
            /// Called when the presenter is about to be destroyed.
            ///
            void OnDestroy() noexcept override;
            
            std::vector<CS::KeyCode> m_keysHeld;
            std::vector<CS::EventConnectionUPtr> m_eventConnections;
            
            CS::WidgetSPtr m_rootUI;
            CS::TextUIComponent* m_textComponent = nullptr;
        };
    }
}

#endif