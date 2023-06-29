import logging
from enum import Enum
from html import escape
from pathlib import Path

SYMBOLS = [
    "{",
    "}",
    "(",
    ")",
    "[",
    "]",
    ".",
    ",",
    ";",
    "+",
    "-",
    "*",
    "/",
    "&",
    "|",
    "<",
    ">",
    "=",
    "~",
]

ENCODED_SYMBOLS = [
    "<",
    ">",
    "&",
    '"',
]

KEYWORDS = [
    "class",
    "constructor",
    "function",
    "method",
    "field",
    "static",
    "var",
    "int",
    "char",
    "boolean",
    "void",
    "true",
    "false",
    "null",
    "this",
    "let",
    "do",
    "if",
    "else",
    "while",
    "return",
]


class TokenType(Enum):
    KEYWORD = "keyword"
    SYMBOL = "symbol"
    IDENTIFIER = "identifier"
    INTEGER_CONST = "integerConstant"
    STRING_CONST = "stringConstant"


class Token:
    def __init__(self, type: TokenType, value: str, line: int, lineIndex: int):
        self.type = type
        self.value = value
        self.line = line
        self.lineIndex = lineIndex

    def getType(self):
        return self.type

    def getValue(self):
        return self.value

    def formatValue(self, value: str) -> str:
        if value in ENCODED_SYMBOLS:
            return escape(value)
        return value

    def toXML(self) -> str:
        type = self.getType().value
        return f"<{type}> {self.formatValue(self.getValue())} </{type}>"



Tokens = list[Token]


class TokenizerException(Exception):
    pass


def tokenizeFile(path: Path) -> Tokens:
    """Returns a list of Jack tokens given a file path."""
    tokens: Tokens = []
    commentActive = False

    with path.open() as file:
        logging.debug(f"Tokenizing file '{file.name}'")

        lineNumber = 0
        for line in file:
            line = line.rstrip()
            lineNumber += 1
            lineIndex = 0
            while len(line) > 0:
                # Is a multi-line comment active?
                if commentActive:
                    # Check if a comment end exists on the line
                    commentEnd = line.find("*/")

                    # If no comment end, go to next line
                    if commentEnd == -1:
                        break

                    # Otherwise split the remainder of the comment from the line
                    line = line[commentEnd + 2 :]
                    commentActive = False
                    continue

                # If no characters left, go to next line
                line = line.lstrip()
                if len(line) == 0:
                    break

                # Skip line if line comment
                if line.startswith("//"):
                    break

                # Check for block comment start
                if line.startswith("/*") or line.startswith("/**"):
                    # Check if a comment end exists on same line
                    commentEnd = line.find("*/")

                    # If no comment end, go to next line
                    if commentEnd == -1:
                        commentActive = True
                        break

                    # Otherwise split the remainder of the comment from the line
                    line = line[commentEnd + 2 :]
                    commentActive = False
                    continue

                # Check for symbol
                if line[0] in SYMBOLS:
                    tokens.append(
                        Token(TokenType.SYMBOL, line[0], lineNumber, lineIndex)
                    )
                    line = line[1:]
                    lineIndex += 1
                    continue

                # Check for integer constant
                if line[0].isdigit():
                    i = 0
                    for i in range(0, len(line)):
                        if line[i].isalpha():
                            raise TokenizerException(
                                f"Invalid character found in integer constant on line {lineNumber}"
                            )
                        elif not line[i].isdigit():
                            break

                    tokens.append(
                        Token(TokenType.INTEGER_CONST, line[:i], lineNumber, lineIndex)
                    )
                    line = line[i:]
                    lineIndex += 1
                    continue

                # Check for string constant
                if line[0] == '"':
                    stringEnd = line.find('"', 1)
                    if stringEnd == -1:
                        raise TokenizerException(
                            f"Cannot find closing symbol for string constant on line {lineNumber}"
                        )
                    tokens.append(
                        Token(
                            TokenType.STRING_CONST,
                            line[1:stringEnd],
                            lineNumber,
                            lineIndex,
                        )
                    )
                    line = line[stringEnd + 1 :]
                    lineIndex += 1
                    continue

                # Check for keyword
                found = False
                for keyword in KEYWORDS:
                    if line.startswith(keyword):
                        found = True
                        tokens.append(
                            Token(TokenType.KEYWORD, keyword, lineNumber, lineIndex)
                        )
                        line = line[len(keyword) :]
                        lineIndex += 1
                        break
                if found:
                    continue

                # Check for identifier
                if line[0].isalpha() or line[0] == "_":
                    i = 0
                    for i in range(0, len(line)):
                        if not line[i].isalnum() and not line[i] == "_":
                            break

                    tokens.append(
                        Token(TokenType.IDENTIFIER, line[:i], lineNumber, lineIndex)
                    )
                    line = line[i:]
                    lineIndex += 1
                    continue

                raise TokenizerException("Invalid character", line[0], line)

    return tokens


def tokensToXML(tokens: Tokens) -> str:
    """Converts a list of Jack tokens into XML"""
    xml = "<tokens>\n"

    for token in tokens:
        xml += token.toXML() + "\n"

    xml += "</tokens>\n"

    return xml
