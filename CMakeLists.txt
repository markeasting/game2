cmake_minimum_required(VERSION 3.15)

project(game VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE DEBUG)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    # src/Game.cpp 
    # src/Renderer.cpp 
    # src/InputHandler.cpp 
)

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad glm::glm SDL2::SDL2main SDL2::SDL2-static)

# Environment specific settings
# if(APPLE)
#     target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")
# endif()
