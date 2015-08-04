//
//  ModelFactory.cpp
//  CSTest
//  Created by Ian Copland on 03/08/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <Common/Rendering/MaterialFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Threading.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Texture.h>

namespace CSTest
{
    namespace Common
    {
        CS_DEFINE_NAMEDTYPE(MaterialFactory);
        
        MaterialFactoryUPtr MaterialFactory::Create()
        {
            return MaterialFactoryUPtr(new MaterialFactory());
        }
        
        bool MaterialFactory::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (MaterialFactory::InterfaceID == in_interfaceId);
        }
        
        CSRendering::MaterialCSPtr MaterialFactory::CreateStaticBlinnColour(const CSCore::Colour& in_colour) const
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create materials on a background thread.");
            
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto materialName = "MF_StaticBlinnColour(" + CSCore::ToString(in_colour) + ")";
            auto material = resourcePool->GetResource<CSRendering::Material>(materialName);
            
            if (material == nullptr)
            {
                auto texture = resourcePool->LoadResource<CSRendering::Texture>(CSCore::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                auto mutableMaterial = CSCore::Application::Get()->GetSystem<CSRendering::MaterialFactory>()->CreateStaticBlinnShadowed(materialName, texture);
                
                mutableMaterial->SetEmissive(CSCore::Colour::k_black);
                mutableMaterial->SetAmbient(in_colour);
                mutableMaterial->SetDiffuse(in_colour);
                mutableMaterial->SetSpecular(CSCore::Colour(0.5f, 0.5f, 0.5f, 10.0f));
                mutableMaterial->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                
                material = mutableMaterial;
            }
            
            return material;
        }
        
        
    }
}