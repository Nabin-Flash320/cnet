

add_library(cusSocket cusSocket.c)

add_dependencies(cusSocket 
                    cusHttpParser 
                    trace 
                    cusHttpUrlRouter)

target_include_directories(cusSocket PUBLIC 
                            ../cusHttpParser 
                            ../trace 
                            ../cusHttpUrlRouter)
