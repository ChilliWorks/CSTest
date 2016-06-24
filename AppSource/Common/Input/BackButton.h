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
        /// the back button on Android, allowing for simple navigation
        /// of the testing menus, while doubling as a smoke test for
        /// DeviceButtonSystem.
        ///
        class BackButtonSystem final : public CS::StateSystem
        {
        public:

        private:
            /// Called when the system is initialized.
            /// Sets up the DeviceButtonSystem delegate.
            ///
            void OnInit() noexcept override;

            /// Called when a device button is pressed.
            ///
            void OnDeviceButtonPressed(const ChilliSource::DeviceButtonSystem::DeviceButton& buttonPressed) noexcept;

        };
    }
}

#endif