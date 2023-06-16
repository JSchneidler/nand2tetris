from pathlib import Path

from jack_compiler.tokenizer import tokenizeFile
from jack_compiler.jack_parser import parseTokens, treeToXML

DIRECTORIES = ["ArrayTest", "ExpressionLessSquare", "Square"]

def compareXML(xml: str, expectedXMLPath: Path):
    with open(expectedXMLPath) as f:
        assert xml == f.read()

def test_parser_ArrayTest():
    tokens = tokenizeFile(Path("tests/jackFiles/ArrayTest/Main.jack"))
    expectedXML = Path("tests/jackFiles/ArrayTest/Main.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)

    compareXML(xml, expectedXML)

def test_parser_ExpressionLessSquare():
    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/Main.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/Main.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/Square.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/Square.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/ExpressionLessSquare/SquareGame.jack"))
    expectedXML = Path("tests/jackFiles/ExpressionLessSquare/SquareGame.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)
    compareXML(xml, expectedXML)

def test_parser_Square():
    tokens = tokenizeFile(Path("tests/jackFiles/Square/Main.jack"))
    expectedXML = Path("tests/jackFiles/Square/Main.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/Square/Square.jack"))
    expectedXML = Path("tests/jackFiles/Square/Square.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)
    compareXML(xml, expectedXML)

    tokens = tokenizeFile(Path("tests/jackFiles/Square/SquareGame.jack"))
    expectedXML = Path("tests/jackFiles/Square/SquareGame.xml")
    tree = parseTokens(tokens)
    xml = treeToXML(tree)
    compareXML(xml, expectedXML)