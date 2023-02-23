add_executable(main main.cpp)
add_executable(call call.cpp)

target_link_libraries(main PRIVATE fmt::fmt spdlog::spdlog)
