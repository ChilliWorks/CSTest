//
//  Vector4.cpp
//  CSTest
//  Created by Ian Copland on 31/07/2015.
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

#include <CSTest.h>

#include <Common/Core/Approx.h>

#include <ChilliSource/Core/Math.h>

#include <catch.hpp>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A series of test for the math Vector4 class.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        TEST_CASE("Vector4", "[Math]")
        {
            //------------------------------------------------------------------------------
            /// Confirms that a Vector4 can be successfully constructed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Construction and component validation")
            {
                CS::Vector4 a;
                CS::Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
                
                REQUIRE(Common::Approx(a.x, 0.0f));
                REQUIRE(Common::Approx(a.y, 0.0f));
                REQUIRE(Common::Approx(a.z, 0.0f));
                REQUIRE(Common::Approx(a.w, 0.0f));
                REQUIRE(Common::Approx(b.x, 1.0f));
                REQUIRE(Common::Approx(b.y, 2.0f));
                REQUIRE(Common::Approx(b.z, 3.0f));
                REQUIRE(Common::Approx(b.w, 4.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that basic arithmatic works correctly for Vector4.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Basic arithmatic")
            {
                CS::Vector4 a1(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a2(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a3(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a4(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a5(1.0f, 2.0f, 3.0f, 1.0f);
                CS::Vector4 a6(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a7(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 b1(2.0f, 3.0f, 4.0f, 5.0f);
                auto b2 = CS::Matrix4::CreateTransform(CS::Vector3(1.0f, 2.0f, 3.0f), CS::Vector3(1.0f, 2.0f, 3.0f), CS::Quaternion(CS::Vector3::Normalise(CS::Vector3(1.0f, 1.0f, 1.0f)), -0.5f));
                f32 b3 = 2.0f;
                
                REQUIRE(a1 == a2);
                REQUIRE(a1 != a5);
                REQUIRE(Common::Approx(-a1, CS::Vector4(-1.0f, -1.0f, -1.0f, -1.0f)));
                REQUIRE(Common::Approx(a1 + b1, CS::Vector4(3.0f, 4.0f, 5.0f, 6.0f)));
                REQUIRE(Common::Approx(a2 - b1, CS::Vector4(-1.0f, -2.0f, -3.0f, -4.0f)));
                REQUIRE(Common::Approx(a3 * b1, CS::Vector4(2.0f, 3.0f, 4.0f, 5.0f)));
                REQUIRE(Common::Approx(a4 / b1, CS::Vector4(0.5f, 1.0f / 3.0f, 0.25f, 0.2f)));
                REQUIRE(Common::Approx(a5 * b2, CS::Vector4(1.06488132f, 8.29598331f, 10.6391354f, 1.0f)));
                REQUIRE(Common::Approx(a6 * b3, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(b3 * a6, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a7 / b3, CS::Vector4(0.5f, 0.5f, 0.5f, 0.5f)));
                
                a1 += b1;
                a2 -= b1;
                a3 *= b1;
                a4 /= b1;
                a5 *= b2;
                a6 *= b3;
                a7 /= b3;
                
                REQUIRE(Common::Approx(a1, CS::Vector4(3.0f, 4.0f, 5.0f, 6.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector4(-1.0f, -2.0f, -3.0f, -4.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector4(2.0f, 3.0f, 4.0f, 5.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector4(0.5f, 1.0f / 3.0f, 0.25f, 0.2f)));
                REQUIRE(Common::Approx(a5, CS::Vector4(1.06488132f, 8.29598331f, 10.6391354f, 1.0f)));
                REQUIRE(Common::Approx(a6, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a7, CS::Vector4(0.5f, 0.5f, 0.5f, 0.5f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that vector length can be successfully calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Length")
            {
                CS::Vector4 a(1.0f, 4.0f, 8.0f, 12.0f);
                CS::Vector4 b(0.0f, 0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(a.Length(), 15.0f));
                REQUIRE(Common::Approx(b.Length(), 0.0f));
                REQUIRE(Common::Approx(a.LengthSquared(), 225.0f));
                REQUIRE(Common::Approx(b.LengthSquared(), 0.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms a vectorcan be successfully normalised.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Normalise")
            {
                CS::Vector4 a(1.0f, 4.0f, 8.0f, 12.0f);
                CS::Vector4 b(0.0f, 0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Normalise(a), CS::Vector4(1.0f / 15.0f, 4.0f / 15.0f, 8.0f / 15.0f, 4.0f / 5.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Normalise(b), CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f)));
                
                a.Normalise();
                b.Normalise();
                
                REQUIRE(Common::Approx(a, CS::Vector4(1.0f / 15.0f, 4.0f / 15.0f, 8.0f / 15.0f, 4.0f / 5.0f)));
                REQUIRE(Common::Approx(b, CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the inverse of a vector can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Inverse")
            {
                CS::Vector4 a(2.0f, 4.0f, 8.0f, 12.0f);
                CS::Vector4 b(0.0f, 0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Inverse(a), CS::Vector4(0.5f, 0.25f, 0.125f, 1.0f / 12.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Inverse(b), CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f)));
                
                a.Inverse();
                b.Inverse();
                
                REQUIRE(Common::Approx(a, CS::Vector4(0.5f, 0.25f, 0.125f, 1.0f / 12.0f)));
                REQUIRE(Common::Approx(b, CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the absolute value of a vector can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Absolute")
            {
                CS::Vector4 a(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 b(-1.0f, -1.0f, -1.0f, -1.0f);
                CS::Vector4 c(0.0f, 0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Abs(a), CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Abs(b), CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Abs(c), CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f)));
                
                a.Abs();
                b.Abs();
                c.Abs();
                
                REQUIRE(Common::Approx(a, CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(b, CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(c, CS::Vector4(0.0f, 0.0f, 0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the minimum value of two vectors can be correctly evaluated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Minimum")
            {
                CS::Vector4 a1(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a2(2.0f, 2.0f, 2.0f, 2.0f);
                CS::Vector4 a3(1.0f, 2.0f, 1.0f, 1.0f);
                CS::Vector4 a4(-1.0f, -1.0f, -1.0f, -1.0f);
                CS::Vector4 b1(2.0f, 2.0f, 2.0f, 2.0f);
                CS::Vector4 b2(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 b3(2.0f, 1.0f, 2.0f, 2.0f);
                CS::Vector4 b4(-2.0f, -2.0f, -2.0f, -2.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Min(a1, b1), CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Min(a2, b2), CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Min(a3, b3), CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Min(a4, b4), CS::Vector4(-2.0f, -2.0f, -2.0f, -2.0f)));
                
                a1.Min(b1);
                a2.Min(b2);
                a3.Min(b3);
                a4.Min(b4);
                
                REQUIRE(Common::Approx(a1, CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector4(-2.0f, -2.0f, -2.0f, -2.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the maximum value of two vectors can be correctly evaluated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Maximum")
            {
                CS::Vector4 a1(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a2(2.0f, 2.0f, 2.0f, 2.0f);
                CS::Vector4 a3(1.0f, 2.0f, 1.0f, 1.0f);
                CS::Vector4 a4(-1.0f, -1.0f, -1.0f, -1.0f);
                CS::Vector4 b1(2.0f, 2.0f, 2.0f, 2.0f);
                CS::Vector4 b2(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 b3(2.0f, 1.0f, 2.0f, 2.0f);
                CS::Vector4 b4(-2.0f, -2.0f, -2.0f, -2.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Max(a1, b1), CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Max(a2, b2), CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Max(a3, b3), CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Max(a4, b4), CS::Vector4(-1.0f, -1.0f, -1.0f, -1.0f)));
                
                a1.Max(b1);
                a2.Max(b2);
                a3.Max(b3);
                a4.Max(b4);
                
                REQUIRE(Common::Approx(a1, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector4(-1.0f, -1.0f, -1.0f, -1.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vectors components can be clamped within a range.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Clamp")
            {
                CS::Vector4 value1(0.0f, 0.0f, 0.0f, 0.0f);
                CS::Vector4 value2(2.0f, 2.0f, 2.0f, 2.0f);
                CS::Vector4 value3(4.0f, 4.0f, 4.0f, 4.0f);
                CS::Vector4 value4(0.0f, 4.0f, 0.0f, 0.0f);
                CS::Vector4 value5(0.0f, 0.0f, 0.0f, 0.0f);
                CS::Vector4 value6(-2.0f, -2.0f, -2.0f, -2.0f);
                CS::Vector4 value7(-4.0f, -4.0f, -4.0f, -4.0f);
                CS::Vector4 min1(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 min2(-3.0f, -3.0f, -3.0f, -3.0f);
                CS::Vector4 max1(3.0f, 3.0f, 3.0f, 3.0f);
                CS::Vector4 max2(-1.0f, -1.0f, -1.0f, -1.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value1, min1, max1), CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value2, min1, max1), CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value3, min1, max1), CS::Vector4(3.0f, 3.0f, 3.0f, 3.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value4, min1, max1), CS::Vector4(1.0f, 3.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value5, min2, max2), CS::Vector4(-1.0f, -1.0f, -1.0f, -1.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value6, min2, max2), CS::Vector4(-2.0f, -2.0f, -2.0f, -2.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Clamp(value7, min2, max2), CS::Vector4(-3.0f, -3.0f, -3.0f, -3.0f)));
                
                value1.Clamp(min1, max1);
                value2.Clamp(min1, max1);
                value3.Clamp(min1, max1);
                value4.Clamp(min1, max1);
                value5.Clamp(min2, max2);
                value6.Clamp(min2, max2);
                value7.Clamp(min2, max2);
                
                REQUIRE(Common::Approx(value1, CS::Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(value2, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(value3, CS::Vector4(3.0f, 3.0f, 3.0f, 3.0f)));
                REQUIRE(Common::Approx(value4, CS::Vector4(1.0f, 3.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(value5, CS::Vector4(-1.0f, -1.0f, -1.0f, -1.0f)));
                REQUIRE(Common::Approx(value6, CS::Vector4(-2.0f, -2.0f, -2.0f, -2.0f)));
                REQUIRE(Common::Approx(value7, CS::Vector4(-3.0f, -3.0f, -3.0f, -3.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vector can be linearly interpolated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Lerp")
            {
                CS::Vector4 a1(1.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 a2(-1.0f, -1.0f, -1.0f, -1.0f);
                CS::Vector4 b1(3.0f, 3.0f, 3.0f, 3.0f);
                CS::Vector4 b2(-3.0f, -3.0f, -3.0f, -3.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::Lerp(a1, b1, 0.5f), CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector4::Lerp(a2, b2, 0.5f), CS::Vector4(-2.0f, -2.0f, -2.0f, -2.0f)));
                
                a1.Lerp(b1, 0.5f);
                a2.Lerp(b2, 0.5f);
                
                REQUIRE(Common::Approx(a1, CS::Vector4(2.0f, 2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector4(-2.0f, -2.0f, -2.0f, -2.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the dot product of two vectors can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Dot Product")
            {
                CS::Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
                CS::Vector4 b(0.0f, 1.0f, 1.0f, 1.0f);
                CS::Vector4 c(-1.0f, 0.0f, 0.0f, 0.0f);
                CS::Vector4 d(1.0f, 2.0f, 3.0f, 4.0f);
                CS::Vector4 e(5.0f, 6.0f, 7.0f, 8.0f);
                
                REQUIRE(Common::Approx(CS::Vector4::DotProduct(a, a), 1.0f));
                REQUIRE(Common::Approx(CS::Vector4::DotProduct(a, b), 0.0f));
                REQUIRE(Common::Approx(CS::Vector4::DotProduct(a, c), -1.0f));
                REQUIRE(Common::Approx(CS::Vector4::DotProduct(d, e), 70.0f));
            }
        }
    }
}
