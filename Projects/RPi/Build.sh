#!/bin/bash
#  The MIT License (MIT)
#
#  Copyright (c) 2017 Tag Games Limited
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
#-----------------------------------------------------------------------------------

# 1. Clean the output files so make knows to try and remake
# 2. Make the exe
# 3. Copy the assets and libs ready for packaging

export DEBUG=0
export CS_PROJECT_ROOT=../..

if [ "$1" = debug ]; then
    DEBUG=1
elif [ "$1" = release ]; then
    DEBUG=0
else
    echo "No target scheme specified (debug/release)"
    exit 1
fi

make cleangenerated -f "${CS_PROJECT_ROOT}/ChilliSource/Projects/RPi/ChilliSource.mk" ;
make cleangenerated -f Application.mk ;
make -f "${CS_PROJECT_ROOT}/ChilliSource/Projects/RPi/ChilliSource.mk" &&
make -f Application.mk &&
python "${CS_PROJECT_ROOT}/ChilliSource/Tools/Scripts/copy_rpi_resources.py" "." "Output/$1"
