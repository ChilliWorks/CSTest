#ifndef FrameTimeCalculator_hpp
#define FrameTimeCalculator_hpp

#include <ChilliSource/ChilliSource.h>

#include <vector>

namespace CSTest
{
    namespace MultiThreadedRenderer
    {
        class FrameTimeCalculator
        {
        public:
            FrameTimeCalculator();
            void Update(f32 in_deltaTime);
            
        private:
            u32 m_skipFrames = 5;
            u32 m_numSamples = 0;
            f32 m_accumulatedTime = 0.0f;
        };
    }
}

#endif
