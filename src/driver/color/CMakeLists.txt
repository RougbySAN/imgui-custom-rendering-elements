project(color)

add_library(${PROJECT_NAME} STATIC 
                                color.cpp
)


target_include_directories(${PROJECT_NAME} 
                                PUBLIC
                                    ${CMAKE_CURRENT_SOURCE_DIR}
                                PRIVATE

                                INTERFACE
)

target_link_libraries(${PROJECT_NAME} 
                                        PUBLIC
                                            # Public directories
                                        PRIVATE
                                            # Private directories
                                        INTERFACE
                                            # Interface directories
) 