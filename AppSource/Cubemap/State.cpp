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

#include <Cubemap/State.h>

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
    namespace Cubemap
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
                
                CS::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(CS::Colour(0.8f, 0.5f, 0.4f), CS::Vector4(15.0f, 15.0f, 1.0f, 15.0f));
                scene->Add(directionalLight);
                directionalLight->GetTransform().SetLookAt(directionLightPos, dirLightTarget, CS::Vector3::k_unitPositiveY);
            }
        }
        
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
            CreateSystem<Common::TestNavigator>("Cubemap");
        }
        
        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetMainScene()->SetClearColour(CS::Colour::k_black);
            
            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            auto primitiveEntityFactory = CS::Application::Get()->GetSystem<CS::PrimitiveEntityFactory>();
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            
            CS::EntitySPtr room = basicEntityFactory->CreateRoom(CS::Vector3(20.0f, 20.0f, 20.0f), "Materials/Skybox.csmaterial");
            GetMainScene()->Add(room);
            
            CS::EntitySPtr model = primitiveEntityFactory->CreateBox(resourcePool->LoadResource<CS::Material>(CS::StorageLocation::k_package, "Materials/Reflective.csmaterial"), CS::Vector3(3, 3, 3));
            GetMainScene()->Add(model);
            
            auto camera = basicEntityFactory->CreateThirdPersonCamera(model, CS::Vector3::k_zero, 7.0f, 0.0f, -CS::MathUtils::k_pi / 7.0f);
            GetMainScene()->Add(std::move(camera));
            
            CreateLights(GetMainScene());
        }
    }
}
