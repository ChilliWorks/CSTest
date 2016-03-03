//
//  GesturePresenter.cpp
//  CSTest
//  Created by Ian Copland on 03/03/2016.
//
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

#include <Gesture/GesturePresenter.h>

#include <Common/UI/BasicWidgetFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Input/Gesture.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Gesture
    {
        CS_DEFINE_NAMEDTYPE(GesturePresenter);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        GesturePresenterUPtr GesturePresenter::Create()
        {
            return GesturePresenterUPtr(new GesturePresenter());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool GesturePresenter::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (GesturePresenter::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::InitGestures()
        {
            auto gestureSystem = GetState()->GetSystem<CSInput::GestureSystem>();
            CS_ASSERT(gestureSystem, "Must have gesture system to use gesture presenter");
            
            // Tap
            auto tapGesture = std::make_shared<CSInput::TapGesture>();
            m_eventConnections.push_back(tapGesture->GetTappedEvent().OpenConnection([=](const CSInput::TapGesture*, const CSCore::Vector2&)
            {
                m_gestureInfo.m_numTaps++;
                m_uiDirty = true;
            }));
            gestureSystem->AddGesture(tapGesture);

            // Double tap
            auto doubleTapGesture = std::make_shared<CSInput::TapGesture>(2);
            m_eventConnections.push_back(doubleTapGesture->GetTappedEvent().OpenConnection([=](const CSInput::TapGesture*, const CSCore::Vector2&)
            {
                m_gestureInfo.m_numDoubleTaps++;
                m_uiDirty = true;
            }));
            gestureSystem->AddGesture(doubleTapGesture);
            
            // Two finger tap
            auto twoFingerTapGesture = std::make_shared<CSInput::TapGesture>(1, 2);
            m_eventConnections.push_back(twoFingerTapGesture->GetTappedEvent().OpenConnection([=](const CSInput::TapGesture*, const CSCore::Vector2&)
            {
                m_gestureInfo.m_numTwoFingerTaps++;
                m_uiDirty = true;
            }));
            gestureSystem->AddGesture(twoFingerTapGesture);

            // Two finger double tap
            auto twoFingerDoubleTapGesture = std::make_shared<CSInput::TapGesture>(2, 2);
            m_eventConnections.push_back(twoFingerDoubleTapGesture->GetTappedEvent().OpenConnection([=](const CSInput::TapGesture*, const CSCore::Vector2&)
            {
                m_gestureInfo.m_numTwoFingerDoubleTaps++;
                m_uiDirty = true;
            }));
            gestureSystem->AddGesture(twoFingerDoubleTapGesture);
            
            // Hold
            auto holdGesture = std::make_shared<CSInput::HoldGesture>();
            m_eventConnections.push_back(holdGesture->GetHeldEvent().OpenConnection([=](const CSInput::HoldGesture*, const CSCore::Vector2&)
            {
                m_gestureInfo.m_numHolds++;
                m_uiDirty = true;
            }));
            gestureSystem->AddGesture(holdGesture);

            // Two finger hold
            auto twoFingerHoldGesture = std::make_shared<CSInput::HoldGesture>(2);
            m_eventConnections.push_back(twoFingerHoldGesture->GetHeldEvent().OpenConnection([=](const CSInput::HoldGesture*, const CSCore::Vector2&)
            {
                m_gestureInfo.m_numTwoFingerHolds++;
                m_uiDirty = true;
            }));
            gestureSystem->AddGesture(twoFingerHoldGesture);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::InitUI()
        {
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CSRendering::Font>(CSCore::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CSCore::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            auto label = basicWidgetFactory->CreateLabel(CSCore::Vector2(0.9f, 0.8f), smallFont, "", CSRendering::AlignmentAnchor::k_middleCentre,
                                                         CSRendering::HorizontalTextJustification::k_left, CSRendering::VerticalTextJustification::k_top);
            m_textComponent = label->GetComponent<CSUI::TextComponent>();

            auto uiCanvas = GetState()->GetUICanvas();
            uiCanvas->AddWidget(std::move(label));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::OnInit()
        {
            InitGestures();
            InitUI();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::OnUpdate(f32 in_deltaTime)
        {
            if (m_uiDirty)
            {
                m_uiDirty = false;
                
                std::string text = "Taps: " + CSCore::ToString(m_gestureInfo.m_numTaps) + "\n";
                text += "Double Taps: " + CSCore::ToString(m_gestureInfo.m_numDoubleTaps) + "\n";
                text += "Two Finger Taps: " + CSCore::ToString(m_gestureInfo.m_numTwoFingerTaps) + "\n";
                text += "Two Finger Double Taps: " + CSCore::ToString(m_gestureInfo.m_numTwoFingerDoubleTaps) + "\n";
                text += "Holds: " + CSCore::ToString(m_gestureInfo.m_numHolds) + "\n";
                text += "Two Finger Holds: " + CSCore::ToString(m_gestureInfo.m_numTwoFingerHolds) + "\n";
                m_textComponent->SetText(text);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::OnDestroy()
        {
            m_eventConnections.clear();
            m_gestures.clear();
        }
    }
}
