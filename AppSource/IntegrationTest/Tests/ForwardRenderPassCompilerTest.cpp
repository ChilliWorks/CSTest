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
#include <ChilliSource/Rendering/Sprite.h>

#include <array>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            constexpr f32 k_cameraDistanceFromLookAt = 10.0f;
            
            const CS::Integer2 k_resolution(100, 100);
            const CS::Vector3 k_cameraLookAt(0.0f, 0.0f, 0.0f);
            const CS::Vector3 k_cameraPosition = k_cameraLookAt - CS::Vector3(0.0f, 0.0f, k_cameraDistanceFromLookAt);
            const CS::Vector3 k_onScreenObjectPosition = k_cameraLookAt;
            const CS::Vector3 k_offScreenObjectPosition = k_cameraLookAt - (2 * k_cameraDistanceFromLookAt * CS::Vector3(0.0f, 0.0f, 1.0f));
            const CS::Vector2 k_uiObjectPosition(50.0f, 50.0f);
            
            /// Creates an render camera at (0, 0, -10) and looking at the
            /// origin.
            ///
            /// @return Created RenderCamera
            ///
            CS::RenderCamera CreateRenderCamera() noexcept
            {
                auto worldMatrix = CS::Matrix4::CreateLookAt(k_cameraPosition, k_cameraLookAt, CS::Vector3::k_unitPositiveZ);
                auto projectionMatrix = CS::Matrix4::CreatePerspectiveProjectionLH(CS::MathUtils::k_pi / 3.0f, 1.0f, 1.0f, 10.0f);
                
                CS::Vector3 position, scale;
                CS::Quaternion orientation;
                worldMatrix.Decompose(position, scale, orientation);
                
                return CS::RenderCamera(worldMatrix, projectionMatrix, orientation);
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
                return renderMaterialGroupManager->CreateUnlitRenderMaterialGroup(renderTexture, true, true, true, true, false, false,
                                                                                  CS::TestFunc::k_lessEqual,
                                                                                  CS::BlendMode::k_one, CS::BlendMode::k_oneMinusSourceAlpha,
                                                                                  CS::StencilOp::k_keep, CS::StencilOp::k_keep, CS::StencilOp::k_keep, CS::TestFunc::k_always, 1, 0xff,
                                                                                  CS::CullFace::k_back, CS::Colour::k_white, CS::Colour::k_white);
            }
            
            /// Creates an skybox material group with a cubemap
            ///
            /// @return RenderMaterialGroup
            ///
            const CS::RenderMaterialGroup* CreateSkyboxMaterialGroup() noexcept
            {
                auto renderMaterialGroupManager = CS::Application::Get()->GetSystem<CS::RenderMaterialGroupManager>();
                auto resourcePool = CS::Application::Get()->GetResourcePool();
                
                auto cubemap = resourcePool->LoadResource<CS::Cubemap>(CS::StorageLocation::k_package, "Cubemaps/EnvMap.cscubemap");
                auto renderCubemap = cubemap->GetRenderTexture();
                return renderMaterialGroupManager->CreateSkyboxRenderMaterialGroup(renderCubemap);
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
            /// @param position
            ///     The world space position of the render object.
            ///
            /// @return The render object.
            ///
            CS::RenderObject CreateStandardRenderObject(const CS::RenderMaterialGroup* renderMaterialGroup, const CS::Vector3& position) noexcept
            {
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                auto renderMesh = primitiveModelFactory->CreateBox(CS::Vector3::k_one)->GetRenderMesh(0);
                auto worldMatrix = CS::Matrix4::CreateTransform(position, CS::Vector3::k_one, CS::Quaternion::k_identity);
                
                const auto& localBoundingSphere = renderMesh->GetBoundingSphere();
                CS::Sphere worldBoundingSphere(worldMatrix.GetTranslation() + localBoundingSphere.vOrigin, localBoundingSphere.fRadius);
                
                return CS::RenderObject(renderMaterialGroup, renderMesh, worldMatrix, worldBoundingSphere, false, CS::RenderLayer::k_standard);
            }
            
            /// Creates a skybox render object with the given render material
            /// group.
            ///
            /// @param renderMaterialGroup
            ///     The render material group that should be used to render the object.
            ///
            /// @return The render object.
            ///
            CS::RenderObject CreateSkyboxRenderObject(const CS::RenderMaterialGroup* renderMaterialGroup) noexcept
            {
                auto primitiveModelFactory = CS::Application::Get()->GetSystem<CS::PrimitiveModelFactory>();
                auto renderMesh = primitiveModelFactory->CreateBox(CS::Vector3::k_one)->GetRenderMesh(0);
                
                return CS::RenderObject(renderMaterialGroup, renderMesh, CS::Matrix4::k_identity, renderMesh->GetBoundingSphere(), false, CS::RenderLayer::k_skybox);
            }
            
            /// Creates a new UI render object with the given material group, dynamic mesh
            /// and position.
            ///
            /// @param renderMaterialGroup
            ///     The render material group that should be used to render the object.
            /// @param renderDynamicMesh
            ///     The dynamic mesh that the UI render object should use.
            /// @param position
            ///     The screen space position of the UI object.
            ///
            /// @return The render object.
            ///
            CS::RenderObject CreateUIRenderObject(const CS::RenderMaterialGroup* renderMaterialGroup, const CS::RenderDynamicMesh* renderDynamicMesh, const CS::Vector2& position) noexcept
            {
                auto worldMatrix = CS::Matrix4::CreateTransform(CS::Vector3(position, 0.0f), CS::Vector3::k_one, CS::Quaternion::k_identity);
                
                const auto& localBoundingSphere = renderDynamicMesh->GetBoundingSphere();
                CS::Sphere worldBoundingSphere(worldMatrix.GetTranslation() + localBoundingSphere.vOrigin, localBoundingSphere.fRadius);
                
                return CS::RenderObject(renderMaterialGroup, renderDynamicMesh, worldMatrix, worldBoundingSphere, false, CS::RenderLayer::k_ui);
            }
        }
        
        CSIT_TESTCASE(ForwardRenderPassCompiler)
        {
            /// Validates that a RenderFrame with a opaque renderObject and ambient light is setup correctly
            ///
            CSIT_TEST(SuccessAmbientLitOpaque)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::AmbientRenderLight ambientLight(CS::Colour::k_red);
                
                auto materialGroup = CreateLitOpaqueMaterialGroup();
                std::vector<CS::DirectionalRenderLight> directionalLights;
                std::vector<CS::PointRenderLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateStandardRenderObject(materialGroup, k_onScreenObjectPosition) };
                
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup list.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 1, "Unexpected size of CameraRenderPassGroup group. Expecting only base pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain 1 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected number of render passes in the Transparent CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of render objects in the Transparent pass, should be 0.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");

                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent renderObject and ambient light is setup correctly
            ///
            CSIT_TEST(SuccessAmbientUnlitTransparent)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::AmbientRenderLight ambientLight(CS::Colour::k_red);
                
                auto materialGroup = CreateUnlitTransparentMaterialGroup();
                
                std::vector<CS::DirectionalRenderLight> directionalLights;
                std::vector<CS::PointRenderLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateStandardRenderObject(materialGroup, k_onScreenObjectPosition) };
                
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 1, "Unexpected size of CameraRenderPassGroup group. Expecting only base pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected size of Transparent CameraRenderPassGroup group. Expecting only Transparent pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the Transparent pass, should contain 1 RenderObject");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");

                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a opaque lit renderObject, ambient and directional light is setup correctly
            ///
            CSIT_TEST(SuccessDirectionalLitOpaque)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::AmbientRenderLight ambientLight(CS::Colour::k_red);
                CS::DirectionalRenderLight directionalLight(CS::Colour::k_red, CS::Vector3::k_unitNegativeZ);
                
                auto materialGroup = CreateLitOpaqueMaterialGroup();
                
                std::vector<CS::DirectionalRenderLight> directionalLights { directionalLight };
                std::vector<CS::PointRenderLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateStandardRenderObject(materialGroup, k_onScreenObjectPosition) };
                
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 2, "Unexpected size of CameraRenderPassGroup group. Expecting only base and directional passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::LightType::k_directional, "Unexpected light type of second pass, should contain directional light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain 1 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the second pass, should contain 1 directional RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected size of Transparent CameraRenderPassGroup group. Expecting only Transparent pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the Transparent pass, should contain 0 RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");

                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent renderObject, ambient and directional light is setup correctly
            ///
            CSIT_TEST(SuccessDirectionalUnlitTransparent)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::AmbientRenderLight ambientLight(CS::Colour::k_red);
                CS::DirectionalRenderLight directionalLight(CS::Colour::k_red, CS::Vector3::k_unitNegativeZ);
                
                auto materialGroup = CreateUnlitTransparentMaterialGroup();
                
                std::vector<CS::DirectionalRenderLight> directionalLights { directionalLight };
                std::vector<CS::PointRenderLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateStandardRenderObject(materialGroup, k_onScreenObjectPosition) };
                
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 2, "Unexpected size of CameraRenderPassGroup group. Expecting only base and directional passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::LightType::k_directional, "Unexpected light type of second pass, should contain directional light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the second pass, should contain 0 directional RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the Transparent pass, should contain 1 RenderObject");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");

                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with UI objects will be correct converted to UI RenderPasses.
            ///
            CSIT_TEST(SuccessUI)
            {
                auto allocator = std::make_shared<CS::PagedLinearAllocator>(1024 * 1024);
                
                auto transparentMaterialGroup = CreateUnlitTransparentMaterialGroup();
                CS::RenderDynamicMeshASPtr renderDynamicMesh = CS::SpriteMeshBuilder::Build(allocator.get(), CS::Vector3::k_zero, CS::Vector2::k_one, CS::UVs(), CS::Colour::k_red, CS::AlignmentAnchor::k_middleCentre);
                
                std::vector<CS::RenderObject> renderObjects { CreateUIRenderObject(transparentMaterialGroup, renderDynamicMesh.get(), k_uiObjectPosition) };
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, CreateRenderCamera(), CS::AmbientRenderLight(CS::Colour::k_red), std::vector<CS::DirectionalRenderLight>(), std::vector<CS::PointRenderLight>(), renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    //Need to force these to be captured so these aren't deleted.
                    auto allocatorRef = allocator;
                    auto renderDynamicMeshRef = renderDynamicMesh;
                    
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 1, "Unexpected size of CameraRenderPassGroup group. Expecting only base passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected size of Transparent CameraRenderPassGroup group. Expecting only Transparent pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the Transparent pass, should contain 0 RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 1, "Unexpected number of render passes in the UI CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 1, "Unexpected number of render passes in the UI CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_none, "Unexpected light type for UI render pass.");
                    
                    DestroyMaterialGroup(transparentMaterialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a Skybox object will be correct converted to UI RenderPasses.
            ///
            CSIT_TEST(SuccessSkybox)
            {
                auto allocator = std::make_shared<CS::PagedLinearAllocator>(1024 * 1024);
                
                auto materialGroup = CreateSkyboxMaterialGroup();

                std::vector<CS::RenderObject> renderObjects { CreateSkyboxRenderObject(materialGroup) };
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, CreateRenderCamera(), CS::AmbientRenderLight(CS::Colour::k_red), std::vector<CS::DirectionalRenderLight>(), std::vector<CS::PointRenderLight>(), renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 1, "Unexpected number of passes in the Scene CameraRenderPassGroup group");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 1, "Unexpected number of passes in the Skybox CameraRenderPassGroup group. Only expecting Skybox pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain a Skybox object");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected size of Transparent CameraRenderPassGroup group. Expecting only Transparent pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the Transparent pass, should contain 0 RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");
                    
                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent, opaque renderObject, ambient and directional light is setup correctly
            ///
            CSIT_TEST(SuccessMultipleRenderObjectTypes)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::AmbientRenderLight ambientLight(CS::Colour::k_red);
                CS::DirectionalRenderLight directionalLight(CS::Colour::k_red, CS::Vector3::k_unitNegativeZ);
                
                auto transparentMaterialGroup = CreateUnlitTransparentMaterialGroup();
                auto opaqueMaterialGroup = CreateLitOpaqueMaterialGroup();
                
                auto transparentObject = CreateStandardRenderObject(transparentMaterialGroup, k_onScreenObjectPosition);
                auto opaqueObject = CreateStandardRenderObject(opaqueMaterialGroup, k_onScreenObjectPosition);
                
                std::vector<CS::DirectionalRenderLight> directionalLights { directionalLight };
                std::vector<CS::PointRenderLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { transparentObject, opaqueObject };
                
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 2, "Unexpected size of CameraRenderPassGroup group. Expecting only base and directional passes");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetLightType() == CS::RenderPass::LightType::k_directional, "Unexpected light type of second pass, should contain directional light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the first pass, should contain 1 opaque RenderObject");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[1].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the second pass, should contain 1 directional RenderObject");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected number of render passes in the Transparent CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 1, "Unexpected number of objects in the Transparent pass, should contain 1 RenderObject");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");

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
                CS::AmbientRenderLight ambientLight(CS::Colour::k_red);
                
                auto materialGroup = CreateLitOpaqueMaterialGroup();
                
                std::vector<CS::DirectionalRenderLight> directionalLights;
                std::vector<CS::PointRenderLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CreateStandardRenderObject(materialGroup, k_offScreenObjectPosition) };
                
                CS::RenderFrame renderFrame(nullptr, k_resolution, CS::Colour::k_black, renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
                auto taskScheduler = CS::Application::Get()->GetTaskScheduler();
                taskScheduler->ScheduleTask(CS::TaskType::k_small, [=](const CS::TaskContext& taskContext)
                {
                    CS::ForwardRenderPassCompiler renderCompiler;
                    std::vector<CS::RenderFrame> renderFrames = {std::move(renderFrame)};
                    auto renderPassGroups = renderCompiler.CompileTargetRenderPassGroups(taskContext, std::move(renderFrames));
                    
                    CSIT_ASSERT(renderPassGroups.size() == 1, "Unexpected size of TargetRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups().size() == 4, "Unexpected size of CameraRenderPassGroup group.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses().size() == 1, "Unexpected size of CameraRenderPassGroup group. Expecting only base");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetLightType() == CS::RenderPass::LightType::k_ambient, "Unexpected light type of first pass, should contain ambient light");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[0].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the first pass, should contain 0 opaque RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[1].GetRenderPasses().size() == 0, "Unexpected number of render passes in the Skybox CameraRenderPassGroup.");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses().size() == 1, "Unexpected size of Transparent CameraRenderPassGroup group. Expecting only Transparent pass");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[2].GetRenderPasses()[0].GetRenderPassObjects().size() == 0, "Unexpected number of objects in the Transparent pass, should contain 0 RenderObjects");
                    CSIT_ASSERT(renderPassGroups[0].GetRenderCameraGroups()[3].GetRenderPasses().size() == 0, "Unexpected number of render passes in the UI CameraRenderPassGroup.");

                    DestroyMaterialGroup(materialGroup);
                    
                    CSIT_PASS();
                });
            }
        }
    }
}
