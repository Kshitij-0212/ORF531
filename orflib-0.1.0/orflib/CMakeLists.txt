set(orflib_SOURCES 
    empty.cpp 
)

add_library(orflib STATIC ${orflib_SOURCES})

target_include_directories(orflib PRIVATE 
    .. 
)

# compiler and linker option adjustments for targets
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(orflib PRIVATE "/permissive-")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
    target_compile_options(orflib PRIVATE -fPIC)
endif()
