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
            const std::string k_testBinaryFileName = k_rootDirectory + "TestBinaryFile.bin";
            const std::string k_testBadBinaryFileName = k_rootDirectory + "BadTestBinaryFile.txt";
            
            const char k_binaryFileContents[] = "\x0F\x27\x0F\x27\x00\x00";
            const std::string k_testBinaryFileExpectedSHA1Hash = "0abb165dea1506edb46b14a8b82eb448f5593215";
            
            constexpr u32 k_binaryFileContentsSizeBytes = 6;
            constexpr u32 k_streamReadPositionLength = 4;
            constexpr u32 k_streamReadStartingPosition = 1;
            constexpr u32 k_streamReadEndingPosition = 2;
            constexpr u32 k_streamFailedReadLength = 8;
        }
        
        /// A series of integration tests to verify the functionality of an BinaryInputStream.
        ///
        CSIT_TESTCASE(BinaryInputStream)
        {
            /// Validate that a file stream can be opened
            ///
            CSIT_TEST(SuccessBinaryInputStreamCreate)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                CSIT_PASS();
            }
            
            /// Validate that a file stream created with an invalid path is handled
            ///
            CSIT_TEST(ErrorBinaryInputStreamCreateBadStream)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBadBinaryFileName);
                CSIT_ASSERT(!inputFileStream, "File should not exist: " + k_testBadBinaryFileName);
                CSIT_PASS();
            }
            
            /// Validate that the whole file can be read
            ///
            CSIT_TEST(SuccessBinaryInputStreamReadAll)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);

                auto fileContents = inputFileStream->ReadAll();
                CSIT_ASSERT(fileContents->GetLength() == k_binaryFileContentsSizeBytes, "File contents size does not match what is expected.");
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(fileContents->GetData()), k_binaryFileContents, fileContents->GetLength()) == 0, "File contents do not match expected contents.");
                CSIT_PASS();
            }

            /// Validate that the length of the file
            /// can be read
            ///
            CSIT_TEST(SuccessBinaryInputStreamLength)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream->GetLength() == k_binaryFileContentsSizeBytes, "File length does not match expected length.");
                CSIT_PASS();
            }
            
            /// Validate that the starting read position can be set
            ///
            CSIT_TEST(SuccessBinaryInputStreamSetReadPosition)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                inputFileStream->SetReadPosition(0);
                
                char expectedResult[k_streamReadPositionLength];
                memcpy(expectedResult, &k_binaryFileContents[0], k_streamReadPositionLength);
                
                auto readResult = inputFileStream->Read(k_streamReadPositionLength);
                
                CSIT_ASSERT(readResult, "Failed to read from file.");
                CSIT_ASSERT(readResult->GetLength() == k_streamReadPositionLength, "File contents size does not match what is expected.");
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(readResult->GetData()), expectedResult, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                inputFileStream->SetReadPosition(0);
                auto reReadResult = inputFileStream->Read(k_streamReadPositionLength);
                
                CSIT_ASSERT(reReadResult, "Failed to read from file.");
                CSIT_ASSERT(reReadResult->GetLength() == k_streamReadPositionLength, "File contents size does not match what is expected.");
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(reReadResult->GetData()), expectedResult, reReadResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                CSIT_PASS();
            }
            
            /// Validate that the starting read position can be read
            ///
            CSIT_TEST(SuccessBinaryInputStreamGetReadPosition)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                auto readResult = inputFileStream->Read(k_streamReadPositionLength);
                
                CSIT_ASSERT(readResult, "Failed to read from file.");
                CSIT_ASSERT(readResult->GetLength() == k_streamReadPositionLength, "File contents size does not match what is expected.");
                
                auto readPosition = inputFileStream->GetReadPosition();
                
                CSIT_ASSERT(readPosition == k_streamReadPositionLength, "Read position is not in the expected place.");
                CSIT_PASS();
            }
            
            /// Validate the read function
            ///
            CSIT_TEST(SuccessBinaryInputStreamRead)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                u32 startingPos = k_streamReadStartingPosition;
                u32 endingPos = k_streamReadEndingPosition;
				u32 length = u32(endingPos - startingPos);
                
				char* expectedResult = new char[length];
                memcpy(expectedResult, &k_binaryFileContents[startingPos], length);
                inputFileStream->SetReadPosition(startingPos);
                auto readResult = inputFileStream->Read(length);
                
                CSIT_ASSERT(readResult, "Failed to carry out read.");
                CSIT_ASSERT(readResult->GetLength() == length, "File contents size does not match what is expected.");
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(readResult->GetData()), expectedResult, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                inputFileStream->SetReadPosition(inputFileStream->GetReadPosition());
                u64 remainingLength = inputFileStream->GetLength() - inputFileStream->GetReadPosition();
                char* overrunExpectedResult = new char[remainingLength];
                memcpy(overrunExpectedResult, &k_binaryFileContents[inputFileStream->GetReadPosition()], remainingLength);
                auto overrunreadResult = inputFileStream->Read(k_streamFailedReadLength);
                
                CSIT_ASSERT(overrunreadResult, "Failed to carry out overrun read.");
                CSIT_ASSERT(overrunreadResult->GetLength() == remainingLength, "File contents size does not match what is expected.");
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(overrunreadResult->GetData()), overrunExpectedResult, overrunreadResult->GetLength()) == 0, "File contents do not match expected contents.");

				CS_SAFEDELETE_ARRAY(expectedResult);
				CS_SAFEDELETE_ARRAY(overrunExpectedResult);

                CSIT_PASS();
            }
            
            /// Validate the fail cases for the read function
            ///
            CSIT_TEST(ErrorBinaryInputStreamFailedRead)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto inputFileStream = fileSystem->CreateBinaryInputStream(CS::StorageLocation::k_package, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                inputFileStream->SetReadPosition(inputFileStream->GetLength());
                auto readResult = inputFileStream->Read(k_streamFailedReadLength);
                
                CSIT_ASSERT(!readResult, "Read did not handle error case as expected, operation should have failed.");
                CSIT_PASS();
            }
        }
    }
}