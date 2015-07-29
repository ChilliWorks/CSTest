//
//  MathVectors.cpp
//  CSTest
//  Created by Ian Copland on 29/07/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <CSTest.h>

#include <ChilliSource/Core/Math.h>

#include <catch.hpp>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_a - A floating point value.
            /// @param in_b - Another floating point value.
            ///
            /// @return Whether or not the two floats are effectively equal. This allows for
            /// a very small small delta to avoid floating point precision issues.
            //------------------------------------------------------------------------------
            bool Equals(f32 in_a, f32 in_b)
            {
                const f32 k_tolerance = std::numeric_limits<f32>::epsilon() * 100.0f;
                f32 difference = in_a - in_b;
                return (difference < k_tolerance && difference > -k_tolerance);
            }
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_a - A vector.
            /// @param in_b - Another vector.
            ///
            /// @return Whether or not the two vectors are effectively equal. This allows for
            /// a very small small delta to avoid floating point precision issues.
            //------------------------------------------------------------------------------
            bool Equals(CSCore::Vector2 in_a, CSCore::Vector2 in_b)
            {
                return (Equals(in_a.x, in_b.x) && Equals(in_a.y, in_b.y));
            }
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_a - A vector.
            /// @param in_b - Another vector.
            ///
            /// @return Whether or not the two vectors are effectively equal. This allows for
            /// a very small small delta to avoid floating point precision issues.
            //------------------------------------------------------------------------------
            bool Equals(CSCore::Vector3 in_a, CSCore::Vector3 in_b)
            {
                return (Equals(in_a.x, in_b.x) && Equals(in_a.y, in_b.y) && Equals(in_a.z, in_b.z));
            }
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param in_a - A vector.
            /// @param in_b - Another vector.
            ///
            /// @return Whether or not the two vectors are effectively equal. This allows for
            /// a very small small delta to avoid floating point precision issues.
            //------------------------------------------------------------------------------
            bool Equals(CSCore::Vector4 in_a, CSCore::Vector4 in_b)
            {
                return (Equals(in_a.x, in_b.x) && Equals(in_a.y, in_b.y) && Equals(in_a.z, in_b.z) && Equals(in_a.w, in_b.w));
            }
        }
        
        TEST_CASE("Vector2: Construction and component validation", "[Vector2]")
        {
            CSCore::Vector2 a;
            CSCore::Vector2 b(1.0f, 2.0f);
            
            REQUIRE(Equals(a.x, 0.0f));
            REQUIRE(Equals(a.y, 0.0f));
            REQUIRE(Equals(b.x, 1.0f));
            REQUIRE(Equals(b.y, 2.0f));
        }
        
        TEST_CASE("Vector2: Length", "[Vector2]")
        {
            CSCore::Vector2 a(3.0f, 4.0f);
            CSCore::Vector2 b(0.0f, 0.0f);
            
            REQUIRE(Equals(a.Length(), 5.0f));
            REQUIRE(Equals(a.LengthSquared(), 25.0f));
            REQUIRE(Equals(b.Length(), 0.0f));
            REQUIRE(Equals(b.LengthSquared(), 0.0f));
        }
        
        TEST_CASE("Vector2: Normalise", "[Vector2]")
        {
            CSCore::Vector2 a(5.0f, 0.0f);
            CSCore::Vector2 b(3.0f, 4.0f);
            CSCore::Vector2 c(0.0f, 0.0f);
            CSCore::Vector2 d(5.0f, 0.0f);
            CSCore::Vector2 e(3.0f, 4.0f);
            CSCore::Vector2 f(0.0f, 0.0f);
            
            d.Normalise();
            e.Normalise();
            f.Normalise();
            
            REQUIRE(Equals(CSCore::Vector2::Normalise(a), CSCore::Vector2(1.0f, 0.0f)));
            REQUIRE(Equals(CSCore::Vector2::Normalise(b), CSCore::Vector2(0.6f, 0.8f)));
            REQUIRE(Equals(CSCore::Vector2::Normalise(c), CSCore::Vector2(0.0f, 0.0f)));
            REQUIRE(Equals(d, CSCore::Vector2(1.0f, 0.0f)));
            REQUIRE(Equals(e, CSCore::Vector2(0.6f, 0.8f)));
            REQUIRE(Equals(f, CSCore::Vector2(0.0f, 0.0f)));
        }
        
        TEST_CASE("Vector2: Inverse", "[Vector2]")
        {
            CSCore::Vector2 a(2.0f, 4.0f);
            CSCore::Vector2 b(0.0f, 0.0f);
            CSCore::Vector2 c(2.0f, 4.0f);
            CSCore::Vector2 d(0.0f, 0.0f);
            
            c.Inverse();
            d.Inverse();
            
            REQUIRE(Equals(CSCore::Vector2::Inverse(a), CSCore::Vector2(0.5f, 0.25f)));
            REQUIRE(Equals(CSCore::Vector2::Inverse(b), CSCore::Vector2(0.0f, 0.0f)));
            REQUIRE(Equals(c, CSCore::Vector2(0.5f, 0.25f)));
            REQUIRE(Equals(d, CSCore::Vector2(0.0f, 0.0f)));
        }
        
        TEST_CASE("Vector2: Absolute", "[Vector2]")
        {
            CSCore::Vector2 a(1.0f, 1.0f);
            CSCore::Vector2 b(1.0f, -1.0f);
            CSCore::Vector2 c(-1.0f, 1.0f);
            CSCore::Vector2 d(-1.0f, -1.0f);
            CSCore::Vector2 e(0.0f, 0.0f);
            CSCore::Vector2 f(1.0f, 1.0f);
            CSCore::Vector2 g(1.0f, -1.0f);
            CSCore::Vector2 h(-1.0f, 1.0f);
            CSCore::Vector2 i(-1.0f, -1.0f);
            CSCore::Vector2 j(0.0f, 0.0f);
            
            f.Abs();
            g.Abs();
            h.Abs();
            i.Abs();
            j.Abs();
            
            REQUIRE(Equals(CSCore::Vector2::Abs(a), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(b), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(c), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(d), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(e), CSCore::Vector2(0.0f, 0.0f)));
            REQUIRE(Equals(f, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(g, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(h, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(i, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(j, CSCore::Vector2(0.0f, 0.0f)));
        }
        
        TEST_CASE("Vector2: Absolute", "[Vector2]")
        {
            CSCore::Vector2 a(1.0f, 1.0f);
            CSCore::Vector2 b(1.0f, -1.0f);
            CSCore::Vector2 c(-1.0f, 1.0f);
            CSCore::Vector2 d(-1.0f, -1.0f);
            CSCore::Vector2 e(0.0f, 0.0f);
            CSCore::Vector2 f(1.0f, 1.0f);
            CSCore::Vector2 g(1.0f, -1.0f);
            CSCore::Vector2 h(-1.0f, 1.0f);
            CSCore::Vector2 i(-1.0f, -1.0f);
            CSCore::Vector2 j(0.0f, 0.0f);
            
            f.Abs();
            g.Abs();
            h.Abs();
            i.Abs();
            j.Abs();
            
            REQUIRE(Equals(CSCore::Vector2::Abs(a), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(b), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(c), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(d), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(e), CSCore::Vector2(0.0f, 0.0f)));
            REQUIRE(Equals(f, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(g, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(h, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(i, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(j, CSCore::Vector2(0.0f, 0.0f)));
        }
    }
}
