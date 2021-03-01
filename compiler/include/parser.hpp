#pragma once

#include <vector>
#include <variant>

#include "lexer.hpp"

namespace Parser
{
    enum class NonTerminalNodeType
    {
        CLASS,
        CLASS_VAR_DEC,
        SUBROUTINE_DEC,
        PARAMETER_LIST,
        SUBROUTINE_BODY,
        VAR_DEC,
        STATEMENTS,
        WHILE_STATEMENT,
        IF_STATEMENT,
        RETURN_STATEMENT,
        LET_STATEMENT,
        DO_STATEMENT,
        EXPRESSION,
        TERM,
        EXPRESSION_LIST
    };

    class NonTerminalNode;
    typedef std::vector<std::variant<NonTerminalNode, Lexer::Token>> Nodes;

    class NonTerminalNode
    {
    private:
        Nodes nodes;
        NonTerminalNodeType nodeType;
    
    public:
        NonTerminalNode(NonTerminalNodeType nodeType);
        Nodes getNodes() const;
        NonTerminalNodeType getNodeType() const;
    };

    NonTerminalNode parseTokens(Lexer::Tokens tokens);
}