set(MODULE_NAME engine)

# Collect all .cpp files from the src directory and its subdirectories
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Create a library target for the game engine
add_library(${MODULE_NAME} STATIC ${SOURCE_FILES})

# Include directories for the engine
target_include_directories(${MODULE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add third party libraries ----------------------------------------------------
set(THIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Find third party dependencies
find_package(SDL2 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)

# Add third party library includes
target_include_directories(${MODULE_NAME} PUBLIC
    ${THIRDPARTY}/objload
    ${THIRDPARTY}/stb_image
)

# Build and include imgui as a static library ----------------------------------
set(IMGUI_DIR ${THIRDPARTY}/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    # ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
)

# Add imgui include directories
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Link imgui with SDL2
target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS}) 

# Link the engine library with all its dependencies ----------------------------
target_link_libraries(${MODULE_NAME} PUBLIC
    SDL2::SDL2main SDL2::SDL2-static
    glm::glm
    GLEW::GLEW
    imgui # imgui::imgui
)
