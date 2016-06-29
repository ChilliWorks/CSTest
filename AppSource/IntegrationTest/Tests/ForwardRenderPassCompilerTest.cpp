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

#include <Common/Core/BasicEntityFactory.h>

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Threading.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Material.h>
#include <ChilliSource/Rendering/Texture.h>
#include <ChilliSource/Rendering/Model.h>

#include <array>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            constexpr f32 k_cameraDistanceFromLookAt = 10.0f;
            
            const CS::Integer2 k_resolution(100, 100);
            const CS::Vector3 k_cameraLookAt = CS::Vector3(0.0f, 0.0f, 0.0f);
            const CS::Vector3 k_cameraPosition = k_cameraLookAt - CS::Vector3(0.0f, 0.0f, k_cameraDistanceFromLookAt);
            const CS::Matrix4 k_onScreenObjectTransform = CS::Matrix4::CreateTransform(k_cameraLookAt, CS::Vector3(1.0f, 1.0f, 1.0f), CS::Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
            const CS::Matrix4 k_offScreenObjectTransform = CS::Matrix4::CreateTransform(k_cameraLookAt - (2 * k_cameraDistanceFromLookAt * CS::Vector3(0.0f, 0.0f, 1.0f)), CS::Vector3(1.0f, 1.0f, 1.0f), CS::Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
            
            /// Creates an render camera at (0, 0, -10) and looking at the
            /// origin.
            ///
            /// @return Created RenderCamera
            ///
            CS::RenderCamera CreateRenderCamera() noexcept
            {
                auto worldMatrix = CS::Matrix4::CreateLookAt(k_cameraPosition, k_cameraLookAt, CS::Vector3::k_unitPositiveZ);
                auto projectionMatrix = CS::Matrix4::CreatePerspectiveProjectionLH(CS::MathUtils::k_pi / 3.0f, 1.0f, 1.0f, 10.0f);
                return CS::RenderCamera(worldMatrix, projectionMatrix);
            }

            /// Creates an lit opaque material group with a blank texture
            ///
            /// @return RenderMaterialGroup
            ///
            const CS::RenderMaterialGroup* CreateLitOpaqueMaterialGroup() noexcept
            {
                auto renderMaterialGroupManager = CS::Application::Get()->GetSystem<CS::RenderMaterialGroupManager>();
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                
                auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                auto renderTexture = texture->GetRenderTexture();
                return renderMaterialGroupManager->CreateBlinnRenderMaterialGroup(renderTexture, CS::Colour::k_white, CS::Colour::k_white, CS::Colour::k_white, CS::Colour::k_white);
            }
            
            /// Creates an unlit transparent material group with a blank texture
            ///
            /// @return RenderMaterialGroup
            ///
            const CS::RenderMaterialGroup* CreateUnlitTransparentMaterialGroup() noexcept
            {
                auto renderMaterialGroupManager = CS::Application::Get()->GetSystem<CS::RenderMaterialGroupManager>();
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                
                auto texture = resourcePool->LoadResource<CS::Texture>(CS::StorageLocation::k_chilliSource, "Textures/Blank.csimage");
                auto renderTexture = texture->GetRenderTexture();
                return renderMaterialGroupManager->CreateUnlitRenderMaterialGroup(renderTexture, true, true, true, true, false, CS::BlendMode::k_one, CS::BlendMode::k_oneMinusSourceAlpha, CS::CullFace::k_back, CS::Colour::k_white, CS::Colour::k_white);
            }
            
            /// Removes the material group from the manager and queue it for destruction.
            ///
            void DestroyMaterialGroup(const CS::RenderMaterialGroup* materialGroup) noexcept
            {
                auto renderMaterialGroupManager = CS::Application::Get()->GetSystem<CS::RenderMaterialGroupManager>();
                renderMaterialGroupManager->DestroyRenderMaterialGroup(materialGroup);
            }
            
            /// Creates a simple render object at the given position with the given render material
            /// group.
            ///
            /// @param renderMaterialGroup
            ///     The render material group that should be used to render the object.
            /// @param worldMatrix
            ///     The world space transform of the render object.
            ///
            /// @return The render object.
            ///
            CS::RenderObject CreateRenderObject(const CS::RenderMaterialGroup* renderMaterialGroup, const CS::Matrix4& worldMatrix) noexcept
            {
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                auto renderMesh = primitiveModelFactory->CreateBox(CS::Vector3::k_one)->GetRenderMesh(0);
                
                const auto& localBoundingSphere = renderMesh->GetBoundingSphere();
                CS::Sphere worldBoundingSphere(worldMatrix.GetTranslation() + localBoundingSphere.vOrigin, localBoundingSphere.fRadius);
                
                return CS::RenderObject(renderMaterialGroup, renderMesh, worldMatrix, worldBoundingSphere, CS::RenderLayer::k_standard);
            }
        }
        
        CSIT_TESTCASE(ForwardRenderPassCompiler)
        {
            /// Validates that a RenderFrame with a opaque renderObject and ambient light is setup correctly
            ///
            CSIT_TEST(SuccessAmbientLitOpaque)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                
                auto materialGroup = CreateLitOpaqueMaterialGroup();
                std::vector<CS::RenderDirectionalLight> directionalLights;
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateRenderObject(materialGroup, k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(k_resolution, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, renderFrame);
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 1, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 2, "Unexpected size of CameraRenderPassGroup group. Expecting only base and transparent passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of second pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain 1 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the second pass, should contain 0 transparent RenderObjects");
                    
                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent renderObject and ambient light is setup correctly
            ///
            CSIT_TEST(SuccessAmbientUnilitTransparent)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                
                auto materialGroup = CreateUnlitTransparentMaterialGroup();
                
                std::vector<CS::RenderDirectionalLight> directionalLights;
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateRenderObject(materialGroup, k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(k_resolution, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, renderFrame);
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 1, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 2, "Unexpected size of CameraRenderPassGroup group. Expecting only base and transparent passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of second pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the second pass, should contain 1 transparent RenderObjects");
                    
                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a opaque lit renderObject, ambient and directional light is setup correctly
            ///
            CSIT_TEST(SuccessDirectionalLitOpaque)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                CS::RenderDirectionalLight directionalLight(CS::Colour::k_red, CS::Vector3::k_unitNegativeZ);
                
                auto materialGroup = CreateLitOpaqueMaterialGroup();
                
                std::vector<CS::RenderDirectionalLight> directionalLights { directionalLight };
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateRenderObject(materialGroup, k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(k_resolution, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, renderFrame);
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 1, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 3, "Unexpected size of CameraRenderPassGroup group. Expecting only base, directional and transparent passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::k_directional, "Unexpected light type of second pass, should contain directional light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[2].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of second pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain 1 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the second pass, should contain 1 directional RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[2].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the third pass, should contain 0 transparent RenderObjects");
                    
                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent renderObject, ambient and directional light is setup correctly
            ///
            CSIT_TEST(SuccessDirectionalUnilitTransparent)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                CS::RenderDirectionalLight directionalLight(CS::Colour::k_red, CS::Vector3::k_unitNegativeZ);
                
                auto materialGroup = CreateUnlitTransparentMaterialGroup();
                
                std::vector<CS::RenderDirectionalLight> directionalLights { directionalLight };
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateRenderObject(materialGroup, k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(k_resolution, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, renderFrame);
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 1, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 3, "Unexpected size of CameraRenderPassGroup group. Expecting only base, directional and transparent passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::k_directional, "Unexpected light type of second pass, should contain directional light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[2].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of second pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the second pass, should contain 0 directional RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[2].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the third pass, should contain 1 transparent RenderObjects");
                    
                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent, opaque renderObject, ambient and directional light is setup correctly
            ///
            CSIT_TEST(SuccessMultipleRenderObjectTypes)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                CS::RenderDirectionalLight directionalLight(CS::Colour::k_red, CS::Vector3::k_unitNegativeZ);
                
                auto transparentMaterialGroup = CreateUnlitTransparentMaterialGroup();
                auto opaqueMaterialGroup = CreateLitOpaqueMaterialGroup();
                
                auto transparentObject = CreateRenderObject(transparentMaterialGroup, k_onScreenObjectTransform);
                auto opaqueObject = CreateRenderObject(opaqueMaterialGroup, k_onScreenObjectTransform);
                
                std::vector<CS::RenderDirectionalLight> directionalLights { directionalLight };
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { transparentObject, opaqueObject };
                
                CS::RenderFrame renderFrame(k_resolution, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, renderFrame);
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 1, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 3, "Unexpected size of CameraRenderPassGroup group. Expecting only base, directional and transparent passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::k_directional, "Unexpected light type of second pass, should contain directional light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[2].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of second pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain 1 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the second pass, should contain 1 directional RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[2].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the third pass, should contain 1 transparent RenderObjects");
                    
                    DestroyMaterialGroup(transparentMaterialGroup);
                    DestroyMaterialGroup(opaqueMaterialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that offscreen objects are culled
            ///
            CSIT_TEST(SuccessFrustrumCull)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                
                auto materialGroup = CreateLitOpaqueMaterialGroup();
                
                std::vector<CS::RenderDirectionalLight> directionalLights;
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateRenderObject(materialGroup, k_offScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(k_resolution, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, renderFrame);
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 1, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 2, "Unexpected size of CameraRenderPassGroup group. Expecting only base, directional and transparent passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::k_ambient, "Unexpected light type of second pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the second pass, should contain 0 transparent RenderObjects");
                    
                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
        }
    }
}
