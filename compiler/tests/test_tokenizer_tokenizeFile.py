from pathlib import Path
from jack_compiler.tokenizer import tokenizeFile


def test_tokenizer_tokenizeFile_comments():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/comments.jack"))
    assert len(tokens) == 0


def test_tokenizer_tokenizeFile_keywords():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/keywords.jack"))
    assert len(tokens) == 21


def test_tokenizer_tokenizeFile_symbols():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/symbols.jack"))
    assert len(tokens) == 19


def test_tokenizer_tokenizeFile_strings():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/strings.jack"))
    assert len(tokens) == 5


def test_tokenizer_tokenizeFile_integers():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/integers.jack"))
    assert len(tokens) == 6


def test_tokenizer_tokenizeFile_identifiers():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/identifiers.jack"))
    assert len(tokens) == 9
