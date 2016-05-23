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
            const std::string k_testTextFileName = k_rootDirectory + "TestOutFile.txt";
            
            const std::string k_testTextFileFirstLine = "This is a test file.\n";
            const std::string k_testTextFileSecondLine = "I am on another line!";
            const std::string k_testTextFileContents = k_testTextFileFirstLine +  k_testTextFileSecondLine;

			constexpr CS::StorageLocation k_storageLocation = CS::StorageLocation::k_saveData;

            /// Opens the test text file for writing
            ///
            /// @return Output Text Stream
            ///
            CS::TextOutputStreamUPtr OpenOutputTestTextFile()
            {
				const auto fileSystem = CS::Application::Get()->GetFileSystem();
				CS_ASSERT(fileSystem->CreateDirectoryPath(k_storageLocation, k_rootDirectory), "Could not create directory!");
                return fileSystem->CreateTextOutputStream(k_storageLocation, k_testTextFileName);
            }

			/// Opens the test text file for reading
			///
			/// @return Input Text Stream
			///
			CS::ITextInputStreamUPtr OpenInputTestTextFile()
			{
				const auto fileSystem = CS::Application::Get()->GetFileSystem();
				return fileSystem->CreateTextInputStream(k_storageLocation, k_testTextFileName);
			}
            
            /// Delete the test file
            ///
            bool DeleteTestFile()
            {
                const auto fileSystem = CS::Application::Get()->GetFileSystem();
                return !fileSystem->DoesFileExist(k_storageLocation, k_testTextFileName) || fileSystem->DeleteFile(k_storageLocation, k_testTextFileName);
            }
        }
        
        /// A series of integration tests to verify the functionality of an TextOutputStream.
        ///
        CSIT_TESTCASE(TextOutputStream)
        {
            /// Validate that a file stream can be opened
            ///
            CSIT_TEST(SuccessTextOutputStreamCreate)
            {
                CSIT_ASSERT(DeleteTestFile(), "Could not delete test file.");
                
                auto outputFileStream = OpenOutputTestTextFile();
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testTextFileName);
                CSIT_PASS();
            }
            
            /// Validate that single buffer of contents can be written to file
            ///
            CSIT_TEST(SuccessTextOutputStreamWrite)
            {
                CSIT_ASSERT(DeleteTestFile(), "Could not delete test file.");
                
                auto outputFileStream = OpenOutputTestTextFile();
                CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testTextFileName);
                
                outputFileStream->Write(k_testTextFileContents);
                outputFileStream = nullptr;

				auto inputFileStream = OpenInputTestTextFile();
				auto contents = inputFileStream->ReadAll();

				CSIT_ASSERT(contents == k_testTextFileContents, "Written text does not match expectations");
                CSIT_PASS();
            }

			/// Validate that contents can be written to stream multiple times, maintaining correct buffer position
			///
			CSIT_TEST(SuccessTextOutputStreamWriteMultiple)
            {
                CSIT_ASSERT(DeleteTestFile(), "Could not delete test file.");
                
				auto outputFileStream = OpenOutputTestTextFile();
				CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testTextFileName);

				outputFileStream->Write(k_testTextFileFirstLine);
				outputFileStream->Write(k_testTextFileSecondLine);
				outputFileStream = nullptr;

				auto inputFileStream = OpenInputTestTextFile();
				auto contents = inputFileStream->ReadAll();

				CSIT_ASSERT(contents == k_testTextFileContents, "Written text does not match expectations");
				CSIT_PASS();
			}

			/// Validate that contents of a larger size can be overwritten with shorter 
			/// text without remnants of the first remaining
			///
			CSIT_TEST(SuccessTextOutputStreamOverwrite)
            {
                CSIT_ASSERT(DeleteTestFile(), "Could not delete test file.");
                
				auto outputFileStream = OpenOutputTestTextFile();
				CSIT_ASSERT(outputFileStream, "Cannot open output stream to file: " + k_testTextFileName);

				outputFileStream->Write(k_testTextFileFirstLine);
				outputFileStream->Write(k_testTextFileSecondLine);
				outputFileStream = nullptr;

				auto overwriteFileStream = OpenOutputTestTextFile();
				overwriteFileStream->Write(k_testTextFileFirstLine);
				overwriteFileStream = nullptr;

				auto inputFileStream = OpenInputTestTextFile();
				auto contents = inputFileStream->ReadAll();

				CSIT_ASSERT(contents == k_testTextFileFirstLine, "Written text does not match expectations");
				CSIT_PASS();
			}
        }
    }
}
