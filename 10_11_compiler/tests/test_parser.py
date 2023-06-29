from pathlib import Path

from jack_compiler.tokenizer import tokenizeFile
from jack_compiler.jack_parser import parseTokens

DIRECTORIES = ["ArrayTest", "ExpressionLessSquare", "Square"]

def compareXML(xml: str, expectedXMLPath: Path):
    with open(expectedXMLPath) as f:
        assert xml == f.read()

def test_parser_ArrayTest():
    tokens = tokenizeFile(Path("tests/jackFiles/ArrayTest/Main.jack"))
    expectedXML = Path("tests/jackFiles/ArrayTest/Main.xml")
    xml = parseTokens(tokens).toXML()

    compareXML(xml, expectedXML)

def test_parser_ExpressionLessSquare_Main():
    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/Main.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/Main.xml")
    xml = parseTokens(tokens).toXML()
    compareXML(xml, expectedXML)

def test_parser_ExpressionLessSquare_Square():
    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/Square.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/Square.xml")
    xml = parseTokens(tokens).toXML()
    compareXML(xml, expectedXML)

def test_parser_ExpressionLessSquare_SquareGame():
    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/SquareGame.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/SquareGame.xml")
    xml = parseTokens(tokens).toXML()
    compareXML(xml, expectedXML)

def test_parser_Square_Main():
    tokens = tokenizeFile(Path("tests/jackFiles/Square/Main.jack"))
    expectedXML = Path("tests/jackFiles/Square/Main.xml")
    xml = parseTokens(tokens).toXML()
    compareXML(xml, expectedXML)

def test_parser_Square_Square():
    tokens = tokenizeFile(Path("tests/jackFiles/Square/Square.jack"))
    expectedXML = Path("tests/jackFiles/Square/Square.xml")
    xml = parseTokens(tokens).toXML()
    compareXML(xml, expectedXML)

def test_parser_Square_SquareGame():
    tokens = tokenizeFile(Path("tests/jackFiles/Square/SquareGame.jack"))
    expectedXML = Path("tests/jackFiles/Square/SquareGame.xml")
    xml = parseTokens(tokens).toXML()
    compareXML(xml, expectedXML)