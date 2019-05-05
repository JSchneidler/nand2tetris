#include "symboltable.hpp"
#include <iostream>

SymbolTable::SymbolTable() : map{} {}

bool SymbolTable::contains(const std::string &symbolName) const {
  try {
    map.at(symbolName);
    return true;
  } catch (std::out_of_range &e) {
    return false;
  }
}

void SymbolTable::addSymbol(const std::string &symbolName,
                            const short symbolValue) {
  map[symbolName] = symbolValue;
}

short SymbolTable::getSymbol(const std::string &symbolName) const {
  return map.at(symbolName);
}

std::ostream &operator<<(std::ostream &os, const SymbolTable &symbolTable) {
  for (std::pair<std::string, short> element : symbolTable.map) {
    os << element.first << ": " << element.second << std::endl;
  }
  return os;
}