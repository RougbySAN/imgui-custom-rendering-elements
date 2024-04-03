
project(main)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} 
                                        PUBLIC
                                            # Public directories

                                        PRIVATE
                                            # Private directories
                                        
                                        INTEFACE
                                            # Interface directories
)                                           


target_link_libraries(${PROJECT_NAME} 
                                        PUBLIC
                                            # Public directories
                                            imgui
                                        PRIVATE
                                            # Private directories
                                        INTERFACE
                                            # Interface directories
)  