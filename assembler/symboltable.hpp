#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>

class SymbolTable {
 private:
  std::unordered_map<std::string, short> map;

 public:
  SymbolTable();
  bool contains(const std::string& symbol) const;
  void addSymbol(const std::string& symbol, short value);
  short getSymbolValue(const std::string& symbol) const;
};

#endif