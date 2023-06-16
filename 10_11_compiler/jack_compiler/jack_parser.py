from __future__ import annotations

from enum import Enum
from typing import Optional

from .tokenizer import Token, Tokens, TokenType, tokenToXML


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


def assertToken(token: Token, type: TokenType, value: Optional[str] = None):
    if token.getType() != type or (value and token.getValue() != value):
        raise ParserException(
            f"Expected '{value or type.value}', not '{token.getValue()}' @ {token.line}.{token.lineIndex}"
        )


OPS = ["+", "-", "*", "/", "&", "|", "<", ">", "="]
UNARY_OPS = ["-", "~"]
KEYWORD_CONSTANTS = ["true", "false", "null", "this"]


class Parser:
    def __init__(self, tokens: Tokens):
        self.tokens = tokens
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

    def parseClass(self) -> NonTerminalNode:
        assertToken(self.tokens[0], TokenType.KEYWORD, "class")
        root = NonTerminalNode(NonTerminalType.CLASS)
        root.addChild(self.tokens[0])

        assertToken(self.tokens[1], TokenType.IDENTIFIER)
        root.addChild(self.tokens[1])

        assertToken(self.tokens[2], TokenType.SYMBOL, "{")
        root.addChild(self.tokens[2])

        self.tokens = self.tokens[3:]

        while True:
            if self.tokens[0].getValue() in ["static", "field"]:
                root.addChild(self.parseClassVarDec())
            elif self.tokens[0].getValue() in ["constructor", "function", "method"]:
                root.addChild(self.parseSubroutineDec())
            else:
                break

        assertToken(self.tokens[0], TokenType.SYMBOL, "}")
        root.addChild(self.tokens[0])

        return root

    def parseClassVarDec(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.CLASS_VAR_DEC)
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]
        node.addChildren(self.parseGenericVarDec())

        assertToken(self.tokens[0], TokenType.SYMBOL, ";")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseSubroutineDec(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.SUBROUTINE_DEC)
        node.addChild(self.tokens[0])

        if self.tokens[1].getValue() == "void" or self.checkType(self.tokens[1], False):
            node.addChild(self.tokens[1])
        else:
            raise ParserException(
                f"Expected valid type or 'void', not '{self.tokens[1].getValue()}' @ {self.tokens[1].line}.{self.tokens[1].lineIndex}"
            )

        assertToken(self.tokens[2], TokenType.IDENTIFIER)
        node.addChild(self.tokens[2])

        assertToken(self.tokens[3], TokenType.SYMBOL, "(")
        node.addChild(self.tokens[3])

        self.tokens = self.tokens[4:]
        node.addChild(self.parseParameterList())

        assertToken(self.tokens[0], TokenType.SYMBOL, ")")
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]
        node.addChild(self.parseSubroutineBody())

        return node

    def parseParameterList(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.PARAMETER_LIST)

        if self.checkType(self.tokens[0], False):
            node.addChild(self.tokens[0])

            assertToken(self.tokens[1], TokenType.IDENTIFIER)
            node.addChild(self.tokens[1])

            self.tokens = self.tokens[2:]
            while True:
                if self.tokens[0].getValue() == ",":
                    node.addChild(self.tokens[0])

                    self.checkType(self.tokens[1])
                    node.addChild(self.tokens[1])

                    assertToken(self.tokens[2], TokenType.IDENTIFIER)
                    node.addChild(self.tokens[2])
                    self.tokens = self.tokens[3:]
                else:
                    break

        return node

    def parseSubroutineBody(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.SUBROUTINE_BODY)

        assertToken(self.tokens[0], TokenType.SYMBOL, "{")
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]

        while True:
            if self.tokens[0].getValue() == "var":
                node.addChild(self.parseVarDec())
            else:
                break

        node.addChild(self.parseStatements())

        assertToken(self.tokens[0], TokenType.SYMBOL, "}")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseVarDec(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.VAR_DEC)

        assertToken(self.tokens[0], TokenType.KEYWORD, "var")
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]
        node.addChildren(self.parseGenericVarDec())

        assertToken(self.tokens[0], TokenType.SYMBOL, ";")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseStatements(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.STATEMENTS)

        while True:
            match self.tokens[0].getValue():
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
        node.addChild(self.tokens[0])

        assertToken(self.tokens[1], TokenType.IDENTIFIER)
        node.addChild(self.tokens[1])
        self.tokens = self.tokens[2:]

        if self.tokens[0].getValue() == "[":
            node.addChild(self.tokens[0])

            self.tokens = self.tokens[1:]
            node.addChild(self.parseExpression())

            assertToken(self.tokens[0], TokenType.SYMBOL, "]")
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]

        assertToken(self.tokens[0], TokenType.SYMBOL, "=")
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]
        node.addChild(self.parseExpression())

        assertToken(self.tokens[0], TokenType.SYMBOL, ";")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseIfStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.IF_STATEMENT)
        node.addChild(self.tokens[0])

        assertToken(self.tokens[1], TokenType.SYMBOL, "(")
        node.addChild(self.tokens[1])

        self.tokens = self.tokens[2:]
        node.addChild(self.parseExpression())

        assertToken(self.tokens[0], TokenType.SYMBOL, ")")
        node.addChild(self.tokens[0])

        assertToken(self.tokens[1], TokenType.SYMBOL, "{")
        node.addChild(self.tokens[1])

        self.tokens = self.tokens[2:]
        node.addChild(self.parseStatements())

        assertToken(self.tokens[0], TokenType.SYMBOL, "}")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        if self.tokens[0].getValue() == "else":
            node.addChild(self.tokens[0])

            assertToken(self.tokens[1], TokenType.SYMBOL, "{")
            node.addChild(self.tokens[1])

            self.tokens = self.tokens[2:]
            node.addChild(self.parseStatements())

            assertToken(self.tokens[0], TokenType.SYMBOL, "}")
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]

        return node

    def parseWhileStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.WHILE_STATEMENT)
        node.addChild(self.tokens[0])

        assertToken(self.tokens[1], TokenType.SYMBOL, "(")
        node.addChild(self.tokens[1])

        self.tokens = self.tokens[2:]
        node.addChild(self.parseExpression())

        assertToken(self.tokens[0], TokenType.SYMBOL, ")")
        node.addChild(self.tokens[0])

        assertToken(self.tokens[1], TokenType.SYMBOL, "{")
        node.addChild(self.tokens[1])

        self.tokens = self.tokens[2:]
        node.addChild(self.parseStatements())

        assertToken(self.tokens[0], TokenType.SYMBOL, "}")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseDoStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.DO_STATEMENT)
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]
        node.addChildren(self.parseSubroutineCall())

        assertToken(self.tokens[0], TokenType.SYMBOL, ";")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseSubroutineCall(self) -> tuple[NonTerminalNode | Token]:
        tokens: list[NonTerminalNode | Token] = []

        if self.tokens[1].getValue() == "(":
            assertToken(self.tokens[0], TokenType.IDENTIFIER)
            tokens.append(self.tokens[0])

            tokens.append(self.tokens[1])

            self.tokens = self.tokens[2:]
            tokens.append(self.parseExpressionList())

            assertToken(self.tokens[0], TokenType.SYMBOL, ")")
            tokens.append(self.tokens[0])
            self.tokens = self.tokens[1:]
        elif self.tokens[1].getValue() == ".":
            assertToken(self.tokens[0], TokenType.IDENTIFIER)
            tokens.append(self.tokens[0])

            tokens.append(self.tokens[1])

            assertToken(self.tokens[2], TokenType.IDENTIFIER)
            tokens.append(self.tokens[2])

            assertToken(self.tokens[3], TokenType.SYMBOL, "(")
            tokens.append(self.tokens[3])

            self.tokens = self.tokens[4:]
            tokens.append(self.parseExpressionList())

            assertToken(self.tokens[0], TokenType.SYMBOL, ")")
            tokens.append(self.tokens[0])
            self.tokens = self.tokens[1:]
        else:
            raise ParserException(
                f"Expected valid subroutine identifier, not '{self.tokens[0].getValue()}' @ {self.tokens[0].line}.{self.tokens[0].lineIndex}"
            )

        return tuple(tokens)

    def parseReturnStatement(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.RETURN_STATEMENT)
        node.addChild(self.tokens[0])

        self.tokens = self.tokens[1:]
        if self.isTerm():
            node.addChild(self.parseExpression())

        assertToken(self.tokens[0], TokenType.SYMBOL, ";")
        node.addChild(self.tokens[0])
        self.tokens = self.tokens[1:]

        return node

    def parseExpression(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.EXPRESSION)

        node.addChild(self.parseTerm())

        while True:
            if self.tokens[0].getValue() in OPS:
                node.addChild(self.tokens[0])
                self.tokens = self.tokens[1:]
                node.addChild(self.parseTerm())
            else:
                break

        return node

    def parseExpressionList(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.EXPRESSION_LIST)

        if self.isExpression():
            node.addChild(self.parseExpression())
            while True:
                if self.tokens[0].getValue() == ",":
                    node.addChild(self.tokens[0])
                    self.tokens = self.tokens[1:]
                    node.addChild(self.parseExpression())
                else:
                    break

        return node

    def parseTerm(self) -> NonTerminalNode:
        node = NonTerminalNode(NonTerminalType.TERM)

        if self.tokens[0].getType() == TokenType.INTEGER_CONST:
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]
        elif self.tokens[0].getType() == TokenType.STRING_CONST:
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]
        elif (
            self.tokens[0].getType() == TokenType.KEYWORD
            and self.tokens[0].getValue() in KEYWORD_CONSTANTS
        ):
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]
        elif self.tokens[0].getValue() == "(":
            node.addChild(self.tokens[0])

            self.tokens = self.tokens[1:]
            node.addChild(self.parseExpression())

            assertToken(self.tokens[0], TokenType.SYMBOL, ")")
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]
        elif self.tokens[0].getValue() in UNARY_OPS:
            node.addChild(self.tokens[0])

            self.tokens = self.tokens[1:]
            node.addChild(self.parseTerm())
        elif self.isSubroutineCall():
            node.addChildren(self.parseSubroutineCall())
        elif self.isVarName():
            node.addChild(self.tokens[0])
            self.tokens = self.tokens[1:]

            if self.tokens[0].getValue() == "[":
                node.addChild(self.tokens[0])

                self.tokens = self.tokens[1:]
                node.addChild(self.parseExpression())

                assertToken(self.tokens[0], TokenType.SYMBOL, "]")
                node.addChild(self.tokens[0])
                self.tokens = self.tokens[1:]

        return node

    def parseGenericVarDec(self) -> tuple[Token]:
        tokens: list[Token] = []

        self.checkType(self.tokens[0])
        tokens.append(self.tokens[0])

        assertToken(self.tokens[1], TokenType.IDENTIFIER)
        tokens.append(self.tokens[1])

        self.tokens = self.tokens[2:]
        while True:
            if self.tokens[0].getValue() == ",":
                tokens.append(self.tokens[0])
                assertToken(self.tokens[1], TokenType.IDENTIFIER)
                tokens.append(self.tokens[1])
                self.tokens = self.tokens[2:]
            else:
                break

        return tuple(tokens)

    def isExpression(self) -> bool:
        return self.isTerm()

    def isTerm(self) -> bool:
        if self.tokens[0].getType() == TokenType.INTEGER_CONST:
            return True
        elif self.tokens[0].getType() == TokenType.STRING_CONST:
            return True
        elif (
            self.tokens[0].getType() == TokenType.KEYWORD
            and self.tokens[0].getValue() in KEYWORD_CONSTANTS
        ):
            return True
        elif self.tokens[0].getValue() == "(":
            return True
        elif self.tokens[0].getValue() in UNARY_OPS:
            return True
        elif self.isSubroutineCall():
            return True
        elif self.isVarName():
            return True
        return False

    def isClassName(self) -> bool:
        return self.tokens[0].getType() == TokenType.IDENTIFIER

    def isVarName(self) -> bool:
        return self.tokens[0].getType() == TokenType.IDENTIFIER

    def isSubroutineName(self) -> bool:
        return self.tokens[0].getType() == TokenType.IDENTIFIER

    def isSubroutineCall(self) -> bool:
        return (self.isSubroutineName() and self.tokens[1].getValue() == "(") or (
            (self.isClassName() or self.isVarName())
            and self.tokens[1].getValue() == "."
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
            xml += (
                f"{spacing}{' '*TAB_SIZE}{tokenXml}"
            )
        elif isinstance(child, NonTerminalNode):
            xml += treeToXML(child, depth + 1)
        else:
            raise ParserException(f"Invalid child type '{type(child)}'")

    xml += f"{spacing}</{node.type.value}>\n"
    return xml
