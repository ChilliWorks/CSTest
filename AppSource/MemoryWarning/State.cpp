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

#include <MemoryWarning/State.h>

#include <Common/UI/BasicWidgetFactory.h>
#include <Common/UI/OptionsMenuPresenter.h>
#include <Common/UI/OptionsMenuDesc.h>
#include <Common/Core/TestNavigator.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Core/State.h>
#include <ChilliSource/Rendering/Font.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/UI/Base.h>
#include <ChilliSource/UI/Text.h>


namespace CSTest
{
    namespace MemoryWarning
    {
        namespace
        {
            constexpr f32 k_timeBetweenTextureCreations = 0.05f;
        }
        
        //------------------------------------------------------------------------------
        void State::CreateAndDiscardTexture() noexcept
        {
            const CS::Integer2 dimensions(256, 256);
            
            u32 textureDataSize = sizeof(CS::ByteColour) * dimensions.x * dimensions.y;
            std::unique_ptr<u8[]> textureData(new u8[textureDataSize]);
            CS::TextureDesc desc(dimensions, CS::ImageFormat::k_RGBA8888, CS::ImageCompression::k_none, true);
            
            static u32 s_textureId = 0;
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto texture = resourcePool->CreateResource<CS::Texture>("ThrowawayTexture" + CS::ToString(s_textureId++));
            
            texture->Build(std::move(textureData), textureDataSize, desc);
            texture->SetLoadState(CS::Resource::LoadState::k_loaded);
        }
        
        //------------------------------------------------------------------------------
        void State::InitTextureCountDisplay() noexcept
        {
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto smallFont = resourcePool->LoadResource<CS::Font>(CS::StorageLocation::k_package, "Fonts/ArialSmall.csfont");
            
            auto basicWidgetFactory = CS::Application::Get()->GetSystem<Common::BasicWidgetFactory>();
            m_label = basicWidgetFactory->CreateLabel(CS::Vector2(0.9f, 0.8f), smallFont, "");
            m_textComponent = m_label->GetComponent<CS::TextUIComponent>();
            
            GetUICanvas()->AddWidget(m_label);
            
            UpdateTextureCountDisplay();
        }
        
        //------------------------------------------------------------------------------
        void State::UpdateTextureCountDisplay() noexcept
        {
            CS_ASSERT(m_textComponent, "Display hasn't been initialised.");
            
            auto resourcePool = CS::Application::Get()->GetResourcePool();
            auto numTextures = resourcePool->GetAllResources<CS::Texture>().size();
            
            m_textComponent->SetText("Number of textures: " + CS::ToString(numTextures));
        }
        
        //------------------------------------------------------------------------------
        void State::DestroyTextureCountDisplay() noexcept
        {
            m_label->RemoveFromParent();
            m_label.reset();
            m_textComponent = nullptr;
        }
        
        //------------------------------------------------------------------------------
        void State::CreateSystems() noexcept
        {
            CreateSystem<Common::TestNavigator>("Memory Warning");
        }

        //------------------------------------------------------------------------------
        void State::OnInit() noexcept
        {
            GetMainScene()->SetClearColour(CS::Colour(0.9f, 0.9f, 0.9f, 1.0f));
            
            InitTextureCountDisplay();
        }
        
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 deltaTime) noexcept
        {
            m_textureCreationTimer += deltaTime;
            while (m_textureCreationTimer > k_timeBetweenTextureCreations)
            {
                m_textureCreationTimer -= k_timeBetweenTextureCreations;
                
                CreateAndDiscardTexture();
                
                UpdateTextureCountDisplay();
            }
        }
        
        //------------------------------------------------------------------------------
        void State::OnDestroy() noexcept
        {
            DestroyTextureCountDisplay();
        }
    }
}
