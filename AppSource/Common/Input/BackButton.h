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

#ifndef _COMMON_INPUT_BACKBUTTON_H_
#define _COMMON_INPUT_BACKBUTTON_H_

#include <CSTest.h>

#include <ChilliSource/Core/System.h>
#include <ChilliSource/Input/DeviceButtons.h>

namespace CSTest
{
    namespace Common
    {
        /// This is a state system which listens for a press of
        /// the back device button, allowing for simple navigation
        /// of the testing menus, while doubling as a smoke test for
        /// DeviceButtonSystem.
        ///
        class BackButtonSystem final : public CS::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(BackButtonSystem);

            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @param interfaceId 
            ///    The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            ///
            bool IsA(CS::InterfaceIDType interfaceId) const noexcept override;

        private:
            friend class CS::State;

            /// A factory method for creating new instances of the system.
            ///
            /// @return The new instance.
            ///
            static BackButtonSystemUPtr Create() noexcept;

            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<ResultPresenter>().
            ///
            BackButtonSystem() = default;

            /// Called when the state is resumed.
            /// Sets up the DeviceButtonSystem delegate.
            ///
            void OnResume() noexcept override;

            /// Called when the state is suspended.
            /// Removes the button press delegate.
            ///
            void OnSuspend() noexcept override;

            /// Called when the system is destroyed,
            /// and removes the button press delegate.
            ///
            void OnDestroy() noexcept override;

            /// Called when a device button is pressed.
            ///
            void OnDeviceButtonPressed(const ChilliSource::DeviceButtonSystem::DeviceButton& buttonPressed) noexcept;

            CS::EventConnectionUPtr m_deviceButtonConnection;
            

        };
    }
}

#endif