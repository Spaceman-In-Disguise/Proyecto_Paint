# CMakeList.txt : CMake project for proyecto#1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (proyecto1 "src/proyecto#1.cpp" "src/proyecto#1.h" "src/engine2D.h" "src/engine2D.cpp"
        src/line.cpp
        src/line.h
        src/canvas.cpp
        src/canvas.h
        src/rectangle.cpp
        src/rectangle.h
        src/triangle.cpp
        src/triangle.h
        src/ellipse.cpp
        src/ellipse.h
        src/quadTree.cpp
        src/quadTree.h
        src/quadNode.cpp
        src/quadNode.h
        src/editor_memento.h
        src/history_manager.h)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET proyecto1 PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(proyecto1 PRIVATE include)

target_link_libraries(proyecto1 PRIVATE 
    glfw  
    glm
    glad
    imgui
    OpenGL::GL
)