#include <MultiThreadedRenderer/State.h>

#include <Common/Core/BasicEntityFactory.h>
#include <MultiThreadedRenderer/SphereBounceComponent.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Web/Base.h>

#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Rendering/Texture.h>

namespace CSTest
{
    namespace MultiThreadedRenderer
    {
        namespace
        {
            constexpr f32 k_boundingRadius = 3.5f;
            constexpr f32 k_boxSize = 0.1f;
            constexpr u32 k_numBoxes = 2000;
            constexpr f32 k_minSpeed = 0.1f;
            constexpr f32 k_maxSpeed = 0.5f;
            constexpr f32 k_minAngularSpeed = 0.1f;
            constexpr f32 k_maxAngularSpeed = 1.0f;
            
            CS::Vector3 GeneratePointInUnitSphere() noexcept
            {
                f32 dist = std::pow(CS::Random::GenerateNormalised<f32>(), (1.0f / 3.0f));
                return CS::Random::GenerateDirection3D<f32>() * dist;
            }
            
            CS::MaterialCSPtr CreateStaticColourMaterial(const CS::Colour& in_colour) noexcept
            {
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                
                auto materialName = "_StaticColour(" + ToString(in_colour) + ")";
                auto material = resourcePool->GetResource<CS::Material>(materialName);
                
                if (material == nullptr)
                {
                    auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                    
                    auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
                    
                    auto mutableMaterial = materialFactory->CreateStatic(materialName, texture);
                    
                    mutableMaterial->SetEmissive(in_colour);
                    mutableMaterial->SetAmbient(CS::Colour::k_black);
                    mutableMaterial->SetDiffuse(CS::Colour::k_black);
                    mutableMaterial->SetSpecular(CS::Colour::k_black);
                    
                    if (in_colour.a < 0.99f)
                    {
                        mutableMaterial->SetTransparencyEnabled(true);
                    }
                    
                    mutableMaterial->SetLoadState(CS::Resource::LoadState::k_loaded);
                    material = mutableMaterial;
                }
                
                return material;
            }
            
            CS::EntityUPtr CreateStaticColourBox(const CS::Colour& in_colour, const CS::Vector3& in_size)
            {
                static u32 s_entityCount = 0;
                
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                
                CS::ModelCSPtr mesh = primitiveModelFactory->CreateBox(in_size);
                CS::MaterialCSPtr material = CreateStaticColourMaterial(in_colour);
                
                auto renderComponentFactory = CS::Application::Get()->GetSystem<CS::RenderComponentFactory>();
                CS::StaticModelComponentSPtr meshComponent = renderComponentFactory->CreateStaticModelComponent(mesh, material);
                
                auto entity = CS::Entity::Create();
                entity->SetName(CS::ToString(s_entityCount++) + "-ColourBox");
                entity->AddComponent(meshComponent);
                return entity;
            }
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetScene()->SetClearColour(CS::Colour::k_black);

            auto primitiveEntityFactory = CS::Application::Get()->GetSystem<CS::PrimitiveEntityFactory>();
            
            static const std::array<CS::Colour, 5> k_boxColours =
            {{
                CS::Colour(1.0f, 0.1f, 0.1f, 1.0f),
                CS::Colour(0.1f, 1.0f, 0.1f, 1.0f),
                CS::Colour(0.1f, 0.1f, 1.0f, 1.0f),
                CS::Colour(1.0f, 1.0f, 1.0f, 1.0f)
            }};
            
            for (u32 i = 0; i < k_numBoxes; ++i)
            {
                auto position = GeneratePointInUnitSphere() * k_boundingRadius;
                auto size = CS::Vector3(k_boxSize, k_boxSize, k_boxSize);
                auto orientation = CS::Quaternion(CS::Random::GenerateDirection3D<f32>(), CS::Random::GenerateNormalised<f32>());
                auto colour = k_boxColours[CS::Random::Generate<u32>(0, u32(k_boxColours.size()) - 1)];
                auto speed = k_minSpeed + (k_maxSpeed - k_minSpeed) * CS::Random::GenerateNormalised<f32>();
                auto angularSpeed = k_minAngularSpeed + (k_maxAngularSpeed - k_minAngularSpeed) * CS::Random::GenerateNormalised<f32>();
                
                CS::EntitySPtr box;
                if (i % 2 == 0)
                {
                    box = primitiveEntityFactory->CreateBox(colour, size);
                }
                else
                {
                    box = CreateStaticColourBox(colour * 0.75f, size);
                }

                box->AddComponent(std::make_shared<SphereBounceComponent>(k_boundingRadius, speed, angularSpeed));
                box->GetTransform().SetPosition(position);
                box->GetTransform().SetOrientation(orientation);
                GetScene()->Add(std::move(box));
            }

            auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
            
            CS::EntitySPtr cameraTarget = CS::Entity::Create();
            GetScene()->Add(cameraTarget);
            auto camera = basicEntityFactory->CreateThirdPersonCamera(cameraTarget, CS::Vector3::k_zero, 7.5f, 0.0f, 0.0f, 0.25f);
            GetScene()->Add(std::move(camera));

            CS::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CS::Colour(0.3f, 0.3f, 0.3f, 1.0f));
            GetScene()->Add(ambientLight);
            
            CS::EntitySPtr directionalLight = basicEntityFactory->CreateDirectionalLight(CS::Colour(0.7f, 0.7f, 0.7f));
            directionalLight->GetTransform().SetLookAt(CS::Vector3(-2.0f, 10.0f, -4.0f), CS::Vector3::k_zero, CS::Vector3::k_unitPositiveY);
            GetScene()->Add(directionalLight);
        }
        
        void State::OnUpdate(f32 in_deltaTime) noexcept
        {
            m_frameTimeCalculator.Update(in_deltaTime);
        }
    }
}
