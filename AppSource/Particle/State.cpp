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
			CSCore::EntityUPtr CreateFire()
			{
				auto app = CSCore::Application::Get();

				auto resourcePool = app->GetResourcePool();
				auto fireEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particle/Fire.csparticle");
				auto embersEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particle/Embers.csparticle");
				auto smokeEffect = resourcePool->LoadResource<CSRendering::ParticleEffect>(CSCore::StorageLocation::k_package, "Particle/Smoke.csparticle");

				CSRendering::RenderComponentFactory* renderComponentFactory = app->GetSystem<CSRendering::RenderComponentFactory>();

				auto fireComponent = renderComponentFactory->CreateParticleEffectComponent(fireEffect);
				fireComponent->SetPlaybackType(CSRendering::ParticleEffectComponent::PlaybackType::k_looping);
				auto fireEntity = CSCore::Entity::Create();
				fireEntity->AddComponent(std::move(fireComponent));

				auto embersComponent = renderComponentFactory->CreateParticleEffectComponent(embersEffect);
				embersComponent->SetPlaybackType(CSRendering::ParticleEffectComponent::PlaybackType::k_looping);
				auto embersEntity = CSCore::Entity::Create();
				embersEntity->AddComponent(std::move(embersComponent));

				auto smokeComponent = renderComponentFactory->CreateParticleEffectComponent(smokeEffect);
				smokeComponent->SetPlaybackType(CSRendering::ParticleEffectComponent::PlaybackType::k_looping);
				auto smokeEntity = CSCore::Entity::Create();
				smokeEntity->AddComponent(std::move(smokeComponent));

				auto effectEntity = CSCore::Entity::Create();
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
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void State::OnInit()
		{
			GetScene()->SetClearColour(CSCore::Colour::k_black);

			auto basicEntityFactory = CSCore::Application::Get()->GetSystem<Common::BasicEntityFactory>();

			CSCore::EntitySPtr room = basicEntityFactory->CreateRoom();
			room->GetTransform().SetPosition(0.0f, 10.0f, 0.0f);
			GetScene()->Add(room);

			auto camera = basicEntityFactory->CreateThirdPersonCamera(room, CSCore::Vector3(0.0f, -9.0f, 0.0f));
			GetScene()->Add(std::move(camera));

			CSCore::EntitySPtr ambientLight = basicEntityFactory->CreateAmbientLight(CSCore::Colour(0.65f, 0.65f, 0.65f, 1.0f));
			GetScene()->Add(ambientLight);

			auto fire = CreateFire();
			GetScene()->Add(std::move(fire));
		}
	}
}
