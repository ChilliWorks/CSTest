//
//  Vector2.cpp
//  CSTest
//  Created by Ian Copland on 29/07/2015.
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
        /// A series of test for the math Vector2 class.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        TEST_CASE("Vector2", "[Math]")
        {
            //------------------------------------------------------------------------------
            /// Confirms that a Vector2 can be successfully constructed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Construction and component validation")
            {
                CS::Vector2 a;
                CS::Vector2 b(1.0f, 2.0f);
                
                REQUIRE(Common::Approx(a.x, 0.0f));
                REQUIRE(Common::Approx(a.y, 0.0f));
                REQUIRE(Common::Approx(b.x, 1.0f));
                REQUIRE(Common::Approx(b.y, 2.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that basic arithmatic works correctly for Vector2.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Basic arithmatic")
            {
                CS::Vector2 a1(1.0f, 1.0f);
                CS::Vector2 a2(1.0f, 1.0f);
                CS::Vector2 a3(1.0f, 1.0f);
                CS::Vector2 a4(1.0f, 1.0f);
                CS::Vector2 a5(1.0f, 2.0f);
                CS::Vector2 a6(1.0f, 1.0f);
                CS::Vector2 a7(1.0f, 1.0f);
                CS::Vector2 b1(2.0f, 3.0f);
                auto b2 = CS::Matrix3::CreateTransform(CS::Vector2(1.0f, 2.0f), CS::Vector2(3.0f, 4.0f), 5.0f);
                f32 b3 = 2.0f;
                
                REQUIRE(a1 == a2);
                REQUIRE(a1 != a5);
                REQUIRE(Common::Approx(-a1, CS::Vector2(-1.0f, -1.0f)));
                REQUIRE(Common::Approx(a1 + b1, CS::Vector2(3.0f, 4.0f)));
                REQUIRE(Common::Approx(a2 - b1, CS::Vector2(-1.0f, -2.0f)));
                REQUIRE(Common::Approx(a3 * b1, CS::Vector2(2.0f, 3.0f)));
                REQUIRE(Common::Approx(a4 / b1, CS::Vector2(0.5f, 1.0f / 3.0f)));
                REQUIRE(Common::Approx(a5 * b2, CS::Vector2(9.52238082f, 1.39252472f)));
                REQUIRE(Common::Approx(a6 * b3, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(b3 * a6, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(a7 / b3, CS::Vector2(0.5f, 0.5f)));
                
                a1 += b1;
                a2 -= b1;
                a3 *= b1;
                a4 /= b1;
                a5 *= b2;
                a6 *= b3;
                a7 /= b3;
                
                REQUIRE(Common::Approx(a1, CS::Vector2(3.0f, 4.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector2(-1.0f, -2.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector2(2.0f, 3.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector2(0.5f, 1.0f / 3.0f)));
                REQUIRE(Common::Approx(a5, CS::Vector2(9.52238082f, 1.39252472f)));
                REQUIRE(Common::Approx(a6, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(a7, CS::Vector2(0.5f, 0.5f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that vector length can be successfully calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Length")
            {
                CS::Vector2 a(3.0f, 4.0f);
                CS::Vector2 b(0.0f, 0.0f);
                
                REQUIRE(Common::Approx(a.Length(), 5.0f));
                REQUIRE(Common::Approx(b.Length(), 0.0f));
                REQUIRE(Common::Approx(a.LengthSquared(), 25.0f));
                REQUIRE(Common::Approx(b.LengthSquared(), 0.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms a vectorcan be successfully normalised.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Normalise")
            {
                CS::Vector2 a(3.0f, 4.0f);
                CS::Vector2 b(0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Normalise(a), CS::Vector2(0.6f, 0.8f)));
                REQUIRE(Common::Approx(CS::Vector2::Normalise(b), CS::Vector2(0.0f, 0.0f)));
                
                a.Normalise();
                b.Normalise();
                
                REQUIRE(Common::Approx(a, CS::Vector2(0.6f, 0.8f)));
                REQUIRE(Common::Approx(b, CS::Vector2(0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the inverse of a vector can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Inverse")
            {
                CS::Vector2 a(2.0f, 4.0f);
                CS::Vector2 b(0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Inverse(a), CS::Vector2(0.5f, 0.25f)));
                REQUIRE(Common::Approx(CS::Vector2::Inverse(b), CS::Vector2(0.0f, 0.0f)));
                
                a.Inverse();
                b.Inverse();
                
                REQUIRE(Common::Approx(a, CS::Vector2(0.5f, 0.25f)));
                REQUIRE(Common::Approx(b, CS::Vector2(0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the absolute value of a vector can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Absolute")
            {
                CS::Vector2 a(1.0f, 1.0f);
                CS::Vector2 b(-1.0f, -1.0f);
                CS::Vector2 c(0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Abs(a), CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Abs(b), CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Abs(c), CS::Vector2(0.0f, 0.0f)));
                
                a.Abs();
                b.Abs();
                c.Abs();
                
                REQUIRE(Common::Approx(a, CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(b, CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(c, CS::Vector2(0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the minimum value of two vectors can be correctly evaluated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Minimum")
            {
                CS::Vector2 a1(1.0f, 1.0f);
                CS::Vector2 a2(2.0f, 2.0f);
                CS::Vector2 a3(1.0f, 2.0f);
                CS::Vector2 a4(-1.0f, -1.0f);
                CS::Vector2 b1(2.0f, 2.0f);
                CS::Vector2 b2(1.0f, 1.0f);
                CS::Vector2 b3(2.0f, 1.0f);
                CS::Vector2 b4(-2.0f, -2.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Min(a1, b1), CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Min(a2, b2), CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Min(a3, b3), CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Min(a4, b4), CS::Vector2(-2.0f, -2.0f)));
                
                a1.Min(b1);
                a2.Min(b2);
                a3.Min(b3);
                a4.Min(b4);
                
                REQUIRE(Common::Approx(a1, CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector2(-2.0f, -2.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the maximum value of two vectors can be correctly evaluated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Maximum")
            {
                CS::Vector2 a1(1.0f, 1.0f);
                CS::Vector2 a2(2.0f, 2.0f);
                CS::Vector2 a3(1.0f, 2.0f);
                CS::Vector2 a4(-1.0f, -1.0f);
                CS::Vector2 b1(2.0f, 2.0f);
                CS::Vector2 b2(1.0f, 1.0f);
                CS::Vector2 b3(2.0f, 1.0f);
                CS::Vector2 b4(-2.0f, -2.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Max(a1, b1), CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Max(a2, b2), CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Max(a3, b3), CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Max(a4, b4), CS::Vector2(-1.0f, -1.0f)));
                
                a1.Max(b1);
                a2.Max(b2);
                a3.Max(b3);
                a4.Max(b4);
                
                REQUIRE(Common::Approx(a1, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector2(-1.0f, -1.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vectors components can be clamped within a range.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Clamp")
            {
                CS::Vector2 value1(0.0f, 0.0f);
                CS::Vector2 value2(2.0f, 2.0f);
                CS::Vector2 value3(4.0f, 4.0f);
                CS::Vector2 value4(4.0f, 0.0f);
                CS::Vector2 value5(0.0f, 0.0f);
                CS::Vector2 value6(-2.0f, -2.0f);
                CS::Vector2 value7(-4.0f, -4.0f);
                CS::Vector2 min1(1.0f, 1.0f);
                CS::Vector2 min2(-3.0f, -3.0f);
                CS::Vector2 max1(3.0f, 3.0f);
                CS::Vector2 max2(-1.0f, -1.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value1, min1, max1), CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value2, min1, max1), CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value3, min1, max1), CS::Vector2(3.0f, 3.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value4, min1, max1), CS::Vector2(3.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value5, min2, max2), CS::Vector2(-1.0f, -1.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value6, min2, max2), CS::Vector2(-2.0f, -2.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Clamp(value7, min2, max2), CS::Vector2(-3.0f, -3.0f)));
                
                value1.Clamp(min1, max1);
                value2.Clamp(min1, max1);
                value3.Clamp(min1, max1);
                value4.Clamp(min1, max1);
                value5.Clamp(min2, max2);
                value6.Clamp(min2, max2);
                value7.Clamp(min2, max2);
                
                REQUIRE(Common::Approx(value1, CS::Vector2(1.0f, 1.0f)));
                REQUIRE(Common::Approx(value2, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(value3, CS::Vector2(3.0f, 3.0f)));
                REQUIRE(Common::Approx(value4, CS::Vector2(3.0f, 1.0f)));
                REQUIRE(Common::Approx(value5, CS::Vector2(-1.0f, -1.0f)));
                REQUIRE(Common::Approx(value6, CS::Vector2(-2.0f, -2.0f)));
                REQUIRE(Common::Approx(value7, CS::Vector2(-3.0f, -3.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vector can be linearly interpolated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Lerp")
            {
                CS::Vector2 a1(1.0f, 1.0f);
                CS::Vector2 a2(-1.0f, -1.0f);
                CS::Vector2 b1(3.0f, 3.0f);
                CS::Vector2 b2(-3.0f, -3.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Lerp(a1, b1, 0.5f), CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector2::Lerp(a2, b2, 0.5f), CS::Vector2(-2.0f, -2.0f)));
                
                a1.Lerp(b1, 0.5f);
                a2.Lerp(b2, 0.5f);
                
                REQUIRE(Common::Approx(a1, CS::Vector2(2.0f, 2.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector2(-2.0f, -2.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the dot product of two vectors can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Dot Product")
            {
                CS::Vector2 a(1.0f, 0.0f);
                CS::Vector2 b(0.0f, 1.0f);
                CS::Vector2 c(-1.0f, 0.0f);
                CS::Vector2 d(1.0f, 2.0f);
                CS::Vector2 e(3.0f, 4.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::DotProduct(a, a), 1.0f));
                REQUIRE(Common::Approx(CS::Vector2::DotProduct(a, b), 0.0f));
                REQUIRE(Common::Approx(CS::Vector2::DotProduct(a, c), -1.0f));
                REQUIRE(Common::Approx(CS::Vector2::DotProduct(d, e), 11.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the z component of the cross product of two vectors can be
            /// correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Cross Product Z")
            {
                CS::Vector2 a(1.0f, 2.0f);
                CS::Vector2 b(3.0f, 4.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::CrossProductZ(a, b), -2.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the angle between two vectors can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Angle")
            {
                CS::Vector2 a(1.0f, 2.0f);
                CS::Vector2 b(3.0f, 4.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Angle(a, a), 0.0f));
                REQUIRE(Common::Approx(CS::Vector2::Angle(a, b), 0.179853559f));
                REQUIRE(Common::Approx(CS::Vector2::Angle(b, a), 0.179853559f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vector can be rotated correctly.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Rotate")
            {
                CS::Vector2 pointA(1.0f, 2.0f);
                CS::Vector2 pointB(-3.0f, -4.0f);
                f32 angleA = 0.5f;
                f32 angleB = -0.5f;
                
                REQUIRE(Common::Approx(CS::Vector2::Rotate(pointA, angleA), CS::Vector2(1.83643365f, 1.27573955f)));
                REQUIRE(Common::Approx(CS::Vector2::Rotate(pointB, angleB), CS::Vector2(-0.715045452f, -4.94860697f)));
                
                pointA.Rotate(angleA);
                pointB.Rotate(angleB);
                
                REQUIRE(Common::Approx(pointA, CS::Vector2(1.83643365f, 1.27573955f)));
                REQUIRE(Common::Approx(pointB, CS::Vector2(-0.715045452f, -4.94860697f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the fast transform method can be calculated correctly.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Transform2x3")
            {
                CS::Vector2 pointA(1.0f, 2.0f);
                CS::Vector2 pointB(-3.0f, -4.0f);
                auto transformA = CS::Matrix3::CreateTransform(CS::Vector2(1.0f, 2.0f), CS::Vector2(3.0f, 4.0f), 5.0f);
                auto transformB = CS::Matrix3::CreateTransform(CS::Vector2(-1.0f, -2.0f), CS::Vector2(-3.0f, -4.0f), -5.0f);
                
                REQUIRE(Common::Approx(CS::Vector2::Transform2x3(pointA, transformA), CS::Vector2(9.52238082f, 1.39252472f)));
                REQUIRE(Common::Approx(CS::Vector2::Transform2x3(pointB, transformB), CS::Vector2(-13.7898293f, 11.1689138f)));
                
                pointA.Transform2x3(transformA);
                pointB.Transform2x3(transformB);
                
                REQUIRE(Common::Approx(pointA, CS::Vector2(9.52238082f, 1.39252472f)));
                REQUIRE(Common::Approx(pointB, CS::Vector2(-13.7898293f, 11.1689138f)));
            }
        }
    }
}
