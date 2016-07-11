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

#include <AnimatedModel/State.h>

#include <Common/Core/TestNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Gesture/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Model.h>

namespace CSTest
{
    namespace AnimatedModel
    {
        namespace
        {
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
            CreateSystem<Common::TestNavigator>("Animated Model");
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
            
            CreateLights(GetScene());
            AddAnimatedModel(GetScene());
        }
    }
}
