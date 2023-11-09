from enum import Enum


class Segment(Enum):
    CONST = "constant"
    ARG = "argument"
    LOCAL = "local"
    STATIC = "static"
    THIS = "this"
    THAT = "that"
    POINTER = "pointer"
    TEMP = "temp"


class Command(Enum):
    ADD = "add"
    SUB = "sub"
    NEG = "neg"
    EQ = "EQ"
    GT = "GT"
    LT = "LT"
    AND = "AND"
    OR = "OR"
    NOT = "NOT"


def writePush(segment: Segment, index: int) -> str:
    return f"push {segment.value} {index}"


def writePop(segment: Segment, index: int) -> str:
    return f"pop {segment.value} {index}"


def writeOp(command: Command) -> str:
    return command.value


def writeLabel(label: str) -> str:
    return "label " + label


def writeGoto(label: str) -> str:
    return "goto " + label


def writeIf(label: str) -> str:
    return "If-goto " + label


def writeCall(fn: str, nArgs: int) -> str:
    return f"call {fn} {nArgs}"


def writeFunction(name: str, nLocals: int) -> str:
    return f"function {name} {nLocals}"


def writeReturn() -> str:
    return "return"
