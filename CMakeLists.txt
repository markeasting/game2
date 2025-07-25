cmake_minimum_required(VERSION 3.15)

project(game VERSION 1.0)

# Defines
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_BUILD_TYPE DEBUG)
# set(CMAKE_OSX_ARCHITECTURES "arm64") # Apple Silicon support - might not be needed

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Add Debug build option
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available build types" FORCE)

# ---------------------------------- SOURCE ---------------------------------- #

# Collect all .cpp files from the src directory
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/game/src/*.cpp)

# Create an executable target with the source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add project include directory
target_include_directories(${PROJECT_NAME} PRIVATE game/include)

# ----------------------------------- LIBS ----------------------------------- #

# Add engine include and source directories
add_subdirectory(engine)

# Include engine headers
target_include_directories(${PROJECT_NAME} PRIVATE engine/include)

# Link engine library
target_link_libraries(${PROJECT_NAME} PRIVATE engine)

# --------------------------- EXTERNAL DEPENDENCIES -------------------------- #

# Find external dependencies
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
# find_package(glad REQUIRED)

# Link the executable with its dependencies
# @todo these might be redundant if the engine already includes them
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL2::SDL2main SDL2::SDL2-static
    # glad::glad 
    glm::glm 
)

# Environment specific settings
# if(APPLE)
#     target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
# endif()


# --------------------------- ASSETS AND RESOURCES --------------------------- #
# Symlink the assets to the build dir - useful for development, 
# but in release mode we should copy the assets instead.

# Custom target - copy the assets directory
# add_custom_target(copy_assets ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#             ${CMAKE_SOURCE_DIR}/game/assets
#             $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
#     COMMENT "Copying assets directory to build output"
# )

# Custom target - create a symlink for the assets directory
add_custom_target(link_assets ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/game/assets
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "Creating symlink for assets directory in build output"
)
