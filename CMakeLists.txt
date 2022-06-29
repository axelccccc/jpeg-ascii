cmake_minimum_required(VERSION 2.9)
# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
project(jpeg-ascii)

# Choose C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Exports compile_commands.json for clangd language server
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Unoptimized code with debug symbols
set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_FIND_FRAMEWORK LAST)
# find_package(JPEG)

# add_subdirectory adds a subproject, meaning there's something
# to be built or further instructions in the folder's CMakeLists.txt
add_subdirectory(src)

# include_directories adds folder where to search for the headers
include_directories(src)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# create executable with source files taken from 
# variable exported from src/'s CMakeLists.txt
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
                        ${CONAN_LIBS}
                        # libjpeg-turbo::jpeg-static
                        # JPEG::JPEG
                        )