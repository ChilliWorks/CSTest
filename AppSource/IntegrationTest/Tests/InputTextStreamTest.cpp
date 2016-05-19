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
        namespace
        {
            const std::string k_rootDirectory = "IntegrationTest/FileStream/";
            const std::string k_testTextFileName = k_rootDirectory + "TestFile.txt";
            const std::string k_testBadTextFileName = k_rootDirectory + "BadTestFile.txt";
            
            const std::string k_testTextFileFirstLine = "This is a test file.";
            const std::string k_testTextFileSecondLine = "I am on another line!";
            const std::string k_testTextFileContents = k_testTextFileFirstLine + "\n" + k_testTextFileSecondLine;
            
            constexpr u32 k_streamReadStartingPosition = 8;
            constexpr u32 k_streamFailedReadLength = 8;
            
            /// Opens the test text file
            ///
            /// @return Input Text Stream
            ///
            CS::IInputTextStreamUPtr OpenTestTextFile()
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                return fileSystem->CreateInputTextStream(CS::StorageLocation::k_package, k_testTextFileName);
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
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                CSIT_PASS();
            }
            
            /// Validate that a file stream created with an invalid path is handled
            ///
            CSIT_TEST(SuccessInputTextStreamCreateBadStream)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto inputFileStream = fileSystem->CreateInputTextStream(CS::StorageLocation::k_package, k_testBadTextFileName);
                CSIT_ASSERT(!inputFileStream, "File should not exist: " + k_testBadTextFileName);
                CSIT_PASS();
            }
            
            /// Validate that the whole file can be read
            ///
            CSIT_TEST(SuccessInputTextStreamReadAll)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                auto fileContents = inputFileStream->ReadAll();
                CSIT_ASSERT(fileContents == k_testTextFileContents, "File contents do not match expected contents: " + fileContents);
                CSIT_PASS();
            }
            
            /// Validate that a line can be read
            ///
            CSIT_TEST(SuccessInputTextStreamReadLine)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                std::string firstLine, secondLine, expectedFailureLine = "";
                
                CSIT_ASSERT(inputFileStream->ReadLine(firstLine), "Failed to read first line.");
                CSIT_ASSERT(firstLine == k_testTextFileFirstLine, "First line does not match expected line.");
                CSIT_ASSERT(inputFileStream->ReadLine(secondLine), "Failed to read second line.");
                CSIT_ASSERT(secondLine == k_testTextFileSecondLine, "Second line does not match expected line.");
                CSIT_ASSERT(!inputFileStream->ReadLine(expectedFailureLine), "Should not have been able to read a third line.");
                CSIT_ASSERT(expectedFailureLine.empty(), "Failure line should be empty.");
                CSIT_PASS();
            }
            
            /// Validate the fail cases for the readLine function
            ///
            CSIT_TEST(SuccessInputTextStreamFailedReadLine)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                inputFileStream->SetReadPosition(inputFileStream->GetLength());
                std::string failedString;
                
                CSIT_ASSERT(!inputFileStream->ReadLine(failedString), "Read did not handle error case as expected, operation should have failed.");
                CSIT_ASSERT(failedString.empty(), "Failed string should be empty");
                CSIT_PASS();
            }
            
            /// Validate that the length of the file
            /// can be read
            ///
            CSIT_TEST(SuccessInputTextStreamLength)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                CSIT_ASSERT(inputFileStream->GetLength() == k_testTextFileContents.length(), "File length does not match expected length.");
                CSIT_PASS();
            }
            
            /// Validate that the starting read position can be set
            ///
            CSIT_TEST(SuccessInputTextStreamSetReadPosition)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                std::string firstLine, secondLine;
                std::string reFirstLine, reSecondLine;
                
                CSIT_ASSERT(inputFileStream->ReadLine(firstLine), "Failed to read first line.");
                CSIT_ASSERT(firstLine == k_testTextFileFirstLine, "First line does not match expected line.");
                CSIT_ASSERT(inputFileStream->ReadLine(secondLine), "Failed to read second line.");
                CSIT_ASSERT(secondLine == k_testTextFileSecondLine, "Second line does not match expected line.");
                
                inputFileStream->SetReadPosition(0);
                
                CSIT_ASSERT(inputFileStream->ReadLine(reFirstLine), "Failed to re-read first line.");
                CSIT_ASSERT(reFirstLine == firstLine, "Re-read of first line does not match!");
                
                inputFileStream->SetReadPosition(k_testTextFileFirstLine.length() + 1);
                
                CSIT_ASSERT(inputFileStream->ReadLine(reSecondLine), "Failed to re-read second line.");
                CSIT_ASSERT(reSecondLine == secondLine, "Re-read of second line does not match!");
                CSIT_PASS();
            }
            
            /// Validate that the starting read position can be read
            ///
            CSIT_TEST(SuccessInputTextStreamGetReadPosition)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                std::string firstLine;
                
                CSIT_ASSERT(inputFileStream->ReadLine(firstLine), "Failed to read first line.");
                CSIT_ASSERT(firstLine == k_testTextFileFirstLine, "First line does not match expected line.");
                
                auto readPosition = inputFileStream->GetReadPosition();
                
                CSIT_ASSERT(readPosition == k_testTextFileFirstLine.length() + 1, "Read position is not in the expected place.");
                CSIT_PASS();
            }
            
            /// Validate the read function
            ///
            CSIT_TEST(SuccessInputTextStreamRead)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                u32 startingPos = k_streamReadStartingPosition;
                u32 endingPos = u32(k_testTextFileFirstLine.length());
				u32 length = u32(endingPos - startingPos);
                
                const std::string expectedResult = k_testTextFileFirstLine.substr(startingPos, endingPos);
                std::string actualResult;
                std::string overrunResult;
                
                inputFileStream->SetReadPosition(startingPos);
                
                CSIT_ASSERT(inputFileStream->Read(length, actualResult), "Failed to carry out read.");
                CSIT_ASSERT(actualResult == expectedResult, "Read result does not match expected result.");
                
                inputFileStream->SetReadPosition(inputFileStream->GetReadPosition() + 1);
                
                CSIT_ASSERT(inputFileStream->Read(k_testTextFileSecondLine.length() * 2, overrunResult), "Failed to carry out overrun read.");
                CSIT_ASSERT(overrunResult == k_testTextFileSecondLine, "Overrun read result does not match expected result.");
                CSIT_PASS();
            }
            
            /// Validate the fail cases for the read function
            ///
            CSIT_TEST(SuccessInputTextStreamFailedRead)
            {
                auto inputFileStream = OpenTestTextFile();
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testTextFileName);
                
                inputFileStream->SetReadPosition(inputFileStream->GetLength());
                std::string failedString;
                
                CSIT_ASSERT(!inputFileStream->Read(k_streamFailedReadLength, failedString), "Read did not handle error case as expected, operation should have failed.");
                CSIT_ASSERT(failedString.empty(), "Failed string should be empty");
                CSIT_PASS();
            }
        }
    }
}