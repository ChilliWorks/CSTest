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
#include <ChilliSource/Core/File.h>

namespace CSTest
{
    namespace IntegrationTest
    {
        namespace
        {
            const std::string k_rootDirecory = "UnitTest/FileSystem/";
            
            const std::string k_textFilePath = k_rootDirecory + "TextFileA.txt";
            const std::string k_fakeFilePath = k_rootDirecory + "FAKE.txt";
            const std::string k_directoryPath = k_rootDirecory + "DirectoryA/";
            const std::string k_fakeDirectoryPath = k_rootDirecory + "FAKE/";
            
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
            /// Confirms that file existence can be queried in package.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistPackage)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CSCore::StorageLocation::k_package, k_textFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CSCore::StorageLocation::k_package, k_fakeFilePath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in cache.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistCache)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                ClearDirectory(CSCore::StorageLocation::k_cache, k_rootDirecory);
                fileSystem->CopyFile(CSCore::StorageLocation::k_package, k_textFilePath, CSCore::StorageLocation::k_cache, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CSCore::StorageLocation::k_cache, k_textFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CSCore::StorageLocation::k_cache, k_fakeFilePath), "File exists.");
                
                ClearDirectory(CSCore::StorageLocation::k_cache, k_rootDirecory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in save data.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistSaveData)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                ClearDirectory(CSCore::StorageLocation::k_saveData, k_rootDirecory);
                fileSystem->CopyFile(CSCore::StorageLocation::k_package, k_textFilePath, CSCore::StorageLocation::k_saveData, k_textFilePath);
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CSCore::StorageLocation::k_saveData, k_textFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CSCore::StorageLocation::k_saveData, k_fakeFilePath), "File exists.");
                
                ClearDirectory(CSCore::StorageLocation::k_saveData, k_rootDirecory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file existence can be queried in ChilliSource.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesFileExistChilliSource)
            {
                const std::string k_csTextFilePath = "Widgets/Widget.csuidef";
                
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesFileExist(CSCore::StorageLocation::k_chilliSource, k_csTextFilePath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesFileExist(CSCore::StorageLocation::k_chilliSource, k_fakeFilePath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in package
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistPackage)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_package, k_directoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_package, k_fakeDirectoryPath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in cache.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistCache)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                ClearDirectory(CSCore::StorageLocation::k_cache, k_rootDirecory);
                fileSystem->CreateDirectoryPath(CSCore::StorageLocation::k_cache, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_cache, k_directoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_cache, k_fakeDirectoryPath), "File exists.");
                
                ClearDirectory(CSCore::StorageLocation::k_cache, k_rootDirecory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in save data.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistSaveData)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                ClearDirectory(CSCore::StorageLocation::k_saveData, k_rootDirecory);
                fileSystem->CreateDirectoryPath(CSCore::StorageLocation::k_saveData, k_directoryPath);
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_saveData, k_directoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_saveData, k_fakeDirectoryPath), "File exists.");
                
                ClearDirectory(CSCore::StorageLocation::k_saveData, k_rootDirecory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory existence can be queried in ChilliSource.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DoesDirectoryExistChilliSource)
            {
                const std::string k_csDirectoryPath = "Fonts/";
                
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                CSIT_ASSERT(fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_chilliSource, k_csDirectoryPath), "File doesn't exist.");
                CSIT_ASSERT(!fileSystem->DoesDirectoryExist(CSCore::StorageLocation::k_chilliSource, k_fakeDirectoryPath), "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in Package.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFilePackage)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CSCore::StorageLocation::k_package, k_textFilePath, textFileOutput) == true && textFileOutput == "Test"), "Failed to read file.");
                CSIT_ASSERT(fileSystem->ReadFile(CSCore::StorageLocation::k_package, k_fakeFilePath, textFileOutput) == false, "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in cache.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFileCache)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                ClearDirectory(CSCore::StorageLocation::k_cache, k_rootDirecory);
                fileSystem->CopyFile(CSCore::StorageLocation::k_package, k_textFilePath, CSCore::StorageLocation::k_cache, k_textFilePath);
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CSCore::StorageLocation::k_cache, k_textFilePath, textFileOutput) == true && textFileOutput == "Test"), "Failed to read file.");
                CSIT_ASSERT(fileSystem->ReadFile(CSCore::StorageLocation::k_cache, k_fakeFilePath, textFileOutput) == false, "File exists.");
                
                ClearDirectory(CSCore::StorageLocation::k_cache, k_rootDirecory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in save data.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFileSaveData)
            {
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                ClearDirectory(CSCore::StorageLocation::k_saveData, k_rootDirecory);
                fileSystem->CopyFile(CSCore::StorageLocation::k_package, k_textFilePath, CSCore::StorageLocation::k_saveData, k_textFilePath);
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CSCore::StorageLocation::k_saveData, k_textFilePath, textFileOutput) == true && textFileOutput == "Test"), "Failed to read file.");
                CSIT_ASSERT(fileSystem->ReadFile(CSCore::StorageLocation::k_saveData, k_fakeFilePath, textFileOutput) == false, "File exists.");
                
                ClearDirectory(CSCore::StorageLocation::k_saveData, k_rootDirecory);
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be read in save data.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadTextFileChilliSource)
            {
                const std::string k_csTextFilePath = "Widgets/Widget.csuidef";
                const std::string k_fileContents = "{\n  \"Type\": \"Widget\",\n  \n  \"DefaultPropertyValues\": {\n    \"Name\": \"Widget\"\n  }\n}";
                
                auto fileSystem = CSCore::Application::Get()->GetFileSystem();
                
                std::string textFileOutput;
                CSIT_ASSERT((fileSystem->ReadFile(CSCore::StorageLocation::k_chilliSource, k_csTextFilePath, textFileOutput) == true && textFileOutput == k_fileContents), "Failed to read file.");
                CSIT_ASSERT(fileSystem->ReadFile(CSCore::StorageLocation::k_chilliSource, k_fakeFilePath, textFileOutput) == false, "File exists.");
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a text file can be written.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(WriteTextFile)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be read.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(ReadBinaryFile)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a binary file can be written.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(WriteBinaryFile)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be copied
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyFile)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a file can be deleted
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DeleteFile)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directories can be created
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CreateDirectory)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be copied
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(CopyDirectory)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that a directory can be deleted
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(DeleteDirectory)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that file paths can be queried.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetFilePaths)
            {
                //TODO:
                
                CSIT_PASS();
            }
            //------------------------------------------------------------------------------
            /// Confirms that directory paths can be queried.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CSIT_TEST(GetDirectoryPaths)
            {
                //TODO:
                
                CSIT_PASS();
            }
        }
    }
}

#endif
