# cmake_minimum_required(VERSION 3.15)

# # Project name and version
# project(game VERSION 1.0)

# # set(CMAKE_BUILD_TYPE DEBUG)

# # Default to Release build if not specified
# # if(NOT CMAKE_BUILD_TYPE)
# #     set(CMAKE_BUILD_TYPE "conan-release" CACHE STRING "Default build to Release" FORCE)
# # endif()

# # C++ standard
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Add the executable
# add_executable(${PROJECT_NAME} 
#     src/main.cpp 
#     src/Game.cpp 
#     src/Renderer.cpp 
#     src/InputHandler.cpp 
# )

# # Include directories
# target_include_directories(${PROJECT_NAME} 
#     PRIVATE 
#         include 
#         ${CMAKE_BINARY_DIR}/include
# )

# # Conan setup
# # Note: defaults to 'Release' build type
# # if(EXISTS "${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake")
# # endif()
# include("${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake")

# # Link libraries
# find_package(SDL2 REQUIRED)
# find_package(glm REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL" glm::glm SDL2::SDL2main)

cmake_minimum_required(VERSION 3.15)
project(game VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE DEBUG)

find_package(SDL2 REQUIRED)
# find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    # src/Game.cpp 
    # src/Renderer.cpp 
    # src/InputHandler.cpp 
)

target_link_libraries(${PROJECT_NAME} PRIVATE glm:glm SDL2::SDL2)

# Environment specific settings
# if(APPLE)
#     target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
# endif()
