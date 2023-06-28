from enum import Enum
from typing import TypedDict


class SymbolKind(Enum):
    STATIC = "STATIC"
    FIELD = "FIELD"
    ARGUMENT = "ARGUMENT"
    LOCAL = "LOCAL"


class SymbolInfo(TypedDict):
    type: str
    kind: SymbolKind
    i: int


Symbols = dict[str, SymbolInfo]


class SymbolTable:
    def __init__(self):
        self.symbols: Symbols = {}

    def add(self, name: str, type: str, kind: SymbolKind):
        if name in self.symbols:
            raise Exception(f"Symbol '{name}' already exists in table")
        
        self.symbols[name] = {
            "type": type,
            "kind": kind,
            "i": 0
        }

    def get(self, name: str) -> SymbolInfo:
        return self.symbols[name]

    def empty(self):
        self.symbols = {}