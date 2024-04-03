project(imgui)

add_library(${PROJECT_NAME} STATIC 
                                imgui_demo.cpp
                                imgui_draw.cpp
                                imgui_tables.cpp
                                imgui_widgets.cpp
                                imgui.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
                                ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)


find_package(OpenGL REQUIRED)


target_include_directories(${PROJECT_NAME} 
                                PUBLIC
                                    ${CMAKE_CURRENT_SOURCE_DIR}/backends
                                    ${CMAKE_CURRENT_SOURCE_DIR}
                                PRIVATE

                                INTERFACE
)

target_link_libraries(${PROJECT_NAME} 
                                PUBLIC
                                    glfw
                                    "opengl32"
                                PRIVATE

                                INTERFACE
)    