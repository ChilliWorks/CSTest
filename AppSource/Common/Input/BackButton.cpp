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

#include <Common/Input/BackButton.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Delegate.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(BackButtonSystem);

        //-----------------------------------------------------
        BackButtonSystemUPtr BackButtonSystem::Create() noexcept
        {
            return BackButtonSystemUPtr(new BackButtonSystem());
        }

        //-----------------------------------------------------
        bool BackButtonSystem::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (BackButtonSystem::InterfaceID == interfaceId);
        }

        //-----------------------------------------------------
        void BackButtonSystem::OnResume() noexcept
        {
            auto deviceButtonSystem = CS::Application::Get()->GetSystem<ChilliSource::DeviceButtonSystem>();
            CS_ASSERT(deviceButtonSystem, "No device button system active.");

            m_deviceButtonConnection = deviceButtonSystem->GetTriggeredEvent().OpenConnection(ChilliSource::MakeDelegate(this, &BackButtonSystem::OnDeviceButtonPressed));
        }

        //-----------------------------------------------------
        void BackButtonSystem::OnSuspend() noexcept
        {
            m_deviceButtonConnection = nullptr;
        }

        //-----------------------------------------------------
        void BackButtonSystem::OnDeviceButtonPressed(const ChilliSource::DeviceButtonSystem::DeviceButton& buttonPressed) noexcept
        {
            if (buttonPressed == ChilliSource::DeviceButtonSystem::DeviceButton::k_backButton)
            {
                auto stateManager = CS::Application::Get()->GetStateManager();
                CS_ASSERT(stateManager, "No state manager active.");

                if (stateManager->GetNumStates() > 1)
                {
                    stateManager->Pop();
                }
                else
                {
                    CS::Application::Get()->Quit();
                }
            }
        }

        //-------------------------------------------------------
        void BackButtonSystem::OnDestroy() noexcept
        {
            m_deviceButtonConnection = nullptr;
        }
    }
}