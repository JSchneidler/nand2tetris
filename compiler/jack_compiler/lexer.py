from enum import Enum
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
    KEYWORD = 'keyword'
    SYMBOL = 'symbol'
    IDENTIFIER = 'identifier'
    INTEGER_CONST = 'integer'
    STRING_CONST = 'string'


class Token:
    def __init__(self, type: TokenType, value: str):
        self.type = type
        self.value = value


class LexerException(Exception):
    pass


def tokenizeFile(path: Path):
    tokens = []
    commentActive = False
    lineNumber = 0

    with path.open() as file:
        while True:
            lineNumber += 1
            line = file.readline()
            # Empty line means EoF
            if len(line) == 0:
                break

            # Is a multi-line comment active?
            if commentActive:
                # Check if a comment end exists on the line
                commentEnd = line.find("*/")

                # If no comment end, go to next line
                if commentEnd == -1:
                    continue

                # Otherwise split the remainder of the comment from the line
                line = line[commentEnd + 2:]
                commentActive = False

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
                    continue

                # Otherwise split the remainder of the comment from the line
                line = line[commentEnd + 2:]
                commentActive = False

            # Check if symbol
            if line[0] in SYMBOLS:
                # Handle string constant
                if line[0] == "\"":
                    stringEnd = line.find("\"", 1)
                    if stringEnd == -1:
                        raise LexerException("Cannot find closing symbol for string constant on line " + lineNumber)
                    tokens.append(Token(TokenType.STRING_CONST, line[:stringEnd]))
                    line = line[stringEnd:]
                # Handle other symbols
                else:
                    tokens.append(Token(TokenType.SYMBOL, line[0]))
                    line = line[1:]

            # Check for keyword
            for keyword in KEYWORDS:
                if line.startswith(keyword):
                    tokens.append(Token(TokenType.KEYWORD, line[0:len(keyword)]))
                    line = line[len(keyword):]
                    break

            # Check if integer constant
            if line[0] == '-' or line[0].isdigit():
                # TODO
                pass

            # Check if identifier
            if line[0].isalpha():
                # TODO
                pass

    return tokens
