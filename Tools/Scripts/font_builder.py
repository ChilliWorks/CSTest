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

RELATIVE_TOOL_FILE_PATH = "../../ChilliSource/Tools/CSFontBuilder.jar"

#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build(output_directory_path):
    """
    Builds a series of fonts in different sizes.
    
    :Authors: Ian Copland
    
    :param output_directory_path: The path to the destination directory. The 
        path to the directory (excluding the directory itself) must already 
        exist.
    """
    print("-----------------------------------------")
    print("           Building Fonts")
    print("-----------------------------------------")
    
    file_system_utils.delete_directory(output_directory_path)
    os.mkdir(output_directory_path)

    build_font("Arial", "10", os.path.join(output_directory_path, "ArialSmall.low.csfont"))
    build_font("Arial", "20", os.path.join(output_directory_path, "ArialSmall.med.csfont"))
    build_font("Arial", "40", os.path.join(output_directory_path, "ArialSmall.high.csfont"))
    
    build_font("Arial", "16", os.path.join(output_directory_path, "ArialMed.low.csfont"))
    build_font("Arial", "32", os.path.join(output_directory_path, "ArialMed.med.csfont"))
    build_font("Arial", "64", os.path.join(output_directory_path, "ArialMed.high.csfont"))

    print (" ")
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
def build_font(font_name, font_size, font_file_path):
    """
    Builds a single font with the given font name and size to the given file path.
    
    :Authors: Ian Copland
    
    :param font_name: The name of the font which should be used. The font must
        exist as a system font.
    :param font_size: The size the output font should be rendered at.
    :param font_file_path: The path to the output font.
    """
    print("Building font '" + font_name + "' of size '" + font_size + "' to '" + font_file_path + "'")
    
    tool_file_path = file_system_utils.get_path_from_here(RELATIVE_TOOL_FILE_PATH)
    tool_args = ["java", "-Djava.awt.headless=true", "-jar", tool_file_path, "--fontname", font_name, "--fontsize", font_size, "--output", font_file_path]
    subprocess.call(tool_args);
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
def parse_arguments():
    """
    Parses the given argument list.
    
    :Authors: Ian Copland
    
    :returns: A container for the parsed arguments.
    """
    script_desc = 'Builds a series of fonts in different sizes.'
    
    parser = argparse.ArgumentParser(description=script_desc)
    parser.add_argument('-o', '--output', dest='output_directory_path', type=str, required=True, help="The output directory where the fonts should be saved.")
    
    return parser.parse_args()
#------------------------------------------------------------------------------ 
#------------------------------------------------------------------------------ 
if __name__ == "__main__":
    args = parse_arguments();
    build(args.output_directory_path)

