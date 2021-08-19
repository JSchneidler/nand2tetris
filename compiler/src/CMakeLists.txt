add_library(Jack-Compiler fs_utils.cpp tokenizer.cpp)
target_include_directories(Jack-Compiler PUBLIC ../include)

add_executable(JackC main.cpp)
target_link_libraries(JackC PRIVATE Jack-Compiler)