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

#include <Keyboard/KeyboardPresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Input/Keyboard.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Keyboard
    {
        CS_DEFINE_NAMEDTYPE(KeyboardPresenter);

        //------------------------------------------------------------------------------
        KeyboardPresenterUPtr KeyboardPresenter::Create() noexcept
        {
            return KeyboardPresenterUPtr(new KeyboardPresenter());
        }

        //------------------------------------------------------------------------------
        bool KeyboardPresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (KeyboardPresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void KeyboardPresenter::InitUI() noexcept
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
        void KeyboardPresenter::AddKeyPressHandler() noexcept
        {
            auto keyboardSystem = CS::Application::Get()->GetSystem<CS::Keyboard>();

            if (keyboardSystem == nullptr)
            {
                m_textComponent->SetText("No Keyboard.");
                return;
            }

            m_eventConnections.push_back(keyboardSystem->GetKeyPressedEvent().OpenConnection([=](CS::KeyCode keyPressed, std::vector<CS::ModifierKeyCode> modifiers)
            {
                m_keysHeld.push_back(keyPressed);
                m_modifiersHeld = modifiers;

                DisplayKeys();
            }));
        }

        //------------------------------------------------------------------------------
        void KeyboardPresenter::AddKeyReleaseHandler() noexcept
        {
            auto keyboardSystem = CS::Application::Get()->GetSystem<CS::Keyboard>();

            if (keyboardSystem == nullptr)
            {
                m_textComponent->SetText("No Keyboard.");
                return;
            }

            m_eventConnections.push_back(keyboardSystem->GetKeyReleasedEvent().OpenConnection([=](CS::KeyCode keyReleased)
            {
                auto findIterator = std::find(m_keysHeld.begin(), m_keysHeld.end(), keyReleased);
                if (findIterator != m_keysHeld.end())
                {
                    m_keysHeld.erase(findIterator);
                }

                DisplayKeys();
            }));
            
        }

        //------------------------------------------------------------------------------
        void KeyboardPresenter::DisplayKeys() noexcept
        {
            std::string text = "Keys Held: ";

            for (CS::KeyCode key : m_keysHeld)
            {
                text += CS::ToString(u32(key)) + ", ";
            }

            text += "\nModifiers Held: ";
            // Not using m_modifiersHeld because release events are not generated for
            // the modifier list in the same way.
            for (CS::KeyCode mod : m_keysHeld)
            {
                switch (mod)
                {
                    case CS::KeyCode::k_leftAlt:
                    case CS::KeyCode::k_rightAlt:
                        text += "ALT, ";
                        break;
                    case CS::KeyCode::k_leftShift:
                    case CS::KeyCode::k_rightShift:
                        text += "SHIFT, ";
                        break;
                    case CS::KeyCode::k_leftCtrl:
                    case CS::KeyCode::k_rightCtrl:
                        text += "CTRL, ";
                        break;
                    case CS::KeyCode::k_leftSystem:
                    case CS::KeyCode::k_rightSystem:
                        text += "SYSTEM, ";
                        break;
               }
            }

            m_textComponent->SetText(text);
        }

        //------------------------------------------------------------------------------
        void KeyboardPresenter::OnInit() noexcept
        {
            InitUI();
            DisplayKeys();
            AddKeyPressHandler();
            AddKeyReleaseHandler();
        }

        //------------------------------------------------------------------------------
        void KeyboardPresenter::OnDestroy() noexcept
        {
            m_eventConnections.clear();
        }
    }
}