cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME do_ast)
project( ${PROJECT_NAME} )

add_library(
    ${PROJECT_NAME} 
    STATIC

    src/v1_ast.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

set(
    EXAMPLE_NAMES 
    eg00_basic
    eg01_calculator
    eg02_benchmark
    eg03_v2
    eg04_lisp
    eg05_v3_postorder_storage
    eg06_v3_nodes
)
foreach(EXAMPLE_NAME IN LISTS EXAMPLE_NAMES)
    add_executable(${PROJECT_NAME}_${EXAMPLE_NAME} src/examples/${EXAMPLE_NAME}.cpp)
    target_link_libraries(${PROJECT_NAME}_${EXAMPLE_NAME} ${PROJECT_NAME})
endforeach()
