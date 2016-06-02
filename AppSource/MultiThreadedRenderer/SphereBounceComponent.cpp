#include <MultiThreadedRenderer/SphereBounceComponent.h>

#include <ChilliSource/Core/Math.h>

namespace CSTest
{
    namespace MultiThreadedRenderer
    {
        namespace
        {
            CS::Vector3 Reflect(const CS::Vector3& in_vector, const CS::Vector3& in_normal, f32 in_coefficientOfRestitution) noexcept
            {
                return in_coefficientOfRestitution * (in_vector - 2 * CS::Vector3::DotProduct(in_vector, in_normal) * in_normal);
            }
        }
        
        CS_DEFINE_NAMEDTYPE(SphereBounceComponent);
        
        SphereBounceComponent::SphereBounceComponent(f32 in_sphereSize, f32 in_speed, f32 in_angularSpeed) noexcept
            : m_sphereSize(in_sphereSize), m_speed(in_speed), m_angularSpeed(in_angularSpeed)
        {
        }
        
        bool SphereBounceComponent::IsA(CS::InterfaceIDType in_interfaceId) const noexcept
        {
            return (SphereBounceComponent::InterfaceID == in_interfaceId);
        }
        
        void SphereBounceComponent::OnAddedToScene() noexcept
        {
            m_velocity = CS::Random::GenerateDirection3D<f32>() * m_speed;
            
            m_rotationAxis = CS::Random::GenerateDirection3D<f32>();
        }
        
        void SphereBounceComponent::OnUpdate(f32 in_deltaTime) noexcept
        {
            auto& transform = GetEntity()->GetTransform();
            auto position = transform.GetLocalPosition();
            auto orientation = transform.GetLocalOrientation();
            
            position += in_deltaTime * m_velocity;
            orientation *= CS::Quaternion(m_rotationAxis, in_deltaTime * m_angularSpeed);
            
            if (position.LengthSquared() > m_sphereSize * m_sphereSize)
            {
                auto sphereSurfaceNormal = CS::Vector3::Normalise(position);
                m_velocity = Reflect(m_velocity, sphereSurfaceNormal, 1.0f);
                
                position = sphereSurfaceNormal * m_sphereSize;
                
                m_rotationAxis = CS::Random::GenerateDirection3D<f32>();
            }
            
            transform.SetPosition(position);
            transform.SetOrientation(orientation);
        }
    }
}