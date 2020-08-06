configure_file(Main.jack test/Main.jack COPYONLY)
configure_file(Main.xml test/Main.xml COPYONLY)
configure_file(MainT.xml test/MainT.xml COPYONLY)

add_executable(fsUtilsTests fs_utils.test.cpp ../src/fs_utils.cpp)
target_compile_features(fsUtilsTests PRIVATE cxx_std_17)
target_include_directories(fsUtilsTests PRIVATE ${Boost_INCLUDE_DIRS} ../include)
add_test(fsUtils fsUtilsTests)

add_executable(tokenizerTests tokenizer.test.cpp ../src/tokenizer.cpp)
target_compile_features(tokenizerTests PRIVATE cxx_std_17)
target_include_directories(tokenizerTests PRIVATE ${Boost_INCLUDE_DIRS} ../include)
add_test(tokenizer tokenizerTests)