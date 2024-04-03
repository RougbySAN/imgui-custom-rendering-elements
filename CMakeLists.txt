cmake_minimum_required(VERSION 3.0.0)


# Set Cmake CPP Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#Set Cmake C Standard
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)


# Output binary directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

project(orthogonal-line-connector   VERSION 0.1
                                    DESCRIPTION "Dear Imgui based orthogonal connectors for gui applications"
                                    HOMEPAGE_URL "https://github.com/RougbySAN/imgui-orthogonal-line-connector.git"
)


# Set default build type to Debug if not specified by the user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default Debug)" FORCE)
endif()


add_subdirectory(lib)
add_subdirectory(src)


