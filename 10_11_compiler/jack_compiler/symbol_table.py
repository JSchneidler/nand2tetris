from enum import Enum
from dataclasses import dataclass


class SymbolKind(Enum):
    STATIC = "static"
    FIELD = "field"
    ARGUMENT = "argument"
    LOCAL = "local"


@dataclass
class SymbolInfo:
    type: str
    kind: SymbolKind
    index: int


Symbols = dict[str, SymbolInfo]


class SymbolAlreadyExists(Exception):
    pass


class SymbolNotFound(Exception):
    pass


class SymbolTable:
    def __init__(self):
        self.symbols: Symbols = {}
        self.kindCount: dict[SymbolKind, int] = {}

    def getIndex(self, kind: SymbolKind) -> int:
        if not kind in self.kindCount:
            return 0
        return self.kindCount[kind]

    def incrementKind(self, kind: SymbolKind):
        if not kind in self.kindCount:
            self.kindCount[kind] = 1
        else:
            self.kindCount[kind] += 1

    def add(self, name: str, type: str, kind: SymbolKind):
        if name in self.symbols:
            raise SymbolAlreadyExists(f"Symbol '{name}' already exists in table")
        
        self.symbols[name] = SymbolInfo(type, kind, self.getIndex(kind))
        self.incrementKind(kind)

    def get(self, name: str) -> SymbolInfo:
        if name not in self.symbols:
            raise SymbolNotFound(f"Symbol '{name}' doesn't exist in table")
        return self.symbols[name]

    def empty(self):
        self.symbols = {}