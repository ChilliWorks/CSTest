/*
 *  ImageResourceProvider.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 18/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Image/ImageResourceProvider.h>

namespace ChilliSource
{
    namespace Core
    {
        Core::CImage::Format IImageResourceProvider::meDefaultFormat = Core::CImage::Format::k_RGBA8888;
        //--------------------------------------------------
        /// Set Default Format
        ///
        /// @param Image format that will be used in lieu
        /// of specific
        //--------------------------------------------------
        void IImageResourceProvider::SetDefaultFormat(Core::CImage::Format ineFormat)
        {
            meDefaultFormat = ineFormat;
        }
    }
}
