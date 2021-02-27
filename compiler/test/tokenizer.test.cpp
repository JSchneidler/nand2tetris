#define BOOST_TEST_MODULE TokenizerTests

#include <boost/test/included/unit_test.hpp>
#include <filesystem>

#include "tokenizer.hpp"

BOOST_AUTO_TEST_SUITE(TokenizerTestSuite)

BOOST_AUTO_TEST_CASE(tokenizeJackFile_tests)
{
    Tokenizer::Tokens expectedTokens{
        Tokenizer::Token(Tokenizer::TokenType::KEYWORD, "class"),
        Tokenizer::Token(Tokenizer::TokenType::IDENTIFIER, "Main"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "{"),
        Tokenizer::Token(Tokenizer::TokenType::KEYWORD, "function"),
        Tokenizer::Token(Tokenizer::TokenType::KEYWORD, "void"),
        Tokenizer::Token(Tokenizer::TokenType::IDENTIFIER, "main"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "("),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, ")"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "{"),
        Tokenizer::Token(Tokenizer::TokenType::KEYWORD, "var"),
        Tokenizer::Token(Tokenizer::TokenType::IDENTIFIER, "String"),
        Tokenizer::Token(Tokenizer::TokenType::IDENTIFIER, "s"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "="),
        Tokenizer::Token(Tokenizer::TokenType::STRING_CONST, "Hello, world!"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, ";"),
        Tokenizer::Token(Tokenizer::TokenType::KEYWORD, "var"),
        Tokenizer::Token(Tokenizer::TokenType::KEYWORD, "int"),
        Tokenizer::Token(Tokenizer::TokenType::IDENTIFIER, "i"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "="),
        Tokenizer::Token(Tokenizer::TokenType::INTEGER_CONST, "1000"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, ";"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "}"),
        Tokenizer::Token(Tokenizer::TokenType::SYMBOL, "}"),
    };
    Tokenizer::Tokens actualTokens = Tokenizer::tokenizeJackFile("test/TokenizerTest.jack");
    BOOST_TEST(actualTokens == expectedTokens);
}

BOOST_AUTO_TEST_SUITE_END()