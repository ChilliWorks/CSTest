//
//  HttpRequestTest.cpp
//  CSTest
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

#include <CSTest.h>
#include <IntegrationTest/TestSystem/TestCase.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/File.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        const std::string k_rootDirectory = "IntegrationTest/FileStream/";
        const std::string k_testTextFileName = k_rootDirectory + "TestFile.txt";
        const std::string k_testBadTextFileName = k_rootDirectory + "BadTestFile.txt";
        
        const std::string k_testTextFileFirstLine = "This is a test file.";
        const std::string k_testTextFileSecondLine = "I am on another line!";
        const std::string k_testTextFileContents = k_testTextFileFirstLine + "\n" + k_testTextFileSecondLine;
        
        namespace
        {
            /// Opens the test text file
            ///
            /// @return Input Text Stream
            ///
            static ChilliSource::IInputTextStreamUPtr OpenTestTextFile()
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                return fileSystem->CreateInputTextStream(ChilliSource::StorageLocation::k_package, k_testTextFileName);
            }
        }
        
        /// A series of integration tests to verify the functionality of an InputTextStream.
        ///
        CSIT_TESTCASE(InputTextStream)
        {
            /// Validate that a file stream can be opened
            ///
            CSIT_TEST(SuccessInputTextStreamCreate)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    CSIT_PASS();
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            
            /// Validate that a file stream created with an invalid path is handled
            ///
            CSIT_TEST(SuccessInputTextStreamCreateBadStream)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto inputFileStream = fileSystem->CreateInputTextStream(ChilliSource::StorageLocation::k_package, k_testBadTextFileName);
                
                if(inputFileStream == nullptr)
                {
                    CSIT_PASS();
                }
                else
                {
                    CSIT_FAIL("File should not exist - " + k_testBadTextFileName);
                }
            }
            
            /// Validate that the whole file can be read
            ///
            CSIT_TEST(SuccessInputTextStreamReadAll)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    auto fileContents = inputFileStream->ReadAll();
                    if(fileContents == k_testTextFileContents)
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        CSIT_FAIL("File contents do not match expected contents - " + fileContents);
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            
            /// Validate that a line can be read
            ///
            CSIT_TEST(SuccessInputTextStreamReadLine)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    std::string firstLine, secondLine, expectedFailureLine = "";
                    
                    //Try 3 readlines, first 2 should be successfull, third is expected to fail as it should be passed E.O.F.
                    auto firstLineSuccess = inputFileStream->ReadLine(firstLine);
                    auto secondLineSuccess = inputFileStream->ReadLine(secondLine);
                    auto expectedFailure = inputFileStream->ReadLine(expectedFailureLine);
                    
                    bool readsSuccessfull = firstLineSuccess && secondLineSuccess && !expectedFailure;
                    if(readsSuccessfull && firstLine == k_testTextFileFirstLine && secondLine == k_testTextFileSecondLine && expectedFailureLine.empty())
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        CSIT_FAIL("File line contents do not match expected contents.");
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            
            /// Validate that the length of the file
            /// can be read
            ///
            CSIT_TEST(SuccessInputTextStreamLength)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    auto length = inputFileStream->GetLength();
                    
                    if(length == k_testTextFileContents.length())
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        CSIT_FAIL("File length does not match expected length.");
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            
            /// Validate that the starting read position can be set
            ///
            CSIT_TEST(SuccessInputTextStreamSetReadPosition)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    std::string firstLine, secondLine;
                    
                    //Read in the two lines to set the read pointer to end of file
                    auto firstLineSuccess = inputFileStream->ReadLine(firstLine);
                    auto secondLineSuccess = inputFileStream->ReadLine(secondLine);
                    
                    //Set the read position to the beginning
                    inputFileStream->SetReadPosition(0);
                    
                    std::string reFirstLine, reSecondLine;
                    
                    //Get the first line again
                    auto reFirstLineSuccess = inputFileStream->ReadLine(reFirstLine);
                    
                    //Set the read position to after the first line, +1 so not on the newline
                    inputFileStream->SetReadPosition(k_testTextFileFirstLine.length() + 1);
                    auto reSecondLineSuccess = inputFileStream->ReadLine(reSecondLine);
                    
                    bool readsSuccessfull = firstLineSuccess && secondLineSuccess && reFirstLineSuccess && reSecondLineSuccess;
                    if(readsSuccessfull && firstLine == reFirstLine && secondLine == reSecondLine)
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        CSIT_FAIL("File line contents do not match expected contents.");
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            
            /// Validate that the starting read position can be read
            ///
            CSIT_TEST(SuccessInputTextStreamGetReadPosition)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    std::string firstLine;
                    
                    //Read in the two lines to set the read pointer to end of file
                    auto firstLineSuccess = inputFileStream->ReadLine(firstLine);
                    
                    //Get the read position
                    auto readPosition = inputFileStream->GetReadPosition();
                    
                    //Use +1 as the readpointer will not settle on the newline
                    if(firstLineSuccess && firstLine == k_testTextFileFirstLine && readPosition == k_testTextFileFirstLine.length() + 1)
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        CSIT_FAIL("File line contents do not match expected contents.");
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            
            /// Validate that the read function
            ///
            CSIT_TEST(SuccessInputTextStreamRead)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    u32 startingPos = 8;
                    u32 endingPos = (u32)k_testTextFileFirstLine.length();
					u32 length = (u32)(endingPos - startingPos);
                    
                    //Try to read the latter half of the first sentence
                    const std::string expectedResult = k_testTextFileFirstLine.substr(startingPos, endingPos);
                    std::string actualResult;
                    std::string overrunResult;
                    
                    inputFileStream->SetReadPosition(startingPos);
                    
                    //First read up to the end of the first line
                    bool firstReadSuccess = inputFileStream->Read(length, actualResult);
                    
                    //Move the starting position past the newline
                    inputFileStream->SetReadPosition(inputFileStream->GetReadPosition() + 1);
                    
                    //Then we try to read past the second line, purposefully overrunning the stream buffer, the resulting
                    //string should be the 2nd line only
                    bool overrunReadSuccess = inputFileStream->Read(k_testTextFileSecondLine.length() * 2, overrunResult);
                    
                    //Evaluate the results
                    bool normalSuccess = (firstReadSuccess && (actualResult == expectedResult));
                    bool overrunSuccess = (overrunReadSuccess && (overrunResult == k_testTextFileSecondLine));
                    
                    if(normalSuccess && overrunSuccess)
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        std::string failedString = "";
                        
                        if(!normalSuccess)
                        {
                            failedString += "Normal file read contents do not match expected contents.\n";
                        }
                        
                        if(!overrunSuccess)
                        {
                            failedString += "Overrun file read contents do not match expected contents.\n";
                        }
                        
                        CSIT_FAIL(failedString);
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
            /// Validate that the fail cases for the read function
            ///
            CSIT_TEST(SuccessInputTextStreamFailedRead)
            {
                auto inputFileStream = OpenTestTextFile();
                if(inputFileStream != nullptr)
                {
                    //The only fail case for the read is when you try to read chars from the end of the buffer
                    //so set the current position to the last position in the stream
                    inputFileStream->SetReadPosition(inputFileStream->GetLength());
                    
                    std::string failedString;
                    
                    bool failedResult = inputFileStream->Read(8, failedString);
                    if(failedResult == false && failedString.size() == 0)
                    {
                        CSIT_PASS();
                    }
                    else
                    {
                        CSIT_FAIL("Read did not handle error case as expected");
                    }
                }
                else
                {
                    CSIT_FAIL("Cannot open input stream to file - " + k_testTextFileName);
                }
            }
        }
    }
}