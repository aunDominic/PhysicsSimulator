cmake_minimum_required(VERSION 3.10)
project(RigidBodyPhysics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your common source files (excluding main files)
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main_simulation.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main_ode_test.cpp")

# Add your header files
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/SDL3
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glm
)

# Create the simulation executable
add_executable(RigidBodySimulation ${SOURCES} ${HEADERS} src/main_simulation.cpp)

# Create the test executable
add_executable(RigidBodyTest ${SOURCES} ${HEADERS} src/main_ode_test.cpp)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link libraries to RigidBodySimulation
target_link_libraries(RigidBodySimulation PRIVATE OpenGL::GL)
if(WIN32)
    target_link_libraries(RigidBodySimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32.lib)
    target_link_libraries(RigidBodySimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3.lib)
else()
    target_link_libraries(RigidBodySimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEW.a)
    target_link_libraries(RigidBodySimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL3.dylib)
endif()
target_link_libraries(RigidBodySimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspdlog.a)
if(APPLE)
    target_link_libraries(RigidBodySimulation PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# Link libraries to RigidBodyTest
target_link_libraries(RigidBodyTest PRIVATE OpenGL::GL)
if(WIN32)
    target_link_libraries(RigidBodyTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32.lib)
    target_link_libraries(RigidBodyTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3.lib)
else()
    target_link_libraries(RigidBodyTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEW.a)
    target_link_libraries(RigidBodyTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL3.dylib)
endif()
target_link_libraries(RigidBodyTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libspdlog.a)
if(APPLE)
    target_link_libraries(RigidBodyTest PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()

# GLM is header-only, so we don't need to link it
