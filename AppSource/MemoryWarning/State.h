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

#ifndef _MEMORYWARNING_STATE_H_
#define _MEMORYWARNING_STATE_H_

#include <CSTest.h>

#include <ChilliSource/Core/State.h>

namespace CSTest
{
    namespace MemoryWarning
    {
        /// A state for testing the memory warnings on iOS.
        ///
        class State final : public CS::State
        {
            /// Creates a new example texture, but doesn't hold a reference to it. This is called
            /// repeatedly to force a memory warning, which should release all of the textures.
            ///
            void CreateAndDiscardTexture() noexcept;
            
            /// Initialise the texture count display.
            ///
            void InitTextureCountDisplay() noexcept;
            
            /// Updates the texture count display with the current texture count.
            ///
            void UpdateTextureCountDisplay() noexcept;
            
            /// Cleans up the texture count display.
            ///
            void DestroyTextureCountDisplay() noexcept;
            
            /// The life-cycle event for creating all state systems.
            ///
            void CreateSystems() noexcept override;

            /// Initialises the state.
            ///
            void OnInit() noexcept override;
            
            /// Called every frame during the update lifecycle event.
            ///
            /// @param deltaTime
            ///     The delta time.
            ///
            void OnUpdate(f32 deltaTime) noexcept override;
            
            /// Initialises the state.
            ///
            void OnDestroy() noexcept override;
            
            f32 m_textureCreationTimer = 0.0f;
            CS::WidgetSPtr m_label;
            CS::TextUIComponent* m_textComponent = nullptr;
        };
    }
}

#endif
