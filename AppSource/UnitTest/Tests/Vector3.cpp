//
//  Vector3.cpp
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

#include <CSTest.h>

#include <Common/Core/Approx.h>

#include <ChilliSource/Core/Math.h>

#include <catch.hpp>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        /// A series of test for the math Vector3 class.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        TEST_CASE("Vector3", "[Math]")
        {
            //------------------------------------------------------------------------------
            /// Confirms that a Vector3 can be successfully constructed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Construction and component validation")
            {
                CS::Vector3 a;
                CS::Vector3 b(1.0f, 2.0f, 3.0f);
                
                REQUIRE(Common::Approx(a.x, 0.0f));
                REQUIRE(Common::Approx(a.y, 0.0f));
                REQUIRE(Common::Approx(a.z, 0.0f));
                REQUIRE(Common::Approx(b.x, 1.0f));
                REQUIRE(Common::Approx(b.y, 2.0f));
                REQUIRE(Common::Approx(b.z, 3.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that basic arithmatic works correctly for Vector3.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Basic arithmatic")
            {
                CS::Vector3 a1(1.0f, 1.0f, 1.0f);
                CS::Vector3 a2(1.0f, 1.0f, 1.0f);
                CS::Vector3 a3(1.0f, 1.0f, 1.0f);
                CS::Vector3 a4(1.0f, 1.0f, 1.0f);
                CS::Vector3 a5(1.0f, 2.0f, 1.0f);
                CS::Vector3 a6(1.0f, 2.0f, 3.0f);
                CS::Vector3 a7(1.0f, 1.0f, 1.0f);
                CS::Vector3 a8(1.0f, 1.0f, 1.0f);
                CS::Vector3 b1(2.0f, 3.0f, 4.0f);
                auto b2 = CS::Matrix3::CreateTransform(CS::Vector2(1.0f, 2.0f), CS::Vector2(3.0f, 4.0f), 5.0f);
                auto b3 = CS::Matrix4::CreateTransform(CS::Vector3(1.0f, 2.0f, 3.0f), CS::Vector3(1.0f, 2.0f, 3.0f), CS::Quaternion(CS::Vector3::Normalise(CS::Vector3(1.0f, 1.0f, 1.0f)), -0.5f));
                f32 b4 = 2.0f;
                
                REQUIRE(a1 == a2);
                REQUIRE(a1 != a5);
                REQUIRE(Common::Approx(-a1, CS::Vector3(-1.0f, -1.0f, -1.0f)));
                REQUIRE(Common::Approx(a1 + b1, CS::Vector3(3.0f, 4.0f, 5.0f)));
                REQUIRE(Common::Approx(a2 - b1, CS::Vector3(-1.0f, -2.0f, -3.0f)));
                REQUIRE(Common::Approx(a3 * b1, CS::Vector3(2.0f, 3.0f, 4.0f)));
                REQUIRE(Common::Approx(a4 / b1, CS::Vector3(0.5f, 1.0f / 3.0f, 0.25f)));
                REQUIRE(Common::Approx(a5 * b2, CS::Vector3(9.52238082f, 1.39252472f, 1.0f)));
                REQUIRE(Common::Approx(a6 * b3, CS::Vector3(1.06488132f, 8.29598331f, 10.6391354f)));
                REQUIRE(Common::Approx(a7 * b4, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(b4 * a7, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a8 / b4, CS::Vector3(0.5f, 0.5f, 0.5f)));
                
                a1 += b1;
                a2 -= b1;
                a3 *= b1;
                a4 /= b1;
                a5 *= b2;
                a6 *= b3;
                a7 *= b4;
                a8 /= b4;
                
                REQUIRE(Common::Approx(a1, CS::Vector3(3.0f, 4.0f, 5.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector3(-1.0f, -2.0f, -3.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector3(2.0f, 3.0f, 4.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector3(0.5f, 1.0f / 3.0f, 0.25f)));
                REQUIRE(Common::Approx(a5, CS::Vector3(9.52238082f, 1.39252472f, 1.0f)));
                REQUIRE(Common::Approx(a6, CS::Vector3(1.06488132f, 8.29598331f, 10.6391354f)));
                REQUIRE(Common::Approx(a7, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a8, CS::Vector3(0.5f, 0.5f, 0.5f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that vector length can be successfully calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Length")
            {
                CS::Vector3 a(1.0f, 4.0f, 8.0f);
                CS::Vector3 b(0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(a.Length(), 9.0f));
                REQUIRE(Common::Approx(b.Length(), 0.0f));
                REQUIRE(Common::Approx(a.LengthSquared(), 81.0f));
                REQUIRE(Common::Approx(b.LengthSquared(), 0.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms a vectorcan be successfully normalised.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Normalise")
            {
                CS::Vector3 a(1.0f, 4.0f, 8.0f);
                CS::Vector3 b(0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Normalise(a), CS::Vector3(1.0f / 9.0f, 4.0f / 9.0f, 8.0f / 9.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Normalise(b), CS::Vector3(0.0f, 0.0f, 0.0f)));
                
                a.Normalise();
                b.Normalise();
                
                REQUIRE(Common::Approx(a, CS::Vector3(1.0f / 9.0f, 4.0f / 9.0f, 8.0f / 9.0f)));
                REQUIRE(Common::Approx(b, CS::Vector3(0.0f, 0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the inverse of a vector can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Inverse")
            {
                CS::Vector3 a(2.0f, 4.0f, 8.0f);
                CS::Vector3 b(0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Inverse(a), CS::Vector3(0.5f, 0.25f, 0.125f)));
                REQUIRE(Common::Approx(CS::Vector3::Inverse(b), CS::Vector3(0.0f, 0.0f, 0.0f)));
                
                a.Inverse();
                b.Inverse();
                
                REQUIRE(Common::Approx(a, CS::Vector3(0.5f, 0.25f, 0.125f)));
                REQUIRE(Common::Approx(b, CS::Vector3(0.0f, 0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the absolute value of a vector can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Absolute")
            {
                CS::Vector3 a(1.0f, 1.0f, 1.0f);
                CS::Vector3 b(-1.0f, -1.0f, -1.0f);
                CS::Vector3 c(0.0f, 0.0f, 0.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Abs(a), CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Abs(b), CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Abs(c), CS::Vector3(0.0f, 0.0f, 0.0f)));
                
                a.Abs();
                b.Abs();
                c.Abs();
                
                REQUIRE(Common::Approx(a, CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(b, CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(c, CS::Vector3(0.0f, 0.0f, 0.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the minimum value of two vectors can be correctly evaluated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Minimum")
            {
                CS::Vector3 a1(1.0f, 1.0f, 1.0f);
                CS::Vector3 a2(2.0f, 2.0f, 2.0f);
                CS::Vector3 a3(1.0f, 2.0f, 1.0f);
                CS::Vector3 a4(-1.0f, -1.0f, -1.0f);
                CS::Vector3 b1(2.0f, 2.0f, 2.0f);
                CS::Vector3 b2(1.0f, 1.0f, 1.0f);
                CS::Vector3 b3(2.0f, 1.0f, 2.0f);
                CS::Vector3 b4(-2.0f, -2.0f, -2.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Min(a1, b1), CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Min(a2, b2), CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Min(a3, b3), CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Min(a4, b4), CS::Vector3(-2.0f, -2.0f, -2.0f)));
                
                a1.Min(b1);
                a2.Min(b2);
                a3.Min(b3);
                a4.Min(b4);
                
                REQUIRE(Common::Approx(a1, CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector3(-2.0f, -2.0f, -2.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the maximum value of two vectors can be correctly evaluated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Maximum")
            {
                CS::Vector3 a1(1.0f, 1.0f, 1.0f);
                CS::Vector3 a2(2.0f, 2.0f, 2.0f);
                CS::Vector3 a3(1.0f, 2.0f, 1.0f);
                CS::Vector3 a4(-1.0f, -1.0f, -1.0f);
                CS::Vector3 b1(2.0f, 2.0f, 2.0f);
                CS::Vector3 b2(1.0f, 1.0f, 1.0f);
                CS::Vector3 b3(2.0f, 1.0f, 2.0f);
                CS::Vector3 b4(-2.0f, -2.0f, -2.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Max(a1, b1), CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Max(a2, b2), CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Max(a3, b3), CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Max(a4, b4), CS::Vector3(-1.0f, -1.0f, -1.0f)));
                
                a1.Max(b1);
                a2.Max(b2);
                a3.Max(b3);
                a4.Max(b4);
                
                REQUIRE(Common::Approx(a1, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a3, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a4, CS::Vector3(-1.0f, -1.0f, -1.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vectors components can be clamped within a range.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Clamp")
            {
                CS::Vector3 value1(0.0f, 0.0f, 0.0f);
                CS::Vector3 value2(2.0f, 2.0f, 2.0f);
                CS::Vector3 value3(4.0f, 4.0f, 4.0f);
                CS::Vector3 value4(0.0f, 4.0f, 0.0f);
                CS::Vector3 value5(0.0f, 0.0f, 0.0f);
                CS::Vector3 value6(-2.0f, -2.0f, -2.0f);
                CS::Vector3 value7(-4.0f, -4.0f, -4.0f);
                CS::Vector3 min1(1.0f, 1.0f, 1.0f);
                CS::Vector3 min2(-3.0f, -3.0f, -3.0f);
                CS::Vector3 max1(3.0f, 3.0f, 3.0f);
                CS::Vector3 max2(-1.0f, -1.0f, -1.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value1, min1, max1), CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value2, min1, max1), CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value3, min1, max1), CS::Vector3(3.0f, 3.0f, 3.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value4, min1, max1), CS::Vector3(1.0f, 3.0f, 1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value5, min2, max2), CS::Vector3(-1.0f, -1.0f, -1.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value6, min2, max2), CS::Vector3(-2.0f, -2.0f, -2.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Clamp(value7, min2, max2), CS::Vector3(-3.0f, -3.0f, -3.0f)));
                
                value1.Clamp(min1, max1);
                value2.Clamp(min1, max1);
                value3.Clamp(min1, max1);
                value4.Clamp(min1, max1);
                value5.Clamp(min2, max2);
                value6.Clamp(min2, max2);
                value7.Clamp(min2, max2);
                
                REQUIRE(Common::Approx(value1, CS::Vector3(1.0f, 1.0f, 1.0f)));
                REQUIRE(Common::Approx(value2, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(value3, CS::Vector3(3.0f, 3.0f, 3.0f)));
                REQUIRE(Common::Approx(value4, CS::Vector3(1.0f, 3.0f, 1.0f)));
                REQUIRE(Common::Approx(value5, CS::Vector3(-1.0f, -1.0f, -1.0f)));
                REQUIRE(Common::Approx(value6, CS::Vector3(-2.0f, -2.0f, -2.0f)));
                REQUIRE(Common::Approx(value7, CS::Vector3(-3.0f, -3.0f, -3.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vector can be linearly interpolated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Lerp")
            {
                CS::Vector3 a1(1.0f, 1.0f, 1.0f);
                CS::Vector3 a2(-1.0f, -1.0f, -1.0f);
                CS::Vector3 b1(3.0f, 3.0f, 3.0f);
                CS::Vector3 b2(-3.0f, -3.0f, -3.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Lerp(a1, b1, 0.5f), CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Lerp(a2, b2, 0.5f), CS::Vector3(-2.0f, -2.0f, -2.0f)));
                
                a1.Lerp(b1, 0.5f);
                a2.Lerp(b2, 0.5f);
                
                REQUIRE(Common::Approx(a1, CS::Vector3(2.0f, 2.0f, 2.0f)));
                REQUIRE(Common::Approx(a2, CS::Vector3(-2.0f, -2.0f, -2.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the dot product of two vectors can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Dot Product")
            {
                CS::Vector3 a(1.0f, 0.0f, 0.0f);
                CS::Vector3 b(0.0f, 1.0f, 1.0f);
                CS::Vector3 c(-1.0f, 0.0f, 0.0f);
                CS::Vector3 d(1.0f, 2.0f, 3.0f);
                CS::Vector3 e(4.0f, 5.0f, 6.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::DotProduct(a, a), 1.0f));
                REQUIRE(Common::Approx(CS::Vector3::DotProduct(a, b), 0.0f));
                REQUIRE(Common::Approx(CS::Vector3::DotProduct(a, c), -1.0f));
                REQUIRE(Common::Approx(CS::Vector3::DotProduct(d, e), 32.0f));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the the cross product of two vectors can be correctly
            /// calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Cross Product")
            {
                CS::Vector3 a(1.0f, 2.0f, 3.0f);
                CS::Vector3 b(4.0f, 5.0f, 6.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::CrossProduct(a, b), CS::Vector3(-3.0f, 6.0f, -3.0f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the angle between two vectors can be correctly calculated.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Angle")
            {
                //The angle algorithm suffers badly from floating point error problems, so it requires a
                //bigger tolerance than most.
                const f32 k_epsilon = 0.001f;
                
                CS::Vector3 a(1.0f, 2.0f, 3.0f);
                CS::Vector3 b(4.0f, 5.0f, 6.0f);
                
                REQUIRE(Common::Approx(CS::Vector3::Angle(a, a), 0.0f, k_epsilon));
                REQUIRE(Common::Approx(CS::Vector3::Angle(a, b), 0.225726129f, k_epsilon));
                REQUIRE(Common::Approx(CS::Vector3::Angle(b, a), 0.225726129f, k_epsilon));
            }
            //------------------------------------------------------------------------------
            /// Confirms that a vector can be rotated correctly.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Rotate")
            {
                CS::Vector3 pointA(1.0f, 2.0f, 3.0f);
                CS::Vector3 pointB(-4.0f, -5.0f, -6.0f);
                CS::Quaternion rotationA(CS::Vector3(0.0f, 0.0f, 1.0f), 0.5f);
                CS::Quaternion rotationB(CS::Vector3::Normalise(CS::Vector3(1.0f, 1.0f, 1.0f)), -0.5f);
                
                REQUIRE(Common::Approx(CS::Vector3::Rotate(pointA, rotationA), CS::Vector3(-0.0812685415f, 2.23459053f, 3.0f)));
                REQUIRE(Common::Approx(CS::Vector3::Rotate(pointB, rotationB), CS::Vector3(-3.84562111f, -5.55359268f, -5.60078621f)));
                
                pointA.Rotate(rotationA);
                pointB.Rotate(rotationB);
                
                REQUIRE(Common::Approx(pointA, CS::Vector3(-0.0812685415f, 2.23459053f, 3.0f)));
                REQUIRE(Common::Approx(pointB, CS::Vector3(-3.84562111f, -5.55359268f, -5.60078621f)));
            }
            //------------------------------------------------------------------------------
            /// Confirms that the fast transform method can be calculated correctly.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            SECTION("Transform3x4")
            {
                CS::Vector3 point(1.0f, 2.0f, 3.0f);
                auto transform = CS::Matrix4::CreateTransform(CS::Vector3(1.0f, 2.0f, 3.0f), CS::Vector3(1.0f, 2.0f, 3.0f), CS::Quaternion(CS::Vector3::Normalise(CS::Vector3(1.0f, 1.0f, 1.0f)), -0.5f));
                
                REQUIRE(Common::Approx(CS::Vector3::Transform3x4(point, transform), CS::Vector3(1.06488132f, 8.29598331f, 10.6391354f)));
                
                point.Transform3x4(transform);
                
                REQUIRE(Common::Approx(point, CS::Vector3(1.06488132f, 8.29598331f, 10.6391354f)));
            }
        }
    }
}
