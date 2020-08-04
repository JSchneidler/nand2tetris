add_library(Jack-Compiler fs_utils.cpp)
target_compile_features(Jack-Compiler PRIVATE cxx_std_17)
target_include_directories(Jack-Compiler PUBLIC ../include)

add_executable(JackC main.cpp)
target_compile_features(JackC PRIVATE cxx_std_17)
target_link_libraries(JackC PRIVATE Jack-Compiler)