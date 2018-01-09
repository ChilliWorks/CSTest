//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifndef _GAMEPAD_GAMEPADPRESENTER_H_
#define _GAMEPAD_GAMEPADPRESENTER_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Input/Gamepad.h>

namespace CSTest
{
    namespace Gamepad
    {
        /// A system which displays gamepad information on-screen.
        ///
        class GamepadPresenter final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(GamepadPresenter);

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
            static GamepadPresenterUPtr Create() noexcept;
  
            /// Constructor
            ///
            GamepadPresenter() = default;
            
            /// Initialises all of the UI on which the gesture information will be presented.
            ///
            void InitUI() noexcept;
            
            /// Called when the presenter is first created. This will set up the
            /// information which should be presented.
            ///
            void OnInit() noexcept override;

            /// Called during init; adds event receiver for button presses.
            ///
            void AddButtonPressureHandler() noexcept;
            
            /// Called during init; adds event receiver for axis movements.
            ///
            void AddAxisPositionHandler() noexcept;

            /// Displays pressed buttons on screen.
            ///
            void DisplayButtons() noexcept;
            
            /// Displays moved axes on screen.
            ///
            void DisplayAxes() noexcept;
            
            /// Called when the presenter is about to be destroyed.
            ///
            void OnDestroy() noexcept override;
            
            std::vector<std::pair<CS::Gamepad, u32>> m_buttonPressures;
            std::vector<std::pair<CS::Gamepad, CS::GamepadAxis>> m_axisPositions;
            std::vector<CS::EventConnectionUPtr> m_eventConnections;
            
            CS::WidgetSPtr m_rootUI;
            CS::TextUIComponent* m_axisTextComponent = nullptr;
            CS::TextUIComponent* m_buttonTextComponent = nullptr;
        };
    }
}

#endif
