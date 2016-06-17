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

#include <IntegrationTest/TestSystem/TestCase.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Math.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Model.h>
#include <ChilliSource/Rendering/RenderCommand.h>

#include <vector>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            /// @return a new unlit, white material.
            ///
            CS::MaterialCSPtr CreateUnlitMaterial() noexcept
            {
                constexpr char k_materialName[] = "RenderCommandCompiler_Unlit";
                
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
                
                auto material = resourcePool->GetResource<CS::Material>(k_materialName);
                if (material == nullptr)
                {
                    auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                    
                    auto mutableMaterial = materialFactory->CreateStatic(k_materialName, texture);
                    mutableMaterial->SetEmissive(CS::Colour::k_white);
                    mutableMaterial->SetAmbient(CS::Colour::k_black);
                    mutableMaterial->SetDiffuse(CS::Colour::k_black);
                    mutableMaterial->SetSpecular(CS::Colour::k_black);
                    mutableMaterial->SetLoadState(CS::Resource::LoadState::k_loaded);
                    
                    material = mutableMaterial;
                }
                
                return material;
            }
            
            /// @return A render camera at (0, 0, -10) and looking at the origin.
            ///
            CS::RenderCamera CreateRenderCamera() noexcept
            {
                const CS::Vector3 k_cameraPosition(0.0f, 0.0f, -10.0f);
                const CS::Vector3 k_cameraLookAt(0.0f, 0.0f, 0.0f);
                
                auto worldMatrix = CS::Matrix4::CreateLookAt(k_cameraPosition, k_cameraLookAt, CS::Vector3::k_unitPositiveZ);
                auto projectionMatrix = CS::Matrix4::CreatePerspectiveProjectionLH(CS::MathUtils::k_pi / 3.0f, 1.0f, 1.0f, 100.0f);
                return CS::RenderCamera(worldMatrix, projectionMatrix);
            }
            
            /// @return A new unlit render pass object.
            ///
            CS::RenderPassObject CreateRenderPassObject() noexcept
            {
                const CS::Matrix4 k_worldPosition = CS::Matrix4::CreateTransform(CS::Vector3::k_zero, CS::Vector3::k_one, CS::Quaternion::k_identity);
                
                auto material = CreateUnlitMaterial();
                
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                auto model = primitiveModelFactory->CreateBox(CS::Vector3::k_one);
                
                //TODO: Handle deferred rendering
                auto renderMaterialGroup = material->GetRenderMaterialGroup();
                auto renderMaterial = renderMaterialGroup->GetRenderMaterial(CS::VertexFormat::k_staticMesh, static_cast<u32>(CS::ForwardRenderPasses::k_base));
                auto renderMesh = model->GetRenderMesh(0);
                
                return CS::RenderPassObject(renderMaterial, renderMesh, k_worldPosition);
            }
            
            /// @return A basic render pass with a single object in it.
            ///
            CS::RenderPass CreateRenderPass() noexcept
            {
                const std::vector<CS::RenderPassObject> renderPassObjects { CreateRenderPassObject() };
                
                return CS::RenderPass(renderPassObjects);
            }
            
            /// @return A basic camera render pass group with a single render pass in it.
            ///
            CS::CameraRenderPassGroup CreateCameraRenderPassGroup() noexcept
            {
                const std::vector<CS::RenderPass> renderPasses { CreateRenderPass() };
                
                return CS::CameraRenderPassGroup(CreateRenderCamera(), renderPasses);
            }
            
            /// Creates a basic target render pass group with contains a single camera render pass
            /// group.
            ///
            CS::TargetRenderPassGroup CreateBasicTargetRenderPassGroup() noexcept
            {
                const std::vector<CS::CameraRenderPassGroup> cameraRenderPassGroups { CreateCameraRenderPassGroup() };
                
                return CS::TargetRenderPassGroup(cameraRenderPassGroups);
            }
        }
        
        CSIT_TESTCASE(RenderCommandCompilerTest)
        {
            /// Confirms that a list of target render pass groups can be compiled into a render
            /// command queue.
            ///
            CSIT_TEST(Success)
            {
                const std::vector<CS::TargetRenderPassGroup> targetRenderPassGroups { CreateBasicTargetRenderPassGroup() };
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::RenderCommandListUPtr preRenderCommandList(new CS::RenderCommandList());
                    CS::RenderCommandListUPtr postRenderCommandList(new CS::RenderCommandList());
                    
                    auto renderCommandQueue = CS::RenderCommandCompiler::CompileRenderCommands(taskContext, targetRenderPassGroups, CS::Integer2(10, 10), CS::Colour::k_black,
                                                                                               std::move(preRenderCommandList), std::move(postRenderCommandList));
                    
                    //TODO: Implement checks to confirm that the render command queue contains the correct stuff.
                    
                    CSIT_PASS();
                });
                
            }
        }
    }
}
