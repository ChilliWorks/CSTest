//
//  BasicEntityFactory.cpp
//  CSTest
//  Created by Ian Copland on 04/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <Common/Core/BasicEntityFactory.h>

#include <Common/Behaviour/OrbiterComponent.h>
#include <Common/Behaviour/FollowerComponent.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Threading.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Lighting.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Model.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(BasicEntityFactory);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        BasicEntityFactoryUPtr BasicEntityFactory::Create()
        {
            return BasicEntityFactoryUPtr(new BasicEntityFactory());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool BasicEntityFactory::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (BasicEntityFactory::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::EntityUPtr BasicEntityFactory::CreateThirdPersonCamera(const CSCore::EntitySPtr& in_target, const CSCore::Vector3& in_targetOffset, f32 in_distance, f32 in_horizontalAngle, f32 in_verticalAngle,
                                                                       f32 in_horizontalAngularVelocity)
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            CSRendering::CameraComponentSPtr camComponent = m_renderComponentFactory->CreatePerspectiveCameraComponent(3.14f / 3.0f, 1.0f, 30.0f);
            auto followerComponent = std::make_shared<FollowerComponent>(in_target, in_targetOffset, in_distance, in_horizontalAngle, in_verticalAngle);
            auto orbiterComponent = std::make_shared<OrbiterComponent>(in_horizontalAngularVelocity);
            
            CSCore::EntityUPtr entity = CSCore::Entity::Create();
            entity->SetName(CSCore::ToString(m_entityCount++) + "-ThirdPersonCamera");
            entity->AddComponent(camComponent);
            entity->AddComponent(followerComponent);
            entity->AddComponent(orbiterComponent);
            
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::EntityUPtr BasicEntityFactory::CreateAmbientLight(const CSCore::Colour& in_colour)
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            CSRendering::AmbientLightComponentSPtr ambientLightComponent = m_renderComponentFactory->CreateAmbientLightComponent();
            ambientLightComponent->SetColour(in_colour);
            
            auto entity = CSCore::Entity::Create();
            entity->SetName(CSCore::ToString(m_entityCount++) + "-AmbientLight");
            entity->AddComponent(ambientLightComponent);
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::EntityUPtr BasicEntityFactory::CreateDirectionalLight(const CSCore::Colour& in_colour, const CSCore::Vector4& in_shadowVolume)
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            CSRendering::DirectionalLightComponentSPtr directionalLightComponent = m_renderComponentFactory->CreateDirectionalLightComponent(2048);
            directionalLightComponent->SetColour(in_colour);
            directionalLightComponent->SetShadowVolume(in_shadowVolume.x, in_shadowVolume.y, in_shadowVolume.z, in_shadowVolume.w);
            directionalLightComponent->SetShadowTolerance(0.005f);
            
            auto entity = CSCore::Entity::Create();
            entity->SetName(CSCore::ToString(m_entityCount++) + "-DirectionalLight");
            entity->AddComponent(directionalLightComponent);
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::EntityUPtr BasicEntityFactory::CreateRoom(const CSCore::Vector3& in_size)
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            const f32 k_textureRepeatFactor = 0.5f;
            CSCore::Vector2 textureRepeat(in_size.x * k_textureRepeatFactor, in_size.z * k_textureRepeatFactor);
            
            CSRendering::MeshCSPtr mesh = m_primitiveModelFactory->CreateBox(in_size, textureRepeat, true);
            CSRendering::MaterialCSPtr material = m_resourcePool->LoadResource<CSRendering::Material>(CSCore::StorageLocation::k_package, "Materials/CheckeredLit.csmaterial");
            
            CSRendering::StaticMeshComponentSPtr meshComponent = m_renderComponentFactory->CreateStaticMeshComponent(mesh, material);
            meshComponent->SetShadowCastingEnabled(false);
            
            auto entity = CSCore::Entity::Create();
            entity->SetName(CSCore::ToString(m_entityCount++) + "-Room");
            entity->AddComponent(meshComponent);
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void BasicEntityFactory::OnInit()
        {
            m_resourcePool = CSCore::Application::Get()->GetResourcePool();
            
            m_renderComponentFactory = CSCore::Application::Get()->GetSystem<CSRendering::RenderComponentFactory>();
            CS_ASSERT(m_renderComponentFactory, "BasicEntityFactory is missing required app system: RenderComponentFactory");
            
            m_primitiveModelFactory = CSCore::Application::Get()->GetSystem<CSRendering::PrimitiveModelFactory>();
            CS_ASSERT(m_renderComponentFactory, "BasicEntityFactory is missing required app system: PrimitiveModelFactory");

            m_entityCount = 0;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void BasicEntityFactory::OnDestroy()
        {
            m_resourcePool = nullptr;
            m_renderComponentFactory = nullptr;
            m_primitiveModelFactory = nullptr;
            m_entityCount = 0;
        }
    }
}
