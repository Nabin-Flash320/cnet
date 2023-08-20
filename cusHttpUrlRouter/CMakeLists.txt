


add_library(cusHttpUrlRouter cusHttpUrlRouter.c)

add_dependencies(cusHttpUrlRouter 
                    trace)

target_include_directories(cusHttpUrlRouter PUBLIC 
                            ../trace)
