configure_file(Main.jack test/Main.jack COPYONLY)
configure_file(Main.xml test/Main.xml COPYONLY)
configure_file(MainT.xml test/MainT.xml COPYONLY)

link_libraries(Jack-Compiler)

add_executable(fsUtilsTests fs_utils.test.cpp)
add_test(fsUtils fsUtilsTests)

add_executable(tokenizerTests tokenizer.test.cpp)
add_test(tokenizer tokenizerTests)