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
#include <ChilliSource/Rendering/Target/RenderTargetGroup.h>
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
            /// @param in_dirCol
            ///     The colour of the directional light
            ///
            void CreateLights(CS::Scene* scene, const CS::Colour& in_dirCol)
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
                
                CS::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(in_dirCol);
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
            
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto screen = CS::Application::Get()->GetScreen();
            
            //Set up the first texture desc to make a backup of the image for context restoring
            //for our test that will be the one we only render into once
            std::array<CS::TextureDesc, k_numRenderTargets> renderTextureDescs =
            {{
                CS::TextureDesc(CS::Integer2(s32(screen->GetResolution().x * k_displayScreenPercentage), s32(screen->GetResolution().y * k_displayScreenPercentage)), CS::ImageFormat::k_RGB888, CS::ImageCompression::k_none, true),
                CS::TextureDesc(CS::Integer2(s32(screen->GetResolution().x * k_displayScreenPercentage), s32(screen->GetResolution().y * k_displayScreenPercentage)), CS::ImageFormat::k_RGB888, CS::ImageCompression::k_none, false)
            }};
            
            // Create scenes that will be rendererd to texture rather then directly to screen
            for(auto i=0; i<k_numRenderTargets; ++i)
            {
                // Create a new texture that the scene will be rendererd too
                m_renderTextures[i] = resourcePool->CreateResource<CS::Texture>("SmokeTestRenderTex" + CS::ToString(i));
                m_renderTextures[i]->Build(CS::Texture::DataUPtr(), 0, renderTextureDescs[i]);
                m_renderTextures[i]->SetLoadState(CS::Resource::LoadState::k_loaded);
                
                // Convert the texture to a colour target that uses the depth buffer
                // and set it as this scene's render target
                m_rtScenes[i] = CreateSystem<CS::Scene>(CS::TargetGroup::CreateColourTargetGroup(m_renderTextures[i], CS::RenderTargetGroupType::k_colourDepth));
            }
        }
        
        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetMainScene()->SetClearColour(CS::Colour::k_purple);
            
            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            
            std::array<CS::Colour, k_numRenderTargets> dirLightCols =
            {{
                CS::Colour(0.8f, 0.2f, 0.3f),
                CS::Colour(0.3f, 0.2f, 0.8f)
            }};
            
            for(auto i=0; i<k_numRenderTargets; ++i)
            {
                // Populate the display scene with some models, etc
                CS::EntitySPtr room = basicEntityFactory->CreateRoom();
                room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
                m_rtScenes[i]->Add(room);
                
                auto camera = basicEntityFactory->CreateThirdPersonCamera(room, CS::Vector3(0.0f, -9.0f, 0.0f));
                m_rtScenes[i]->Add(std::move(camera));
                
                CreateLights(m_rtScenes[i], dirLightCols[i]);
                AddAnimatedModel(m_rtScenes[i]);
                
                // Setup initial velocity
                m_rtDisplayVelocities[i] = CS::Random::GenerateDirection2D<f32>() * k_displaySpeed;
                
                // Add a UI image to the screen scene, this will be used to display our render texture
                m_rtDisplays[i] = basicWidgetFactory->CreateImage(CS::Vector2(k_displayScreenPercentage, k_displayScreenPercentage), m_renderTextures[i], CS::AlignmentAnchor::k_middleCentre, CS::SizePolicy::k_none);
                auto drawable = m_rtDisplays[i]->GetComponent<CS::DrawableUIComponent>();
                drawable->GetDrawable()->SetUVs(CS::UVs::FlipVertically(drawable->GetDrawable()->GetUVs()));
                GetUICanvas()->AddWidget(m_rtDisplays[i]);
            }
            
            // This scene we will only render to once (rather than each frame) to test that code path works.
            // The other scenes will be rendered each frame
            m_rtScenes[0]->SetEnabled(false);
            m_rtScenes[0]->Render();
        }
        
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 timeSinceLastUpdate) noexcept
        {
            // Move the UI around for effect
            for(auto i=0; i<k_numRenderTargets; ++i)
            {
                CS::Vector2 currentPos = m_rtDisplays[i]->GetLocalRelativePosition();
                if(currentPos.x >= 0.5f)
                {
                    m_rtDisplayVelocities[i].x = -k_displaySpeed;
                }
                else if(currentPos.x <= -0.5f)
                {
                    m_rtDisplayVelocities[i].x = k_displaySpeed;
                }
                
                if(currentPos.y >= 0.5f)
                {
                    m_rtDisplayVelocities[i].y = -k_displaySpeed;
                }
                else if(currentPos.y <= -0.5f)
                {
                    m_rtDisplayVelocities[i].y = k_displaySpeed;
                }
                
                m_rtDisplays[i]->RelativeMoveBy(m_rtDisplayVelocities[i] * timeSinceLastUpdate);
            }
        }
        
        //------------------------------------------------------------------------------
        State::~State()
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            
            for(int i=0; i<k_numRenderTargets; ++i)
            {
                m_rtDisplays[i].reset();
                auto tex = m_renderTextures[i].get();
                m_renderTextures[i].reset();
                resourcePool->Release(tex);
            }
        }
	}
}
