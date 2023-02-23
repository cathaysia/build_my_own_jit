cmake_minimum_required(VERSION 3.18)
project(cmake_template)

set(CMAKE_CXX_STANDARD 11 REQUIRED)
set(CMAKE_C_STANDARD 99)

option(template_BUILD_TEST OFF "是否启用测试")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(extern)
add_subdirectory(src)

if(${template_BUILD_TEST})
  enable_testing()
  add_subdirectory(test)
endif()
