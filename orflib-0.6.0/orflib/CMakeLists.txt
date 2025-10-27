set(orflib_SOURCES
    math/interpol/piecewisepolynomial.cpp 
    math/stats/errorfunction.cpp
    pricers/simplepricers.cpp
    pricers/bsmcpricer.cpp
    market/market.cpp
    market/yieldcurve.cpp
    market/volatilitytermstructure.cpp
)

add_library(orflib STATIC ${orflib_SOURCES})

target_include_directories(orflib PRIVATE 
    .. 
    ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/include
)

# compiler and linker option adjustments for targets
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(orflib PRIVATE "/permissive-")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") 
    target_compile_options(orflib PRIVATE -fPIC)
endif()
