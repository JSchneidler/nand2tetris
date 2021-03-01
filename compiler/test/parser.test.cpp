#define BOOST_TEST_MODULE ParserTests

#include <boost/test/included/unit_test.hpp>
#include <filesystem>

#include "parser.hpp"

BOOST_AUTO_TEST_SUITE(ParserTestSuite)

BOOST_AUTO_TEST_CASE(parseJackFile_tests)
{
    std::ifstream inputXMLStream("test/MainTokens.xml");
    std::string expectedXML((std::istreambuf_iterator<char>(inputXMLStream)), (std::istreambuf_iterator<char>()));

    BOOST_TEST("" == expectedXML);
}

BOOST_AUTO_TEST_SUITE_END()