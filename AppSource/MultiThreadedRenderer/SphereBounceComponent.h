#ifndef SphereBounceComponent_hpp
#define SphereBounceComponent_hpp

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Entity.h>

namespace CSTest
{
    namespace MultiThreadedRenderer
    {
        class SphereBounceComponent final : public CS::Component
        {
        public:
            CS_DECLARE_NAMEDTYPE(SphereBounceComponent);
            SphereBounceComponent(f32 in_sphereSize, f32 in_speed, f32 in_angularSpeed) noexcept;
            bool IsA(CS::InterfaceIDType in_interfaceId) const noexcept override;

        private:
            void OnAddedToScene() noexcept override;
            void OnUpdate(f32 in_deltaTime) noexcept override;
            
            f32 m_sphereSize;
            f32 m_speed;
            f32 m_angularSpeed;
            
            CS::Vector3 m_velocity;
            CS::Vector3 m_rotationAxis;
        };
    }
}

#endif