#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>

class SymbolTable {
 private:
  std::unordered_map<std::string, int> map;

 public:
  SymbolTable();
  bool contains(const std::string& symbol) const;
  void addSymbol(const std::string& symbol, int value);
  int getSymbolValue(const std::string& symbol) const;
};

#endif