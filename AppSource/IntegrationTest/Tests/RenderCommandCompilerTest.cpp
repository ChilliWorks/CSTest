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
            /// @return a new opaque, unlit, white material.
            ///
            CS::MaterialCSPtr CreateOpaqueMaterial() noexcept
            {
                constexpr char k_materialName[] = "RenderCommandCompiler_Opaque";
                
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
                
                auto material = resourcePool->GetResource<CS::Material>(k_materialName);
                if (!material)
                {
                    auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                    material = materialFactory->CreateUnlit(k_materialName, texture, false);
                }
                
                return material;
            }
            
            /// @return a new traparent, unlit, white material.
            ///
            CS::MaterialCSPtr CreateTransparentMaterial() noexcept
            {
                constexpr char k_materialName[] = "RenderCommandCompiler_Transparent";
                
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                auto materialFactory = CS::Application::Get()->GetSystem<CS::MaterialFactory>();
                
                auto material = resourcePool->GetResource<CS::Material>(k_materialName);
                if (!material)
                {
                    auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                    material = materialFactory->CreateUnlit(k_materialName, texture, true);
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
                return CS::RenderCamera(worldMatrix, projectionMatrix, CS::Quaternion::k_identity);
            }
            
            /// @return A new opaque unlit render pass object.
            ///
            CS::RenderPassObject CreateOpaqueRenderPassObject() noexcept
            {
                const CS::Matrix4 k_worldPosition = CS::Matrix4::CreateTransform(CS::Vector3::k_zero, CS::Vector3::k_one, CS::Quaternion::k_identity);
                
                auto material = CreateOpaqueMaterial();
                
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                auto model = primitiveModelFactory->CreateBox(CS::Vector3::k_one);
                
                //TODO: Handle deferred rendering
                auto renderMaterialGroup = material->GetRenderMaterialGroup();
                auto renderMaterial = renderMaterialGroup->GetRenderMaterial(CS::VertexFormat::k_staticMesh, static_cast<u32>(CS::ForwardRenderPasses::k_base));
                auto renderMesh = model->GetRenderMesh(0);
                auto worldBoundingSphere = renderMesh->GetBoundingSphere();
                
                return CS::RenderPassObject(renderMaterial, renderMesh, k_worldPosition, worldBoundingSphere, 0);
            }
            
            /// @return A transparent opaque unlit render pass object.
            ///
            CS::RenderPassObject CreateTransparentRenderPassObject() noexcept
            {
                const CS::Matrix4 k_worldPosition = CS::Matrix4::CreateTransform(CS::Vector3::k_zero, CS::Vector3::k_one, CS::Quaternion::k_identity);
                
                auto material = CreateTransparentMaterial();
                
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                auto model = primitiveModelFactory->CreateBox(CS::Vector3::k_one);
                
                //TODO: Handle deferred rendering
                auto renderMaterialGroup = material->GetRenderMaterialGroup();
                auto renderMaterial = renderMaterialGroup->GetRenderMaterial(CS::VertexFormat::k_staticMesh, static_cast<u32>(CS::ForwardRenderPasses::k_transparent));
                auto renderMesh = model->GetRenderMesh(0);
                auto worldBoundingSphere = renderMesh->GetBoundingSphere();
                
                return CS::RenderPassObject(renderMaterial, renderMesh, k_worldPosition, worldBoundingSphere, 0);
            }
            
            /// @return A basic render pass with a single object in it.
            ///
            CS::RenderPass CreateOpaqueRenderPass() noexcept
            {
                const std::vector<CS::RenderPassObject> renderPassObjects { CreateOpaqueRenderPassObject() };
                
                return CS::RenderPass(renderPassObjects);
            }
            
            /// @return A transparent render pass with a single object in it.
            ///
            CS::RenderPass CreateTransparentRenderPass() noexcept
            {
                const std::vector<CS::RenderPassObject> renderPassObjects { CreateTransparentRenderPassObject() };
                
                return CS::RenderPass(renderPassObjects);
            }
            
            /// @return A basic camera render pass group with a single render pass in it.
            ///
            CS::CameraRenderPassGroup CreateBasicCameraRenderPassGroup() noexcept
            {
                const std::vector<CS::RenderPass> renderPasses { CreateOpaqueRenderPass() };
                
                return CS::CameraRenderPassGroup(CreateRenderCamera(), renderPasses);
            }
            
            /// @return A compex camera render pass group with two render passes in it.
            ///
            CS::CameraRenderPassGroup CreateComplexCameraRenderPassGroup() noexcept
            {
                const std::vector<CS::RenderPass> renderPasses { CreateOpaqueRenderPass(), CreateTransparentRenderPass() };
                
                return CS::CameraRenderPassGroup(CreateRenderCamera(), renderPasses);
            }
            
            /// Creates a basic target render pass group with contains a single camera render pass
            /// group.
            ///
            CS::TargetRenderPassGroup CreateBasicTargetRenderPassGroup() noexcept
            {
                const std::vector<CS::CameraRenderPassGroup> cameraRenderPassGroups { CreateBasicCameraRenderPassGroup() };
                
                return CS::TargetRenderPassGroup(cameraRenderPassGroups);
            }
            
            /// Creates a complex target render pass group with contains a single camera render pass
            /// group.
            ///
            CS::TargetRenderPassGroup CreateComplexTargetRenderPassGroup() noexcept
            {
                const std::vector<CS::CameraRenderPassGroup> cameraRenderPassGroups { CreateComplexCameraRenderPassGroup() };
                
                return CS::TargetRenderPassGroup(cameraRenderPassGroups);
            }
        }
        
        CSIT_TESTCASE(RenderCommandCompilerTest)
        {
            /// Confirms that a basic list of target render pass groups can be compiled into a render
            /// command buffer.
            ///
            CSIT_TEST(SuccessBasic)
            {
                const std::vector<CS::TargetRenderPassGroup> targetRenderPassGroups { CreateBasicTargetRenderPassGroup() };
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::RenderCommandListUPtr preRenderCommandList(new CS::RenderCommandList());
                    CS::RenderCommandListUPtr postRenderCommandList(new CS::RenderCommandList());
                    
                    auto renderCommandBuffer = CS::RenderCommandCompiler::CompileRenderCommands(taskContext, targetRenderPassGroups, CS::Integer2(10, 10), CS::Colour::k_black,
                                                                                                std::vector<CS::RenderDynamicMeshUPtr>(), std::move(preRenderCommandList), std::move(postRenderCommandList));
                    
                    CSIT_ASSERT(renderCommandBuffer->GetNumSlots() == 3, "Incorrect number of render command buffer slots");
                    
                    auto renderCommandsA = renderCommandBuffer->GetQueue()[0]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsA.size() == 1, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsA[0]->GetType() == CS::RenderCommand::Type::k_begin, "Invalid command type.");
                    
                    auto renderCommandsB = renderCommandBuffer->GetQueue()[1]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsB.size() == 4, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsB[0]->GetType() == CS::RenderCommand::Type::k_applyCamera, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsB[1]->GetType() == CS::RenderCommand::Type::k_applyMaterial, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsB[2]->GetType() == CS::RenderCommand::Type::k_applyMesh, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsB[3]->GetType() == CS::RenderCommand::Type::k_renderInstance, "Invalid command type.");
                    
                    auto renderCommandsC = renderCommandBuffer->GetQueue()[2]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsC.size() == 1, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsC[0]->GetType() == CS::RenderCommand::Type::k_end, "Invalid command type.");
                    
                    CSIT_PASS();
                });
            }
            
            /// Confirms that a more complex list of target render pass groups can be compiled into a render
            /// command buffer.
            ///
            CSIT_TEST(SuccessComplex)
            {
                const std::vector<CS::TargetRenderPassGroup> targetRenderPassGroups { CreateComplexTargetRenderPassGroup() };
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    // In a more real work case, the pre and post list would only contain load and unload commands, however we are using
                    // apply camera commands as they're easier to setup for testing.
                    CS::RenderCommandListUPtr preRenderCommandList(new CS::RenderCommandList());
                    preRenderCommandList->AddApplyCameraCommand(CS::Vector3::k_zero, CS::Matrix4::k_identity);
                    
                    CS::RenderCommandListUPtr postRenderCommandList(new CS::RenderCommandList());
                    postRenderCommandList->AddApplyCameraCommand(CS::Vector3::k_zero, CS::Matrix4::k_identity);
                    
                    auto renderCommandBuffer = CS::RenderCommandCompiler::CompileRenderCommands(taskContext, targetRenderPassGroups, CS::Integer2(10, 10), CS::Colour::k_black,
                                                                                               std::vector<CS::RenderDynamicMeshUPtr>(), std::move(preRenderCommandList), std::move(postRenderCommandList));
                    
                    CSIT_ASSERT(renderCommandBuffer->GetNumSlots() == 6, "Incorrect number of render command buffer slots");
                    
                    auto renderCommandsA = renderCommandBuffer->GetQueue()[0]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsA.size() == 1, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsA[0]->GetType() == CS::RenderCommand::Type::k_applyCamera, "Invalid command type.");
                    
                    auto renderCommandsB = renderCommandBuffer->GetQueue()[1]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsB.size() == 1, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsB[0]->GetType() == CS::RenderCommand::Type::k_begin, "Invalid command type.");
                    
                    auto renderCommandsC = renderCommandBuffer->GetQueue()[2]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsC.size() == 4, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsC[0]->GetType() == CS::RenderCommand::Type::k_applyCamera, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsC[1]->GetType() == CS::RenderCommand::Type::k_applyMaterial, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsC[2]->GetType() == CS::RenderCommand::Type::k_applyMesh, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsC[3]->GetType() == CS::RenderCommand::Type::k_renderInstance, "Invalid command type.");
                    
                    auto renderCommandsD = renderCommandBuffer->GetQueue()[3]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsD.size() == 3, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsD[0]->GetType() == CS::RenderCommand::Type::k_applyMaterial, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsD[1]->GetType() == CS::RenderCommand::Type::k_applyMesh, "Invalid command type.");
                    CSIT_ASSERT(renderCommandsD[2]->GetType() == CS::RenderCommand::Type::k_renderInstance, "Invalid command type.");
                    
                    auto renderCommandsE = renderCommandBuffer->GetQueue()[4]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsE.size() == 1, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsE[0]->GetType() == CS::RenderCommand::Type::k_end, "Invalid command type.");
                    
                    auto renderCommandsF = renderCommandBuffer->GetQueue()[5]->GetOrderedList();
                    CSIT_ASSERT(renderCommandsF.size() == 1, "Incorrect number of render command buffer slots");
                    CSIT_ASSERT(renderCommandsF[0]->GetType() == CS::RenderCommand::Type::k_applyCamera, "Invalid command type.");
                    
                    CSIT_PASS();
                });
            }
        }
    }
}
