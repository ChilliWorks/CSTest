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

#include <Device/DevicePresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Device
    {
        CS_DEFINE_NAMEDTYPE(DevicePresenter);

        //------------------------------------------------------------------------------
        DevicePresenterUPtr DevicePresenter::Create() noexcept
        {
            return DevicePresenterUPtr(new DevicePresenter());
        }

        //------------------------------------------------------------------------------
        bool DevicePresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (DevicePresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void DevicePresenter::InitUI() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            auto label = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 0.8f), smallFont, "", CS::AlignmentAnchor::k_middleCentre,
                                                         CS::HorizontalTextJustification::k_left, CS::VerticalTextJustification::k_top);
            m_textComponent = label->GetComponent<CS::TextUIComponent>();

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(label));

            auto device = CS::Application::Get()->GetSystem<CS::Device>();

            m_textComponent->SetText("Device Information:\n Device Model: " + device->GetModel() + "\n Device Model Type: " + device->GetModelType() + "\n Device Manufacturer: " + device->GetManufacturer() + "\n Device UDID: " + device->GetUDID() + "\n Device Locale: " + device->GetLocale() + "\n Device Language: " + device->GetLanguage() + "\n Device OS Version: " + device->GetOSVersion() + "\n Number of CPU Cores: " + CS::ToString(device->GetNumberOfCPUCores()));
        }

        //------------------------------------------------------------------------------
        void DevicePresenter::OnInit() noexcept
        {
            InitUI();
        }

    }
}
