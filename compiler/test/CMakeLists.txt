find_package(Boost 1.73 REQUIRED)

add_executable(fsUtilsTest fs_utils.test.cpp ../src/fs_utils.cpp)

target_compile_features(fsUtilsTest PRIVATE cxx_std_17)
target_include_directories(fsUtilsTest PRIVATE ${Boost_INCLUDE_DIRS} ../include)

add_test(fsUtils fsUtilsTest)

configure_file(Main.jack test/Main.jack COPYONLY)
configure_file(Main.xml test/Main.xml COPYONLY)
configure_file(MainT.xml test/MainT.xml COPYONLY)