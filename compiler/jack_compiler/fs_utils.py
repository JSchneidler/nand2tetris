from pathlib import Path


def validateInputPath(inputPath: Path) -> None:
    """Validates the input path, currently just checks the path exists."""
    if not inputPath.exists():
        raise Exception("Invalid input path: " + str(inputPath))


def generateOutputPath(inputPath: Path) -> Path:
    """Swap the extension of the file/folder name with .xml"""
    return Path(inputPath.stem + ".xml")


def isJackFile(path: Path) -> bool:
    """Does the file end in .jack?"""
    return path.suffix == ".jack"


def getJackFilesInDirectory(directory: Path) -> list[Path]:
    """Returns a list of .jack files in directory"""
    jackFiles: list[Path] = []
    for file in directory.iterdir():
        if isJackFile(file):
            jackFiles.append(file)

    return jackFiles
