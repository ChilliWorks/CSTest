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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Container.h>
#include <ChilliSource/Core/File.h>

#include <catch.hpp>

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
}

TEST_CASE("FileSystem:Package", "[FileSystem]")
{
    const char k_textFilePath[] = "UnitTest/FileSystem/TextFileA.txt";
    const char k_binFilePath[] = "UnitTest/FileSystem/BinaryFileA.bin";
    const char k_directoryPathA[] = "UnitTest/FileSystem/DirectoryA/";
    const char k_directoryPathC[] = "UnitTest/FileSystem/DirectoryA/DirectoryC/";
    const char k_textFilePathFake[] = "UnitTest/FileSystem/TextFileAFake.txt";
    const char k_directoryPathFake[] = "UnitTest/FileSystem/DirectoryFake/";
    const char k_searchFileName[] = "BinaryFileD.bin";
    const std::vector<std::string> k_expectedDirectoryContentsA = { "DirectoryB/BinaryFileB.bin", "DirectoryB/TextFileB.txt", "DirectoryC/BinaryFileC.bin", "DirectoryC/TextFileC.txt", "DirectoryC/DirectoryD/BinaryFileD.bin", "DirectoryC/DirectoryD/TextFileD.txt" };
    const std::vector<std::string> k_expectedDirectoryContentsC = { "BinaryFileC.bin", "TextFileC.txt" };
    const std::vector<std::string> k_expectedDirectoryContentsExtA = { "DirectoryB/BinaryFileB.bin", "DirectoryC/BinaryFileC.bin", "DirectoryC/DirectoryD/BinaryFileD.bin" };
    const std::vector<std::string> k_expectedDirectoryContentsExtC = { "BinaryFileC.bin" };
    const std::vector<std::string> k_expectedDirectoryContentsNameA = { "DirectoryC/DirectoryD/BinaryFileD.bin" };
    const std::vector<std::string> k_expectedDirectoryContentsNameC = { };
    
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
        REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_package, k_directoryPathA, true), k_expectedDirectoryContentsA) == true);
        REQUIRE(EqualContents(fileSystem->GetFilePaths(CSCore::StorageLocation::k_package, k_directoryPathC, false), k_expectedDirectoryContentsC) == true);
        REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_package, k_directoryPathA, true, "bin"), k_expectedDirectoryContentsExtA) == true);
        REQUIRE(EqualContents(fileSystem->GetFilePathsWithExtension(CSCore::StorageLocation::k_package, k_directoryPathC, false, "bin"), k_expectedDirectoryContentsExtC) == true);
        REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_package, k_directoryPathA, true, k_searchFileName), k_expectedDirectoryContentsNameA) == true);
        REQUIRE(EqualContents(fileSystem->GetFilePathsWithFileName(CSCore::StorageLocation::k_package, k_directoryPathC, false, k_searchFileName), k_expectedDirectoryContentsNameC) == true);
    }
}

