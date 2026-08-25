
cmake_minimum_required(VERSION 3.10)
set(PROJ_NAME Asimov)

project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(ReactPhysics3D REQUIRED)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extLibs/imgui)
set(GLAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(IMGUI_LIB "${CMAKE_SOURCE_DIR}/extLibs/libimgui.a")


####### SETTING UP APE static libs
##CORE STATIC LIBS
set(CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Core)
set(CORE_SOURCES
	${CORE_DIR}/impl/APE_engine.cpp
	${CORE_DIR}/impl/APE_shader.cpp
	${CORE_DIR}/impl/APE_camera.cpp
	${CORE_DIR}/impl/APE_mesh.cpp
	${CORE_DIR}/impl/APE_ECS.cpp
)

add_library(APE_core STATIC ${CORE_SOURCES})
target_include_directories(APE_core PUBLIC ${CORE_DIR}/header)

##CORE STATIC LIBS
set(BUFFERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Buffers)
set(BUFFERS_SOURCES
	${BUFFERS_DIR}/impl/APE_VBO.cpp
	${BUFFERS_DIR}/impl/APE_VAO.cpp
	${BUFFERS_DIR}/impl/APE_IBO.cpp
	${BUFFERS_DIR}/impl/APE_FBO.cpp
)

add_library(APE_buffers STATIC ${BUFFERS_SOURCES})
target_include_directories(APE_buffers PUBLIC ${BUFFERS_DIR}/header)

##PHYSICS STATIC LIBS
set(PHYSICS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/Physics)
set(PHYSICS_SOURCES
	${PHYSICS_DIR}/impl/APE_integrator.cpp
	${PHYSICS_DIR}/impl/APE_CollisionDetection.cpp
	${PHYSICS_DIR}/impl/APE_CollisionResolution.cpp
)

add_library(APE_physics STATIC ${PHYSICS_SOURCES})
target_include_directories(APE_physics PUBLIC ${PHYSICS_DIR}/header)

###### CHANGE HERE TO COMPILE YOUR OWN IMGUI

# set(IMGUI_SOURCES
#   ${IMGUI_DIR}/imgui.cpp
#   ${IMGUI_DIR}/imgui_demo.cpp
#   ${IMGUI_DIR}/imgui_draw.cpp
#   ${IMGUI_DIR}/imgui_impl_glfw.cpp
#   ${IMGUI_DIR}/imgui_impl_opengl3.cpp
#   ${IMGUI_DIR}/imgui_tables.cpp
#   ${IMGUI_DIR}/imgui_widgets.cpp
# 	${IMGUI_DIR}/ImGuizmo.cpp
# ) 


# add_library(imgui STATIC ${IMGUI_SOURCES})
# target_include_directories(imgui PUBLIC ${IMGUI_DIR})

###GRABS THE STATIC IMGUI LIB
add_library(imgui STATIC IMPORTED)
set_target_properties(
	imgui
	PROPERTIES
	IMPORTED_LOCATION "${IMGUI_LIB}"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/extLibs/imgui"
)


# file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/assets/shaders DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJ_NAME}
	src/App/app.cpp 
  src/glad.c
)

target_include_directories(${PROJ_NAME} PUBLIC ${GLAD_DIR})

# Include directories
include_directories(include)

# Add the executable and link the libraries

### MAKE SURE TO LINK APE_core to APE_physics
target_link_libraries(APE_core PUBLIC APE_physics)
### MAKE SURE TO LINK APE_core to APE_buffers
target_link_libraries(APE_core PUBLIC APE_buffers)

target_link_libraries(${PROJ_NAME} 
  imgui
	APE_core
	APE_physics
  glfw 
  GL
	ReactPhysics3D::ReactPhysics3D
)
