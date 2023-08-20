

add_library(cusHttpParser cusHttpParser.c)

add_dependencies(cusHttpParser 
                    trace 
                    cusHttpUrlRouter)

target_include_directories(cusHttpParser PUBLIC 
                            ../trace 
                            ../cusHttpUrlRouter)
