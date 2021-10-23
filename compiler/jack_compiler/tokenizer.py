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
    KEYWORD = "keyword"
    SYMBOL = "symbol"
    IDENTIFIER = "identifier"
    INTEGER_CONST = "integer"
    STRING_CONST = "string"


class Token:
    def __init__(self, type: TokenType, value: str):
        self.type = type
        self.value = value


class LexerException(Exception):
    pass


def tokenizeFile(path: Path) -> list[Token]:
    """Returns a list of Jack tokens given a file path."""
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
                line = line[commentEnd + 2 :]
                commentActive = False

            # If no characters left, go to next line
            line = line.strip()
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
                line = line[commentEnd + 2 :]
                commentActive = False

            # Check for integer constant
            if line[0].isdigit() or line[0] == "-":
                for i in range(len(line)):
                    if line[i].isalpha():
                        raise LexerException(
                            "Invalid character found in integer constant on line "
                            + lineNumber
                        )
                    elif not line[i].isdigit():
                        integerValue = line[:i]
                        line = line[i:]
                        tokens.append(Token(TokenType.INTEGER_CONST, integerValue))
                        break
                continue

            # Check for symbol
            if line[0] in SYMBOLS:
                tokens.append(Token(TokenType.SYMBOL, line[0]))
                # Splice token from line
                line = line[1:]
                continue

            # Check for string constant
            if line[0] == '"':
                stringEnd = line.find('"', 1) + 1
                if stringEnd == -1:
                    raise LexerException(
                        "Cannot find closing symbol for string constant on line "
                        + lineNumber
                    )
                tokens.append(Token(TokenType.STRING_CONST, line[1:stringEnd]))
                # Splice tokens from line
                line = line[stringEnd:]
                continue

            # Check for keyword
            for keyword in KEYWORDS:
                if line.startswith(keyword):
                    tokens.append(Token(TokenType.KEYWORD, line[0 : len(keyword)]))
                    # Splice tokens from line
                    line = line[len(keyword) :]
                    break
            if len(line) == 0:
                continue

            # Check for identifier
            if False:
                # TODO
                continue

            raise LexerException("Invalid character", line[0], line)

    return tokens
