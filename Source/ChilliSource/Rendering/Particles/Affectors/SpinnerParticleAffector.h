//
//  ColourChangerParticleAffector.h
//  moFloTest
//
//  Created by Scott Downie on 11/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_RENDERING_PARTICLES_AFFECTORS_SPINNER_PARTICLE_AFFECTOR_H_
#define _MO_FLO_RENDERING_PARTICLES_AFFECTORS_SPINNER_PARTICLE_AFFECTOR_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Rendering/Particles/Affectors/ParticleAffector.h>
#include <ChilliSource/Core/Container/ParamDictionary.h>

namespace ChilliSource
{
    namespace Rendering
    {
        class SpinnerParticleAffector : public ParticleAffector
        {
        public:
            SpinnerParticleAffector(const Core::ParamDictionary& insParams);
            //-----------------------------------------------------
            /// Create
            ///
            /// Create a new affector from the given parameters
            ///
            /// @param Param dictionary
            /// @return Ownership of affector
            //-----------------------------------------------------
            static ParticleAffectorUPtr Create(const Core::ParamDictionary& inParams);
            //-----------------------------------------------------
            /// Init
            ///
            /// The affector will initialise the particles to its
            /// liking 
            ///
            /// @param Particle to intialise
            //-----------------------------------------------------
            void Init(Particle& in_particle);
            //-----------------------------------------------------
            /// Apply
            ///
            /// The affector will apply itself to the given 
            /// particles
            ///
            /// @param Particle to effect
            /// @param Time between frames
            //-----------------------------------------------------
            void Apply(Particle& in_particle, f32 infDt);
            
        private:
            
            f32 mfAngVelocity;
        };
    }
}

#endif