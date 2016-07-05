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
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Sprite.h>

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
        bool BasicEntityFactory::IsA(CS::InterfaceIDType in_interfaceId) const
        {
            return (BasicEntityFactory::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::EntityUPtr BasicEntityFactory::CreateThirdPersonCamera(const CS::EntitySPtr& in_target, const CS::Vector3& in_targetOffset, f32 in_distance, f32 in_horizontalAngle, f32 in_verticalAngle,
                                                                       f32 in_horizontalAngularVelocity)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            CS::CameraComponentSPtr camComponent = m_renderComponentFactory->CreatePerspectiveCameraComponent(3.14f / 3.0f, 1.0f, 30.0f);
            auto followerComponent = std::make_shared<FollowerComponent>(in_target, in_targetOffset, in_distance, in_horizontalAngle, in_verticalAngle);
            auto orbiterComponent = std::make_shared<OrbiterComponent>(in_horizontalAngularVelocity);
            
            CS::EntityUPtr entity = CS::Entity::Create();
            entity->SetName(CS::ToString(m_entityCount++) + "-ThirdPersonCamera");
            entity->AddComponent(camComponent);
            entity->AddComponent(followerComponent);
            entity->AddComponent(orbiterComponent);
            
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::EntityUPtr BasicEntityFactory::CreateAmbientLight(const CS::Colour& in_colour)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            CS::AmbientLightComponentSPtr ambientLightComponent = m_renderComponentFactory->CreateAmbientLightComponent(in_colour);
            
            auto entity = CS::Entity::Create();
            entity->SetName(CS::ToString(m_entityCount++) + "-AmbientLight");
            entity->AddComponent(ambientLightComponent);
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::EntityUPtr BasicEntityFactory::CreateDirectionalLight(const CS::Colour& in_colour, const CS::Vector4& in_shadowVolume)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            //TODO: Change back to creating shadowed component
            CS::DirectionalLightComponentSPtr directionalLightComponent = m_renderComponentFactory->CreateDirectionalLightComponent(CS::DirectionalLightComponent::ShadowQuality::k_high, in_colour);
            directionalLightComponent->SetShadowVolume(in_shadowVolume.x, in_shadowVolume.y, in_shadowVolume.z, in_shadowVolume.w);
            
            auto entity = CS::Entity::Create();
            entity->SetName(CS::ToString(m_entityCount++) + "-DirectionalLight");
            entity->AddComponent(directionalLightComponent);
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::EntityUPtr BasicEntityFactory::CreatePointLight(const CS::Colour& in_colour, f32 in_radius, f32 in_intensity) noexcept
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            auto pointLightComponent = m_renderComponentFactory->CreatePointLightComponent(in_colour, in_radius, in_intensity);
            
            auto entity = CS::Entity::Create();
            entity->SetName(CS::ToString(m_entityCount++) + "-PointLight");
            entity->AddComponent(std::move(pointLightComponent));
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::EntityUPtr BasicEntityFactory::CreateRoom(const CS::Vector3& in_size)
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            const f32 k_textureRepeatFactor = 0.5f;
            CS::Vector2 textureRepeat(in_size.x * k_textureRepeatFactor, in_size.z * k_textureRepeatFactor);
            
            CS::ModelCSPtr mesh = m_primitiveModelFactory->CreateBox(in_size, textureRepeat, true);
            CS::MaterialCSPtr material = m_resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/CheckeredLit.csmaterial");
            
            CS::StaticModelComponentSPtr meshComponent = m_renderComponentFactory->CreateStaticModelComponent(mesh, material);
            meshComponent->SetShadowCastingEnabled(false);
            
            auto entity = CS::Entity::Create();
            entity->SetName(CS::ToString(m_entityCount++) + "-Room");
            entity->AddComponent(meshComponent);
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CS::EntityUPtr BasicEntityFactory::CreateSprite(const std::string& in_imageId, const CS::Colour& in_colour, const CS::Vector2& in_size) noexcept
        {
            CS_ASSERT(CS::Application::Get()->GetTaskScheduler()->IsMainThread(), "Entities must be created on the main thread.");
            
            auto material = m_resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Sprites.csmaterial");
            auto textureAtlas = m_resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Sprites/Sprites.csatlas");
            
            auto spriteComponent = m_renderComponentFactory->CreateSpriteComponent(in_size, textureAtlas, in_imageId, material, CS::SizePolicy::k_fitMaintainingAspect);
            spriteComponent->SetColour(in_colour);
            
            auto entity = CS::Entity::Create();
            entity->SetName(CS::ToString(m_entityCount++) + "-Sprite");
            entity->AddComponent(std::move(spriteComponent));
            return entity;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void BasicEntityFactory::OnInit()
        {
            m_resourcePool = CS::Application::Get()->GetResourcePool();
            
            m_renderComponentFactory = CS::Application::Get()->GetSystem<CS::RenderComponentFactory>();
            CS_ASSERT(m_renderComponentFactory, "BasicEntityFactory is missing required app system: RenderComponentFactory");
            
            m_primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
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
