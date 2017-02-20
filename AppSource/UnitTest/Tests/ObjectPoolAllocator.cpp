//
// The MIT License(MIT)
//
// Copyright(c) 2017 Tag Games Ltd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <CSTest.h>

#include <ChilliSource/Core/Memory.h>

#include <catch.hpp>

namespace CSTest
{
    namespace UnitTest
    {
        namespace
        {
            constexpr u32 k_defaultNumObjects = 5;
        }
        
        /// A series of tests for the ObjectPool
        ///
        TEST_CASE("ObjectPoolAllocator", "[Allocator]")
        {
            /// Confirms that a raw pointer to a fundamental can be allocated from an ObjectPool.
            ///
            SECTION("RawFundamental")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto allocated = pool.Allocate();
                *allocated = 1;
                
                REQUIRE(*allocated == 1);
            }
            
            /// Confirms that a raw pointer to a fundamental can be deallocated using the direct interface
            ///
            SECTION("RawDeallocate")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto allocated1 = pool.Allocate();
                *allocated1 = 1;
                pool.Deallocate(allocated1);
                
                auto allocated2 = pool.Allocate();
                *allocated2 = 2;
                
                REQUIRE(*allocated2 == 2);
            }
            
            /// Confirms that a unique pointer to a fundamental can be allocated from an ObjectPool.
            ///
            SECTION("UniqueFundamental")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeUnique<int>(pool);
                *allocated = 1;
                
                REQUIRE(*allocated == 1);
            }
            
            /// Confirms that a unique pointer to a fundamental with an initial value can be allocated from an ObjectPool.
            ///
            SECTION("UniqueFundamentalInitialValue")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeUnique<int>(pool, 1);
                
                REQUIRE(*allocated == 1);
            }
            
            /// Confirms that a unique pointer to a struct instance can be allocated from an ObjectPool.
            ///
            SECTION("UniqueStruct")
            {
                struct ExampleClass
                {
                    int m_x, m_y;
                };
                
                CS::ObjectPoolAllocator<ExampleClass> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeUnique<ExampleClass>(pool);
                allocated->m_x = 1;
                allocated->m_y = 2;
                
                REQUIRE(allocated->m_x == 1);
                REQUIRE(allocated->m_y == 2);
            }
            
            /// Confirms that a unique pointer to a struct instance with a constructor can be allocated from an ObjectPool.
            ///
            SECTION("UniqueStructConstructor")
            {
                struct ExampleClass
                {
                    ExampleClass(int x, int y) : m_x(x), m_y(y) {}
                    int m_x, m_y;
                };
                
                CS::ObjectPoolAllocator<ExampleClass> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeUnique<ExampleClass>(pool, 1, 2);
                
                REQUIRE(allocated->m_x == 1);
                REQUIRE(allocated->m_y == 2);
            }
            
            /// Confirms that a unique pointer to a struct instance can be copy constructed from an ObjectPool.
            ///
            SECTION("UniqueStructCopyConstructor")
            {
                struct ExampleClass
                {
                    int m_x, m_y;
                };
                
                ExampleClass exampleClass;
                exampleClass.m_x = 1;
                exampleClass.m_y = 2;
                
                CS::ObjectPoolAllocator<ExampleClass> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeUnique<ExampleClass>(pool, exampleClass);
                
                REQUIRE(allocated->m_x == 1);
                REQUIRE(allocated->m_y == 2);
            }
            
            /// Confirms that a shared pointer to a fundamental can be allocated from an ObjectPool.
            ///
            SECTION("SharedFundamental")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeShared<int>(pool);
                *allocated = 1;
                
                REQUIRE(*allocated == 1);
            }
            
            /// Confirms that a shared pointer to a fundamental with an initial value can be allocated from an ObjectPool.
            ///
            SECTION("SharedFundamentalInitialValue")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeShared<int>(pool, 1);
                
                REQUIRE(*allocated == 1);
            }
            
            /// Confirms that a shared pointer to a struct instance can be allocated from an ObjectPool.
            ///
            SECTION("SharedStruct")
            {
                struct ExampleClass
                {
                    int m_x, m_y;
                };
                
                CS::ObjectPoolAllocator<ExampleClass> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeShared<ExampleClass>(pool);
                allocated->m_x = 1;
                allocated->m_y = 2;
                
                REQUIRE(allocated->m_x == 1);
                REQUIRE(allocated->m_y == 2);
            }
            
            /// Confirms that a shared pointer to a struct instance with a constructor can be allocated from an ObjectPool.
            ///
            SECTION("SharedStructConstructor")
            {
                struct ExampleClass
                {
                    ExampleClass(int x, int y) : m_x(x), m_y(y) {}
                    int m_x, m_y;
                };
                
                CS::ObjectPoolAllocator<ExampleClass> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeShared<ExampleClass>(pool, 1, 2);
                
                REQUIRE(allocated->m_x == 1);
                REQUIRE(allocated->m_y == 2);
            }
            
            /// Confirms that a shared pointer to a struct instance can be copy constructed from an ObjectPool.
            ///
            SECTION("SharedStructCopyConstructor")
            {
                struct ExampleClass
                {
                    int m_x, m_y;
                };
                
                ExampleClass exampleClass;
                exampleClass.m_x = 1;
                exampleClass.m_y = 2;
                
                CS::ObjectPoolAllocator<ExampleClass> pool(k_defaultNumObjects);
                
                auto allocated = CS::MakeShared<ExampleClass>(pool, exampleClass);
                
                REQUIRE(allocated->m_x == 1);
                REQUIRE(allocated->m_y == 2);
            }
            
            /// Confirms that multiple objects can be allocated from an ObjectPool.
            ///
            SECTION("MultipleObjects")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto valueA = CS::MakeUnique<int>(pool, 1);
                auto valueB = CS::MakeUnique<int>(pool, 2);
                auto valueC = CS::MakeUnique<int>(pool, 3);
                
                REQUIRE(*valueA == 1);
                REQUIRE(*valueB == 2);
                REQUIRE(*valueC == 3);
            }
            
            /// Confirms that deallocating an object allocated from an ObjectPool does not affect other allocations.
            ///
            SECTION("Deallocation")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                
                auto valueA = CS::MakeUnique<int>(pool, 1);
                auto valueB = CS::MakeUnique<int>(pool, 2);
                valueB.reset();
                auto valueC = CS::MakeUnique<int>(pool, 3);
                valueB = CS::MakeUnique<int>(pool, 4);
                
                REQUIRE(*valueA == 1);
                REQUIRE(*valueB == 4);
                REQUIRE(*valueC == 3);
            }
            
            /// Confirms that the pool can handle multiple allocations and deallocations beyond the capacity
            ///
            SECTION("Fixed")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects);
                std::vector<std::unique_ptr<int>> allocated;
                
                for(u32 i=0; i<k_defaultNumObjects; ++i)
                {
                    allocated.push_back(CS::MakeUnique<int>(pool, i));
                }
                
                allocated.clear();
                
                auto allocated1 = CS::MakeUnique<int>(pool, -1);
                REQUIRE(*allocated1 == -1);
            }
            
            /// Confirms that the pool can expand beyond the initial capacity
            ///
            SECTION("Expand")
            {
                CS::ObjectPoolAllocator<int> pool(k_defaultNumObjects, CS::ObjectPool<int>::LimitPolicy::k_expand);
                std::vector<std::unique_ptr<int>> allocated;
                
                for(u32 i=0; i<k_defaultNumObjects * 2; ++i)
                {
                    allocated.push_back(CS::MakeUnique<int>(pool, i));
                }
                
                for(u32 i=0; i<allocated.size(); ++i)
                {
                    REQUIRE(*allocated[i] == i);
                }
            }
        }
    }
}
