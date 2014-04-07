//
//  ResourcePool.cpp
//  Chilli Source
//
//  Created by Scott Downie on 03/04/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Core/Resource/ResourcePool.h>

#include <ChilliSource/Core/Resource/ResourceProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(ResourcePool);
        
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        ResourcePoolUPtr ResourcePool::Create()
        {
            return ResourcePoolUPtr(new ResourcePool());
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        bool ResourcePool::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ResourcePool::InterfaceID;
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        void ResourcePool::AddProvider(ResourceProvider* in_provider)
        {
            CS_ASSERT(in_provider != nullptr, "Cannot add null resource provider to pool");
            auto itDescriptor = m_descriptors.find(in_provider->GetResourceType());
            
            if(itDescriptor == m_descriptors.end())
            {
                PoolDesc desc;
                desc.m_providers.push_back(in_provider);
                m_descriptors.insert(std::make_pair(in_provider->GetResourceType(), desc));
            }
            else
            {
                itDescriptor->second.m_providers.push_back(in_provider);
            }
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        ResourceProvider* ResourcePool::FindProvider(const std::string& in_filePath, const PoolDesc& in_desc)
        {
            //Find the resource provider that can handle this extension
            std::string fileName;
            std::string fileExtension;
            Core::StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
            
            ResourceProvider* provider = nullptr;
            u32 numProviders = in_desc.m_providers.size();
            for(u32 i=0; i<numProviders; ++i)
            {
                provider = in_desc.m_providers[i];
                if(provider->CanCreateResourceWithFileExtension(fileExtension))
                {
                    break;
                }
            }
            
            if(provider == nullptr)
            {
                CS_LOG_ERROR("Failed to find resource provider for extension " + fileExtension);
            }
            
            return provider;
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        ResourcePool::ResourceId ResourcePool::GenerateResourceId(StorageLocation in_location, const std::string& in_filePath) const
        {
            std::string combined(ToString((u32)in_location) + in_filePath);
            return HashCRC32::GenerateHashCode(combined);
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        ResourcePool::ResourceId ResourcePool::GenerateResourceId(const std::string& in_uniqueId) const
        {
            return HashCRC32::GenerateHashCode(in_uniqueId);
        }
        //-------------------------------------------------------------------------------------
        /// Resources often have references to other resources and therefore multiple release passes
        /// are required until no more resources are released
        //-------------------------------------------------------------------------------------
        void ResourcePool::ReleaseAllUnused()
        {
            u32 numReleased = 0;
            
            do
            {
                numReleased = 0;
                
                for(auto& descEntry : m_descriptors)
                {
                    for(auto itResource = descEntry.second.m_cachedResources.begin(); itResource != descEntry.second.m_cachedResources.end(); /*NO INCREMENT*/)
                    {
                        if(itResource->second.use_count() == 1)
                        {
                            //The pool is the sole owner so we can safely release the object
                            CS_LOG_DEBUG("Releasing resource from pool " + itResource->second->GetFilePath());
                            itResource = descEntry.second.m_cachedResources.erase(itResource);
                            numReleased++;
                        }
                        else
                        {
                            ++itResource;
                        }
                    }
                }
            }
            while(numReleased > 0);
        }
        //-------------------------------------------------------------------------------------
        //-------------------------------------------------------------------------------------
        void ResourcePool::Release(const Resource* in_resource)
        {
            CS_ASSERT(in_resource != nullptr, "Pool cannot release raw resource");
            //Find the descriptor that handles this type of resource
            auto itDescriptor = m_descriptors.find(in_resource->GetInterfaceID());
            CS_ASSERT(itDescriptor != m_descriptors.end(), "Failed to find resource provider for " + in_resource->GetInterfaceTypeName());
            
            auto& cachedResources(itDescriptor->second.m_cachedResources);
            
            for(auto itResource = cachedResources.begin(); itResource != cachedResources.end(); /*NO INCREMENT*/)
            {
                if(itResource->second.get() == in_resource)
                {
					ResourceSPtr& resource = itResource->second;
					CS_ASSERT((resource.use_count() <= 1), "Cannot release a resource if it is owned by another object (i.e. use_count > 0) : (" + resource->GetFilePath() + ")");
					CS_LOG_DEBUG("Releasing resource from pool " + resource->GetFilePath());
					cachedResources.erase(itResource);
                    return;
                }
            }
        }
        //------------------------------------------------------------------------------------
        /// At this stage in the app lifecycle all app and system references to resource
        /// should have been released. If the resource pool still has resources then this
        /// indicated leaks.
        //------------------------------------------------------------------------------------
        void ResourcePool::Destroy()
        {
            ReleaseAllUnused();
            
            bool error = false;
            
            for(auto& descEntry : m_descriptors)
            {
                for(auto itResource = descEntry.second.m_cachedResources.begin(); itResource != descEntry.second.m_cachedResources.end(); /*NO INCREMENT*/)
                {
                    //The pool is the sole owner so we can safely release the object
                    CS_LOG_ERROR("Resource still in user: " + itResource->second->GetFilePath());
                }
            }
            
            if(error == true)
            {
                CS_LOG_FATAL("Resources are still in use. Indicates that there is leaky references");
            }
        }
        //------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------
        void ResourcePool::OnMemoryWarning()
        {
            ReleaseAllUnused();
        }
    }
}