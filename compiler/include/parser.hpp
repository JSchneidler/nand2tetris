#pragma once

#include "node.hpp"
#include "lexer.hpp"

namespace Parser
{
    class Parser
    {
    private:
        Node::TerminalNodes nodes;
        Node::NonTerminalNode rootNode {Node::NonTerminalNodeType::CLASS};

        template <typename T>
        void consumeNode(Node::NonTerminalNode node, const T nodeType, const std::string error);
        void consumeNode(Node::NonTerminalNode node, const std::vector<std::variant<Node::Keyword, Node::Symbol, Node::TerminalNode>> allowedTypes, const std::string error);

        Node::NonTerminalNode parseExpressionList();
        Node::NonTerminalNode parseTerm();
        Node::NonTerminalNode parseExpression();
        Node::NonTerminalNode parseIf();
        Node::NonTerminalNode parseReturn();
        Node::NonTerminalNode parseWhile();
        Node::NonTerminalNode parseLet();
        Node::NonTerminalNode parseDo();
        Node::NonTerminalNode parseStatements();
        Node::NonTerminalNode parseVarDec();
        Node::NonTerminalNode parseParameterList();
        Node::NonTerminalNode parseSubroutine();
        Node::NonTerminalNode parseClassVarDec();
        void parseClass();

    public:
        Parser(Node::TerminalNodes tokens);
    };
}