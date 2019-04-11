#ifndef TOKEN_STORE_HPP
#define TOKEN_STORE_HPP

#include <memory>

#include "../Token.hpp"

#include "fmt/ostream.h"

namespace Lexer {
namespace Token {
class Store : public Token {
public:
  explicit Store(std::string t, size_t tLine, size_t tColumn,
                 const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile,
              Requirements(1, {Match(TokenType::LABEL)})) {}

  Store(const Store &) = default;
  Store(Store &&) = default;

  Store &operator=(const Store &) = default;
  Store &operator=(Store &&) = default;

  TokenType tokenType() const final { return STORE; }

  void assemble(int16_t &programCounter, size_t width,
                const std::map<std::string, Symbol> &symbols) override {
    auto sym = std::find_if(symbols.begin(), symbols.end(),
                            [programCounter](const auto &sym) {
                              return sym.second.address() == programCounter;
                            });

    const auto &symbol = symbols.find(operands().front()->getToken());
    const uint16_t bin = 0x1000 | (symbol->second.address() & 0xFFF);

    setAssembled(AssembledToken(
        bin, fmt::format("({0:0>4X}) {1:0>4X} {1:0>16b} ({2: >4d}) {3: <{4}s} "
                         "STORE {5:s}",
                         programCounter++, bin, line(),
                         sym == symbols.end() ? "" : sym->second.name(), width,
                         symbol->first)));
  }

  word memoryRequired() const override { return 1_word; }

private:
};
} // namespace Token
} // namespace Lexer

#endif