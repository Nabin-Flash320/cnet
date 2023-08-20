
set(INCLUDE_DIRS 
        cusSocket 
        cusHttpParser 
        trace 
        cusHttpUrlRouter
    )

set(LINK_LIBS
        cusSocket 
        cusHttpParser 
        trace 
        cusHttpUrlRouter
    )


cmake_minimum_required(VERSION 3.5)
project(cnet)


add_executable(main main.c)

add_subdirectory(cusSocket)
add_subdirectory(cusHttpParser)
add_subdirectory(trace)
add_subdirectory(cusHttpUrlRouter)

target_include_directories(main PUBLIC ${INCLUDE_DIRS})

target_link_libraries(main ${LINK_LIBS})


