//
//  FileSystem.cpp
//  CSTest
//  Created by Ian Copland on 13/07/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Container.h>
#include <ChilliSource/Core/File.h>

#include <catch.hpp>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            //------------------------------------------------------------------------------
            /// Reads a binary file and returns the first 4 bytes as an unsigned integer. If
            /// the file cannot be read 0 will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param in_storageLocation - The storage location
            /// @param in_filePath - The file path
            ///
            /// @return The first 4 bytes as an unsigned integer, or 0
            //------------------------------------------------------------------------------
            u32 ReadBinaryFileTest(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                auto fileStream = fileSystem->CreateFileStream(in_storageLocation, in_filePath, CSCore::FileMode::k_readBinary);
                if (fileStream.get() == nullptr)
                {
                    return 0;
                }
                
                fileStream->SeekG(0, CSCore::SeekDir::k_end);
                u32 length = fileStream->TellG();
                if (length < 4)
                {
                    return 0;
                }
                
                u32 output = 0;
                fileStream->SeekG(0, CSCore::SeekDir::k_beginning);
                fileStream->Read(reinterpret_cast<s8*>(&output), 4);
                
                return output;
            }
            //------------------------------------------------------------------------------
            /// Clears out the given directory. If the directory couldn't be cleared out
            /// (i.e a previous version couldn't be deleted, or a new one couldn't be
            /// created) then this will assert.
            ///
            /// @author Ian Copland
            ///
            /// @param in_storageLocation - The storage location of the directory.
            /// @param in_directoryPath - The directory which should be cleared.
            //------------------------------------------------------------------------------
            void ClearDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                CS_ASSERT(fileSystem->IsStorageLocationWritable(in_storageLocation) == true, "Cannot clear directory in non-writable storage location.");
                
                if ((in_storageLocation == CSCore::StorageLocation::k_DLC && fileSystem->DoesDirectoryExistInCachedDLC(in_directoryPath) == true) ||
                    (in_storageLocation != CSCore::StorageLocation::k_DLC && fileSystem->DoesDirectoryExist(in_storageLocation, in_directoryPath) == true))
                {
                    bool deleted = fileSystem->DeleteDirectory(in_storageLocation, in_directoryPath);
                    CS_ASSERT(deleted == true, "Cannot perform integration tests because test directory couldn't be deleted.");
                }
                
                
                bool created = fileSystem->CreateDirectoryPath(in_storageLocation, in_directoryPath);
                CS_ASSERT(created == true, "Cannot perform integration tests because test directory couldn't be created.");
            }
            
            //------------------------------------------------------------------------------
            /// Performs a series of tests on the given storage location. This must be
            /// called from within a test case.
            ///
            /// @author Ian Copland
            ///
            /// @param in_storageLocation - The storage location to perform the tests on
            //------------------------------------------------------------------------------
            void TestWritableStorageLocation(CSCore::StorageLocation in_storageLocation)
            {
                const std::string k_integrationTestDirectory = "IntegrationTest/FileSystem/";
                const std::string k_textFilePath = k_integrationTestDirectory + "TextFileZ.txt";
                const std::string k_binFilePath = k_integrationTestDirectory + "BinaryFileZ.bin";
                const std::string k_directoryPath = k_integrationTestDirectory + "DirectoryZ/";
                const char k_fileCopySource[] = "IntegrationTest/FileSystem/TextFileA.txt";
                const std::string k_fileCopyDest = k_integrationTestDirectory + "CopiedTextFileA.txt";
                const char k_directoryCopySource[] = "IntegrationTest/FileSystem/DirectoryA/";
                const std::string k_directoryCopyDestA = k_integrationTestDirectory + "CopiedDirectoryA/";
                const std::string k_copiedDirectoryC = k_directoryCopyDestA + "DirectoryC/";
                const char k_searchFileName[] = "BinaryFileD.bin";
                const u32 k_binaryFileContents = 9999;
                const std::vector<std::string> k_expectedFilePathsA = { "DirectoryB/BinaryFileB.bin", "DirectoryB/TextFileB.txt", "DirectoryC/BinaryFileC.bin", "DirectoryC/TextFileC.txt", "DirectoryC/DirectoryD/BinaryFileD.bin",
                    "DirectoryC/DirectoryD/TextFileD.txt" };
                const std::vector<std::string> k_expectedFilePathsC = { "BinaryFileC.bin", "TextFileC.txt" };
                const std::vector<std::string> k_expectedFilePathsExtA = { "DirectoryB/BinaryFileB.bin", "DirectoryC/BinaryFileC.bin", "DirectoryC/DirectoryD/BinaryFileD.bin" };
                const std::vector<std::string> k_expectedFilePathsExtC = { "BinaryFileC.bin" };
                const std::vector<std::string> k_expectedFilePathsNameA = { "DirectoryC/DirectoryD/BinaryFileD.bin" };
                const std::vector<std::string> k_expectedFilePathsNameC = { };
                const std::vector<std::string> k_expectedDirectoryPathsA = { "DirectoryB/", "DirectoryC/" };
                const std::vector<std::string> k_expectedDirectoryPathsRecursiveA = { "DirectoryB/", "DirectoryC/", "DirectoryC/DirectoryD/" };
                
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                CS_ASSERT(fileSystem->IsStorageLocationWritable(in_storageLocation) == true, "Storage location must be writable");
                
                ClearDirectory(in_storageLocation, k_integrationTestDirectory);

                SECTION("Reading and writing text files")
                {
                    REQUIRE(fileSystem->DoesFileExist(in_storageLocation, k_textFilePath) == false);
                    REQUIRE(fileSystem->WriteFile(in_storageLocation, k_textFilePath, "Test") == true);
                    REQUIRE(fileSystem->DoesFileExist(in_storageLocation, k_textFilePath) == true);
                    
                    std::string textFileOutput;
                    REQUIRE((fileSystem->ReadFile(in_storageLocation, k_textFilePath, textFileOutput) == true && textFileOutput == "Test"));
                    REQUIRE(fileSystem->ReadFile(in_storageLocation, k_fileCopyDest, textFileOutput) == false);
                }
                
                SECTION("Reading and writing binary files")
                {
                    REQUIRE(fileSystem->WriteFile(in_storageLocation, k_binFilePath, reinterpret_cast<const s8*>(&k_binaryFileContents), 4) == true);
                    REQUIRE(ReadBinaryFileTest(in_storageLocation, k_binFilePath) == k_binaryFileContents);
                    REQUIRE(ReadBinaryFileTest(in_storageLocation, k_fileCopyDest) == 0);
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryPath) == false);
                }
                
                SECTION("Creating directories")
                {
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryPath) == false);
                    REQUIRE(fileSystem->CreateDirectoryPath(in_storageLocation, k_directoryPath) == true);

                    //Check that creating a directory that already exists will not report an error.
                    REQUIRE(fileSystem->CreateDirectoryPath(in_storageLocation, k_directoryPath) == true);
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryPath) == true);
                }
                
                SECTION("Copy file")
                {
                    REQUIRE(fileSystem->DoesFileExist(in_storageLocation, k_fileCopyDest) == false);
                    REQUIRE(fileSystem->CopyFile(CSCore::StorageLocation::k_package, k_fileCopySource, in_storageLocation, k_fileCopyDest) == true);
                    REQUIRE(fileSystem->DoesFileExist(in_storageLocation, k_fileCopyDest) == true);
                }
                
                SECTION("Copy directory")
                {
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryCopyDestA) == false);
                    REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_directoryCopySource, in_storageLocation, k_directoryCopyDestA) == true);
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryCopyDestA) == true);
                }
                
                SECTION("Get file paths")
                {
                    fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_directoryCopySource, in_storageLocation, k_directoryCopyDestA);

                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(in_storageLocation, k_directoryCopyDestA, true), k_expectedFilePathsA) == true);
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(in_storageLocation, k_copiedDirectoryC, false), k_expectedFilePathsC) == true);
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(in_storageLocation, k_directoryCopyDestA, true, "bin"), k_expectedFilePathsExtA) == true);
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(in_storageLocation, k_copiedDirectoryC, false, "bin"), k_expectedFilePathsExtC) == true);
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(in_storageLocation, k_directoryCopyDestA, true, k_searchFileName), k_expectedFilePathsNameA) == true);
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(in_storageLocation, k_copiedDirectoryC, false, k_searchFileName), k_expectedFilePathsNameC) == true);
                }
                
                SECTION("Get directory paths")
                {
                    fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_directoryCopySource, in_storageLocation, k_directoryCopyDestA);
                    
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(in_storageLocation, k_directoryCopyDestA, true), k_expectedDirectoryPathsRecursiveA) == true);
                    REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(in_storageLocation, k_directoryCopyDestA, false), k_expectedDirectoryPathsA) == true);
                }
                
                SECTION("Delete file")
                {
                    fileSystem->CopyFile(CSCore::StorageLocation::k_package, k_fileCopySource, in_storageLocation, k_fileCopyDest);
                    
                    REQUIRE(fileSystem->DoesFileExist(in_storageLocation, k_fileCopyDest) == true);
                    REQUIRE(fileSystem->DeleteFile(in_storageLocation, k_fileCopyDest) == true);
                    REQUIRE(fileSystem->DoesFileExist(in_storageLocation, k_fileCopyDest) == false);
                    
                    //ensure that trying to delete a file that doesn't exist reports an error.
                    REQUIRE(fileSystem->DeleteFile(in_storageLocation, k_fileCopyDest) == false);
                }
                
                SECTION("Delete directory")
                {
                    fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_directoryCopySource, in_storageLocation, k_directoryCopyDestA);
                    
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryCopyDestA) == true);
                    REQUIRE(fileSystem->DeleteDirectory(in_storageLocation, k_directoryCopyDestA) == true);
                    REQUIRE(fileSystem->DoesDirectoryExist(in_storageLocation, k_directoryCopyDestA) == false);

                    //ensure that trying to delete a directory that doesn't exist reports an error.
                    REQUIRE(fileSystem->DeleteDirectory(in_storageLocation, k_directoryCopyDestA) == false);
                }
                
                ClearDirectory(in_storageLocation, k_integrationTestDirectory);
            }
        }

        TEST_CASE("FileSystem: Package storage location", "[FileSystem]")
        {
            const char k_textFilePath[] = "IntegrationTest/FileSystem/TextFileA.txt";
            const char k_binFilePath[] = "IntegrationTest/FileSystem/BinaryFileA.bin";
            const char k_directoryPathA[] = "IntegrationTest/FileSystem/DirectoryA/";
            const char k_directoryPathC[] = "IntegrationTest/FileSystem/DirectoryA/DirectoryC/";
            const char k_textFilePathFake[] = "IntegrationTest/FileSystem/TextFileAFake.txt";
            const char k_directoryPathFake[] = "IntegrationTest/FileSystem/DirectoryFake/";
            const char k_searchFileName[] = "BinaryFileD.bin";
            const std::vector<std::string> k_expectedFilePathsA = { "DirectoryB/BinaryFileB.bin", "DirectoryB/TextFileB.txt", "DirectoryC/BinaryFileC.bin", "DirectoryC/TextFileC.txt",
                "DirectoryC/DirectoryD/BinaryFileD.bin", "DirectoryC/DirectoryD/TextFileD.txt" };
            const std::vector<std::string> k_expectedFilePathsC = { "BinaryFileC.bin", "TextFileC.txt" };
            const std::vector<std::string> k_expectedFilePathsExtA = { "DirectoryB/BinaryFileB.bin", "DirectoryC/BinaryFileC.bin", "DirectoryC/DirectoryD/BinaryFileD.bin" };
            const std::vector<std::string> k_expectedFilePathsExtC = { "BinaryFileC.bin" };
            const std::vector<std::string> k_expectedFilePathsNameA = { "DirectoryC/DirectoryD/BinaryFileD.bin" };
            const std::vector<std::string> k_expectedFilePathsNameC = { };
            const std::vector<std::string> k_expectedDirectoryPathsA = { "DirectoryB/", "DirectoryC/" };
            const std::vector<std::string> k_expectedDirectoryPathsRecursiveA = { "DirectoryB/", "DirectoryC/", "DirectoryC/DirectoryD/" };
            
            auto fileSystem = CSCore::Application::Get()->GetFileSystem();
            
            SECTION("Checking if files and directories exist")
            {
                REQUIRE(fileSystem->DoesFileExist(CSCore::StorageLocation::k_package, k_textFilePath) == true);
                REQUIRE(fileSystem->DoesFileExist(CSCore::StorageLocation::k_package, k_textFilePathFake) == false);
                REQUIRE(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_package, k_directoryPathA) == true);
                REQUIRE(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_package, k_directoryPathFake) == false);
            }

            SECTION("Reading text files")
            {
                std::string textFileOutput;
                REQUIRE((fileSystem->ReadFile(CSCore::StorageLocation::k_package, k_textFilePath, textFileOutput) == true && textFileOutput == "Test"));
                REQUIRE(fileSystem->ReadFile(CSCore::StorageLocation::k_package, k_textFilePathFake, textFileOutput) == false);
            }
            
            SECTION("Reading binary files")
            {
                REQUIRE(ReadBinaryFileTest(CSCore::StorageLocation::k_package, k_binFilePath) == 9999);
                REQUIRE(ReadBinaryFileTest(CSCore::StorageLocation::k_chilliSource, k_textFilePathFake) == 0);
            }
            
            SECTION("Get file paths")
            {
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_package, k_directoryPathA, true), k_expectedFilePathsA) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_package, k_directoryPathC, false), k_expectedFilePathsC) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_package, k_directoryPathA, true, "bin"), k_expectedFilePathsExtA) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_package, k_directoryPathC, false, "bin"), k_expectedFilePathsExtC) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_package, k_directoryPathA, true, k_searchFileName), k_expectedFilePathsNameA) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_package, k_directoryPathC, false, k_searchFileName), k_expectedFilePathsNameC) == true);
            }
            
            SECTION("Get directory paths")
            {
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_package, k_directoryPathA, true), k_expectedDirectoryPathsRecursiveA) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_package, k_directoryPathA, false), k_expectedDirectoryPathsA) == true);
            }
        }

        TEST_CASE("FileSystem: ChilliSource storage location", "[FileSystem]")
        {
            const char k_textFilePath[] = "Widgets/Widget.csuidef";
            const char k_binFilePath[] = "Fonts/CarlitoMed.low.csfont";
            const char k_directoryPath[] = "Fonts/";
            const char k_textFilePathFake[] = "TextFileAFake.txt";
            const char k_directoryPathFake[] = "DirectoryFake/";
            const char k_searchFileName[] = "CarlitoMed.high.csfont";
            const char k_widgetFileContents[] = "{\n  \"Type\": \"Widget\",\n  \n  \"DefaultPropertyValues\": {\n    \"Name\": \"Widget\"\n  }\n}";
            const std::vector<std::string> k_expectedFilePaths = { "CarlitoMed.high.csfont", "CarlitoMed.high.csimage", "CarlitoMed.low.csfont", "CarlitoMed.low.csimage", "CarlitoMed.med.csfont", "CarlitoMed.med.csimage" };
            const std::vector<std::string> k_expectedFilePathsExt = { "CarlitoMed.high.csfont", "CarlitoMed.low.csfont", "CarlitoMed.med.csfont", };
            const std::vector<std::string> k_expectedFilePathsName = { "CarlitoMed.high.csfont" };
            const std::vector<std::string> k_expectedDirectoryPaths = { "Fonts/", "Shaders/", "Textures/", "Widgets/" };
            const u32 k_cscsInt = 1396921155; //'CSCS' in little endian

            auto fileSystem = CSCore::Application::Get()->GetFileSystem();
            
            SECTION("Checking if files and directories exist")
            {
                REQUIRE(fileSystem->DoesFileExist(CSCore::StorageLocation::k_chilliSource, k_textFilePath) == true);
                REQUIRE(fileSystem->DoesFileExist(CSCore::StorageLocation::k_chilliSource, k_textFilePathFake) == false);
                REQUIRE(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_chilliSource, k_directoryPath) == true);
                REQUIRE(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_chilliSource, k_directoryPathFake) == false);
            }
            
            SECTION("Reading text files")
            {
                std::string textFileOutput;
                REQUIRE((fileSystem->ReadFile(CSCore::StorageLocation::k_chilliSource, k_textFilePath, textFileOutput) && textFileOutput == k_widgetFileContents));
                REQUIRE(fileSystem->ReadFile(CSCore::StorageLocation::k_chilliSource, k_textFilePathFake, textFileOutput) == false);
            }
            
            SECTION("Reading binary files")
            {
                REQUIRE(ReadBinaryFileTest(CSCore::StorageLocation::k_chilliSource, k_binFilePath) == k_cscsInt);
                REQUIRE(ReadBinaryFileTest(CSCore::StorageLocation::k_chilliSource, k_textFilePathFake) == 0);
            }

            SECTION("Get file paths")
            {
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_chilliSource, k_directoryPath, false), k_expectedFilePaths) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_chilliSource, k_directoryPath, false, "csfont"), k_expectedFilePathsExt) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_chilliSource, k_directoryPath, false, k_searchFileName), k_expectedFilePathsName) == true);
            }
            
            SECTION("Get directory paths")
            {
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_chilliSource, "", false), k_expectedDirectoryPaths) == true);
            }
        }

        TEST_CASE("FileSystem: Cache storage location", "[FileSystem]")
        {
            TestWritableStorageLocation(CSCore::StorageLocation::k_cache);
        }

        TEST_CASE("FileSystem: SaveData storage location", "[FileSystem]")
        {
            TestWritableStorageLocation(CSCore::StorageLocation::k_saveData);
        }

        TEST_CASE("FileSystem: DLC storage location", "[FileSystem]")
        {
            TestWritableStorageLocation(CSCore::StorageLocation::k_DLC);
        }

        TEST_CASE("FileSystem: Copying accross storage locations", "[FileSystem]")
        {
            auto fileSystem = CSCore::Application::Get()->GetFileSystem();
            
            const std::string k_integrationTestDirectory = "IntegrationTest/FileSystem/";
            const char k_packageDirectoryPath[] = "IntegrationTest/FileSystem/DirectoryA/";
            const std::string k_copiedDirectoryPath = k_integrationTestDirectory + "CopiedDirectoryA/";
            const std::vector<std::string> k_expectedFilePaths = { "DirectoryB/BinaryFileB.bin", "DirectoryB/TextFileB.txt", "DirectoryC/BinaryFileC.bin", "DirectoryC/TextFileC.txt",
                "DirectoryC/DirectoryD/BinaryFileD.bin", "DirectoryC/DirectoryD/TextFileD.txt" };
            
            ClearDirectory(CSCore::StorageLocation::k_cache, k_integrationTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_saveData, k_integrationTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_DLC, k_integrationTestDirectory);
            
            SECTION("Copy from Cache")
            {
                fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_packageDirectoryPath, CSCore::StorageLocation::k_cache, k_copiedDirectoryPath);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, true), k_expectedFilePaths) == true);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, true), k_expectedFilePaths) == true);
            }
            
            SECTION("Copy from SaveData")
            {
                fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_packageDirectoryPath, CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, CSCore::StorageLocation::k_cache, k_copiedDirectoryPath) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, true), k_expectedFilePaths) == true);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, true), k_expectedFilePaths) == true);
            }
            
            SECTION("Copy from DLC")
            {
                fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_packageDirectoryPath, CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, CSCore::StorageLocation::k_cache, k_copiedDirectoryPath) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, true), k_expectedFilePaths) == true);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, true), k_expectedFilePaths) == true);
            }
            
            ClearDirectory(CSCore::StorageLocation::k_cache, k_integrationTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_saveData, k_integrationTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_DLC, k_integrationTestDirectory);
        }
        
    	TEST_CASE("FileSystem: packaged DLC and cached DLC", "[FileSystem]")
        {
            const std::string k_integrationTestDirectory = "IntegrationTest/FileSystemDLC/";
            const std::string k_textFilePath = k_integrationTestDirectory + "TextFileA.txt";
            const std::string k_directoryPath = k_integrationTestDirectory + "DirectoryA/";
            const std::string k_replacementFilePath = k_directoryPath + "TextFileB.txt";
            const std::string k_extraDirectoryPath = k_directoryPath + "DirectoryC/";
            const std::string k_extraFilePath1 = k_directoryPath + "DirectoryC/BinaryFileD.bin";
            const std::string k_extraFilePath2 = k_directoryPath + "BinaryFileC.bin";
            const std::string k_textFilePathFake = k_integrationTestDirectory + "TextFileAFake.txt";
            const std::string k_directoryPathFake = k_integrationTestDirectory + "DirectoryFake/";
            const char k_searchFileName[] = "BinaryFileC.bin";
            const std::vector<std::string> k_expectedPackageDLCFilePathsRecursive = { "BinaryFileB.bin", "TextFileB.txt", "DirectoryB/BinaryFileC.bin", "DirectoryB/TextFileC.txt" };
            const std::vector<std::string> k_expectedPackageDLCFilePaths = { "BinaryFileB.bin", "TextFileB.txt" };
            const std::vector<std::string> k_expectedPackageDLCFilePathsExtRecursive = { "BinaryFileB.bin", "DirectoryB/BinaryFileC.bin" };
            const std::vector<std::string> k_expectedPackageDLCFilePathsExt = { "BinaryFileB.bin" };
            const std::vector<std::string> k_expectedPackageDLCFilePathsNameRecursive = { "DirectoryB/BinaryFileC.bin" };
            const std::vector<std::string> k_expectedPackageDLCFilePathsName = { };
            const std::vector<std::string> k_expectedPackageDLCDirectoryPaths = { "DirectoryB/" };
            const std::vector<std::string> k_expectedMixedDLCFilePathsRecursive = { "BinaryFileB.bin", "BinaryFileC.bin", "TextFileB.txt", "DirectoryB/BinaryFileC.bin", "DirectoryB/TextFileC.txt",
                "DirectoryC/BinaryFileD.bin" };
            const std::vector<std::string> k_expectedMixedDLCFilePaths = { "BinaryFileB.bin", "BinaryFileC.bin", "TextFileB.txt" };
            const std::vector<std::string> k_expectedMixedDLCFilePathsExtRecursive = { "BinaryFileB.bin", "BinaryFileC.bin", "DirectoryB/BinaryFileC.bin", "DirectoryC/BinaryFileD.bin" };
            const std::vector<std::string> k_expectedMixedDLCFilePathsExt = { "BinaryFileB.bin", "BinaryFileC.bin" };
            const std::vector<std::string> k_expectedMixedDLCFilePathsNameRecursive = { "BinaryFileC.bin", "DirectoryB/BinaryFileC.bin" };
            const std::vector<std::string> k_expectedMixedDLCFilePathsName = { "BinaryFileC.bin" };
            const std::vector<std::string> k_expectedMixedDLCDirectoryPaths = { "DirectoryB/", "DirectoryC/" };
            const u32 k_binaryFileContents = 9999;

            auto fileSystem = CSCore::Application::Get()->GetFileSystem();

            ClearDirectory(CSCore::StorageLocation::k_DLC, k_integrationTestDirectory);

            SECTION("Does file exist in packaged DLC")
            {
                REQUIRE(fileSystem->DoesFileExist(CSCore::StorageLocation::k_DLC, k_textFilePath) == true);
                REQUIRE(fileSystem->DoesFileExist(CSCore::StorageLocation::k_DLC, k_textFilePathFake) == false);
                REQUIRE(fileSystem->DoesFileExistInPackageDLC(k_textFilePath) == true);
                REQUIRE(fileSystem->DoesFileExistInPackageDLC(k_textFilePathFake) == false);
                REQUIRE(fileSystem->DoesFileExistInCachedDLC(k_textFilePath) == false);
            }
            
            SECTION("Does directory exist in packaged DLC")
            {
                REQUIRE(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_DLC, k_directoryPath) == true);
                REQUIRE(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_DLC, k_directoryPathFake) == false);
                REQUIRE(fileSystem->DoesDirectoryExistInPackageDLC(k_directoryPath) == true);
                REQUIRE(fileSystem->DoesDirectoryExistInPackageDLC(k_directoryPathFake) == false);
                REQUIRE(fileSystem->DoesDirectoryExistInCachedDLC(k_directoryPath) == false);
            }

            SECTION("Read text file in packaged DLC")
            {
                std::string packageTextFileOutput;
                REQUIRE((fileSystem->ReadFile(CSCore::StorageLocation::k_DLC, k_textFilePath, packageTextFileOutput) == true && packageTextFileOutput == "Package"));
            }

            SECTION("Get file amd directory paths in packaged DLC")
            {
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_directoryPath, true), k_expectedPackageDLCFilePathsRecursive) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_directoryPath, false), k_expectedPackageDLCFilePaths) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_DLC, k_directoryPath, true, "bin"), k_expectedPackageDLCFilePathsExtRecursive) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_DLC, k_directoryPath, false, "bin"), k_expectedPackageDLCFilePathsExt) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_DLC, k_directoryPath, true, k_searchFileName), k_expectedPackageDLCFilePathsNameRecursive) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_DLC, k_directoryPath, false, k_searchFileName), k_expectedPackageDLCFilePathsName) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_DLC, k_directoryPath, false), k_expectedPackageDLCDirectoryPaths) == true);
            }
            
            SECTION("Does file exist in cached DLC")
            {
                fileSystem->WriteFile(CSCore::StorageLocation::k_DLC, k_textFilePath, "Cache");
                
                REQUIRE(fileSystem->DoesFileExistInCachedDLC(k_textFilePath) == true);
                REQUIRE(fileSystem->DoesFileExistInCachedDLC(k_textFilePathFake) == false);
            }
            
            SECTION("Does directory exist in cached DLC")
            {
                fileSystem->CreateDirectoryPath(CSCore::StorageLocation::k_DLC, k_directoryPath);
                
                REQUIRE(fileSystem->DoesDirectoryExistInCachedDLC(k_directoryPath) == true);
                REQUIRE(fileSystem->DoesDirectoryExistInCachedDLC(k_directoryPathFake) == false);
            }
            
            SECTION("Read text file in cached DLC")
            {
                fileSystem->WriteFile(CSCore::StorageLocation::k_DLC, k_textFilePath, "Cache");
                
                std::string cacheTextFileOutput;
                REQUIRE((fileSystem->ReadFile(CSCore::StorageLocation::k_DLC, k_textFilePath, cacheTextFileOutput) == true && cacheTextFileOutput == "Cache"));
            }
            
            SECTION("Get file and directory paths in cached DLC")
            {
                fileSystem->WriteFile(CSCore::StorageLocation::k_DLC, k_textFilePath, "Cache");
                fileSystem->CreateDirectoryPath(CSCore::StorageLocation::k_DLC, k_directoryPath);
                fileSystem->CreateDirectoryPath(CSCore::StorageLocation::k_DLC, k_extraDirectoryPath);
                fileSystem->WriteFile(CSCore::StorageLocation::k_DLC, k_replacementFilePath, "Cache");
                fileSystem->WriteFile(CSCore::StorageLocation::k_DLC, k_extraFilePath1, reinterpret_cast<const s8*>(&k_binaryFileContents), 4);
                fileSystem->WriteFile(CSCore::StorageLocation::k_DLC, k_extraFilePath2, reinterpret_cast<const s8*>(&k_binaryFileContents), 4);
                
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_directoryPath, true), k_expectedMixedDLCFilePathsRecursive) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_directoryPath, false), k_expectedMixedDLCFilePaths) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_DLC, k_directoryPath, true, "bin"), k_expectedMixedDLCFilePathsExtRecursive) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_DLC, k_directoryPath, false, "bin"), k_expectedMixedDLCFilePathsExt) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_DLC, k_directoryPath, true, k_searchFileName), k_expectedMixedDLCFilePathsNameRecursive) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_DLC, k_directoryPath, false, k_searchFileName), k_expectedMixedDLCFilePathsName) == true);
                REQUIRE(CSCore::VectorUtils::EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_DLC, k_directoryPath, false), k_expectedMixedDLCDirectoryPaths) == true);
            }
            
            ClearDirectory(CSCore::StorageLocation::k_DLC, k_integrationTestDirectory);
        }

    }
}
