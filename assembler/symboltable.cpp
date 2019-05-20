#include "symboltable.hpp"
#include <iostream>

SymbolTable::SymbolTable()
    : map({
          {"SP", 0x0},   {"LCL", 0x1},       {"ARG", 0x2},    {"THIS", 0x3},
          {"THAT", 0x4}, {"SCREEN", 0x4000}, {"KBD", 0x6000}, {"R0", 0x0},
          {"R1", 0x1},   {"R2", 0x2},        {"R3", 0x3},     {"R4", 0x4},
          {"R5", 0x5},   {"R6", 0x6},        {"R7", 0x7},     {"R8", 0x8},
          {"R9", 0x9},   {"R10", 0xA},       {"R11", 0xB},    {"R12", 0xC},
          {"R13", 0xD},  {"R14", 0xE},       {"R15", 0xF},
      }) {}

bool SymbolTable::contains(const std::string& symbol) const {
  if (map.find(symbol) == map.end()) return false;
  return true;
}

void SymbolTable::addSymbol(const std::string& symbol, int value) {
  if (map.find(symbol) != map.end())
    // TODO Throw more useful error
    throw std::invalid_argument("Symbol already exists in table");

  map[symbol] = value;
}

int SymbolTable::getSymbolValue(const std::string& symbol) const {
  return map.at(symbol);
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& symbolTable) {
  for (std::pair<std::string, int> element : symbolTable.map) {
    os << element.first << ": " << element.second << std::endl;
  }
  return os;
}