from enum import Enum
from pathlib import Path
from html import escape

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
    "\"",
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
    INTEGER_CONST = "integer"
    STRING_CONST = "string"


class Token:
    def __init__(self, type: TokenType, value: str):
        self.type = type
        self.value = value

    def getType(self):
        return self.type

    def getValue(self):
        return self.value


Tokens = list[Token]


class TokenizerException(Exception):
    pass


def tokenizeFile(path: Path) -> Tokens:
    """Returns a list of Jack tokens given a file path."""
    tokens: Tokens = []
    commentActive = False

    with path.open() as file:
        lineNumber = 0
        for line in file:
            lineNumber += 1
            line = line.rstrip()
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
                    value = line[0]
                    if value in ENCODED_SYMBOLS:
                        value = escape(value)
                    tokens.append(Token(TokenType.SYMBOL, value))
                    line = line[1:]
                    continue

                # Check for integer constant
                if line[0].isdigit():
                    i = 0
                    for i in range(0, len(line)):
                        if line[i].isalpha():
                            raise TokenizerException(f"Invalid character found in integer constant on line {lineNumber}")
                        elif not line[i].isdigit():
                            break

                    tokens.append(Token(TokenType.INTEGER_CONST, line[:i]))
                    line = line[i:]
                    continue

                # Check for string constant
                if line[0] == '"':
                    stringEnd = line.find('"', 1)
                    if stringEnd == -1:
                        raise TokenizerException(f"Cannot find closing symbol for string constant on line {lineNumber}")
                    tokens.append(Token(TokenType.STRING_CONST, line[1:stringEnd]))
                    line = line[stringEnd + 1:]
                    continue

                # Check for keyword
                found = False
                for keyword in KEYWORDS:
                    if line.startswith(keyword):
                        found = True
                        tokens.append(Token(TokenType.KEYWORD, keyword))
                        line = line[len(keyword):]
                        break
                if found:
                    continue

                # Check for identifier
                if line[0].isalpha() or line[0] == "_":
                    i = 0
                    for i in range(0, len(line)):
                        if not line[i].isalnum() and not line[i] == "_":
                            break

                    tokens.append(Token(TokenType.IDENTIFIER, line[:i]))
                    line = line[i:]
                    continue

                raise TokenizerException("Invalid character", line[i], line)

    return tokens


def tokensToXML(tokens: Tokens) -> str:
    """Converts a list of Jack tokens into XML"""
    xml = "<tokens>\n"

    for token in tokens:
        type = token.getType()
        if type == TokenType.STRING_CONST:
            type = "stringConstant"
        elif type == TokenType.INTEGER_CONST:
            type = "integerConstant"
        else:
            type = type.value

        xml += f"<{type}> {token.getValue()} </{type}>\n"

    xml += "</tokens>\n"

    return xml