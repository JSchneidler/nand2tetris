from pathlib import Path
from jack_compiler.tokenizer import tokenizeFile, tokensToXML, Tokens, TokenType


def countTokens(tokens: Tokens, tokenType: TokenType, count: int):
    sum = 0
    for token in tokens:
        if token.getType() == tokenType:
            sum += 1

    assert count == sum


def compareXML(xml: str, expectedXMLPath: Path):
    with open(expectedXMLPath) as f:
        assert xml == f.read()

def test_tokenizer_tokenizeFile_comments():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/comments.jack"))
    assert len(tokens) == 0


def test_tokenizer_tokenizeFile_keywords():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/keywords.jack"))
    countTokens(tokens, TokenType.KEYWORD, 21)


def test_tokenizer_tokenizeFile_symbols():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/symbols.jack"))
    countTokens(tokens, TokenType.SYMBOL, 19)


def test_tokenizer_tokenizeFile_strings():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/strings.jack"))
    countTokens(tokens, TokenType.STRING_CONST, 5)


def test_tokenizer_tokenizeFile_integers():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/integers.jack"))
    countTokens(tokens, TokenType.INTEGER_CONST, 6)


def test_tokenizer_tokenizeFile_identifiers():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/identifiers.jack"))
    countTokens(tokens, TokenType.IDENTIFIER, 9)

def test_tokenizer_tokenizeFile_ArrayTest():
    tokens = tokenizeFile(Path("tests/jackFiles/ArrayTest/Main.jack"))
    expectedXML = Path("tests/jackFiles/ArrayTest/MainT.xml")
    xml = tokensToXML(tokens)

    compareXML(xml, expectedXML)

def test_tokenizer_tokenizeFile_ExpressionLessSquare():
    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/Main.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/MainT.xml")
    xml = tokensToXML(tokens)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/Square.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/SquareT.xml")
    xml = tokensToXML(tokens)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/SquareGame.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/SquareGameT.xml")
    xml = tokensToXML(tokens)
    compareXML(xml, expectedXML)

def test_tokenizer_tokenizeFile_Square():
    tokens = tokenizeFile(Path("tests/jackFiles/Square/Main.jack"))
    expectedXML = Path("tests/jackFiles/Square/MainT.xml")
    xml = tokensToXML(tokens)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/Square/Square.jack"))
    expectedXML = Path("tests/jackFiles/Square/SquareT.xml")
    xml = tokensToXML(tokens)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/Square/SquareGame.jack"))
    expectedXML = Path("tests/jackFiles/Square/SquareGameT.xml")
    xml = tokensToXML(tokens)
    compareXML(xml, expectedXML)
