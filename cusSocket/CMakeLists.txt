

add_library(cusSocket cusSocket.c)

add_dependencies(cusSocket 
                    cusHttpParser 
                    trace 
                    cusHttpUrlRouter)

target_include_directories(cusSocket PUBLIC 
                            ../cusHttpParser 
                            ../trace 
                            ../cusHttpUrlRouter)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   target_link_libraries(cusSocket ws2_32)
endif()
