from pathlib import Path
from jack_compiler.tokenizer import tokenizeFile, Tokens, TokenType


def assertTokensType(tokens: Tokens, tokenType: TokenType):
    for token in tokens:
        assert token.getType() == tokenType


def test_tokenizer_tokenizeFile_comments():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/comments.jack"))
    assert len(tokens) == 0


def test_tokenizer_tokenizeFile_keywords():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/keywords.jack"))
    assert len(tokens) == 21
    assertTokensType(tokens, TokenType.KEYWORD)


def test_tokenizer_tokenizeFile_symbols():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/symbols.jack"))
    assert len(tokens) == 19
    assertTokensType(tokens, TokenType.SYMBOL)


def test_tokenizer_tokenizeFile_strings():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/strings.jack"))
    assert len(tokens) == 5
    assertTokensType(tokens, TokenType.STRING_CONST)


def test_tokenizer_tokenizeFile_integers():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/integers.jack"))
    assert len(tokens) == 6
    assertTokensType(tokens, TokenType.INTEGER_CONST)


def test_tokenizer_tokenizeFile_identifiers():
    # TODO: Add negative test cases
    tokens = tokenizeFile(Path("tests/tokenizer/identifiers.jack"))
    assert len(tokens) == 9
    assertTokensType(tokens, TokenType.IDENTIFIER)
