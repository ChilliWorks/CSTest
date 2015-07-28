#!/usr/bin/python
#
#  font_builder.py
#  CSTest
#  Created by Ian Copland on 28/07/2015.
#
#  The MIT License (MIT)
#
#  Copyright (c) 2015 Tag Games Limited
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#

import sys
import os
import subprocess
import shutil
import file_system_utils

csfont_tool_filepath = file_system_utils.get_path_from_here("../../ChilliSource/Tools/CSFontBuilder.jar")

#------------------------------------------------------------------------------
# Builds a single font with the given font name and size to the given file path.
#
# @author Ian Copland
#
# @param font_name - The name of the font which should be used. The font must
# exist as a system font.
# @param font_size - The size the output font should be rendered at.
# @param font_file_path - The path to the output font.
#------------------------------------------------------------------------------
def build_font(font_name, font_size, font_file_path):
    print("Building font '" + font_name + "' of size '" + font_size + "' to '" + font_file_path + "'")
    subprocess.call(["java", "-Djava.awt.headless=true", "-jar", csfont_tool_filepath, "--fontname", font_name, "--fontsize", font_size, "--output", font_file_path]);

#------------------------------------------------------------------------------
# Builds all of the fonts using the CSFontBuilder tool.
#
# @author Ian Copland
#
# @param destination_directory_path - The path to the destination directory.
# The path to the directory (excluding the directory itself) must already
# exist.
#------------------------------------------------------------------------------
def build(destination_directory_path):
	print("-----------------------------------------")
	print("           Building Fonts")
	print("-----------------------------------------")
	
	file_system_utils.delete_directory(destination_directory_path)
	os.mkdir(destination_directory_path)

	build_font("Arial", "10", os.path.join(destination_directory_path, "ArialSmall.low.csfont"))
	build_font("Arial", "20", os.path.join(destination_directory_path, "ArialSmall.med.csfont"))
	build_font("Arial", "40", os.path.join(destination_directory_path, "ArialSmall.high.csfont"))
	
	build_font("Arial", "16", os.path.join(destination_directory_path, "ArialMed.low.csfont"))
	build_font("Arial", "32", os.path.join(destination_directory_path, "ArialMed.med.csfont"))
	build_font("Arial", "64", os.path.join(destination_directory_path, "ArialMed.high.csfont"))

	print (" ")

#------------------------------------------------------------------------------
# The entry point into the script.
#
# @author Ian Copland
#
# @param The list of arguments.
#------------------------------------------------------------------------------
def main(args):
	if not len(args) is 2:
		print("ERROR: Invalid parameters supplied.")
		return

	destination_directory_path = args[1]
	build(destination_directory_path)

if __name__ == "__main__":
	main(sys.argv)

