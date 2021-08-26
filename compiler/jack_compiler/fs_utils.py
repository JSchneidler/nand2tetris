from pathlib import Path


def validateInputPath(inputPath: Path):
    if not inputPath.exists():
        raise Exception('Invalid input path: ' + str(inputPath))


def generateOutputPath(inputPath: Path):
    return inputPath.stem + '.xml'


def isJackFile(path: Path):
    return path.suffix == '.jack'


def getJackFilesInDirectory(directory: Path):
    jackFiles = []
    for file in directory.iterdir():
        if isJackFile(file):
            jackFiles.append(file)

    return jackFiles
