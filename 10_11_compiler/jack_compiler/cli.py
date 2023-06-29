import argparse
from pathlib import Path

from . import tokenizer, jack_parser


def isJackFile(path: Path) -> bool:
    return path.suffix == ".jack"


def compile(file: Path):
    outputPath = Path(file.stem + ".xml")

    tokens = tokenizer.tokenizeFile(file)
    parseTree = jack_parser.parseTokens(tokens)
    xml = parseTree.toXML()

    with open(outputPath, "w") as outputFile:
        outputFile.write(xml)


parser = argparse.ArgumentParser(description="CLI interface for Jack compiler")
parser.add_argument(
    "inputPath",
    metavar="I",
    help="Path to a .jack file or directory containing .jack file(s)",
)

args = parser.parse_args()

inputPath = Path(args.inputPath)

if not inputPath.exists():
    raise Exception("Invalid input path: " + str(inputPath))


jackFiles = []
if inputPath.is_dir():
    for file in inputPath.iterdir():
        if isJackFile(file):
            jackFiles.append(file)
    if len(jackFiles) == 0:
        raise Exception("No .jack files found in directory " + inputPath.name)
    for file in jackFiles:
        compile(file)
elif isJackFile(inputPath):
    compile(inputPath)
else:
    raise Exception(
        f"Invalid input path, must be .jack file or directory containing .jack file(s): {inputPath}"
    )
