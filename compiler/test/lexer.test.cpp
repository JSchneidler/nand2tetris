#define BOOST_TEST_MODULE LexerTests

#include <boost/test/included/unit_test.hpp>
#include <filesystem>
#include <fstream>

#include "lexer.hpp"

BOOST_AUTO_TEST_SUITE(LexerTestSuite)

BOOST_AUTO_TEST_CASE(tokenizeJackFile_tests)
{
    Node::TerminalNodes expectedNodes{
        Node::TerminalNode(Node::TerminalNodeType::KEYWORD, "class"),
        Node::TerminalNode(Node::TerminalNodeType::IDENTIFIER, "Main"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "{"),
        Node::TerminalNode(Node::TerminalNodeType::KEYWORD, "function"),
        Node::TerminalNode(Node::TerminalNodeType::KEYWORD, "void"),
        Node::TerminalNode(Node::TerminalNodeType::IDENTIFIER, "main"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "("),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, ")"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "{"),
        Node::TerminalNode(Node::TerminalNodeType::KEYWORD, "var"),
        Node::TerminalNode(Node::TerminalNodeType::IDENTIFIER, "String"),
        Node::TerminalNode(Node::TerminalNodeType::IDENTIFIER, "s"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "="),
        Node::TerminalNode(Node::TerminalNodeType::STRING_CONST, "Hello, world!"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, ";"),
        Node::TerminalNode(Node::TerminalNodeType::KEYWORD, "var"),
        Node::TerminalNode(Node::TerminalNodeType::KEYWORD, "int"),
        Node::TerminalNode(Node::TerminalNodeType::IDENTIFIER, "i"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "="),
        Node::TerminalNode(Node::TerminalNodeType::INTEGER_CONST, "1000"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, ";"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "}"),
        Node::TerminalNode(Node::TerminalNodeType::SYMBOL, "}"),
    };
    Node::TerminalNodes actualNodes = Lexer::tokenizeJackFile("test/LexerTest.jack");

    BOOST_TEST(actualNodes == expectedNodes);
}

BOOST_AUTO_TEST_CASE(tokenizeJackFileToXML_tests)
{
    std::ifstream inputXMLStream("test/MainTokens.xml");
    std::string expectedXML((std::istreambuf_iterator<char>(inputXMLStream)), (std::istreambuf_iterator<char>()));
    Node::TerminalNodes nodes = Lexer::tokenizeJackFile("test/Main.jack");

    BOOST_TEST(Lexer::getXML(nodes) == expectedXML);
}

BOOST_AUTO_TEST_SUITE_END()