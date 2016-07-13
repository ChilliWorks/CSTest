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

#include <Screen/ScreenPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Screen
    {
        namespace
        {
            constexpr u32 k_resolutionsPerLine = 4;
        }
        CS_DEFINE_NAMEDTYPE(ScreenPresenter);

        //------------------------------------------------------------------------------
        ScreenPresenterUPtr ScreenPresenter::Create() noexcept
        {
            return ScreenPresenterUPtr(new ScreenPresenter());
        }

        //------------------------------------------------------------------------------
        bool ScreenPresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (ScreenPresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void ScreenPresenter::InitUI() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            auto label = basicWidgetFactory->CreateLabel(CS::Vector2(1.0f, 0.9f), smallFont, "", CS::AlignmentAnchor::k_middleCentre,
                                                         CS::HorizontalTextJustification::k_left, CS::VerticalTextJustification::k_top);
            m_textComponent = label->GetComponent<CS::TextUIComponent>();
            label->SetRelativePosition(CS::Vector2(0.0f, -0.35f));

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(label));

            UpdateUI();

            auto screen = CS::Application::Get()->GetSystem<CS::Screen>();

            m_eventConnection = screen->GetResolutionChangedEvent().OpenConnection([=](const CS::Vector2& newResolution)
            {
                UpdateUI();
            });      
        }

        //------------------------------------------------------------------------------
        void ScreenPresenter::UpdateUI() noexcept
        {
            auto screen = CS::Application::Get()->GetSystem<CS::Screen>();

            std::string screenInfo = "Screen Information:";
            screenInfo += "\nScreen Resolution: X: " + CS::ToString(screen->GetResolution().x) + " Y: " + CS::ToString(screen->GetResolution().y);
            screenInfo += "\nDensity Scale: " + CS::ToString(screen->GetDensityScale());
            screenInfo += "\nInverse Density Scale: " + CS::ToString(screen->GetInverseDensityScale());
            screenInfo += "\nSupported Resolutions:\n";

            int count = 0;
            for (auto screenResolution : screen->GetSupportedResolutions())
            {
                screenInfo += CS::ToString(screenResolution.x) + "x" + CS::ToString(screenResolution.y);
                count++;
                if (count < screen->GetSupportedResolutions().size())
                {
                    screenInfo += ", ";
                }
                if (count % k_resolutionsPerLine == 0)
                {
                    screenInfo += "\n";
                }
            }
            m_textComponent->SetText(screenInfo);
        }

        //------------------------------------------------------------------------------
        void ScreenPresenter::OnInit() noexcept
        {
            InitUI();
        }

    }
}
