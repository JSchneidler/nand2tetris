#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <unordered_map>

class SymbolTable {
 public:
  SymbolTable();
  bool contains(const std::string &symbolName) const;
  void addSymbol(const std::string &symbolName, const short symbolValue);
  short getSymbol(const std::string &symbolName) const;

  friend std::ostream& operator<<(std::ostream &os,
                                  const SymbolTable &symbolTable);

 private:
  std::unordered_map<std::string, short> map;
};

#endif