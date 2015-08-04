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

#include <Common/Rendering/ModelFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Resource.h>
#include <ChilliSource/Core/Threading.h>
#include <ChilliSource/Rendering/Base.h>
#include <ChilliSource/Rendering/Model.h>

namespace CSTest
{
    namespace Common
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// A struct containing the contents of a single vertex.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct Vertex
            {
                CSCore::Vector4 m_position;
                CSCore::Vector2 m_texCoord;
                CSCore::Vector3 m_normal;
            };
            //------------------------------------------------------------------------------
            /// Creates a new mesh description describing a plane model.
            ///
            /// @author Ian Copland
            ///
            /// @param in_size - The size of the model.
            ///
            /// @return The new mesh descriptor.
            //------------------------------------------------------------------------------
            CSRendering::MeshDescriptor CreatePlaneDesc(const CSCore::Vector2& in_size)
            {
                CSCore::Vector2 halfSize = in_size * 0.5f;
                
                //Setup the mesh description
                CSRendering::MeshDescriptor meshDescriptor;
                meshDescriptor.mFeatures.mbHasMaterial = false;
                meshDescriptor.mFeatures.mbHasAnimationData = false;
                meshDescriptor.mudwIndexSize = sizeof(u16);
                
                //Setup vertex description
                CSRendering::VertexElement vertexDec[3];
                vertexDec[0].eType = CSRendering::VertexDataType::k_float4;
                vertexDec[0].eSemantic = CSRendering::VertexDataSemantic::k_position;
                vertexDec[1].eType = CSRendering::VertexDataType::k_float2;
                vertexDec[1].eSemantic = CSRendering::VertexDataSemantic::k_uv;
                vertexDec[2].eType = CSRendering::VertexDataType::k_float3;
                vertexDec[2].eSemantic = CSRendering::VertexDataSemantic::k_normal;
                meshDescriptor.mVertexDeclaration = CSRendering::VertexDeclaration(3, vertexDec);
                
                //setup sub mesh
                CSRendering::SubMeshDescriptor subMesh;
                subMesh.mvMinBounds = CSCore::Vector3(-halfSize.x, 0.0f, -halfSize.y);
                subMesh.mvMaxBounds = CSCore::Vector3(halfSize.x, 0.0f, halfSize.y);
                subMesh.mstrName = "SubMesh";
                subMesh.mudwNumVertices = 4;
                subMesh.mudwNumIndices = 6;
                subMesh.ePrimitiveType = CSRendering::PrimitiveType::k_tri;
                
                //Vertices
                Vertex* vertices = new Vertex[subMesh.mudwNumVertices];
                subMesh.mpVertexData = reinterpret_cast<u8*>(vertices);
                
                //front
                vertices[0] = { CSCore::Vector4(-halfSize.x, 0.0f, -halfSize.y, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(0.0f, 1.0f, 0.0f) };
                vertices[1] = { CSCore::Vector4(-halfSize.x, 0.0f, halfSize.y, 1.0f), CSCore::Vector2(0.0f, 1.0f), CSCore::Vector3(0.0f, 1.0f, 0.0f) };
                vertices[2] = { CSCore::Vector4(halfSize.x, 0.0f, -halfSize.y, 1.0f), CSCore::Vector2(1.0f, 0.0f), CSCore::Vector3(0.0f, 1.0f, 0.0f) };
                vertices[3] = { CSCore::Vector4(halfSize.x, 0.0f, halfSize.y, 1.0f), CSCore::Vector2(1.0f, 1.0f), CSCore::Vector3(0.0f, 1.0f, 0.0f) };
                
                //Indices
                u16 *indices(new u16[subMesh.mudwNumIndices]);
                subMesh.mpIndexData = reinterpret_cast<u8*>(indices);
                
                //front
                indices[0] = 0;
                indices[1] = 2;
                indices[2] = 1;
                indices[3] = 1;
                indices[4] = 2;
                indices[5] = 3;
                
                //Create the mesh.
                meshDescriptor.mMeshes.push_back(subMesh);
                meshDescriptor.mvMinBounds = CSCore::Vector3(-halfSize.x, 0.0f, -halfSize.y);
                meshDescriptor.mvMaxBounds = CSCore::Vector3(halfSize.x, 0.0f, halfSize.y);
                
                return meshDescriptor;
            }
            //------------------------------------------------------------------------------
            /// Creates a new mesh description describing a box model.
            ///
            /// @author Ian Copland
            ///
            /// @param in_size - The size of the model.
            /// @param in_textureRepeat - The number of times the texture is repeated on
            /// each face.
            /// @param in_flipNormals - Whether or not to flip the box inside out.
            /// This is useful for creating rooms
            ///
            /// @return The new mesh descriptor.
            //------------------------------------------------------------------------------
            CSRendering::MeshDescriptor CreateBoxDesc(const CSCore::Vector3& in_size, const CSCore::Vector2& in_textureRepeat, bool in_flipNormals)
            {
                CSCore::Vector3 halfSize = in_size * 0.5f;
                
                //Setup the mesh description
                CSRendering::MeshDescriptor meshDescriptor;
                meshDescriptor.mFeatures.mbHasMaterial = true;
                meshDescriptor.mFeatures.mbHasAnimationData = false;
                meshDescriptor.mudwIndexSize = sizeof(u16);
                
                //Setup vertex description
                CSRendering::VertexElement vertexDec[3];
                vertexDec[0].eType = CSRendering::VertexDataType::k_float4;
                vertexDec[0].eSemantic = CSRendering::VertexDataSemantic::k_position;
                vertexDec[1].eType = CSRendering::VertexDataType::k_float2;
                vertexDec[1].eSemantic = CSRendering::VertexDataSemantic::k_uv;
                vertexDec[2].eType = CSRendering::VertexDataType::k_float3;
                vertexDec[2].eSemantic = CSRendering::VertexDataSemantic::k_normal;
                meshDescriptor.mVertexDeclaration = CSRendering::VertexDeclaration(3, vertexDec);
                
                //setup sub mesh
                CSRendering::SubMeshDescriptor subMesh;
                subMesh.mvMinBounds = CSCore::Vector3(-halfSize.x, -halfSize.y, -halfSize.z);
                subMesh.mvMaxBounds = CSCore::Vector3(halfSize.x, halfSize.y, halfSize.z);
                subMesh.mstrName = "SubMesh";
                subMesh.mudwNumVertices = 24;
                subMesh.mudwNumIndices = 36;
                subMesh.ePrimitiveType = CSRendering::PrimitiveType::k_tri;
                
                //Vertices
                Vertex* vertices = new Vertex[subMesh.mudwNumVertices];
                subMesh.mpVertexData = reinterpret_cast<u8*>(vertices);
                
                float normalDir = (in_flipNormals == false) ? 1.0f : -1.0f;
                
                //bottom
                vertices[0] = { CSCore::Vector4(-halfSize.x, -halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(0.0f, -normalDir, 0.0f) };
                vertices[1] = { CSCore::Vector4(-halfSize.x, -halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(0.0f, in_textureRepeat.y), CSCore::Vector3(0.0f, -normalDir, 0.0f) };
                vertices[2] = { CSCore::Vector4(halfSize.x, -halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, 0.0f), CSCore::Vector3(0.0f, -normalDir, 0.0f) };
                vertices[3] = { CSCore::Vector4(halfSize.x, -halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, in_textureRepeat.y), CSCore::Vector3(0.0f, -normalDir, 0.0f) };
                
                //top
                vertices[4] = { CSCore::Vector4(halfSize.x, halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(0.0f, normalDir, 0.0f) };
                vertices[5] = { CSCore::Vector4(halfSize.x, halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(0.0f, in_textureRepeat.y), CSCore::Vector3(0.0f, normalDir, 0.0f) };
                vertices[6] = { CSCore::Vector4(-halfSize.x, halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, 0.0f), CSCore::Vector3(0.0f, normalDir, 0.0f) };
                vertices[7] = { CSCore::Vector4(-halfSize.x, halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, in_textureRepeat.y), CSCore::Vector3(0.0f, normalDir, 0.0f) };
                
                //left
                vertices[8] = { CSCore::Vector4(-halfSize.x, halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(-normalDir, 0.0f, 0.0f) };
                vertices[9] = { CSCore::Vector4(-halfSize.x, halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(0.0f, in_textureRepeat.y), CSCore::Vector3(-normalDir, 0.0f, 0.0f) };
                vertices[10] = { CSCore::Vector4(-halfSize.x, -halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, 0.0f), CSCore::Vector3(-normalDir, 0.0f, 0.0f) };
                vertices[11] = { CSCore::Vector4(-halfSize.x, -halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, in_textureRepeat.y), CSCore::Vector3(-normalDir, 0.0f, 0.0f) };
                
                //right
                vertices[12] = { CSCore::Vector4(halfSize.x, -halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(normalDir, 0.0f, 0.0f) };
                vertices[13] = { CSCore::Vector4(halfSize.x, -halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(0.0f, in_textureRepeat.y), CSCore::Vector3(normalDir, 0.0f, 0.0f) };
                vertices[14] = { CSCore::Vector4(halfSize.x, halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, 0.0f), CSCore::Vector3(normalDir, 0.0f, 0.0f) };
                vertices[15] = { CSCore::Vector4(halfSize.x, halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, in_textureRepeat.y), CSCore::Vector3(normalDir, 0.0f, 0.0f) };
                
                //front
                vertices[16] = { CSCore::Vector4(-halfSize.x, halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(0.0f, 0.0f, -normalDir) };
                vertices[17] = { CSCore::Vector4(-halfSize.x, -halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(0.0f, in_textureRepeat.y), CSCore::Vector3(0.0f, 0.0f, -normalDir) };
                vertices[18] = { CSCore::Vector4(halfSize.x, halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, 0.0f), CSCore::Vector3(0.0f, 0.0f, -normalDir) };
                vertices[19] = { CSCore::Vector4(halfSize.x, -halfSize.y, -halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, in_textureRepeat.y), CSCore::Vector3(0.0f, 0.0f, -normalDir) };
                
                //back
                vertices[20] = { CSCore::Vector4(-halfSize.x, -halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(0.0f, 0.0f), CSCore::Vector3(0.0f, 0.0f, normalDir) };
                vertices[21] = { CSCore::Vector4(-halfSize.x, halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(0.0f, in_textureRepeat.y), CSCore::Vector3(0.0f, 0.0f, normalDir) };
                vertices[22] = { CSCore::Vector4(halfSize.x, -halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, 0.0f), CSCore::Vector3(0.0f, 0.0f, normalDir) };
                vertices[23] = { CSCore::Vector4(halfSize.x, halfSize.y, halfSize.z, 1.0f), CSCore::Vector2(in_textureRepeat.x, in_textureRepeat.y), CSCore::Vector3(0.0f, 0.0f, normalDir) };
                
                //Indices
                u16 *indices(new u16[subMesh.mudwNumIndices]);
                subMesh.mpIndexData = reinterpret_cast<u8*>(indices);
                
                //bottom
                indices[0] = 0;
                indices[1] = 1;
                indices[2] = 2;
                indices[3] = 1;
                indices[4] = 3;
                indices[5] = 2;
                
                //top
                indices[6] = 4;
                indices[7] = 5;
                indices[8] = 6;
                indices[9] = 5;
                indices[10] = 7;
                indices[11] = 6;
                
                //left
                indices[12] = 8;
                indices[13] = 9;
                indices[14] = 10;
                indices[15] = 9;
                indices[16] = 11;
                indices[17] = 10;
                
                //right
                indices[18] = 12;
                indices[19] = 13;
                indices[20] = 14;
                indices[21] = 13;
                indices[22] = 15;
                indices[23] = 14;
                
                //front
                indices[24] = 16;
                indices[25] = 17;
                indices[26] = 18;
                indices[27] = 17;
                indices[28] = 19;
                indices[29] = 18;
                
                //back
                indices[30] = 20;
                indices[31] = 21;
                indices[32] = 22;
                indices[33] = 21;
                indices[34] = 23;
                indices[35] = 22;
                
                if (in_flipNormals == true)
                {
                    for (u32 i = 0; i < subMesh.mudwNumIndices; i += 3)
                    {
                        u16 temp = indices[i + 1];
                        indices[i + 1] = indices[i + 2];
                        indices[i + 2] = temp;
                    }
                }
                
                //Create the mesh.
                meshDescriptor.mMeshes.push_back(subMesh);
                meshDescriptor.mvMinBounds = CSCore::Vector3(-halfSize.x, -halfSize.y, -halfSize.z);
                meshDescriptor.mvMaxBounds = CSCore::Vector3(halfSize.x, halfSize.y, halfSize.z);
                
                return meshDescriptor;
            }
            //------------------------------------------------------------------------------
            /// Deletes all heap memory associated with the given mesh descriptor.
            ///
            /// @author Ian Copland
            ///
            /// @param in_desc - The mesh descriptor which should have its contents cleaned
            /// up.
            //------------------------------------------------------------------------------
            void CleanupDesc(CSRendering::MeshDescriptor& in_desc)
            {
                //cleanup.
                for (u32 i = 0; i < in_desc.mMeshes.size(); i++)
                {
                    delete[] reinterpret_cast<Vertex*>(in_desc.mMeshes[i].mpVertexData);
                    delete[] reinterpret_cast<u16*>(in_desc.mMeshes[i].mpIndexData);
                }
            }
        }
        
        CS_DEFINE_NAMEDTYPE(ModelFactory);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ModelFactoryUPtr ModelFactory::Create()
        {
            return ModelFactoryUPtr(new ModelFactory());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ModelFactory::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (ModelFactory::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSRendering::MeshCSPtr ModelFactory::CreatePlane(const CSCore::Vector2& in_size) const
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create models on a background thread.");
            
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto meshName = "MF_Plane(" + CSCore::ToString(in_size) + ")";
            auto mesh = resourcePool->GetResource<CSRendering::Mesh>(meshName);
            
            if (mesh == nullptr)
            {
                auto mutableMesh = resourcePool->CreateResource<CSRendering::Mesh>(meshName);
                
                auto desc = CreatePlaneDesc(in_size);
                mutableMesh->Build(desc);
                CleanupDesc(desc);
                mutableMesh->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                
                mesh = mutableMesh;
            }
            
            return mesh;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSRendering::MeshCSPtr ModelFactory::CreateBox(const CSCore::Vector3& in_size, const CSCore::Vector2& in_textureRepeat, bool in_flipNormals) const
        {
            CS_ASSERT(CSCore::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot create models on a background thread.");
            
            auto resourcePool = CSCore::Application::Get()->GetResourcePool();
            auto meshName = "MF_Box(" + CSCore::ToString(in_size) + "|" + CSCore::ToString(in_textureRepeat) + "|" + CSCore::ToString(in_flipNormals) + ")";
            auto mesh = resourcePool->GetResource<CSRendering::Mesh>(meshName);
            
            if (mesh == nullptr)
            {
                auto mutableMesh = resourcePool->CreateResource<CSRendering::Mesh>(meshName);
                
                auto desc = CreateBoxDesc(in_size, in_textureRepeat, in_flipNormals);
                mutableMesh->Build(desc);
                CleanupDesc(desc);
                mutableMesh->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                
                mesh = mutableMesh;
            }
            
            return mesh;
        }
    }
}