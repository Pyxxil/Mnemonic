#ifndef TOKEN_JUMPEQ_HPP
#define TOKEN_JUMPEQ_HPP

#include "Token.hpp"

namespace Lexer {
namespace Token {
class Jumpeq : public Token {
public:
  explicit Jumpeq(std::string t, size_t tLine, size_t tColumn,
                  const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile,
              Requirements(1, {Match(TokenType::LABEL)})) {}

  Jumpeq(const Jumpeq &) = default;
  Jumpeq(Jumpeq &&) = default;

  Jumpeq &operator=(const Jumpeq &) = default;
  Jumpeq &operator=(Jumpeq &&) = default;

  TokenType tokenType() const final { return JUMPEQ; }

  void assemble(int16_t &programCounter, size_t width,
                const std::map<std::string, Symbol> &symbols) override {
    auto sym = std::find_if(symbols.begin(), symbols.end(),
                            [programCounter](const auto &sym) {
                              return sym.second.address() == programCounter;
                            });

    const auto &symbol = symbols.find(operands().front()->getToken());
    const uint16_t bin = 0xA000 | (symbol->second.address() & 0xFFF);

    setAssembled(AssembledToken(
        bin, fmt::format("({0:0>4X}) {1:0>4X} {1:0>16b} ({2: >4d}) {3: <{4}s} "
                         "JUMPEQ {5:s}",
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