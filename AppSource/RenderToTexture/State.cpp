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

#include <RenderToTexture/State.h>

#include <Common/Core/TestNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Common/UI/BasicWidgetFactory.h>
#include <Gesture/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Rendering/Base/Renderer.h>
#include <ChilliSource/Rendering/Camera/CameraComponent.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Drawable/DrawableUIComponent.h>

namespace CSTest
{
	namespace RenderToTexture
	{
        namespace
        {
            constexpr f32 k_displaySpeed = 0.1f; //Percentage per second
            constexpr f32 k_displayScreenPercentage = 0.5f;
            
            /// Creates an ambient light and a directional light and adds them to the scene.
            ///
            /// @param scene
            ///     The scene to add the lights to.
            ///
            void CreateLights(CS::Scene* scene)
            {
                auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
                
                // ambient light
                CS::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CS::Colour(0.3f, 0.3f, 0.3f, 1.0f));
                scene->Add(ambientLight);
                
                // directional light
                constexpr auto k_dirLightDist = 10.0f;
                const auto dirLightDir = CS::Vector3::Normalise(CS::Vector3(1.0f, -1.0f, 1.0f));
                const CS::Vector3 dirLightTarget = CS::Vector3(0.0f, 0.0f, 0.0f);
                const auto directionLightPos = dirLightTarget - dirLightDir * k_dirLightDist;
                
                CS::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(CS::Colour(0.8f, 0.5f, 0.4f));
                scene->Add(directionalLight);
                directionalLight->GetTransform().SetLookAt(directionLightPos, dirLightTarget, CS::Vector3::k_unitPositiveY);
            }
            
            /// Adds a single animated model to the centre of the scene.
            ///
            /// @param scene
            ///     The scene to add the model.
            ///
            void AddAnimatedModel(CS::Scene* scene) noexcept
            {
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto model = resourcePool->LoadResource<CS::Model>(CS::StorageLocation::k_package, "AnimatedModels/Cylinder.csmodel");
                auto material = resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Cylinder.csmaterial");
                auto animation = resourcePool->LoadResource<CS::SkinnedAnimation>(CS::StorageLocation::k_package, "Animations/CylinderFlex.csanim");
                
                auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
                auto entity = basicEntityFactory->CreateAnimatedModel(model, material, animation, CS::AnimatedModelComponent::PlaybackType::k_looping);
                scene->Add(std::move(entity));
            }
        }
        
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
            CreateSystem<Common::TestNavigator>("Render to Texture");
            
            // Create a scene that will be rendererd to texture rather then directly to screen
            m_rtScene = CreateSystem<CS::Scene>();
        }
        
        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetScene()->SetClearColour(CS::Colour::k_purple);
            
            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto screen = CS::Application::Get()->GetScreen();
            
            m_rtDisplayVelocity = CS::Random::GenerateDirection2D<f32>() * k_displaySpeed;
            
            // Populate the display scene with some models, etc
            CS::EntitySPtr room = basicEntityFactory->CreateRoom();
            room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
            m_rtScene->Add(room);
            
            auto camera = basicEntityFactory->CreateThirdPersonCamera(room, CS::Vector3(0.0f, -9.0f, 0.0f));
            m_rtScene->Add(std::move(camera));
            
            CreateLights(m_rtScene);
            AddAnimatedModel(m_rtScene);
            
            // Create a new texture that the scene will be rendererd too
            auto renderTexture = resourcePool->CreateResource<CS::Texture>("SmokeTestRenderTex");
            renderTexture->Build(CS::Texture::DataUPtr(), 0, CS::TextureDesc(CS::Integer2(s32(screen->GetResolution().x * k_displayScreenPercentage), s32(screen->GetResolution().y * k_displayScreenPercentage)), CS::ImageFormat::k_RGB888, CS::ImageCompression::k_none));
            renderTexture->SetLoadState(CS::Resource::LoadState::k_loaded);
            
            // Convert the texture to a colour target that uses the depth buffer
            m_renderTarget = CS::TargetGroup::CreateColourTargetGroup(renderTexture);
            
            // Add a UI image to the screen scene, this will be used to display our render texture
            m_rtDisplay = basicWidgetFactory->CreateImage(CS::Vector2(k_displayScreenPercentage, k_displayScreenPercentage), renderTexture, CS::AlignmentAnchor::k_middleCentre, CS::SizePolicy::k_none);
            auto drawable = m_rtDisplay->GetComponent<CS::DrawableUIComponent>();
            drawable->GetDrawable()->SetUVs(CS::UVs::FlipVertically(drawable->GetDrawable()->GetUVs()));
            GetUICanvas()->AddWidget(m_rtDisplay);
        }
        
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 timeSinceLastUpdate) noexcept
        {
            m_rtScene->UpdateEntities(timeSinceLastUpdate);
            
            CS::Application::Get()->RenderToTarget(m_rtScene, m_renderTarget.get());
            
            CS::Vector2 currentPos = m_rtDisplay->GetLocalRelativePosition();
            if(currentPos.x >= 0.5f)
            {
                m_rtDisplayVelocity.x = -k_displaySpeed;
            }
            else if(currentPos.x <= -0.5f)
            {
                m_rtDisplayVelocity.x = k_displaySpeed;
            }
            
            if(currentPos.y >= 0.5f)
            {
                m_rtDisplayVelocity.y = -k_displaySpeed;
            }
            else if(currentPos.y <= -0.5f)
            {
                m_rtDisplayVelocity.y = k_displaySpeed;
            }
            
            m_rtDisplay->RelativeMoveBy(m_rtDisplayVelocity * timeSinceLastUpdate);
        }
	}
}
