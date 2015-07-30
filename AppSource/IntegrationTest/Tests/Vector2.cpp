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
        }
        
        TEST_CASE("Vector2: Construction and component validation", "[Math][Vector2]")
        {
            CSCore::Vector2 a;
            CSCore::Vector2 b(1.0f, 2.0f);
            
            REQUIRE(Equals(a.x, 0.0f));
            REQUIRE(Equals(a.y, 0.0f));
            REQUIRE(Equals(b.x, 1.0f));
            REQUIRE(Equals(b.y, 2.0f));
        }
        
        TEST_CASE("Vector2: Basic arithmatic", "[Math][Vector2]")
        {
            CSCore::Vector2 a1(1.0f, 1.0f);
            CSCore::Vector2 a2(1.0f, 1.0f);
            CSCore::Vector2 a3(1.0f, 1.0f);
            CSCore::Vector2 a4(1.0f, 1.0f);
            CSCore::Vector2 a5(1.0f, 2.0f);
            CSCore::Vector2 a6(1.0f, 1.0f);
            CSCore::Vector2 a7(1.0f, 1.0f);
            CSCore::Vector2 b1(2.0f, 3.0f);
            auto b2 = CSCore::Matrix3::CreateTransform(CSCore::Vector2(1.0f, 2.0f), CSCore::Vector2(3.0f, 4.0f), 5.0f);
            f32 b3 = 2.0f;
            
            REQUIRE(a1 == a2);
            REQUIRE(a1 != a5);
            REQUIRE(Equals(-a1, CSCore::Vector2(-1.0f, -1.0f)));
            REQUIRE(Equals(a1 + b1, CSCore::Vector2(3.0f, 4.0f)));
            REQUIRE(Equals(a2 - b1, CSCore::Vector2(-1.0f, -2.0f)));
            REQUIRE(Equals(a3 * b1, CSCore::Vector2(2.0f, 3.0f)));
            REQUIRE(Equals(a4 / b1, CSCore::Vector2(0.5f, 1.0f / 3.0f)));
            REQUIRE(Equals(a5 * b2, CSCore::Vector2(9.52238082, 1.39252472)));
            REQUIRE(Equals(a6 * b3, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(b3 * a6, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a7 / b3, CSCore::Vector2(0.5f, 0.5f)));
            
            a1 += b1;
            a2 -= b1;
            a3 *= b1;
            a4 /= b1;
            a5 *= b2;
            a6 *= b3;
            a7 /= b3;
            
            REQUIRE(Equals(a1, CSCore::Vector2(3.0f, 4.0f)));
            REQUIRE(Equals(a2, CSCore::Vector2(-1.0f, -2.0f)));
            REQUIRE(Equals(a3, CSCore::Vector2(2.0f, 3.0f)));
            REQUIRE(Equals(a4, CSCore::Vector2(0.5f, 1.0f / 3.0f)));
            REQUIRE(Equals(a5, CSCore::Vector2(9.52238082, 1.39252472)));
            REQUIRE(Equals(a6, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a7, CSCore::Vector2(0.5f, 0.5f)));
        }
        
        TEST_CASE("Vector2: Length", "[Math][Vector2]")
        {
            CSCore::Vector2 a(3.0f, 4.0f);
            CSCore::Vector2 b(0.0f, 0.0f);
            
            REQUIRE(Equals(a.Length(), 5.0f));
            REQUIRE(Equals(b.Length(), 0.0f));
            REQUIRE(Equals(a.LengthSquared(), 25.0f));
            REQUIRE(Equals(b.LengthSquared(), 0.0f));
        }
        
        TEST_CASE("Vector2: Normalise", "[Math][Vector2]")
        {
            CSCore::Vector2 a(3.0f, 4.0f);
            CSCore::Vector2 b(0.0f, 0.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Normalise(a), CSCore::Vector2(0.6f, 0.8f)));
            REQUIRE(Equals(CSCore::Vector2::Normalise(b), CSCore::Vector2(0.0f, 0.0f)));
            
            a.Normalise();
            b.Normalise();
            
            REQUIRE(Equals(a, CSCore::Vector2(0.6f, 0.8f)));
            REQUIRE(Equals(b, CSCore::Vector2(0.0f, 0.0f)));
        }
        
        TEST_CASE("Vector2: Inverse", "[Math][Vector2]")
        {
            CSCore::Vector2 a(2.0f, 4.0f);
            CSCore::Vector2 b(0.0f, 0.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Inverse(a), CSCore::Vector2(0.5f, 0.25f)));
            REQUIRE(Equals(CSCore::Vector2::Inverse(b), CSCore::Vector2(0.0f, 0.0f)));
            
            a.Inverse();
            b.Inverse();
            
            REQUIRE(Equals(a, CSCore::Vector2(0.5f, 0.25f)));
            REQUIRE(Equals(b, CSCore::Vector2(0.0f, 0.0f)));
        }
        
        TEST_CASE("Vector2: Absolute", "[Math][Vector2]")
        {
            CSCore::Vector2 a(1.0f, 1.0f);
            CSCore::Vector2 b(1.0f, -1.0f);
            CSCore::Vector2 c(-1.0f, 1.0f);
            CSCore::Vector2 d(-1.0f, -1.0f);
            CSCore::Vector2 e(0.0f, 0.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Abs(a), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(b), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(c), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(d), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Abs(e), CSCore::Vector2(0.0f, 0.0f)));
            
            a.Abs();
            b.Abs();
            c.Abs();
            d.Abs();
            e.Abs();
            
            REQUIRE(Equals(a, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(b, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(c, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(d, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(e, CSCore::Vector2(0.0f, 0.0f)));
        }
        
        TEST_CASE("Vector2: Minimum", "[Math][Vector2]")
        {
            CSCore::Vector2 a1(1.0f, 1.0f);
            CSCore::Vector2 a2(2.0f, 1.0f);
            CSCore::Vector2 a3(1.0f, 2.0f);
            CSCore::Vector2 a4(2.0f, 2.0f);
            CSCore::Vector2 a5(-1.0f, -1.0f);
            
            CSCore::Vector2 b1(2.0f, 2.0f);
            CSCore::Vector2 b2(1.0f, 2.0f);
            CSCore::Vector2 b3(2.0f, 1.0f);
            CSCore::Vector2 b4(1.0f, 1.0f);
            CSCore::Vector2 b5(-2.0f, -2.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Min(a1, b1), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Min(a2, b2), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Min(a3, b3), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Min(a4, b4), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Min(a5, b5), CSCore::Vector2(-2.0f, -2.0f)));
            
            a1.Min(b1);
            a2.Min(b2);
            a3.Min(b3);
            a4.Min(b4);
            a5.Min(b5);
            
            REQUIRE(Equals(a1, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(a2, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(a3, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(a4, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(a5, CSCore::Vector2(-2.0f, -2.0f)));
        }
        
        TEST_CASE("Vector2: Maximum", "[Math][Vector2]")
        {
            CSCore::Vector2 a1(1.0f, 1.0f);
            CSCore::Vector2 a2(2.0f, 1.0f);
            CSCore::Vector2 a3(1.0f, 2.0f);
            CSCore::Vector2 a4(2.0f, 2.0f);
            CSCore::Vector2 a5(-1.0f, -1.0f);
            CSCore::Vector2 b1(2.0f, 2.0f);
            CSCore::Vector2 b2(1.0f, 2.0f);
            CSCore::Vector2 b3(2.0f, 1.0f);
            CSCore::Vector2 b4(1.0f, 1.0f);
            CSCore::Vector2 b5(-2.0f, -2.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Max(a1, b1), CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Max(a2, b2), CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Max(a3, b3), CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Max(a4, b4), CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Max(a5, b5), CSCore::Vector2(-1.0f, -1.0f)));
            
            a1.Max(b1);
            a2.Max(b2);
            a3.Max(b3);
            a4.Max(b4);
            a5.Max(b5);
            
            REQUIRE(Equals(a1, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a2, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a3, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a4, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a5, CSCore::Vector2(-1.0f, -1.0f)));
        }
        
        TEST_CASE("Vector2: Clamp", "[Math][Vector2]")
        {
            CSCore::Vector2 value1(0.0f, 0.0f);
            CSCore::Vector2 value2(2.0f, 2.0f);
            CSCore::Vector2 value3(4.0f, 4.0f);
            CSCore::Vector2 value4(4.0f, 0.0f);
            CSCore::Vector2 value5(0.0f, 4.0f);
            CSCore::Vector2 value6(0.0f, 0.0f);
            CSCore::Vector2 value7(-2.0f, -2.0f);
            CSCore::Vector2 value8(-4.0f, -4.0f);
            CSCore::Vector2 min1(1.0f, 1.0f);
            CSCore::Vector2 min2(-3.0f, -3.0f);
            CSCore::Vector2 max1(3.0f, 3.0f);
            CSCore::Vector2 max2(-1.0f, -1.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Clamp(value1, min1, max1), CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value2, min1, max1), CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value3, min1, max1), CSCore::Vector2(3.0f, 3.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value4, min1, max1), CSCore::Vector2(3.0f, 1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value5, min1, max1), CSCore::Vector2(1.0f, 3.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value6, min2, max2), CSCore::Vector2(-1.0f, -1.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value7, min2, max2), CSCore::Vector2(-2.0f, -2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Clamp(value8, min2, max2), CSCore::Vector2(-3.0f, -3.0f)));
            
            value1.Clamp(min1, max1);
            value2.Clamp(min1, max1);
            value3.Clamp(min1, max1);
            value4.Clamp(min1, max1);
            value5.Clamp(min1, max1);
            value6.Clamp(min2, max2);
            value7.Clamp(min2, max2);
            value8.Clamp(min2, max2);
            
            REQUIRE(Equals(value1, CSCore::Vector2(1.0f, 1.0f)));
            REQUIRE(Equals(value2, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(value3, CSCore::Vector2(3.0f, 3.0f)));
            REQUIRE(Equals(value4, CSCore::Vector2(3.0f, 1.0f)));
            REQUIRE(Equals(value5, CSCore::Vector2(1.0f, 3.0f)));
            REQUIRE(Equals(value6, CSCore::Vector2(-1.0f, -1.0f)));
            REQUIRE(Equals(value7, CSCore::Vector2(-2.0f, -2.0f)));
            REQUIRE(Equals(value8, CSCore::Vector2(-3.0f, -3.0f)));
        }
        
        TEST_CASE("Vector2: Lerp", "[Math][Vector2]")
        {
            CSCore::Vector2 a1(1.0f, 1.0f);
            CSCore::Vector2 a2(-1.0f, -1.0f);
            CSCore::Vector2 b1(3.0f, 3.0f);
            CSCore::Vector2 b2(-3.0f, -3.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Lerp(a1, b1, 0.5f), CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(CSCore::Vector2::Lerp(a2, b2, 0.5f), CSCore::Vector2(-2.0f, -2.0f)));
            
            a1.Lerp(b1, 0.5f);
            a2.Lerp(b2, 0.5f);
            
            REQUIRE(Equals(a1, CSCore::Vector2(2.0f, 2.0f)));
            REQUIRE(Equals(a2, CSCore::Vector2(-2.0f, -2.0f)));
        }
        
        TEST_CASE("Vector2: Dot Product", "[Math][Vector2]")
        {
            CSCore::Vector2 a(1.0f, 0.0f);
            CSCore::Vector2 b(0.0f, 1.0f);
            CSCore::Vector2 c(-1.0f, 0.0f);
            CSCore::Vector2 d(1.0f, 2.0f);
            CSCore::Vector2 e(3.0f, 4.0f);
            
            REQUIRE(Equals(CSCore::Vector2::DotProduct(a, a), 1.0f));
            REQUIRE(Equals(CSCore::Vector2::DotProduct(a, b), 0.0f));
            REQUIRE(Equals(CSCore::Vector2::DotProduct(a, c), -1.0f));
            REQUIRE(Equals(CSCore::Vector2::DotProduct(d, e), 11.0f));
        }
        
        TEST_CASE("Vector2: Cross Product Z", "[Math][Vector2]")
        {
            CSCore::Vector2 a(1.0f, 2.0f);
            CSCore::Vector2 b(3.0f, 4.0f);
            
            REQUIRE(Equals(CSCore::Vector2::CrossProductZ(a, b), -2.0f));
        }
        
        TEST_CASE("Vector2: Angle", "[Math][Vector2]")
        {
            CSCore::Vector2 a(1.0f, 2.0f);
            CSCore::Vector2 b(3.0f, 4.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Angle(a, a), 0.0f));
            REQUIRE(Equals(CSCore::Vector2::Angle(a, b), -0.179853559));
            REQUIRE(Equals(CSCore::Vector2::Angle(b, a), 0.179853559));
        }
        
        TEST_CASE("Vector2: Rotate", "[Math][Vector2]")
        {
            CSCore::Vector2 pointA(1.0f, 2.0f);
            CSCore::Vector2 pointB(-3.0f, -4.0f);
            f32 angleA = 0.5f;
            f32 angleB = -0.5f;
            
            REQUIRE(Equals(CSCore::Vector2::Rotate(pointA, angleA), CSCore::Vector2(1.83643365, 1.27573955)));
            REQUIRE(Equals(CSCore::Vector2::Rotate(pointB, angleB), CSCore::Vector2(-0.715045452, -4.94860697)));
            
            pointA.Rotate(angleA);
            pointB.Rotate(angleB);
            
            REQUIRE(Equals(pointA, CSCore::Vector2(1.83643365, 1.27573955)));
            REQUIRE(Equals(pointB, CSCore::Vector2(-0.715045452, -4.94860697)));
        }
        
        TEST_CASE("Vector2: Transform2x3", "[Math][Vector2]")
        {
            CSCore::Vector2 pointA(1.0f, 2.0f);
            CSCore::Vector2 pointB(-3.0f, -4.0f);
            auto transformA = CSCore::Matrix3::CreateTransform(CSCore::Vector2(1.0f, 2.0f), CSCore::Vector2(3.0f, 4.0f), 5.0f);
            auto transformB = CSCore::Matrix3::CreateTransform(CSCore::Vector2(-1.0f, -2.0f), CSCore::Vector2(-3.0f, -4.0f), -5.0f);
            
            REQUIRE(Equals(CSCore::Vector2::Transform2x3(pointA, transformA), CSCore::Vector2(9.52238082, 1.39252472)));
            REQUIRE(Equals(CSCore::Vector2::Transform2x3(pointB, transformB), CSCore::Vector2(-13.7898293, 11.1689138)));
            
            pointA.Transform2x3(transformA);
            pointB.Transform2x3(transformB);
            
            REQUIRE(Equals(pointA, CSCore::Vector2(9.52238082, 1.39252472)));
            REQUIRE(Equals(pointB, CSCore::Vector2(-13.7898293, 11.1689138)));
        }
    }
}
