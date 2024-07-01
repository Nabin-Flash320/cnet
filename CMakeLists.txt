


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

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 11)

project(cnet)

# Check the operating system
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
    add_definitions(-D_WIN32)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

add_executable(main ./main.c)

add_subdirectory(cusSocket)
add_subdirectory(cusHttpParser)
add_subdirectory(trace)
add_subdirectory(cusHttpUrlRouter)

target_include_directories(main PUBLIC ${INCLUDE_DIRS})

target_link_libraries(main ${LINK_LIBS})



