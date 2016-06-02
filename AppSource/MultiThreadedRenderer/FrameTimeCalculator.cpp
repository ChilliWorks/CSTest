#include <MultiThreadedRenderer/FrameTimeCalculator.h>

namespace CSTest
{
    namespace MultiThreadedRenderer
    {
        namespace
        {
            constexpr u32 k_reportFrequency = 60;
        }
        
        FrameTimeCalculator::FrameTimeCalculator()
        {
            
        }
        
        void FrameTimeCalculator::Update(f32 in_deltaTime)
        {
            if (--m_skipFrames > 0)
            {
                ++m_numSamples;
                m_accumulatedTime += in_deltaTime;
                
                if (m_numSamples % k_reportFrequency == 0)
                {
                    u32 reportNumber = m_numSamples / k_reportFrequency;
                    f32 averageSeconds = m_accumulatedTime / m_numSamples;
                    f32 averageMs = averageSeconds * 1000.0f;
                    
                    CS_LOG_ERROR("Mean frame time report #" + CS::ToString(reportNumber) + ": " + CS::ToString(averageMs) + "ms");
                }
            }
        }
    }
}
