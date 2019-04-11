#ifndef TOKEN_JUMPLT_HPP
#define TOKEN_JUMPLT_HPP

#include "Token.hpp"

namespace Lexer {
namespace Token {
class Jumplt : public Token {
public:
  explicit Jumplt(std::string t, size_t tLine, size_t tColumn,
                  const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile,
              Requirements(1, {Match(TokenType::LABEL)})) {}

  Jumplt(const Jumplt &) = default;
  Jumplt(Jumplt &&) = default;

  Jumplt &operator=(const Jumplt &) = default;
  Jumplt &operator=(Jumplt &&) = default;

  TokenType tokenType() const final { return JUMPLT; }

  void assemble(int16_t &programCounter, size_t width,
                const std::map<std::string, Symbol> &symbols) override {
    auto sym = std::find_if(symbols.begin(), symbols.end(),
                            [programCounter](const auto &sym) {
                              return sym.second.address() == programCounter;
                            });

    const auto &symbol = symbols.find(operands().front()->getToken());
    const uint16_t bin = 0xB000 | (symbol->second.address() & 0xFFF);

    setAssembled(AssembledToken(
        bin, fmt::format("({0:0>4X}) {1:0>4X} {1:0>16b} ({2: >4d}) {3: <{4}s} "
                         "JUMPLT {5:s}",
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