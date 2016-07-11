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

#include <CustomShader/State.h>

#include <Common/Core/TestNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Gesture/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Shader.h>
#include <ChilliSource/Rendering/Sprite.h>
#include <ChilliSource/Rendering/Texture.h>

namespace CSTest
{
	namespace CustomShader
	{
        namespace
        {
            const std::string k_uniformHappiness = "u_happiness";
            
            /// Create a custom material that uses the faces shader.
            ///
            CS::MaterialSPtr CreateFacesMaterial() noexcept
            {
                static u32 s_id = 0;
                
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto shader = resourcePool->LoadResource<CS::Shader>(CS::StorageLocation::k_package, "CustomShader/Sprite-Faces.csshader");
                auto angryTexture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "Textures/AngryFace.csimage");
                auto happyTexture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_package, "Textures/HappyFace.csimage");
                
                auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
                auto material = materialFactory->CreateCustom("Faces-" + CS::ToString(s_id++));
                material->SetShadingType(CS::Material::ShadingType::k_custom);
                material->SetCustomShader(CS::VertexFormat::k_sprite, shader);
                material->AddTexture(angryTexture);
                material->AddTexture(happyTexture);
                material->SetFaceCullingEnabled(false);
                material->SetShaderVar("u_happiness", 0.0f);
                
                return material;
            }
            
            /// Adds a face sprite to the centre of the scene with a custom shader applied to it.
            ///
            /// @param scene
            ///     The scene to add the circles to.
            ///
            void AddFace(CS::Scene* scene, const CS::MaterialCSPtr& material) noexcept
            {
                CS::Vector2 size(2.0f, 2.0f);
                auto worldPosition = CS::Vector3(0.0f, 0.5f * size.y, 0.0f);
                
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto textureAtlas = resourcePool->LoadResource<CS::TextureAtlas>(CS::StorageLocation::k_package, "TextureAtlases/Sprites/Sprites.csatlas");
                
                auto renderComponentFactory = CS::Application::Get()->GetSystem<CS::RenderComponentFactory>();
                auto spriteComponent = renderComponentFactory->CreateSpriteComponent(size, material, CS::SizePolicy::k_fitMaintainingAspect);
                
                auto entity = CS::Entity::Create();
                entity->SetName("Face");
                entity->AddComponent(std::move(spriteComponent));
                entity->GetTransform().SetPosition(worldPosition);
                scene->Add(std::move(entity));
            }
        }
        
		//------------------------------------------------------------------------------
		void State::CreateSystems() noexcept
		{
			CreateSystem<Common::TestNavigator>("Custom Shader");
		}
        
		//------------------------------------------------------------------------------
		void State::OnInit() noexcept
		{
			GetScene()->SetClearColour(CS::Colour::k_black);

			auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();

			CS::EntitySPtr room = basicEntityFactory->CreateRoom();
			room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
			GetScene()->Add(room);

			auto camera = basicEntityFactory->CreateThirdPersonCamera(room, CS::Vector3(0.0f, -9.0f, 0.0f));
			GetScene()->Add(std::move(camera));

			CS::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CS::Colour(0.65f, 0.65f, 0.65f, 1.0f));
			GetScene()->Add(ambientLight);

            m_material = CreateFacesMaterial();
            AddFace(GetScene(), m_material);
		}
        
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 deltaTime) noexcept
        {
            m_happiness += deltaTime * m_happinessVelocity;
            
            if (m_happiness >= 1.0f)
            {
                m_happiness = 1.0f;
                m_happinessVelocity = -std::abs(m_happinessVelocity);
            }
            else if (m_happiness <= 0.0f)
            {
                m_happiness = 0.0f;
                m_happinessVelocity = std::abs(m_happinessVelocity);
            }
            
            m_material->SetShaderVar(k_uniformHappiness, CS::Interpolate::SmoothStep(m_happiness));
        }
	}
}
