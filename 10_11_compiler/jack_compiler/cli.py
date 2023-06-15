import argparse
from pathlib import Path

from . import fs_utils, tokenizer, jack_parser

parser = argparse.ArgumentParser(description="CLI interface for Jack compiler")
parser.add_argument(
    "inputPath",
    metavar="I",
    help="A path to a .jack file or directory containing one or more .jack files",
)

args = parser.parse_args()

inputPath = Path(args.inputPath)

fs_utils.validateInputPath(inputPath)
outputPath = fs_utils.generateOutputPath(inputPath)

jackFiles = []
if inputPath.is_dir():
    jackFiles = fs_utils.getJackFilesInDirectory(inputPath)
    if len(jackFiles) == 0:
        raise Exception("No .jack files found in directory " + inputPath.name)
elif fs_utils.isJackFile(inputPath):
    jackFiles = [inputPath]
else:
    raise Exception(inputPath.name + " is not a .jack file")

print(jackFiles)
print(outputPath)

for file in jackFiles:
    tokens = tokenizer.tokenizeFile(file)
    parseTree = jack_parser.parseTokens(tokens)
    print(parseTree)
