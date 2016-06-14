//
//  FileSystemTest.cpp
//  CSTest
//  Created by Ian Copland on 18/03/2016.
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

#ifndef _INTEGRATIONTEST_TESTS_FILESYSTEMTEST_H_
#define _INTEGRATIONTEST_TESTS_FILESYSTEMTEST_H_

#include <CSTest.h>
#include <IntegrationTest/TestSystem/TestCase.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Container.h>
#include <ChilliSource/Core/Cryptographic.h>
#include <ChilliSource/Core/File.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            const std::string k_rootDirectory = "IntegrationTest/FileSystem/";
            
            const std::string k_textFilePath = k_rootDirectory + "TextFile.txt";
            const std::string k_binaryFilePath = k_rootDirectory + "BinaryFile.bin";
            const std::string k_copiedTextFilePath = k_rootDirectory + "TextFile-Copy.txt";
            const std::string k_directoryPath = k_rootDirectory + "Directory/";
            const std::string k_copiedDirectoryPath = k_rootDirectory + "Directory-Copy/";
            const std::string k_nestedDirectoryPath = k_rootDirectory + "Directory/DirectoryA/Directory/";
            
            const std::string k_fakeFilePath = k_rootDirectory + "FAKE.txt";
            const std::string k_fakeDirectoryPath = k_rootDirectory + "FAKE/";
            
            const std::string k_textFileContents = "Test";
            const u32 k_binaryFileLength = 4;
            const char k_binaryFileContents[] = "\x0F\x27\x00\x00";
            const std::string k_binaryFileChecksum = "EBAB98776D1DE85288E0EE94F3B92DD74DD8C0D6";
            
            const std::vector<std::string> k_filePathsInDirectory =
            {
                "BinaryFile.bin",
                "TextFile.txt"
            };
            
            const std::vector<std::string> k_filePathsInDirectoryRecursive =
            {
                "BinaryFile.bin",
                "TextFile.txt",
                "DirectoryA/Directory/BinaryFile.bin",
                "DirectoryA/Directory/TextFile.txt",
                "DirectoryB/BinaryFile.bin",
                "DirectoryB/TextFile.txt"
            };
            
            const std::vector<std::string> k_binaryFilePathsInDirectory =
            {
                "BinaryFile.bin"
            };
            
            const std::vector<std::string> k_binaryFilePathsInDirectoryRecursive =
            {
                "BinaryFile.bin",
                "DirectoryA/Directory/BinaryFile.bin",
                "DirectoryB/BinaryFile.bin"
            };
            
            const std::vector<std::string> k_textFilePathsInDirectory =
            {
                "TextFile.txt"
            };
            
            const std::vector<std::string> k_textFilePathsInDirectoryRecursive =
            {
                "TextFile.txt",
                "DirectoryA/Directory/TextFile.txt",
                "DirectoryB/TextFile.txt"
            };
            
            const std::vector<std::string> k_directoryPathsInDirectory =
            {
                "DirectoryA/",
                "DirectoryB/"
            };
            
            const std::vector<std::string> k_directoryPathsInDirectoryRecursive =
            {
                "DirectoryA/",
                "DirectoryA/Directory/",
                "DirectoryB/"
            };
            
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
            void ClearDirectory(CS::StorageLocation in_storageLocation, const std::string& in_directoryPath)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                CS_ASSERT(fileSystem->IsStorageLocationWritable(in_storageLocation) == true, "Cannot clear directory in non-writable storage location.");
                
                if ((in_storageLocation == CS::StorageLocation::k_DLC && fileSystem->DoesDirectoryExistInCachedDLC(in_directoryPath) == true) ||
                    (in_storageLocation != CS::StorageLocation::k_DLC && fileSystem->DoesDirectoryExist(in_storageLocation, in_directoryPath) == true))
                {
                    bool deleted = fileSystem->DeleteDirectory(in_storageLocation, in_directoryPath);
                    CS_ASSERT(deleted == true, "Cannot perform integration tests because test directory couldn't be deleted.");
                }
                
                bool created = fileSystem->CreateDirectoryPath(in_storageLocation, in_directoryPath);
                CS_ASSERT(created == true, "Cannot perform integration tests because test directory couldn't be created.");
            }
            //------------------------------------------------------------------------------
            /// Calculates the SHA1 checksum of the given file. If the file cannot be read
            /// an empty checksum will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param in_storageLocation - The storage location
            /// @param in_filePath - The file path
            ///
            /// @return The checksum of the binary file.
            //------------------------------------------------------------------------------
            std::string CalculateFileChecksumSHA1(CS::StorageLocation in_storageLocation, const std::string& in_filePath)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                auto fileStream = fileSystem->CreateBinaryInputStream(in_storageLocation, in_filePath);
                if (!fileStream.get())
                {
                    return "";
                }
                
                u32 length = u32(fileStream->GetLength());
                
                if (length == 0)
                {
                    return "";
                }
                
                std::unique_ptr<u8[]> fileContents(new u8[length]);
                fileStream->Read(fileContents.get(), length);
                
                return CS::HashSHA1::GenerateHexHashCode(reinterpret_cast<s8*>(fileContents.get()), length);
            }
        }
        
        //------------------------------------------------------------------------------
        /// A series of integration tests for the file system. Tests for both the root
        /// and DLC storage locations have been omitted.
        ///
        /// Note that these tests require specific files to exist on disk.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        CSIT_TESTCASE(HttpRequest)
        {
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistPackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_package, k_textFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CS::StorageLocation::k_package, k_fakeFilePath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_cache, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_textFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_fakeFilePath), "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in the save data storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_saveData, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_textFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_fakeFilePath), "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in the ChilliSource storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistChilliSource)
            {
                const std::string csTextFilePath = "Widgets/Widget.csuidef";
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_chilliSource, csTextFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CS::StorageLocation::k_chilliSource, k_fakeFilePath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in the package storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistPackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_package, k_directoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CS::StorageLocation::k_package, k_fakeDirectoryPath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in the cache storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CreateDirectoryPath(CS::StorageLocation::k_cache, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_cache, k_directoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CS::StorageLocation::k_cache, k_fakeDirectoryPath), "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in the save data storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CreateDirectoryPath(CS::StorageLocation::k_saveData, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_saveData, k_directoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CS::StorageLocation::k_saveData, k_fakeDirectoryPath), "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in the ChilliSource storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistChilliSource)
            {
                const std::string csDirectoryPath = "Fonts/";
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_chilliSource, csDirectoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CS::StorageLocation::k_chilliSource, k_fakeDirectoryPath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFilePackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CS::StorageLocation::k_package, k_textFilePath, textFileOutput) && textFileOutput == k_textFileContents), "Failed to read file.");
                CSIT_ASSERT(!fileSystem->ReadFile(CS::StorageLocation::k_package, k_fakeFilePath, textFileOutput), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFileCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_cache, k_textFilePath);
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CS::StorageLocation::k_cache, k_textFilePath, textFileOutput) && textFileOutput == k_textFileContents), "Failed to read file.");
                CSIT_ASSERT(!fileSystem->ReadFile(CS::StorageLocation::k_cache, k_fakeFilePath, textFileOutput), "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFileSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_saveData, k_textFilePath);
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CS::StorageLocation::k_saveData, k_textFilePath, textFileOutput) && textFileOutput == k_textFileContents), "Failed to read file.");
                CSIT_ASSERT(!fileSystem->ReadFile(CS::StorageLocation::k_saveData, k_fakeFilePath, textFileOutput), "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in the ChilliSource storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFileChilliSource)
            {
                const std::string csTextFilePath = "Widgets/Widget.csuidef";
                const std::string fileContents = "{\n  \"Type\": \"Widget\",\n  \n  \"DefaultPropertyValues\": {\n    \"Name\": \"Widget\"\n  }\n}";
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CS::StorageLocation::k_chilliSource, csTextFilePath, textFileOutput) && textFileOutput == fileContents), "Failed to read file.");
                CSIT_ASSERT(!fileSystem->ReadFile(CS::StorageLocation::k_chilliSource, k_fakeFilePath, textFileOutput), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be written in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(WriteTextFileCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->WriteFile(CS::StorageLocation::k_cache, k_textFilePath, k_textFileContents), "Failed to write file.");
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CS::StorageLocation::k_cache, k_textFilePath, textFileOutput) && textFileOutput == k_textFileContents), "Failed to read file.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be written in the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(WriteTextFileSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->WriteFile(CS::StorageLocation::k_saveData, k_textFilePath, k_textFileContents), "Failed to write file.");
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CS::StorageLocation::k_saveData, k_textFilePath, textFileOutput) && textFileOutput == k_textFileContents), "Failed to read file.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be read in the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadBinaryFilePackage)
            {
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_package, k_binaryFilePath) == k_binaryFileChecksum, "Failed to read file.");
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_package, k_fakeFilePath) == "", "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be read in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadBinaryFileCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_binaryFilePath, CS::StorageLocation::k_cache, k_binaryFilePath);
                
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_cache, k_binaryFilePath) == k_binaryFileChecksum, "Failed to read file.");
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_cache, k_fakeFilePath) == "", "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be read in the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadBinaryFileSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_binaryFilePath, CS::StorageLocation::k_saveData, k_binaryFilePath);
                
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_saveData, k_binaryFilePath) == k_binaryFileChecksum, "Failed to read file.");
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_saveData, k_fakeFilePath) == "", "File exists.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be read in the ChilliSource storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadBinaryFileChilliSource)
            {
                const std::string csBinaryFilePath = "Fonts/CarlitoMed.low.csfont";
                const std::string csBinaryFileChecksum = "AF722ED69AC29A53402ABF006C4CFD80B4D808CE";
                
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_chilliSource, csBinaryFilePath) == csBinaryFileChecksum, "Failed to read file.");
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_chilliSource, k_fakeFilePath) == "", "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be written to the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(WriteBinaryFileCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->WriteFile(CS::StorageLocation::k_cache, k_binaryFilePath, k_binaryFileContents, k_binaryFileLength), "Failed to write file.");
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_cache, k_binaryFilePath) == k_binaryFileChecksum, "Failed to read file.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be written to the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(WriteBinaryFileSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->WriteFile(CS::StorageLocation::k_saveData, k_binaryFilePath, k_binaryFileContents, k_binaryFileLength), "Failed to write file.");
                CSIT_ASSERT(CalculateFileChecksumSHA1(CS::StorageLocation::k_saveData, k_binaryFilePath) == k_binaryFileChecksum, "Failed to read file.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be copied from the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyFilePackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_cache, k_textFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_textFilePath), "File does not exist.");
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_saveData, k_textFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_textFilePath), "File does not exist.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be copied from the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyFileCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_cache, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_cache, k_textFilePath, CS::StorageLocation::k_cache, k_copiedTextFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_copiedTextFilePath), "File does not exist.");
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_cache, k_textFilePath, CS::StorageLocation::k_saveData, k_textFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_textFilePath), "File does not exist.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be copied from the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyFileSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_saveData, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_saveData, k_textFilePath, CS::StorageLocation::k_saveData, k_copiedTextFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_copiedTextFilePath), "File does not exist.");
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_saveData, k_textFilePath, CS::StorageLocation::k_cache, k_textFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_textFilePath), "File does not exist.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be copied from the ChilliSource storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyFileChilliSource)
            {
                const std::string csTextFilePath = "Widgets/Widget.csuidef";
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_chilliSource, csTextFilePath, CS::StorageLocation::k_cache, k_textFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_textFilePath), "File does not exist.");
                
                CSIT_ASSERT(fileSystem->CopyFile(CS::StorageLocation::k_chilliSource, csTextFilePath, CS::StorageLocation::k_saveData, k_textFilePath), "Failed to copy file.");
                CSIT_ASSERT(fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_textFilePath), "File does not exist.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be deleted from the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DeleteFileCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_cache, k_textFilePath);

                CSIT_ASSERT(fileSystem->DeleteFile(CS::StorageLocation::k_cache, k_textFilePath), "Failed to delete file.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CS::StorageLocation::k_cache, k_textFilePath), "File still exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be deleted from the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DeleteFileSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyFile(CS::StorageLocation::k_package, k_textFilePath, CS::StorageLocation::k_saveData, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->DeleteFile(CS::StorageLocation::k_saveData, k_textFilePath), "Failed to delete file.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CS::StorageLocation::k_saveData, k_textFilePath), "File still exists.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directories can be created in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CreateDirectoryCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->CreateDirectoryPath(CS::StorageLocation::k_cache, k_directoryPath), "Failed to create directory.");
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_cache, k_directoryPath), "Directory doesn't exist.");
                
                CSIT_ASSERT(fileSystem->CreateDirectoryPath(CS::StorageLocation::k_cache, k_nestedDirectoryPath), "Failed to create directory.");
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_cache, k_nestedDirectoryPath), "Directory doesn't exist.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directories can be created in the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CreateDirectorySaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->CreateDirectoryPath(CS::StorageLocation::k_saveData, k_directoryPath), "Failed to create directory.");
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_saveData, k_directoryPath), "Directory doesn't exist.");
                
                CSIT_ASSERT(fileSystem->CreateDirectoryPath(CS::StorageLocation::k_saveData, k_nestedDirectoryPath), "Failed to create directory.");
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CS::StorageLocation::k_saveData, k_nestedDirectoryPath), "Directory doesn't exist.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be copied from the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyDirectoryPackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_cache, k_directoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_cache, k_directoryPath, true), k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_saveData, k_directoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_saveData, k_directoryPath, true), k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be copied from the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyDirectoryCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_cache, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_cache, k_directoryPath, CS::StorageLocation::k_cache, k_copiedDirectoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_cache, k_copiedDirectoryPath, true), k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_cache, k_directoryPath, CS::StorageLocation::k_saveData, k_directoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_saveData, k_directoryPath, true), k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be copied from the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyDirectorySaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_saveData, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_saveData, k_directoryPath, CS::StorageLocation::k_saveData, k_copiedDirectoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_saveData, k_copiedDirectoryPath, true), k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_saveData, k_directoryPath, CS::StorageLocation::k_cache, k_directoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_cache, k_directoryPath, true), k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be copied from the ChiliSource storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyDirectoryChilliSource)
            {
                const std::string csDirectoryPath = "Fonts/";
                
                const std::vector<std::string> csFilePathsInDirectory =
                {
                    "CarlitoMed.high.csfont",
                    "CarlitoMed.high.csimage",
                    "CarlitoMed.low.csfont",
                    "CarlitoMed.low.csimage",
                    "CarlitoMed.med.csfont",
                    "CarlitoMed.med.csimage"
                };
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_chilliSource, csDirectoryPath, CS::StorageLocation::k_cache, csDirectoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_cache, csDirectoryPath, true), csFilePathsInDirectory), "Directory has incorrect contents.");
                
                CSIT_ASSERT(fileSystem->CopyDirectory(CS::StorageLocation::k_chilliSource, csDirectoryPath, CS::StorageLocation::k_saveData, csDirectoryPath), "Failed to copy directory.");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(fileSystem->GetFilePaths(CS::StorageLocation::k_saveData, csDirectoryPath, true), csFilePathsInDirectory), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be deleted from the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DeleteDirectoryCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_cache, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->DeleteDirectory(CS::StorageLocation::k_cache, k_directoryPath), "Failed to delete directory.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CS::StorageLocation::k_cache, k_directoryPath), "Directory still exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be deleted from the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DeleteDirectorySaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_saveData, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->DeleteDirectory(CS::StorageLocation::k_saveData, k_directoryPath), "Failed to delete directory.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CS::StorageLocation::k_saveData, k_directoryPath), "Directory still exists.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file paths can be queried in the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetFilePathsPackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                auto contents = fileSystem->GetFilePaths(CS::StorageLocation::k_package, k_directoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_filePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePaths(CS::StorageLocation::k_package, k_directoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_package, k_directoryPath, false, "bin");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_binaryFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_package, k_directoryPath, true, "bin");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_binaryFilePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_package, k_directoryPath, false, "TextFile.txt");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_textFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_package, k_directoryPath, true, "TextFile.txt");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_textFilePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file paths can be queried in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetFilePathsCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_cache, k_directoryPath);
                
                auto contents = fileSystem->GetFilePaths(CS::StorageLocation::k_cache, k_directoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_filePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePaths(CS::StorageLocation::k_cache, k_directoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_cache, k_directoryPath, false, "bin");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_binaryFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_cache, k_directoryPath, true, "bin");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_binaryFilePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_cache, k_directoryPath, false, "TextFile.txt");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_textFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_cache, k_directoryPath, true, "TextFile.txt");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_textFilePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file paths can be queried in the save data storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetFilePathsSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_saveData, k_directoryPath);
                
                auto contents = fileSystem->GetFilePaths(CS::StorageLocation::k_saveData, k_directoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_filePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePaths(CS::StorageLocation::k_saveData, k_directoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_filePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_saveData, k_directoryPath, false, "bin");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_binaryFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_saveData, k_directoryPath, true, "bin");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_binaryFilePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_saveData, k_directoryPath, false, "TextFile.txt");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_textFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_saveData, k_directoryPath, true, "TextFile.txt");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_textFilePathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file paths can be queried in the ChilliSource storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetFilePathsChilliSource)
            {
                const std::string csDirectoryPath = "Fonts/";
                
                const std::vector<std::string> csFilePathsInDirectory =
                {
                    "CarlitoMed.high.csfont",
                    "CarlitoMed.high.csimage",
                    "CarlitoMed.low.csfont",
                    "CarlitoMed.low.csimage",
                    "CarlitoMed.med.csfont",
                    "CarlitoMed.med.csimage"
                };
                
                const std::vector<std::string> csfontFilePathsInDirectory =
                {
                    "CarlitoMed.high.csfont",
                    "CarlitoMed.low.csfont",
                    "CarlitoMed.med.csfont"
                };
                
                const std::vector<std::string> csCarlitoMedHighImageFilePathsInDirectory =
                {
                    "CarlitoMed.high.csimage"
                };
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                auto contents = fileSystem->GetFilePaths(CS::StorageLocation::k_chilliSource, csDirectoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePaths(CS::StorageLocation::k_chilliSource, csDirectoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_chilliSource, csDirectoryPath, false, "csfont");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csfontFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithExtension(CS::StorageLocation::k_chilliSource, csDirectoryPath, true, "csfont");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csfontFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_chilliSource, csDirectoryPath, false, "CarlitoMed.high.csimage");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csCarlitoMedHighImageFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetFilePathsWithFileName(CS::StorageLocation::k_chilliSource, csDirectoryPath, true, "CarlitoMed.high.csimage");
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csCarlitoMedHighImageFilePathsInDirectory), "Directory has incorrect contents.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory paths can be queried in the package storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetDirectoryPathsPackage)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                auto contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_package, k_directoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_directoryPathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_package, k_directoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_directoryPathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory paths can be queried in the cache storage location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetDirectoryPathsCache)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_cache, k_directoryPath);
                
                auto contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_cache, k_directoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_directoryPathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_cache, k_directoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_directoryPathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_cache, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory paths can be queried in the save data storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetDirectoryPathsSaveData)
            {
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                fileSystem->CopyDirectory(CS::StorageLocation::k_package, k_directoryPath, CS::StorageLocation::k_saveData, k_directoryPath);
                
                auto contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_saveData, k_directoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_directoryPathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_saveData, k_directoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, k_directoryPathsInDirectoryRecursive), "Directory has incorrect contents.");
                
                ClearDirectory(CS::StorageLocation::k_saveData, k_rootDirectory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory paths can be queried in the ChilliSource storage
            /// location.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetDirectoryPathsChilliSource)
            {
                const std::string csDirectoryPath = "";
                
                const std::vector<std::string> csFilePathsInDirectory =
                {
                    "Fonts/",
                    "Shaders/",
                    "Textures/",
                    "Widgets/"
                };
                
                auto fileSystem = CS::Application::Get()->GetFileSystem();
                
                auto contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_chilliSource, csDirectoryPath, false);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csFilePathsInDirectory), "Directory has incorrect contents.");
                
                contents = fileSystem->GetDirectoryPaths(CS::StorageLocation::k_chilliSource, csDirectoryPath, true);
                CSIT_ASSERT(CS::VectorUtils::EqualContents(contents, csFilePathsInDirectory), "Directory has incorrect contents.");
                
                CSIT_PASS();
            }
        }
    }
}

#endif
