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
            
            constexpr char k_binaryFileContents[] = "\x0F\x27\x0F\x27\x00\x00";
            
            constexpr int k_intData = 1;
            constexpr float k_floatData = 2.5f;
            constexpr bool k_boolData = true;
            
            constexpr CS::StorageLocation k_storageLocation = CS::StorageLocation::k_saveData;
            constexpr u32 k_binaryFileContentsSizeBytes = 6;
            
            /// Simple class to test writing/reading a classes data
            ///
            class DataClass
            {
            public:
                int m_data;
            };
            
            /// Simple class to test writing/reading a structs data
            ///
            struct DataStruct
            {
                int m_data;
            };
            
            /// Clears all written information from the test directory
            ///
            void ClearTestDirectory()
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                CS_ASSERT(fileSystem->CreateDirectoryPath(k_storageLocation, k_rootDirectory), "Could not create directory!");
                
                if(fileSystem->DoesFileExist(k_storageLocation, k_testBinaryFileName))
                {
                    fileSystem->DeleteFile(k_storageLocation, k_testBinaryFileName);
                }
            }
        }
        
        /// A series of integration tests to verify the functionality of an BinaryOutputStream.
        ///
        CSIT_TESTCASE(BinaryOutputStream)
        {
            /// Validate that a file stream can be opened
            ///
            CSIT_TEST(SuccessStreamCreate)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }

            
            /// Validate that single buffer of contents can be written to file
            ///
            CSIT_TEST(SuccessWriteRawData)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                outputFileStream->Write(reinterpret_cast<const u8*>(k_binaryFileContents), k_binaryFileContentsSizeBytes);
                outputFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                auto readResult = inputFileStream->ReadAll();
                
                CSIT_ASSERT(memcmp(reinterpret_cast<const s8*>(readResult->GetData()), k_binaryFileContents, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that single buffer of contents can be written to file
            ///
            CSIT_TEST(SuccessWriteByteBuffer)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                //Copy data, as UPtr will try to destroy it
                s8* data = new s8[k_binaryFileContentsSizeBytes];
                memcpy(data, k_binaryFileContents, k_binaryFileContentsSizeBytes);
                
                std::unique_ptr<const u8> uniqueData(reinterpret_cast<const u8*>(data));
                auto byteBuffer = CS::ByteBufferUPtr(new CS::ByteBuffer(std::move(uniqueData), k_binaryFileContentsSizeBytes));
                
                outputFileStream->Write(byteBuffer);
                outputFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                auto readResult = inputFileStream->ReadAll();
                
                CSIT_ASSERT(memcmp(reinterpret_cast<const s8*>(readResult->GetData()), k_binaryFileContents, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that the contents of a number of data types can be written to file
            ///
            CSIT_TEST(SuccessWritePOD)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                DataClass dataInstance;
                dataInstance.m_data = k_intData;
                DataStruct dataStruct;
                dataStruct.m_data = k_intData;
                
                outputFileStream->Write(k_intData);
                outputFileStream->Write(k_floatData);
                outputFileStream->Write(k_boolData);
                outputFileStream->Write<DataClass>(dataInstance);
                outputFileStream->Write<DataStruct>(dataStruct);
                outputFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                CSIT_ASSERT(inputFileStream->Read<decltype(k_intData)>() == k_intData, "Could not read first data Type correctly");
                CSIT_ASSERT(inputFileStream->Read<decltype(k_floatData)>() == k_floatData, "Could not read second data Type correctly");
                CSIT_ASSERT(inputFileStream->Read<decltype(k_boolData)>() == k_boolData, "Could not read third data Type correctly");
                
                DataClass resultInstance = inputFileStream->Read<DataClass>();
                CSIT_ASSERT(resultInstance.m_data == k_intData, "Could not read fourth data Type correctly");
                DataStruct resultStruct = inputFileStream->Read<DataStruct>();
                CSIT_ASSERT(resultStruct.m_data == k_intData, "Could not read fifth data Type correctly");

                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that contents of a file can be appended to
            ///
            CSIT_TEST(SuccessAppend)
            {
                ClearTestDirectory();
            
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName, CS::FileWriteMode::k_overwrite);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);

                outputFileStream->Write(reinterpret_cast<const u8*>(k_binaryFileContents), k_binaryFileContentsSizeBytes);
                outputFileStream = nullptr;
                
                outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName, CS::FileWriteMode::k_append);
                outputFileStream->Write(reinterpret_cast<const u8*>(k_binaryFileContents), k_binaryFileContentsSizeBytes);
                outputFileStream = nullptr;
                
                
                s8 combinedResults[k_binaryFileContentsSizeBytes * 2];
                memcpy(combinedResults, k_binaryFileContents, k_binaryFileContentsSizeBytes);
                memcpy(&combinedResults[k_binaryFileContentsSizeBytes], k_binaryFileContents, k_binaryFileContentsSizeBytes);

                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                auto readResult = inputFileStream->ReadAll();
                
                CSIT_ASSERT(memcmp(reinterpret_cast<const s8*>(readResult->GetData()), combinedResults, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that contents of a larger size can be overwritten with shorter
            /// data without remnants of the first remaining
            ///
            CSIT_TEST(SuccessOverwrite)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                outputFileStream->Write(reinterpret_cast<const u8*>(k_binaryFileContents), k_binaryFileContentsSizeBytes);
                outputFileStream->Write(reinterpret_cast<const u8*>(k_binaryFileContents), k_binaryFileContentsSizeBytes);
                outputFileStream = nullptr;
                
                auto overwriteFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(overwriteFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                overwriteFileStream->Write(reinterpret_cast<const u8*>(k_binaryFileContents), k_binaryFileContentsSizeBytes);
                overwriteFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                auto contents = inputFileStream->ReadAll();
                
                CSIT_ASSERT(contents->GetLength() == k_binaryFileContentsSizeBytes, "File size does not match expected size!");
                CSIT_ASSERT(memcmp(reinterpret_cast<const s8*>(contents->GetData()), k_binaryFileContents, contents->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
        }
    }
}