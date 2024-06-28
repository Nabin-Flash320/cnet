

add_library(cusHttpParser cusHttpRequestParser.c cusHttpResponseParser.c)

add_dependencies(cusHttpParser 
                    trace)

target_include_directories(cusHttpParser PUBLIC 
                            ../trace)
