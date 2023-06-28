from __future__ import annotations

from enum import Enum
from typing import Optional

from .tokenizer import Token, Tokens, TokenType, tokenToXML
from .symbol_table import SymbolTable


class NonTerminalType(Enum):
    CLASS = "class"
    CLASS_VAR_DEC = "classVarDec"
    SUBROUTINE_DEC = "subroutineDec"
    PARAMETER_LIST = "parameterList"
    SUBROUTINE_BODY = "subroutineBody"
    VAR_DEC = "varDec"

    STATEMENTS = "statements"
    WHILE_STATEMENT = "whileStatement"
    IF_STATEMENT = "ifStatement"
    RETURN_STATEMENT = "returnStatement"
    LET_STATEMENT = "letStatement"
    DO_STATEMENT = "doStatement"

    EXPRESSION = "expression"
    TERM = "term"
    EXPRESSION_LIST = "expressionList"


class NonTerminalNode:
    def __init__(self, type: NonTerminalType):
        self.children: list[NonTerminalNode | Token] = []
        self.type = type

    def addChild(self, child: NonTerminalNode | Token):
        self.children.append(child)

    def addChildren(self, children: tuple[NonTerminalNode | Token]):
        for child in children:
            self.addChild(child)


class ParserException(Exception):
    pass


OPS = ["+", "-", "*", "/", "&", "|", "<", ">", "="]
UNARY_OPS = ["-", "~"]
KEYWORD_CONSTANTS = ["true", "false", "null", "this"]


class Parser:
    def __init__(self, tokens: Tokens):
        self.tokens = tokens
        self.tokenIndex = 0
        self.classSymbols = SymbolTable()
        self.subroutineSymbols = SymbolTable()
        self.root = self.parseClass()

    def checkType(self, token: Token, shouldRaise: Optional[bool] = True) -> bool:
        if not (
            token.getType() == TokenType.IDENTIFIER
            or token.getValue() in ["int", "char", "boolean"]
        ):
            if shouldRaise:
                raise ParserException(
                    f"Expected valid type, not '{token.getValue()}' @ {token.line}.{token.lineIndex}"
                )
            return False
        return True

    def consumeToken(
        self,
        parentNode: NonTerminalNode,
        type: Optional[TokenType] = None,
        value: Optional[str] = None,
    ):
        token = self.tokens[self.tokenIndex]
        if (type and token.getType() != type) or (value and token.getValue() != value):
            raise ParserException(
                f"Expected '{value or type.value}', not '{token.getValue()}' @ {token.line}.{token.lineIndex}"
            )
        parentNode.addChild(token)
        self.tokenIndex += 1

    def currentToken(self) -> Token:
        return self.tokens[self.tokenIndex]

    def getNthToken(self, i: int) -> Token:
        return self.tokens[self.tokenIndex + i]

    def parseClass(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.CLASS)

        self.consumeToken(node, TokenType.KEYWORD, "class")
        self.consumeToken(node, TokenType.IDENTIFIER)
        self.consumeToken(node, TokenType.SYMBOL, "{")

        while True:
            if self.currentToken().getValue() in ["static", "field"]:
                node.addChild(self.parseClassVarDec())
            elif self.currentToken().getValue() in [
                "constructor",
                "function",
                "method",
            ]:
                node.addChild(self.parseSubroutineDec())
            else:
                break

        self.consumeToken(node, TokenType.SYMBOL, "}")

        return node

    def parseClassVarDec(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.CLASS_VAR_DEC)

        self.consumeToken(node, TokenType.KEYWORD)
        self.parseGenericVarDec(node)
        self.consumeToken(node, TokenType.SYMBOL, ";")

        return node

    def parseSubroutineDec(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.SUBROUTINE_DEC)
        self.consumeToken(node, TokenType.KEYWORD)

        if self.currentToken().getValue() == "void" or self.checkType(
            self.currentToken(), False
        ):
            self.consumeToken(node)
        else:
            raise ParserException(
                f"Expected valid type or 'void', not '{self.currentToken().getValue()}' @ {self.currentToken().line}.{self.currentToken().lineIndex}"
            )

        self.consumeToken(node, TokenType.IDENTIFIER)
        self.consumeToken(node, TokenType.SYMBOL, "(")
        node.addChild(self.parseParameterList())
        self.consumeToken(node, TokenType.SYMBOL, ")")
        node.addChild(self.parseSubroutineBody())

        return node

    def parseParameterList(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.PARAMETER_LIST)

        if self.checkType(self.currentToken(), False):
            self.consumeToken(node)
            self.consumeToken(node, TokenType.IDENTIFIER)
            while True:
                if self.currentToken().getValue() == ",":
                    self.consumeToken(node, TokenType.SYMBOL, ",")
                    self.checkType(self.currentToken())
                    self.consumeToken(node)
                    self.consumeToken(node, TokenType.IDENTIFIER)
                else:
                    break

        return node

    def parseSubroutineBody(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.SUBROUTINE_BODY)

        self.consumeToken(node, TokenType.SYMBOL, "{")

        while True:
            if self.currentToken().getValue() == "var":
                node.addChild(self.parseVarDec())
            else:
                break

        node.addChild(self.parseStatements())
        self.consumeToken(node, TokenType.SYMBOL, "}")

        return node

    def parseVarDec(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.VAR_DEC)

        self.consumeToken(node, TokenType.KEYWORD, "var")
        self.parseGenericVarDec(node)
        self.consumeToken(node, TokenType.SYMBOL, ";")

        return node

    def parseStatements(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.STATEMENTS)

        while True:
            match self.currentToken().getValue():
                case "let":
                    node.addChild(self.parseLetStatement())
                case "if":
                    node.addChild(self.parseIfStatement())
                case "while":
                    node.addChild(self.parseWhileStatement())
                case "do":
                    node.addChild(self.parseDoStatement())
                case "return":
                    node.addChild(self.parseReturnStatement())
                case _:
                    break

        return node

    def parseLetStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.LET_STATEMENT)

        self.consumeToken(node, TokenType.KEYWORD, "let")
        self.consumeToken(node, TokenType.IDENTIFIER)

        if self.currentToken().getValue() == "[":
            self.consumeToken(node)
            node.addChild(self.parseExpression())
            self.consumeToken(node, TokenType.SYMBOL, "]")

        self.consumeToken(node, TokenType.SYMBOL, "=")
        node.addChild(self.parseExpression())
        self.consumeToken(node, TokenType.SYMBOL, ";")

        return node

    def parseIfStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.IF_STATEMENT)

        self.consumeToken(node, TokenType.KEYWORD, "if")
        self.consumeToken(node, TokenType.SYMBOL, "(")
        node.addChild(self.parseExpression())
        self.consumeToken(node, TokenType.SYMBOL, ")")
        self.consumeToken(node, TokenType.SYMBOL, "{")
        node.addChild(self.parseStatements())
        self.consumeToken(node, TokenType.SYMBOL, "}")

        if self.currentToken().getValue() == "else":
            self.consumeToken(node, TokenType.KEYWORD, "else")
            self.consumeToken(node, TokenType.SYMBOL, "{")
            node.addChild(self.parseStatements())
            self.consumeToken(node, TokenType.SYMBOL, "}")

        return node

    def parseWhileStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.WHILE_STATEMENT)

        self.consumeToken(node, TokenType.KEYWORD, "while")
        self.consumeToken(node, TokenType.SYMBOL, "(")
        node.addChild(self.parseExpression())
        self.consumeToken(node, TokenType.SYMBOL, ")")
        self.consumeToken(node, TokenType.SYMBOL, "{")
        node.addChild(self.parseStatements())
        self.consumeToken(node, TokenType.SYMBOL, "}")

        return node

    def parseDoStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.DO_STATEMENT)

        self.consumeToken(node, TokenType.KEYWORD, "do")
        self.parseSubroutineCall(node)
        self.consumeToken(node, TokenType.SYMBOL, ";")

        return node

    def parseSubroutineCall(self, parentNode: NonTerminalNode):
        if self.getNthToken(1).getValue() == "(":
            self.consumeToken(parentNode, TokenType.IDENTIFIER)
            self.consumeToken(parentNode, TokenType.SYMBOL, "(")
            parentNode.addChild(self.parseExpressionList())
            self.consumeToken(parentNode, TokenType.SYMBOL, ")")
        elif self.getNthToken(1).getValue() == ".":
            self.consumeToken(parentNode, TokenType.IDENTIFIER)
            self.consumeToken(parentNode, TokenType.SYMBOL, ".")
            self.consumeToken(parentNode, TokenType.IDENTIFIER)
            self.consumeToken(parentNode, TokenType.SYMBOL, "(")
            parentNode.addChild(self.parseExpressionList())
            self.consumeToken(parentNode, TokenType.SYMBOL, ")")
        else:
            raise ParserException(
                f"Expected valid subroutine identifier, not '{self.currentToken().getValue()}' @ {self.currentToken().line}.{self.currentToken().lineIndex}"
            )

    def parseReturnStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.RETURN_STATEMENT)

        self.consumeToken(node)
        if self.isTerm():
            node.addChild(self.parseExpression())
        self.consumeToken(node, TokenType.SYMBOL, ";")

        return node

    def parseExpression(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.EXPRESSION)

        node.addChild(self.parseTerm())

        while True:
            if self.currentToken().getValue() in OPS:
                self.consumeToken(node, TokenType.SYMBOL)
                node.addChild(self.parseTerm())
            else:
                break

        return node

    def parseExpressionList(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.EXPRESSION_LIST)

        if self.isExpression():
            node.addChild(self.parseExpression())
            while True:
                if self.currentToken().getValue() == ",":
                    self.consumeToken(node, TokenType.SYMBOL, ",")
                    node.addChild(self.parseExpression())
                else:
                    break

        return node

    def parseTerm(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.TERM)

        if self.currentToken().getType() == TokenType.INTEGER_CONST:
            self.consumeToken(node)
        elif self.currentToken().getType() == TokenType.STRING_CONST:
            self.consumeToken(node)
        elif (
            self.currentToken().getType() == TokenType.KEYWORD
            and self.currentToken().getValue() in KEYWORD_CONSTANTS
        ):
            self.consumeToken(node)
        elif self.currentToken().getValue() == "(":
            self.consumeToken(node)
            node.addChild(self.parseExpression())
            self.consumeToken(node, TokenType.SYMBOL, ")")
        elif self.currentToken().getValue() in UNARY_OPS:
            self.consumeToken(node)
            node.addChild(self.parseTerm())
        elif self.isSubroutineCall():
            self.parseSubroutineCall(node)
        elif self.isVarName():
            self.consumeToken(node)

            if self.currentToken().getValue() == "[":
                self.consumeToken(node)
                node.addChild(self.parseExpression())
                self.consumeToken(node, TokenType.SYMBOL, "]")

        return node

    def parseGenericVarDec(self, parentNode: NonTerminalNode):
        self.checkType(self.currentToken())

        self.consumeToken(parentNode)
        self.consumeToken(parentNode, TokenType.IDENTIFIER)

        while True:
            if self.currentToken().getValue() == ",":
                self.consumeToken(parentNode)
                self.consumeToken(parentNode, TokenType.IDENTIFIER)
            else:
                break

    def isExpression(self) -> bool:
        return self.isTerm()

    def isTerm(self) -> bool:
        if self.currentToken().getType() == TokenType.INTEGER_CONST:
            return True
        elif self.currentToken().getType() == TokenType.STRING_CONST:
            return True
        elif (
            self.currentToken().getType() == TokenType.KEYWORD
            and self.currentToken().getValue() in KEYWORD_CONSTANTS
        ):
            return True
        elif self.currentToken().getValue() == "(":
            return True
        elif self.currentToken().getValue() in UNARY_OPS:
            return True
        elif self.isSubroutineCall():
            return True
        elif self.isVarName():
            return True
        return False

    def isClassName(self) -> bool:
        return self.currentToken().getType() == TokenType.IDENTIFIER

    def isVarName(self) -> bool:
        return self.currentToken().getType() == TokenType.IDENTIFIER

    def isSubroutineName(self) -> bool:
        return self.currentToken().getType() == TokenType.IDENTIFIER

    def isSubroutineCall(self) -> bool:
        return (self.isSubroutineName() and self.getNthToken(1).getValue() == "(") or (
            (self.isClassName() or self.isVarName())
            and self.getNthToken(1).getValue() == "."
        )


def parseTokens(tokens: Tokens) -> NonTerminalNode:
    return Parser(tokens).root


def treeToXML(node: NonTerminalNode, depth: int = 0) -> str:
    TAB_SIZE = 2
    spacing = " " * depth * TAB_SIZE

    xml = f"{spacing}<{node.type.value}>\n"

    for child in node.children:
        if isinstance(child, Token):
            tokenXml = tokenToXML(child)
            xml += f"{spacing}{' '*TAB_SIZE}{tokenXml}"
        elif isinstance(child, NonTerminalNode):
            xml += treeToXML(child, depth + 1)
        else:
            raise ParserException(f"Invalid child type '{type(child)}'")

    xml += f"{spacing}</{node.type.value}>\n"
    return xml
