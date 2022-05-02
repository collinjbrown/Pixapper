# CMakeList.txt : CMake project for Pixapper, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (Pixapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set (BASE_SRCS
	Pixapper.cpp
	Pixapper.h
	Setup.h
	external/lodepng/lodepng.cpp
	external/lodepng/lodepng.h
)

# Add source to this project's executable.
add_executable (Pixapper ${BASE_SRCS})

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(Pixapper copy_assets)
