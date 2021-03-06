//
//  HttpRequestTest.cpp
//  CSTest
//  Created by Ian Copland on 16/03/2016.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <IntegrationTest/TestSystem/TestCase.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Networking/Http.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        //------------------------------------------------------------------------------
        /// A series of integration tests for the HTTP request system.
        ///
        /// Note that these tests require a stable internet connection.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        CSIT_TESTCASE(HttpRequest)
        {
            //------------------------------------------------------------------------------
            /// Tests that basic HTTPS GET requests can be successfully made.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessHttpsGet)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                httpRequestSystem->MakeGetRequest("https://httpbin.org/get", [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
                {
                    CSIT_ASSERT(in_response.GetResult() == CS::HttpResponse::Result::k_completed, "The HTTP request failed.");
                    CSIT_ASSERT(in_response.GetCode() == 200, "Recieved incorrect response code. " + CS::ToString(in_response.GetCode()));
                    
                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a HTTPS GET request with headers can be successfully made.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessHttpsGetHeaders)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                CS::ParamDictionary headers;
                headers["Example-Header"] = "ExampleValue";
                
                httpRequestSystem->MakeGetRequest("https://httpbin.org/headers", headers, [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
                {
                    CSIT_ASSERT(in_response.GetResult() == CS::HttpResponse::Result::k_completed, "The HTTP request failed.");
                    CSIT_ASSERT(in_response.GetCode() == 200, "Recieved incorrect response code. " + CS::ToString(in_response.GetCode()));

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a HTTPS GET request can receive error response codes.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessHttpsGetResponseCode)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                httpRequestSystem->MakeGetRequest("https://httpbin.org/status/418", [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
                {
                    CSIT_ASSERT(in_response.GetResult() == CS::HttpResponse::Result::k_completed, "The HTTP request failed.");
                    CSIT_ASSERT(in_response.GetCode() == 418, "Recieved incorrect response code. " + CS::ToString(in_response.GetCode()));

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that basic HTTPS POST requests can be successfully made.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessHttpsPost)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                httpRequestSystem->MakePostRequest("https://httpbin.org/post", "SomeData", [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
                {
                    CSIT_ASSERT(in_response.GetResult() == CS::HttpResponse::Result::k_completed, "The HTTP request failed.");
                    CSIT_ASSERT(in_response.GetCode() == 200, "Recieved incorrect response code. " + CS::ToString(in_response.GetCode()));

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a HTTPS POST request with headers can be successfully made.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessHttpsPostHeaders)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                CS::ParamDictionary headers;
                headers["Example-Header"] = "ExampleValue";
                
                httpRequestSystem->MakePostRequest("https://httpbin.org/post", "SomeData", headers, [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
                {
                    CSIT_ASSERT(in_response.GetResult() == CS::HttpResponse::Result::k_completed, "The HTTP request failed.");
                    CSIT_ASSERT(in_response.GetCode() == 200, "Recieved incorrect response code. " + CS::ToString(in_response.GetCode()));

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Tests that a HTTPS POST request can receive error response codes.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessHttpsPostReponseCode)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                httpRequestSystem->MakePostRequest("https://httpbin.org/status/418", "SomeData", [=](const CS::HttpRequest* in_request, const CS::HttpResponse& in_response)
                {
                    CSIT_ASSERT(in_response.GetResult() == CS::HttpResponse::Result::k_completed, "The HTTP request failed.");
                    CSIT_ASSERT(in_response.GetCode() == 418, "Recieved incorrect response code. " + CS::ToString(in_response.GetCode()));

                    CSIT_PASS();
                });
            }
            //------------------------------------------------------------------------------
            /// Confirms that reachability works as intended.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(SuccessReachability)
            {
                auto httpRequestSystem = CS::Application::Get()->GetSystem<CS::HttpRequestSystem>();
                if(httpRequestSystem == nullptr)
                {
                    CSIT_FAIL("No HTTP system exists");
                    return;
                }
                
                httpRequestSystem->CheckReachability([=](bool isReachable)
                {
                    CSIT_ASSERT(isReachable, "Reachability check failed.");
                    CSIT_PASS();
                });
            }
        }
    }
}
