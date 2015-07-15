//
//  FileSystem.cpp
//  CSTest
//  Created by Ian Copland on 13/07/2015.
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
            /// Tests that two vectors of strings have the same contents. Both must have
            /// identical contents and be the same size, but the elements do not need to be
            /// in the same order.
            ///
            /// @author Ian Copland
            ///
            /// @param in_a - The first vector
            /// @param in_b - The second vector
            ///
            /// @return Whether or not they have the same contents.
            //------------------------------------------------------------------------------
            bool EqualContents(const std::vector<std::string>& in_a, const std::vector<std::string>& in_b)
            {
                if (in_a.size() != in_b.size())
                {
                    return false;
                }
                
                for (const auto& itemA : in_a)
                {
                    if (CSCore::VectorUtils::Contains(in_b, itemA) == false)
                    {
                        return false;
                    }
                }
                
                return true;
            }
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
                    CS_ASSERT(deleted == true, "Cannot perform unit tests becuase test directory couldn't be deleted.");
                }
                
                
                bool created = fileSystem->CreateDirectoryPath(in_storageLocation, in_directoryPath);
                CS_ASSERT(created == true, "Cannot perform unit tests becuase test directory couldn't be created.");
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
                const std::string k_unitTestDirectory = "IntegrationTest/FileSystem/";
                const std::string k_textFilePath = k_unitTestDirectory + "TextFileZ.txt";
                const std::string k_binFilePath = k_unitTestDirectory + "BinaryFileZ.bin";
                const std::string k_directoryPath = k_unitTestDirectory + "DirectoryZ/";
                const char k_fileCopySource[] = "IntegrationTest/FileSystem/TextFileA.txt";
                const std::string k_fileCopyDest = k_unitTestDirectory + "CopiedTextFileA.txt";
                const char k_directoryCopySource[] = "IntegrationTest/FileSystem/DirectoryA/";
                const std::string k_directoryCopyDestA = k_unitTestDirectory + "CopiedDirectoryA/";
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
                
                ClearDirectory(in_storageLocation, k_unitTestDirectory);

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

                    REQUIRE(EqualContents(fileSystem->GetFilePaths(in_storageLocation, k_directoryCopyDestA, true), k_expectedFilePathsA) == true);
                    REQUIRE(EqualContents(fileSystem->GetFilePaths(in_storageLocation, k_copiedDirectoryC, false), k_expectedFilePathsC) == true);
                    REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(in_storageLocation, k_directoryCopyDestA, true, "bin"), k_expectedFilePathsExtA) == true);
                    REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(in_storageLocation, k_copiedDirectoryC, false, "bin"), k_expectedFilePathsExtC) == true);
                    REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(in_storageLocation, k_directoryCopyDestA, true, k_searchFileName), k_expectedFilePathsNameA) == true);
                    REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(in_storageLocation, k_copiedDirectoryC, false, k_searchFileName), k_expectedFilePathsNameC) == true);
                }
                
                SECTION("Get directory paths")
                {
                    fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_directoryCopySource, in_storageLocation, k_directoryCopyDestA);
                    
                    REQUIRE(EqualContents(fileSystem->GetDirectoryPaths(in_storageLocation, k_directoryCopyDestA, true), k_expectedDirectoryPathsRecursiveA) == true);
                    REQUIRE(EqualContents(fileSystem->GetDirectoryPaths(in_storageLocation, k_directoryCopyDestA, false), k_expectedDirectoryPathsA) == true);
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
                
                ClearDirectory(in_storageLocation, k_unitTestDirectory);
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
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_package, k_directoryPathA, true), k_expectedFilePathsA) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_package, k_directoryPathC, false), k_expectedFilePathsC) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_package, k_directoryPathA, true, "bin"), k_expectedFilePathsExtA) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_package, k_directoryPathC, false, "bin"), k_expectedFilePathsExtC) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_package, k_directoryPathA, true, k_searchFileName), k_expectedFilePathsNameA) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_package, k_directoryPathC, false, k_searchFileName), k_expectedFilePathsNameC) == true);
            }
            
            SECTION("Get directory paths")
            {
                REQUIRE(EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_package, k_directoryPathA, true), k_expectedDirectoryPathsRecursiveA) == true);
                REQUIRE(EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_package, k_directoryPathA, false), k_expectedDirectoryPathsA) == true);
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
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_chilliSource, k_directoryPath, false), k_expectedFilePaths) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_chilliSource, k_directoryPath, false, "csfont"), k_expectedFilePathsExt) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_chilliSource, k_directoryPath, false, k_searchFileName), k_expectedFilePathsName) == true);
            }
            
            SECTION("Get directory paths")
            {
                REQUIRE(EqualContents(fileSystem->GetDirectoryPaths(CSCore::StorageLocation::k_chilliSource, "", false), k_expectedDirectoryPaths) == true);
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
            
            const std::string k_unitTestDirectory = "IntegrationTest/FileSystem/";
            const char k_packageDirectoryPath[] = "IntegrationTest/FileSystem/DirectoryA/";
            const std::string k_copiedDirectoryPath = k_unitTestDirectory + "CopiedDirectoryA/";
            const std::vector<std::string> k_expectedDirectoryContents = { "DirectoryB/BinaryFileB.bin", "DirectoryB/TextFileB.txt", "DirectoryC/BinaryFileC.bin", "DirectoryC/TextFileC.txt", "DirectoryC/DirectoryD/BinaryFileD.bin", "DirectoryC/DirectoryD/TextFileD.txt" };
            
            ClearDirectory(CSCore::StorageLocation::k_cache, k_unitTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_saveData, k_unitTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_DLC, k_unitTestDirectory);
            
            SECTION("Copy from Cache")
            {
                fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_packageDirectoryPath, CSCore::StorageLocation::k_cache, k_copiedDirectoryPath);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, true), k_expectedDirectoryContents) == true);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, true), k_expectedDirectoryContents) == true);
            }
            
            SECTION("Copy from SaveData")
            {
                fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_packageDirectoryPath, CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, CSCore::StorageLocation::k_cache, k_copiedDirectoryPath) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, true), k_expectedDirectoryContents) == true);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, true), k_expectedDirectoryContents) == true);
            }
            
            SECTION("Copy from DLC")
            {
                fileSystem->CopyDirectory(CSCore::StorageLocation::k_package, k_packageDirectoryPath, CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, CSCore::StorageLocation::k_cache, k_copiedDirectoryPath) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_cache, k_copiedDirectoryPath, true), k_expectedDirectoryContents) == true);
                
                REQUIRE(fileSystem->CopyDirectory(CSCore::StorageLocation::k_DLC, k_copiedDirectoryPath, CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath) == true);
                REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_saveData, k_copiedDirectoryPath, true), k_expectedDirectoryContents) == true);
            }
            
            ClearDirectory(CSCore::StorageLocation::k_cache, k_unitTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_saveData, k_unitTestDirectory);
            ClearDirectory(CSCore::StorageLocation::k_DLC, k_unitTestDirectory);
        }
    }
}
