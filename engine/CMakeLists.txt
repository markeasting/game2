set(MODULE_NAME engine)

# Collect all .cpp files from the src directory and its subdirectories
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create a library target for the game engine
add_library(${MODULE_NAME} STATIC ${SOURCE_FILES})

# Include directories for the engine
target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/objload
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image
    # ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include
)

# Find external dependencies
find_package(SDL2 REQUIRED)
find_package(glew REQUIRED)
# find_package(glad REQUIRED)
find_package(glm REQUIRED)

# Link the library with its dependencies
target_link_libraries(${MODULE_NAME} PUBLIC
    SDL2::SDL2main SDL2::SDL2-static
    glm::glm
    GLEW::GLEW
    # glad::glad
)
