import pytest
from pathlib import Path
from jack_compiler import fs_utils


def test_validateInputPath():
    # Success scenarios
    fs_utils.validateInputPath(Path("tests/jackFiles/ArrayTest"))
    fs_utils.validateInputPath(Path("tests/jackFiles/ArrayTest/"))
    fs_utils.validateInputPath(Path("tests/jackFiles/ArrayTest/Main.jack"))
    fs_utils.validateInputPath(Path("./tests/jackFiles/ArrayTest"))
    fs_utils.validateInputPath(Path("./tests/jackFiles/ArrayTest/"))
    fs_utils.validateInputPath(Path("./tests/jackFiles/ArrayTest/Main.jack"))

    # Failure scenarios
    with pytest.raises(Exception):
        fs_utils.validateInputPath("invalid")
        fs_utils.validateInputPath("./invalid")
        fs_utils.validateInputPath("tests/invalid")
        fs_utils.validateInputPath("./tests/invalid")


def test_generateOutputPath():
    path = Path("Main.jack")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "Main.xml"

    path = Path("./Main.jack")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "Main.xml"

    path = Path("/Main.jack")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "Main.xml"

    path = Path("tests/Main.jack")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "Main.xml"

    path = Path("tests/Main.jack")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "Main.xml"

    path = Path("tests/jackFiles")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "jackFiles.xml"

    path = Path("tests/jackFiles/")
    outputPath = fs_utils.generateOutputPath(path)
    assert outputPath.name == "jackFiles.xml"


def test_isJackFile():
    assert fs_utils.isJackFile(Path("Main.jack")) is True
    assert fs_utils.isJackFile(Path("./Main.jack")) is True
    assert fs_utils.isJackFile(Path("/Main.jack")) is True
    assert fs_utils.isJackFile(Path("tests")) is False
    assert fs_utils.isJackFile(Path("./tests")) is False
    assert fs_utils.isJackFile(Path("/tests")) is False
    assert fs_utils.isJackFile(Path("tests/Main.jack")) is True
    assert fs_utils.isJackFile(Path("./tests/Main.jack")) is True
    assert fs_utils.isJackFile(Path("/tests/Main.jack")) is True
    assert fs_utils.isJackFile(Path("tests/Main.abc")) is False
    assert fs_utils.isJackFile(Path("./tests/Main.abc")) is False
    assert fs_utils.isJackFile(Path("/tests/Main.abc")) is False


def test_getJackFilesInDirectory():
    files = fs_utils.getJackFilesInDirectory(Path("tests/jackFiles/ArrayTest"))
    assert files == [Path("tests/jackFiles/ArrayTest/Main.jack")]

    files = fs_utils.getJackFilesInDirectory(Path("tests"))
    assert files == []
