cmake_minimum_required(VERSION 3.10)
project(RigidBodyPhysics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add your header files
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL3
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glm
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link OpenGL
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)

# Link GLEW
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32.lib)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEW.a)
endif()

# Link SDL3
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3.lib)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL3.dylib)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspdlog.a)
# If you're on macOS, you might need these frameworks
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# GLM is header-only, so we don't need to link it