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

#include <Gyroscope/GyroscopePresenter.h>

#include <Common/UI/BasicWidgetFactory.h>
#include <Common/Core/BasicEntityFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Input/Gyroscope.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/Rendering/Lighting.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>

namespace CSTest
{
    namespace Gyroscope
    {
        CS_DEFINE_NAMEDTYPE(GyroscopePresenter);

        //------------------------------------------------------------------------------
        GyroscopePresenterUPtr GyroscopePresenter::Create() noexcept
        {
            return GyroscopePresenterUPtr(new GyroscopePresenter());
        }

        //------------------------------------------------------------------------------
        bool GyroscopePresenter::IsA(CS::InterfaceIDType interfaceId) const noexcept
        {
            return (GyroscopePresenter::InterfaceID == interfaceId);
        }

        //------------------------------------------------------------------------------
        void GyroscopePresenter::InitUI() noexcept
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
        void GyroscopePresenter::AddGyroscopeHandler() noexcept
        {
            CS_ASSERT(!m_gyro, "Gyroscope reference unexpected - this method should only be called once.");
            m_gyro = CS::Application::Get()->GetSystem<CS::Gyroscope>();
            
            if (m_gyro == nullptr)
            {
                m_textComponent->SetText("No Gyroscope.");
                return;
            }
            
            m_gyro->StartUpdating();

            m_eventConnection = m_gyro->GetOrientationUpdatedEvent().OpenConnection([=](const CS::Quaternion& orientation)
            {
                DisplayOrientation(orientation);
            });
        }

        //------------------------------------------------------------------------------
        void GyroscopePresenter::DisplayOrientation(const CS::Quaternion& orientation) noexcept
        {
            CS::Quaternion landscape;
            landscape.x = -orientation.y;
            landscape.y = orientation.z;
            landscape.z = -orientation.x;
            landscape.w = orientation.w;
            landscape *= CS::Quaternion(CS::Vector3::k_unitPositiveX, CS::MathUtils::DegToRad(90.0f));
            
            std::string text = "Device Orientation landscaped: ";

            text += "\nX: " + CS::ToString(landscape.x);
            text += "\nY: " + CS::ToString(landscape.y);
            text += "\nZ: " + CS::ToString(landscape.z);
            text += "\nW: " + CS::ToString(landscape.w);

            m_textComponent->SetText(text);
            
            m_box->GetTransform().SetOrientation(landscape);
        }

        //------------------------------------------------------------------------------
        void GyroscopePresenter::OnInit() noexcept
        {
            InitUI();
            
            auto renderComponentFactory = CS::Application::Get()->GetSystem<CS::RenderComponentFactory>();
            CS::CameraComponentSPtr camComponent = renderComponentFactory->CreatePerspectiveCameraComponent(3.14f / 3.0f, 0.5f, 30.0f);
            CS::EntityUPtr camera = CS::Entity::Create();
            camera->AddComponent(camComponent);
            camera->GetTransform().SetPosition(CS::Vector3(0.0f, 0.0f, -5.0f));
            GetState()->GetMainScene()->Add(std::move(camera));
            
            auto primitiveEntityFactory = CS::Application::Get()->GetSystem<CS::PrimitiveEntityFactory>();
            m_box = primitiveEntityFactory->CreateBox(CS::Colour::k_red, CS::Vector3(1.0f, 3.0f, 1.0f));
            GetState()->GetMainScene()->Add(m_box);
            
            CreateLights();
            
            AddGyroscopeHandler();
        }
        
        //------------------------------------------------------------------------------
        void GyroscopePresenter::CreateLights()
        {
            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            
            // ambient light
            CS::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CS::Colour(0.3f, 0.3f, 0.3f, 1.0f));
            GetState()->GetMainScene()->Add(ambientLight);
            
            CS::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(CS::Colour(0.8f, 0.5f, 0.4f));
            directionalLight->GetTransform().SetLookAt(CS::Vector3::k_one, CS::Vector3::k_zero, CS::Vector3::k_unitPositiveY);
            GetState()->GetMainScene()->Add(directionalLight);
        }

        //------------------------------------------------------------------------------
        void GyroscopePresenter::OnDestroy() noexcept
        {
            if (m_gyro)
            {
                m_gyro->StopUpdating();
            }
            m_eventConnection.reset();
        }
    }
}
