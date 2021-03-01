#define BOOST_TEST_MODULE LexerTests

#include <boost/test/included/unit_test.hpp>
#include <filesystem>
#include <fstream>

#include "lexer.hpp"

BOOST_AUTO_TEST_SUITE(LexerTestSuite)

BOOST_AUTO_TEST_CASE(tokenizeJackFile_tests)
{
    Lexer::Tokens expectedTokens{
        Lexer::Token(Lexer::TokenType::KEYWORD, "class"),
        Lexer::Token(Lexer::TokenType::IDENTIFIER, "Main"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "{"),
        Lexer::Token(Lexer::TokenType::KEYWORD, "function"),
        Lexer::Token(Lexer::TokenType::KEYWORD, "void"),
        Lexer::Token(Lexer::TokenType::IDENTIFIER, "main"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "("),
        Lexer::Token(Lexer::TokenType::SYMBOL, ")"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "{"),
        Lexer::Token(Lexer::TokenType::KEYWORD, "var"),
        Lexer::Token(Lexer::TokenType::IDENTIFIER, "String"),
        Lexer::Token(Lexer::TokenType::IDENTIFIER, "s"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "="),
        Lexer::Token(Lexer::TokenType::STRING_CONST, "Hello, world!"),
        Lexer::Token(Lexer::TokenType::SYMBOL, ";"),
        Lexer::Token(Lexer::TokenType::KEYWORD, "var"),
        Lexer::Token(Lexer::TokenType::KEYWORD, "int"),
        Lexer::Token(Lexer::TokenType::IDENTIFIER, "i"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "="),
        Lexer::Token(Lexer::TokenType::INTEGER_CONST, "1000"),
        Lexer::Token(Lexer::TokenType::SYMBOL, ";"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "}"),
        Lexer::Token(Lexer::TokenType::SYMBOL, "}"),
    };
    Lexer::Tokens actualTokens = Lexer::tokenizeJackFile("test/LexerTest.jack");

    BOOST_TEST(actualTokens == expectedTokens);
}

BOOST_AUTO_TEST_CASE(tokenizeJackFileToXML_tests)
{
    std::ifstream inputXMLStream("test/MainTokens.xml");
    std::string expectedXML((std::istreambuf_iterator<char>(inputXMLStream)), (std::istreambuf_iterator<char>()));
    Lexer::Tokens tokens = Lexer::tokenizeJackFile("test/Main.jack");

    BOOST_TEST(Lexer::getXML(tokens) == expectedXML);
}

BOOST_AUTO_TEST_SUITE_END()