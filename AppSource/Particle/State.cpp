//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 14/03/2016.
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

#include <Particle/State.h>

#include <Common/Core/TestNavigator.h>
#include <Common/Core/BasicEntityFactory.h>
#include <Common/Input/BackButtonSystem.h>
#include <Gesture/State.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Particle.h>

namespace CSTest
{
	namespace Particle
	{
		namespace
		{
			//------------------------------------------------------------------------------
			/// Creates the three particle effects that make up the fire effect and attachs
			/// them to a new entity.
			///
			/// @return The fire effect entity.
			//------------------------------------------------------------------------------
			CS::EntityUPtr CreateFire()
			{
				auto app = CS::Application::Get();

				auto resourcePool = app->GetResourcePool();
				auto fireEffect = resourcePool->LoadResource<CS::ParticleEffect>(CS::StorageLocation::k_package, "Particle/Fire.csparticle");
				auto embersEffect = resourcePool->LoadResource<CS::ParticleEffect>(CS::StorageLocation::k_package, "Particle/Embers.csparticle");
				auto smokeEffect = resourcePool->LoadResource<CS::ParticleEffect>(CS::StorageLocation::k_package, "Particle/Smoke.csparticle");

				CS::RenderComponentFactory* renderComponentFactory = app->GetSystem<CS::RenderComponentFactory>();

				auto fireComponent = renderComponentFactory->CreateParticleEffectComponent(fireEffect);
				fireComponent->SetPlaybackType(CS::ParticleEffectComponent::PlaybackType::k_looping);
				auto fireEntity = CS::Entity::Create();
				fireEntity->AddComponent(std::move(fireComponent));

				auto embersComponent = renderComponentFactory->CreateParticleEffectComponent(embersEffect);
				embersComponent->SetPlaybackType(CS::ParticleEffectComponent::PlaybackType::k_looping);
				auto embersEntity = CS::Entity::Create();
				embersEntity->AddComponent(std::move(embersComponent));

				auto smokeComponent = renderComponentFactory->CreateParticleEffectComponent(smokeEffect);
				smokeComponent->SetPlaybackType(CS::ParticleEffectComponent::PlaybackType::k_looping);
				auto smokeEntity = CS::Entity::Create();
				smokeEntity->AddComponent(std::move(smokeComponent));

				auto effectEntity = CS::Entity::Create();
				effectEntity->AddEntity(std::move(fireEntity));
				effectEntity->AddEntity(std::move(embersEntity));
				effectEntity->AddEntity(std::move(smokeEntity));

				return effectEntity;
			}
		}

		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void State::CreateSystems()
		{
			CreateSystem<Common::TestNavigator>("Particles");
            CreateSystem<Common::BackButtonSystem>();
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void State::OnInit()
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

			auto fire = CreateFire();
			GetScene()->Add(std::move(fire));
		}
	}
}
