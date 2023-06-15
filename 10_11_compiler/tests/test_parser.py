from pathlib import Path

from jack_compiler.fs_utils import getJackFilesInDirectory
from jack_compiler.tokenizer import tokenizeFile
from jack_compiler.jack_parser import parseTokens

DIRECTORIES = ["ArrayTest", "ExpressionLessSquare", "Square"]

def test_parser():
    for dir in DIRECTORIES:
        files = getJackFilesInDirectory(Path(f"tests/jackFiles/{dir}"))
        for file in files:
            tokens = tokenizeFile(file)
            parseTokens(tokens)