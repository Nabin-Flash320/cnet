
add_library(cusHttpRequestHandler cusHttpRequestHandler.c)

add_dependencies(cusHttpRequestHandler
                    trace
                    cusHttpUrlRouter)

target_include_directories(cusHttpRequestHandler PUBLIC 
                            ../trace
                            ../cusHttpUrlRouter)

