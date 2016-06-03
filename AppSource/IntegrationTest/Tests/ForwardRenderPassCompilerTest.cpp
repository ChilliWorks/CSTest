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
            
            const CS::Vector3 k_cameraLookAt = CS::Vector3::k_zero;
            const CS::Matrix4 k_onScreenObjectTransform = CS::Matrix4::CreateTransform(k_cameraLookAt, CS::Vector3::k_one, CS::Quaternion::k_identity);
            const CS::Matrix4 k_offScreenObjectTransform = CS::Matrix4::CreateTransform(k_cameraLookAt - (2 * k_cameraDistanceFromLookAt * CS::Vector3(0.0f, 0.0f, 1.0f)), CS::Vector3::k_one, CS::Quaternion::k_identity);
            
            struct Vertex
            {
                CS::Vector4 m_position;
                CS::Vector3 m_normal;
                CS::Vector2 m_texCoord;
            };
            
            /// Creates an render camera
            ///
            CS::RenderCamera CreateRenderCamera()
            {
                auto basicEntityFactory = CS::Application::Get()->GetSystem<Common::BasicEntityFactory>();
                CS::EntitySPtr cameraTarget = CS::Entity::Create();
                cameraTarget->GetTransform().SetPosition(k_cameraLookAt);
                const auto cameraEntity = basicEntityFactory->CreateThirdPersonCamera(cameraTarget, CS::Vector3::k_zero, k_cameraDistanceFromLookAt, 0.0f, 0.0f, 0.0f);
                const auto& cameraComponent = cameraEntity->GetComponent<CS::CameraComponent>();
                const auto screenSystem = CS::Application::Get()->GetScreen();
                CS::Integer2 dimensions(s32(screenSystem->GetResolution().x), s32(screenSystem->GetResolution().y));
                return CS::RenderCamera(dimensions, cameraEntity->GetTransform().GetWorldTransform(), cameraComponent->GetProjection());
            }

            /// Creates an lit opaque material group
            ///
            /// @return RenderMaterialGroup
            ///
            const CS::RenderMaterialGroup* CreateLitOpaqueMaterialGroup()
            {
                auto renderMaterialGroupManager = CS::Application::Get()->GetSystem<CS::RenderMaterialGroupManager>();
                auto renderTextureGroupManager = CS::Application::Get()->GetSystem<CS::RenderTextureManager>();
                
                constexpr u32 textureSize = 16;
                u8* textureData = new u8[textureSize];
                memset(textureData, 0, textureSize);
                std::unique_ptr<const u8 []> uniqueTextureData(textureData);
                auto renderTexture = renderTextureGroupManager->CreateRenderTexture(std::move(uniqueTextureData), textureSize, CS::Integer2(4, 4), CS::ImageFormat::k_Lum8, CS::ImageCompression::k_none, CS::TextureFilterMode::k_nearest, CS::TextureWrapMode::k_clamp, CS::TextureWrapMode::k_clamp, false);
                return renderMaterialGroupManager->CreateBlinnRenderMaterialGroup(renderTexture, CS::Colour::k_white, CS::Colour::k_white, CS::Colour::k_white, CS::Colour::k_white);
            }
            
            /// Creates an unlit transparent material group
            ///
            /// @return RenderMaterialGroup
            ///
            const CS::RenderMaterialGroup* CreateUnlitTransparentMaterialGroup()
            {
                auto renderMaterialGroupManager = CS::Application::Get()->GetSystem<CS::RenderMaterialGroupManager>();
                auto renderTextureGroupManager = CS::Application::Get()->GetSystem<CS::RenderTextureManager>();
                
                constexpr u32 textureSize = 16;
                u8* textureData = new u8[textureSize];
                memset(textureData, 0, textureSize);
                std::unique_ptr<const u8 []> uniqueTextureData(textureData);
                auto renderTexture = renderTextureGroupManager->CreateRenderTexture(std::move(uniqueTextureData), textureSize, CS::Integer2(4, 4), CS::ImageFormat::k_Lum8, CS::ImageCompression::k_none, CS::TextureFilterMode::k_nearest, CS::TextureWrapMode::k_clamp, CS::TextureWrapMode::k_clamp, false);
                return renderMaterialGroupManager->CreateUnlitRenderMaterialGroup(renderTexture, true, true, true, true, false, CS::BlendMode::k_one, CS::BlendMode::k_oneMinusSourceAlpha, CS::CullFace::k_back, CS::Colour::k_white, CS::Colour::k_white);
            }
            
            /// Creates a simple render mesh
            ///
            /// @param boundingSpherePosition
            ///     The position to create the bounding sphere for this mesh
            /// @return RenderMesh
            ///
            const CS::RenderMesh* CreateRenderMesh(const CS::Vector3& boundingSpherePosition)
            {
                constexpr u32 k_numVertices = 3;
                constexpr u32 k_numIndices = 3;
                
                auto renderResourceManager = CS::Application::Get()->GetSystem<CS::RenderMeshManager>();
                CS::Sphere boundingSphere(boundingSpherePosition, 1.0f);
                
                Vertex* vertices = new Vertex[k_numVertices];
                vertices[0] = { CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f), CS::Vector3::k_one, CS::Vector2::k_zero };
                vertices[1] = { CS::Vector4(1.0f, 0.0f, 0.0f, 0.0f), CS::Vector3::k_one, CS::Vector2::k_zero };
                vertices[2] = { CS::Vector4(1.0f, 1.0f, 0.0f, 0.0f), CS::Vector3::k_one, CS::Vector2::k_one };
                
                u16 *indices(new u16[k_numIndices]);
                indices[0] = 0;
                indices[1] = 1;
                indices[2] = 2;
                
                std::unique_ptr<const u8 []> vertexData(reinterpret_cast<u8*>(vertices));
                std::unique_ptr<const u8 []> indexData(reinterpret_cast<u8*>(indices));
                
                return renderResourceManager->CreateRenderMesh(CS::PolygonType::k_short, ChilliSource::VertexFormat::k_staticMesh, CS::IndexType(0), 3, 3, boundingSphere, std::move(vertexData), k_numVertices, std::move(indexData), k_numIndices);
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
                
                std::vector<CS::RenderDirectionalLight> directionalLights;
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CS::RenderObject(CreateLitOpaqueMaterialGroup(), CreateRenderMesh(k_onScreenObjectTransform.GetTranslation()), k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
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
                    CSIT_PASS();
                });
            }
            
            /// Validates that a RenderFrame with a transparent renderObject and ambient light is setup correctly
            ///
            CSIT_TEST(SuccessAmbientUnilitTransparent)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                
                std::vector<CS::RenderDirectionalLight> directionalLights;
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CS::RenderObject(CreateUnlitTransparentMaterialGroup(), CreateRenderMesh(k_onScreenObjectTransform.GetTranslation()), k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
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
                
                std::vector<CS::RenderDirectionalLight> directionalLights { directionalLight };
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CS::RenderObject(CreateLitOpaqueMaterialGroup(), CreateRenderMesh(k_onScreenObjectTransform.GetTranslation()), k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
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
                
                std::vector<CS::RenderDirectionalLight> directionalLights { directionalLight };
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CS::RenderObject(CreateUnlitTransparentMaterialGroup(), CreateRenderMesh(k_onScreenObjectTransform.GetTranslation()), k_onScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
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
                CS::RenderObject transparentObject(CreateUnlitTransparentMaterialGroup(), CreateRenderMesh(k_onScreenObjectTransform.GetTranslation()), k_onScreenObjectTransform);
                CS::RenderObject opaqueObject(CreateLitOpaqueMaterialGroup(), CreateRenderMesh(k_onScreenObjectTransform.GetTranslation()), k_onScreenObjectTransform);
                
                std::vector<CS::RenderDirectionalLight> directionalLights { directionalLight };
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { transparentObject, opaqueObject };
                
                CS::RenderFrame renderFrame(renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
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
                    CSIT_PASS();
                });
            }
            
            /// Validates that offscreen objects are culled
            ///
            CSIT_TEST(SuccessFrustrumCull)
            {
                CS::RenderCamera renderCamera = CreateRenderCamera();
                CS::RenderAmbientLight ambientLight(CS::Colour::k_red);
                
                std::vector<CS::RenderDirectionalLight> directionalLights;
                std::vector<CS::RenderPointLight> pointLights;
                std::vector<CS::RenderObject> renderObjects { CS::RenderObject(CreateLitOpaqueMaterialGroup(), CreateRenderMesh(k_offScreenObjectTransform.GetTranslation()), k_offScreenObjectTransform) };
                
                CS::RenderFrame renderFrame(renderCamera, ambientLight, directionalLights, pointLights, renderObjects);
                
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
                    CSIT_PASS();
                });
            }
        }
    }
}
