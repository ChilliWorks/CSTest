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

#include <Accelerometer/AccelerometerPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Input/Accelerometer.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Accelerometer
    {
        CS_DEFINE_NAMEDTYPE(AccelerometerPresenter);

        //------------------------------------------------------------------------------
        AccelerometerPresenterUPtr AccelerometerPresenter::Create() noexcept
        {
            return AccelerometerPresenterUPtr(new AccelerometerPresenter());
        }

        //------------------------------------------------------------------------------
        bool AccelerometerPresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (AccelerometerPresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void AccelerometerPresenter::InitUI() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            auto label = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 0.8f), smallFont, "", CS::AlignmentAnchor::k_middleCentre,
                                                         CS::HorizontalTextJustification::k_left, CS::VerticalTextJustification::k_top);
            m_textComponent = label->GetComponent<CS::TextUIComponent>();

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(label));
        }

        //------------------------------------------------------------------------------
        void AccelerometerPresenter::AddAccelerationHandler() noexcept
        {
            CS_ASSERT(!m_accelerometer, "Accelerometer reference unexpected - this method should only be called once.");
            m_accelerometer = CS::Application::Get()->GetSystem<CS::Accelerometer>();
            
            if (m_accelerometer == nullptr)
            {
                m_textComponent->SetText("No Accelerometer.");
                return;
            }
            
            m_accelerometer->StartUpdating();

            m_eventConnection = m_accelerometer->GetAccelerationUpdatedEvent().OpenConnection([=](const CS::Vector3& acceleration)
            {
                DisplayAcceleration(acceleration);
            });
        }

        //------------------------------------------------------------------------------
        void AccelerometerPresenter::DisplayAcceleration(const CS::Vector3& acceleration) noexcept
        {
            std::string text = "Device Acceleration: ";

            text += "\nX: " + CS::ToString(acceleration.x);
            text += "\nY: " + CS::ToString(acceleration.y);
            text += "\nZ: " + CS::ToString(acceleration.z);

            m_textComponent->SetText(text);
        }

        //------------------------------------------------------------------------------
        void AccelerometerPresenter::OnInit() noexcept
        {
            InitUI();
            AddAccelerationHandler();
        }

        //------------------------------------------------------------------------------
        void AccelerometerPresenter::OnDestroy() noexcept
        {
            if (m_accelerometer)
            {
                m_accelerometer->StopUpdating();
            }
            m_eventConnection.reset();
        }
    }
}
