#!/bin/bash
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
(cd `echo $SCRIPT_DIR`; rm -rf build devel src/CMakeLists.txt; catkin_make)