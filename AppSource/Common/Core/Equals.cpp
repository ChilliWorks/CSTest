//
//  Equals.cpp
//  CSTest
//  Created by Ian Copland on 30/07/2015.
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
#ifndef _COMMON_CORE_EQUALS_H_
#define _COMMON_CORE_EQUALS_H_

#include <CSTest.h>

#include <ChilliSource/Core/Math.h>

namespace CSTest
{
    namespace Common
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool Equals(f32 in_a, f32 in_b)
        {
            const f32 k_tolerance = std::numeric_limits<f32>::epsilon() * 100.0f;
            f32 difference = in_a - in_b;
            return (difference < k_tolerance && difference > -k_tolerance);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool Equals(const CSCore::Vector2& in_a, const CSCore::Vector2& in_b)
        {
            return (Equals(in_a.x, in_b.x) && Equals(in_a.y, in_b.y));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool Equals(const CSCore::Vector3& in_a, const CSCore::Vector3& in_b)
        {
            return (Equals(in_a.x, in_b.x) && Equals(in_a.y, in_b.y) && Equals(in_a.z, in_b.z));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool Equals(const CSCore::Vector4& in_a, const CSCore::Vector4& in_b)
        {
            return (Equals(in_a.x, in_b.x) && Equals(in_a.y, in_b.y) && Equals(in_a.z, in_b.z) && Equals(in_a.w, in_b.w));
        }
    }
}

#endif