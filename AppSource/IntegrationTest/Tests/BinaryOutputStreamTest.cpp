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
            
            constexpr int k_podData1 = 1;
            constexpr float k_podData2 = 2.5f;
            constexpr bool k_podData3 = true;
            constexpr CS::StorageLocation k_podData4 = CS::StorageLocation::k_saveData;
            
            constexpr CS::StorageLocation k_storageLocation = CS::StorageLocation::k_saveData;
            constexpr u32 k_binaryFileContentsSizeBytes = 6;
            
            /// Simple class to test writing/reading a instance pointers data to/from file
            ///
            class TestPODClass
            {
            public:
                /// @param testInt
                /// @param testFloat
                /// @param testBool
                ///
                TestPODClass(int testInt, float testFloat, bool testBool)
                :m_testInt(testInt)
                ,m_testFloat(testFloat)
                ,m_testBool(testBool)
                {
                }
                
                /// @param testInt
                /// @param testFloat
                /// @param testBool
                ///
                /// @return If the contents match the params
                ///
                bool ContentsEqual(int testInt, float testFloat, bool testBool)
                {
                    return testInt == m_testInt && m_testFloat == testFloat && m_testBool == testBool;
                }
                
            private:
                int m_testInt;
                float m_testFloat;
                bool m_testBool;
            };
            
            /// Simple struct to test writing/reading a struct that contains an instance pointer
            ///
            struct TestPODStruct
            {
                TestPODClass* m_data;
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
            CSIT_TEST(SuccessBinaryOutputStreamCreate)
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                const auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                CSIT_PASS();
            }
            
            /// Validate that single buffer of contents can be written to file
            ///
            CSIT_TEST(SuccessBinaryOutputStreamWriteRawData)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                outputFileStream->Write((void*)k_binaryFileContents, k_binaryFileContentsSizeBytes);
                outputFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                auto readResult = inputFileStream->ReadAll();
                
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(readResult->GetData()), k_binaryFileContents, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that single buffer of contents can be written to file
            ///
            CSIT_TEST(SuccessBinaryOutputStreamWriteByteBuffer)
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
                
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(readResult->GetData()), k_binaryFileContents, readResult->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that single buffer of contents can be written to file
            ///
            CSIT_TEST(SuccessBinaryOutputStreamWritePOD)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                TestPODClass* dataInstance = new TestPODClass(k_podData1, k_podData2, k_podData3);
                TestPODStruct dataStruct;
                dataStruct.m_data = dataInstance;
                
                outputFileStream->Write(k_podData1);
                outputFileStream->Write(k_podData2);
                outputFileStream->Write(k_podData3);
                outputFileStream->Write(k_podData4);
                outputFileStream->Write<TestPODClass*>(dataInstance);
                outputFileStream->Write<TestPODStruct>(dataStruct);
                outputFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                CSIT_ASSERT(inputFileStream->Read<decltype(k_podData1)>() == k_podData1, "Could not read first POD Type correctly");
                CSIT_ASSERT(inputFileStream->Read<decltype(k_podData2)>() == k_podData2, "Could not read second POD Type correctly");
                CSIT_ASSERT(inputFileStream->Read<decltype(k_podData3)>() == k_podData3, "Could not read third POD Type correctly");
                CSIT_ASSERT(inputFileStream->Read<decltype(k_podData4)>() == k_podData4, "Could not read fourth POD Type correctly");
                
                TestPODClass* resultInstance = inputFileStream->Read<TestPODClass*>();
                CSIT_ASSERT(resultInstance->ContentsEqual(k_podData1, k_podData2, k_podData3), "Could not read fifth POD Type correctly");
                TestPODStruct resultStruct = inputFileStream->Read<TestPODStruct>();
                CSIT_ASSERT(resultStruct.m_data && resultStruct.m_data->ContentsEqual(k_podData1, k_podData2, k_podData3), "Could not read sixth POD Type correctly");

                CS_SAFEDELETE(dataInstance);
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
            
            /// Validate that contents of a larger size can be overwritten with shorter
            /// data without remnants of the first remaining
            ///
            CSIT_TEST(SuccessBinaryOutputStreamOverwrite)
            {
                ClearTestDirectory();
                
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                auto outputFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                outputFileStream->Write((void*)k_binaryFileContents, k_binaryFileContentsSizeBytes);
                outputFileStream->Write((void*)k_binaryFileContents, k_binaryFileContentsSizeBytes);
                outputFileStream = nullptr;
                
                auto overwriteFileStream = fileSystem->CreateBinaryOutputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(overwriteFileStream, "Cannot open output stream to file: " + k_testBinaryFileName);
                
                overwriteFileStream->Write((void*)k_binaryFileContents, k_binaryFileContentsSizeBytes);
                overwriteFileStream = nullptr;
                
                auto inputFileStream = fileSystem->CreateBinaryInputStream(k_storageLocation, k_testBinaryFileName);
                CSIT_ASSERT(inputFileStream, "Cannot open input stream to file: " + k_testBinaryFileName);
                
                auto contents = inputFileStream->ReadAll();
                
                CSIT_ASSERT(contents->GetLength() == k_binaryFileContentsSizeBytes, "File size does not match expected size!");
                CSIT_ASSERT(strncmp(reinterpret_cast<const s8*>(contents->GetData()), k_binaryFileContents, contents->GetLength()) == 0, "File contents do not match expected contents.");
                
                ClearTestDirectory();
                
                CSIT_PASS();
            }
        }
    }
}