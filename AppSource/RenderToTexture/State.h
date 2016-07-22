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

#ifndef _RENDERTOTEXTURE_STATE_H_
#define _RENDERTOTEXTURE_STATE_H_

#include <CSTest.h>

#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Base.h>

namespace CSTest
{
    namespace RenderToTexture
    {
        /// A state which contains the smoke test for rendering to texture. Has a simple scene and then
        /// renders it to 2 mini-cam style UI images. One image is fixed (only rendered once) the other is updated
        /// each frame
        ///
        class State final : public CS::State
        {
            /// The life-cycle event for creating all state systems.
            ///
            void CreateSystems() noexcept override;
            
            /// Called during the OnInit lifecycle event. This will set up the smoke test.
            ///
            void OnInit() noexcept override;
            
            /// Called while the state is active and updates the render texture
            ///
            void OnUpdate(f32 timeSinceLastUpdate) noexcept override;
            
            /// Clean up any resources when the state is destroyed
            ///
            void OnDestroy() noexcept override;
            
        private:
            
            static constexpr u32 k_numRenderTargets = 2;
            static constexpr u32 k_updatedRenderTargetIndex = k_numRenderTargets - 1;
            
            std::array<CS::TargetGroupUPtr, k_numRenderTargets> m_renderTargets;
            std::array<CS::Scene*, k_numRenderTargets> m_rtScenes;
            std::array<CS::WidgetSPtr, k_numRenderTargets> m_rtDisplays;
            std::array<CS::Vector2, k_numRenderTargets> m_rtDisplayVelocities;
            std::array<CS::TextureSPtr, k_numRenderTargets> m_renderTextures;
        };
    }
}

#endif
