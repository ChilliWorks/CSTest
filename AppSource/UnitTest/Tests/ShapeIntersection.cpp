//
//  ShapeIntersection.cpp
//  CSTest
//  Created by Jordan Brown on 21/02/2017.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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
#include <ChilliSource/Core/Math/Geometry/ShapeIntersection.h>

#include <catch.hpp>

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------
        /// A series of tests for Shape Intersection logic.
        ///
        /// @author J Brown
        //------------------------------------------------------
        TEST_CASE("ShapeIntersection", "[Math]")
        {
            //---------------------------------------------------------
            /// Confirms that AABB-Ray intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Ray vs AABB intersection")
            {
                CS::Ray hitRay;
                CS::Ray notHitRay;
                CS::AABB aabb;
                
                aabb.SetOrigin(CS::Vector3(0.0f, 0.0f, 0.0f));
                aabb.SetSize(CS::Vector3(1.0f, 1.0f, 1.0f));
                
                hitRay.vOrigin = CS::Vector3(0.0f, 0.0f, -1.0f);
                hitRay.vDirection = CS::Vector3(0.0f, 0.0f, 1.0f);
                
                notHitRay.vOrigin = CS::Vector3(0.05, 0.0f, -1.0f);
                notHitRay.vDirection = CS::Vector3(0.0f, 0.0f, -1.0f);
                
                f32 outT1, outT2;
                
                REQUIRE(CS::ShapeIntersection::Intersects(aabb, hitRay, outT1, outT2) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(aabb, notHitRay, outT1, outT2) == false);
            }
            //---------------------------------------------------------
            /// Confirms that AABB-Point intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Point vs AABB intersection")
            {
                CS::Vector3 hitPoint(0.0f, 0.0f, 0.0f);
                CS::Vector3 notHitPoint(2.0f, 2.0f, 2.0f);
                CS::AABB aabb;
                
                aabb.SetOrigin(CS::Vector3(0.0f, 0.0f, 0.0f));
                aabb.SetSize(CS::Vector3(1.0f, 1.0f, 1.0f));


                
                REQUIRE(CS::ShapeIntersection::Intersects(aabb, hitPoint) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(aabb, notHitPoint) == false);
            }
            //---------------------------------------------------------
            /// Confirms that AABB-AABB intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("AABB vs AABB intersection")
            {
                CS::AABB aabbLHS;
                CS::AABB aabbHitRHS;
                CS::AABB aabbNotHitRHS;
                
                aabbLHS.SetOrigin(CS::Vector3(0.0f, 0.0f, 0.0f));
                aabbLHS.SetSize(CS::Vector3(1.0f, 1.0f, 1.0f));
                
                aabbHitRHS.SetOrigin(CS::Vector3(0.0f, 0.0f, 0.0f));
                aabbHitRHS.SetSize(CS::Vector3(0.5f, 0.5f, 0.5f));
                
                aabbNotHitRHS.SetOrigin(CS::Vector3(2.0f, 2.0f, 2.0f));
                aabbNotHitRHS.SetSize(CS::Vector3(0.5f, 0.5f, 0.5f));
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(aabbLHS, aabbHitRHS) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(aabbLHS, aabbNotHitRHS) == false);
            }
            //---------------------------------------------------------
            /// Confirms that Sphere-Ray intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Sphere vs Ray intersection")
            {
                CS::Ray rayHit;
                CS::Ray rayNotHit;
                CS::Sphere sphere;
              
                sphere.vOrigin = CS::Vector3(0.0f, 0.0f, 0.0f);
                sphere.fRadius = 1.0f;
   
                rayHit.vOrigin = CS::Vector3(0.0f, 0.0f, -2.0f);
                rayHit.vDirection = CS::Vector3(0.0f, 0.0f, 1.0f);
                
                rayNotHit.vOrigin = CS::Vector3(2.0f, 0.0f, -2.0f);
                rayNotHit.vDirection = CS::Vector3(0.0f, 0.0f, 1.0f);
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, rayHit) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, rayNotHit) == false);
            }
            //---------------------------------------------------------
            /// Confirms that Sphere-Point intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Sphere vs Point intersection")
            {
                CS::Vector3 hitPoint(0.0f, 0.0f, 0.0f);
                CS::Vector3 notHitPoint(2.0f, 0.0f, 0.0f);
                CS::Sphere sphere;
                
                sphere.vOrigin = CS::Vector3(0.0f, 0.0f, 0.0f);
                sphere.fRadius = 1.0f;

                
                
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, hitPoint) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, notHitPoint) == false);
            }
            //---------------------------------------------------------
            /// Confirms that Sphere-Sphere intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Sphere vs Sphere intersection")
            {
                CS::Sphere sphereLHS;
                CS::Sphere sphereHitRHS;
                CS::Sphere sphereNotHitRHS;
                
                sphereLHS.vOrigin = CS::Vector3(0.0f, 0.5f, 0.0f);
                sphereLHS.fRadius = 1.0f;
                
                sphereHitRHS.vOrigin = CS::Vector3(0.0f, 0.0f, 0.0f);
                sphereHitRHS.fRadius = 1.0f;
                
                sphereNotHitRHS.vOrigin = CS::Vector3(0.0f, 3.0f, 0.0f);
                sphereNotHitRHS.fRadius = 1.0f;
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(sphereLHS, sphereHitRHS) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(sphereLHS, sphereNotHitRHS) == false);
            }
            //---------------------------------------------------------
            /// Confirms that Sphere-Plane intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Sphere vs Plane intersection")
            {
                CS::Sphere sphere;
                CS::Plane planeInside;
                CS::Plane planeOutside;
                CS::Plane planeIntersect;
                
                sphere.vOrigin = CS::Vector3(0.0f, 0.5f, 0.0f);
                sphere.fRadius = 1.0f;
                
                planeInside.mfD = 2.0f;
                planeInside.mvNormal = CS::Vector3(0.0f, 1.0f, 0.0f);
                
                planeOutside.mfD = -2.0f;
                planeOutside.mvNormal = CS::Vector3(0.0f, 1.0f, 0.0f);
                
                planeIntersect.mfD = 0.0f;
                planeIntersect.mvNormal = CS::Vector3(0.0f, 1.0f, 0.0f);
 
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, planeInside) == CS::ShapeIntersection::Result::k_inside);
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, planeOutside) == CS::ShapeIntersection::Result::k_outside);
                REQUIRE(CS::ShapeIntersection::Intersects(sphere, planeIntersect) == CS::ShapeIntersection::Result::k_intersect);
            }
            //---------------------------------------------------------
            /// Confirms that Plane-Plane intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Plane vs Plane intersection")
            {
                CS::Plane planeLHS;
                CS::Plane planeHitRHS;
                CS::Plane planeNotHitRHS;
                CS::Ray rayResult;
                
                planeLHS.mfD = 0.0f;
                planeLHS.mvNormal = CS::Vector3(0.0f, 1.0f, 0.0f);
                
                planeHitRHS.mfD = 0.0f;
                planeHitRHS.mvNormal = CS::Vector3(1.0f, 0.0f, 0.0f);
                
                planeNotHitRHS.mfD = -1.0f;
                planeNotHitRHS.mvNormal = CS::Vector3(0.0f, -1.0f, 0.0f);
                
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(planeLHS, planeHitRHS, rayResult) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(planeLHS, planeNotHitRHS, rayResult) == false);

            }
            //---------------------------------------------------------
            /// Confirms that Circle-Circle intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Circle vs Circle intersection")
            {
                CS::Circle circleLHS;
                CS::Circle circleHitRHS;
                CS::Circle circleNotHitRHS;
          
                circleLHS.fRadius = 1.0f;
                circleLHS.vOrigin = CS::Vector2(0.0f, 0.0f);
                
                circleHitRHS.fRadius = 1.0f;
                circleHitRHS.vOrigin = CS::Vector2(0.5f, 0.0f);
                
                circleNotHitRHS.fRadius = 1.0f;
                circleNotHitRHS.vOrigin = CS::Vector2(2.0f, 2.0f);
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(circleLHS, circleHitRHS) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(circleLHS, circleNotHitRHS) == false);
                
            }
            //---------------------------------------------------------
            /// Confirms that Circle-Point intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Circle vs Point intersection")
            {
                CS::Circle circleLHS;
                CS::Vector2 pointHit(0.0f, 0.0f);
                CS::Vector2 pointNotHit(2.0f, 2.0f);
                
                circleLHS.fRadius = 1.0f;
                circleLHS.vOrigin = CS::Vector2(0.0f, 0.0f);
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(circleLHS, pointHit) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(circleLHS, pointNotHit) == false);
                
            }
            //---------------------------------------------------------
            /// Confirms that Line-Line intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Line vs Line intersection")
            {
                CS::Line lineLHS;
                CS::Line lineHitRHS;
                CS::Line lineNotHitRHS;
                CS::Vector3 hitResult;
                
                lineLHS.vStartPos = CS::Vector3(0.0f, 0.0f, 0.0f);
                lineLHS.vEndPos = CS::Vector3(1.0f, 0.0f, 0.0f);
                
                lineHitRHS.vStartPos = CS::Vector3(0.5f, -1.0f, 0.0f);
                lineHitRHS.vEndPos = CS::Vector3(0.5f, 1.0f, 0.0f);
                
                lineNotHitRHS.vStartPos = CS::Vector3(-1.0f, -1.0f, 0.0f);
                lineNotHitRHS.vEndPos = CS::Vector3(-1.0f, 1.0f, 0.0f);
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(lineLHS, lineHitRHS, hitResult) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(lineLHS, lineNotHitRHS, hitResult) == false);
                
            }
            //---------------------------------------------------------
            /// Confirms that Rect-Rect intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Rect vs Rect intersection")
            {
                CS::Rectangle rectLHS;
                CS::Rectangle rectHitRHS;
                CS::Rectangle rectNotHitRHS;
                
                rectLHS.vOrigin = CS::Vector2(0.0f, 0.0f);
                rectLHS.vSize = CS::Vector2(1.0f, 2.0f);
                
                rectHitRHS.vOrigin = CS::Vector2(0.5f, 0.0f);
                rectHitRHS.vSize = CS::Vector2(1.0f, 2.0f);
                
                rectNotHitRHS.vOrigin = CS::Vector2(2.0f, 4.0f);
                rectNotHitRHS.vSize = CS::Vector2(1.0f, 2.0f);
                
                
                REQUIRE(CS::ShapeIntersection::Intersects(rectLHS, rectHitRHS) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(rectLHS, rectNotHitRHS) == false);
                
            }
            //---------------------------------------------------------
            /// Confirms that Rect-Point intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Rect vs Point intersection")
            {
                CS::Rectangle rectLHS;
                CS::Vector2 pointHit(0.0f, 0.5f);
                CS::Vector2 pointNotHit(-1.0f, 0.5f);
                
                rectLHS.vOrigin = CS::Vector2(0.0f, 0.0f);
                rectLHS.vSize = CS::Vector2(1.0f, 2.0f);
                
                REQUIRE(CS::ShapeIntersection::Intersects(rectLHS, pointHit) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(rectLHS, pointNotHit) == false);
                
            }
            //---------------------------------------------------------
            /// Confirms that Ray-Plane intersection works.
            ///
            /// @author J Brown
            //---------------------------------------------------------
            SECTION("Ray vs Plane intersection")
            {
                CS::Plane plane;
                CS::Ray rayHit;
                CS::Ray rayNoHit;
                CS::Vector3 hitIntersection;
                
                plane.mfD = 1.0f;
                plane.mvNormal = CS::Vector3(0.0f, 1.0f, 0.0f);
                
                rayHit.vOrigin = CS::Vector3(0.0f, 0.0f, 0.0f);
                rayHit.vDirection = CS::Vector3(0.0f, -1.0f, 0.0f);
                
                rayNoHit.vOrigin = CS::Vector3(0.0f, 0.0f, 0.0f);
                rayNoHit.vDirection = CS::Vector3(0.0f, 1.0f, 0.0f);
                
                REQUIRE(CS::ShapeIntersection::Intersects(rayHit, plane, hitIntersection) == true);
                REQUIRE(CS::ShapeIntersection::Intersects(rayNoHit, plane, hitIntersection) == false);
                
            }
        }
        
    }
}
