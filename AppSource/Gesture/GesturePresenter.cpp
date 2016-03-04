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
        namespace
        {
            //------------------------------------------------------------------------------
            /// Calculates the event info string for the given drag info.
            ///
            /// @param in_dragInfo - The drag info containing the event info.
            ///
            /// @return The event info string.
            //------------------------------------------------------------------------------
            std::string GetEventInfo(const CSInput::DragGesture::DragInfo& in_dragInfo)
            {
                return CSCore::ToString(in_dragInfo.m_position);
            }
            
            //------------------------------------------------------------------------------
            /// Calculates the event info string for the given pinch info.
            ///
            /// @param in_pinchInfo - The pinch info containing the event info.
            ///
            /// @return The event info string.
            //------------------------------------------------------------------------------
            std::string GetEventInfo(const CSInput::PinchGesture::PinchInfo& in_pinchInfo)
            {
                return CSCore::ToString(in_pinchInfo.m_position) + "; " + CSCore::ToString(in_pinchInfo.m_scale);
            }
            
            //------------------------------------------------------------------------------
            /// Calculates the event info string for the given rotation info.
            ///
            /// @param in_rotationInfo - The rotation info containing the event info.
            ///
            /// @return The event info string.
            //------------------------------------------------------------------------------
            std::string GetEventInfo(const CSInput::RotationGesture::RotationInfo& in_rotationInfo)
            {
                return CSCore::ToString(in_rotationInfo.m_position) + "; " + CSCore::ToString(in_rotationInfo.m_rotation);
            }
        }
        
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
            AddTapGesture("Tap", 1, 1);
            AddTapGesture("Double tap", 2, 1);
            AddTapGesture("Two pointer tap", 1, 2);
            AddTapGesture("Two pointer double tap", 2, 2);
            AddHoldGesture("Hold", 1);
            AddHoldGesture("Two pointer hold", 2);
            AddDragGesture("Drag", 1);
            AddDragGesture("Two pointer drag", 2);
            AddPinchGesture("Pinch");
            AddRotationGesture("Rotation");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::AddTapGesture(const std::string& in_eventName, u32 in_numTaps, u32 in_numPointers)
        {
            auto gestureSystem = GetState()->GetSystem<CSInput::GestureSystem>();
            CS_ASSERT(gestureSystem, "Must have gesture system to use gesture presenter");
            
            auto gesture = std::make_shared<CSInput::TapGesture>(in_numTaps, in_numPointers);
            m_eventConnections.push_back(gesture->GetTappedEvent().OpenConnection([=](const CSInput::TapGesture*, const CSCore::Vector2&)
            {
                m_gestureEventInfo.IncrementEventCounter(in_eventName);
                m_uiDirty = true;
            }));
            
            gestureSystem->AddGesture(gesture);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::AddHoldGesture(const std::string& in_eventName, u32 in_numPointers)
        {
            auto gestureSystem = GetState()->GetSystem<CSInput::GestureSystem>();
            CS_ASSERT(gestureSystem, "Must have gesture system to use gesture presenter");
            
            auto gesture = std::make_shared<CSInput::HoldGesture>(in_numPointers);
            m_eventConnections.push_back(gesture->GetHeldEvent().OpenConnection([=](const CSInput::HoldGesture*, const CSCore::Vector2&)
            {
                m_gestureEventInfo.IncrementEventCounter(in_eventName);
                m_uiDirty = true;
            }));
            
            gestureSystem->AddGesture(gesture);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::AddDragGesture(const std::string& in_eventName, u32 in_numPointers)
        {
            auto gestureSystem = GetState()->GetSystem<CSInput::GestureSystem>();
            CS_ASSERT(gestureSystem, "Must have gesture system to use gesture presenter");
            
            auto gesture = std::make_shared<CSInput::DragGesture>(in_numPointers);
            
            m_eventConnections.push_back(gesture->GetDragStartedEvent().OpenConnection([=](const CSInput::DragGesture*, const CSInput::DragGesture::DragInfo& in_info)
            {
                m_gestureEventInfo.AddActiveEvent(in_eventName, GetEventInfo(in_info));
                m_uiDirty = true;
            }));
            
            m_eventConnections.push_back(gesture->GetDragMovedEvent().OpenConnection([=](const CSInput::DragGesture*, const CSInput::DragGesture::DragInfo& in_info)
            {
                m_gestureEventInfo.UpdateActiveEvent(in_eventName, GetEventInfo(in_info));
                m_uiDirty = true;
            }));

            m_eventConnections.push_back(gesture->GetDragEndedEvent().OpenConnection([=](const CSInput::DragGesture*, const CSInput::DragGesture::DragInfo& in_info)
            {
                m_gestureEventInfo.RemoveActiveEvent(in_eventName);
                m_uiDirty = true;
            }));
            
            gestureSystem->AddGesture(gesture);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::AddPinchGesture(const std::string& in_eventName)
        {
            auto gestureSystem = GetState()->GetSystem<CSInput::GestureSystem>();
            CS_ASSERT(gestureSystem, "Must have gesture system to use gesture presenter");
            
            auto gesture = std::make_shared<CSInput::PinchGesture>();
            
            m_eventConnections.push_back(gesture->GetPinchStartedEvent().OpenConnection([=](const CSInput::PinchGesture*, const CSInput::PinchGesture::PinchInfo& in_info)
            {
                m_gestureEventInfo.AddActiveEvent(in_eventName, GetEventInfo(in_info));
                m_uiDirty = true;
            }));

            m_eventConnections.push_back(gesture->GetPinchMovedEvent().OpenConnection([=](const CSInput::PinchGesture*, const CSInput::PinchGesture::PinchInfo& in_info)
            {
                m_gestureEventInfo.UpdateActiveEvent(in_eventName, GetEventInfo(in_info));
                m_uiDirty = true;
            }));

            m_eventConnections.push_back(gesture->GetPinchEndedEvent().OpenConnection([=](const CSInput::PinchGesture*, const CSInput::PinchGesture::PinchInfo& in_info)
            {
                m_gestureEventInfo.RemoveActiveEvent(in_eventName);
                m_uiDirty = true;
            }));
            
            gestureSystem->AddGesture(gesture);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void GesturePresenter::AddRotationGesture(const std::string& in_eventName)
        {
            auto gestureSystem = GetState()->GetSystem<CSInput::GestureSystem>();
            CS_ASSERT(gestureSystem, "Must have gesture system to use gesture presenter");
            
            auto gesture = std::make_shared<CSInput::RotationGesture>();
            
            m_eventConnections.push_back(gesture->GetRotationStartedEvent().OpenConnection([=](const CSInput::RotationGesture*, const CSInput::RotationGesture::RotationInfo& in_info)
            {
                m_gestureEventInfo.AddActiveEvent(in_eventName, GetEventInfo(in_info));
                m_uiDirty = true;
            }));

            m_eventConnections.push_back(gesture->GetRotationMovedEvent().OpenConnection([=](const CSInput::RotationGesture*, const CSInput::RotationGesture::RotationInfo& in_info)
            {
                m_gestureEventInfo.UpdateActiveEvent(in_eventName, GetEventInfo(in_info));
                m_uiDirty = true;
            }));

            m_eventConnections.push_back(gesture->GetRotationEndedEvent().OpenConnection([=](const CSInput::RotationGesture*, const CSInput::RotationGesture::RotationInfo& in_info)
            {
                m_gestureEventInfo.RemoveActiveEvent(in_eventName);
                m_uiDirty = true;
            }));
            
            gestureSystem->AddGesture(gesture);
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
                
                std::string text = "Immediate events:";
                for (const auto& counter : m_gestureEventInfo.GetEventCounters())
                {
                    text += "\n  " + counter.m_name + ": " + CSCore::ToString(counter.m_count);
                }
                
                text += "\n \nActive events:";
                for (const auto& event : m_gestureEventInfo.GetActiveEvents())
                {
                    text += "\n  " + event.m_name + ": " + event.m_info;
                }
                
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
