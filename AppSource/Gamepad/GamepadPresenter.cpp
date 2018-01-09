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

#include <Gamepad/GamepadPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Gamepad
    {
        CS_DEFINE_NAMEDTYPE(GamepadPresenter);

        //------------------------------------------------------------------------------
        GamepadPresenterUPtr GamepadPresenter::Create() noexcept
        {
            return GamepadPresenterUPtr(new GamepadPresenter());
        }

        //------------------------------------------------------------------------------
        bool GamepadPresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (GamepadPresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void GamepadPresenter::InitUI() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            
            auto buttonLabel = basicWidgetFactory->CreateLabel(CS::Vector2(0.5f, 0.6f), smallFont, "", CS::AlignmentAnchor::k_topLeft,
                                                         CS::HorizontalTextJustification::k_left, CS::VerticalTextJustification::k_top);
            buttonLabel->SetRelativePosition(CS::Vector2(0.01f, -0.25f));
            m_buttonTextComponent = buttonLabel->GetComponent<CS::TextUIComponent>();
            
            auto axesLabel = basicWidgetFactory->CreateLabel(CS::Vector2(0.5f, 0.6f), smallFont, "", CS::AlignmentAnchor::k_topRight,
                                                               CS::HorizontalTextJustification::k_right, CS::VerticalTextJustification::k_top);
            axesLabel->SetRelativePosition(CS::Vector2(-0.01f, -0.25f));
            m_axisTextComponent = axesLabel->GetComponent<CS::TextUIComponent>();

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(buttonLabel));
            uiCanvas->AddWidget(std::move(axesLabel));
        }

        //------------------------------------------------------------------------------
        void GamepadPresenter::AddButtonPressureHandler() noexcept
        {
            auto gamepadSystem = CS::Application::Get()->GetSystem<CS::GamepadSystem>();
            if(gamepadSystem)
            {
                m_eventConnections.push_back(gamepadSystem->GetButtonPressureChangedEvent().OpenConnection([=](const CS::Gamepad& gamepad, f64 timestamp, u32 buttonIndex, f32 pressure)
                {
                    if(pressure <= 0.0f)
                    {
                        auto findIterator = std::find_if(m_buttonPressures.begin(), m_buttonPressures.end(), [=](const std::pair<CS::Gamepad, u32>& pair)
                        {
                            return pair.first.GetId() == gamepad.GetId();
                        });
                        
                        if (findIterator != m_buttonPressures.end())
                        {
                            m_buttonPressures.erase(findIterator);
                        }
                    }
                    else
                    {
                        m_buttonPressures.push_back(std::make_pair(gamepad, buttonIndex));
                    }
                    
                    DisplayButtons();
                }));
            }
        }
        
        //------------------------------------------------------------------------------
        void GamepadPresenter::AddAxisPositionHandler() noexcept
        {
            auto gamepadSystem = CS::Application::Get()->GetSystem<CS::GamepadSystem>();
            if(gamepadSystem)
            {
                m_eventConnections.push_back(gamepadSystem->GetAxisPositionChangedEvent().OpenConnection([=](const CS::Gamepad& gamepad, f64 timestamp, CS::GamepadAxis axis, f32 position)
                {
                    if(std::abs(position) < 0.1f)
                    {
                        auto findIterator = std::find_if(m_axisPositions.begin(), m_axisPositions.end(), [=](const std::pair<CS::Gamepad, CS::GamepadAxis>& pair)
                        {
                            return pair.first.GetId() == gamepad.GetId();
                        });

                        if (findIterator != m_axisPositions.end())
                        {
                            m_axisPositions.erase(findIterator);
                        }
                    }
                    else
                    {
                        m_axisPositions.push_back(std::make_pair(gamepad, axis));
                    }

                    DisplayAxes();
                }));
            }
        }

        //------------------------------------------------------------------------------
        void GamepadPresenter::DisplayButtons() noexcept
        {
            std::string text;
            bool first = true;
            for (const auto& pair : m_buttonPressures)
            {
                if (!first)
                {
                    text += "\n";
                }

                first = false;
                text += pair.first.GetName() + CS::ToString(pair.first.GetIndex()) + " Button: " + CS::ToString(pair.second) + " Pressure: " + CS::ToString(pair.first.GetButtonPressure(pair.second));
            }

            m_buttonTextComponent->SetText(text);
        }
        
        //------------------------------------------------------------------------------
        void GamepadPresenter::DisplayAxes() noexcept
        {
            std::string text;
            bool first = true;
            for (const auto& pair : m_axisPositions)
            {
                if (!first)
                {
                    text += "\n";
                }
                
                first = false;
                text += pair.first.GetName() + CS::ToString(pair.first.GetIndex()) + " Axis: " + CS::ToString((u32)pair.second) + " Position: " + CS::ToString(pair.first.GetAxisPosition(pair.second));
            }
            
            m_axisTextComponent->SetText(text);
        }

        //------------------------------------------------------------------------------
        void GamepadPresenter::OnInit() noexcept
        {
            InitUI();
            AddButtonPressureHandler();
            AddAxisPositionHandler();
        }

        //------------------------------------------------------------------------------
        void GamepadPresenter::OnDestroy() noexcept
        {
            m_eventConnections.clear();
        }
    }
}
