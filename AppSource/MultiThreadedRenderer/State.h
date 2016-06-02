#ifndef _MULTITHREADEDRENDERER_STATE_H_
#define _MULTITHREADEDRENDERER_STATE_H_

#include <CSTest.h>

#include <ChilliSource/Core/State.h>

#include <MultiThreadedRenderer/FrameTimeCalculator.h>

namespace CSTest
{
    namespace MultiThreadedRenderer
    {
        class State final : public CS::State
        {
            void CreateSystems() noexcept override;
            void OnInit() noexcept override;
            void OnUpdate(f32 in_deltaTime) noexcept override;
            
            FrameTimeCalculator m_frameTimeCalculator;
        };
    }
}

#endif
