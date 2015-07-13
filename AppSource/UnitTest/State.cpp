//
//  State.cpp
//  CSTest
//  Created by Ian Copland on 15/10/2014.
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

#include <UnitTest/State.h>

#define CATCH_CONFIG_COLOUR_NONE
#define CATCH_CONFIG_NOSTDOUT
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>


TEST_CASE("A random test", "[Test]")
{
    REQUIRE(1 == 1);
    REQUIRE(2 == 2);
    REQUIRE(0 == 0);
}

TEST_CASE("Another random test", "[Test]")
{
    REQUIRE(1 == 1);
    REQUIRE(1 == 0);
    REQUIRE(0 == 0);
}

//------------------------------------------------------------------------------
/// TODO: !? This stuff should be replaced by a custom reporter. We want on
/// screen confirmation of success / failure and this won't give that.
//------------------------------------------------------------------------------
namespace Catch
{
    std::stringstream g_outputStream;
    std::ostream& cout()
    {
        return g_outputStream;
    }
    
    std::stringstream g_errorStream;
    std::ostream& cerr()
    {
        return g_errorStream;
    }
}

namespace CSTest
{
    namespace UnitTest
    {
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::CreateSystems()
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnInit()
        {
            int result = Catch::Session().run();
            if (result != 0)
            {
                //CS_LOG_FATAL("There was an error running Catch.");
            }
            
            if (Catch::g_errorStream.str().empty() == false)
            {
                CS_LOG_FATAL(Catch::g_errorStream.str());
            }
            
            CS_LOG_VERBOSE(Catch::g_outputStream.str());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnUpdate(f32 in_deltaTime)
        {
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void State::OnDestroy()
        {
        }
    }
}
