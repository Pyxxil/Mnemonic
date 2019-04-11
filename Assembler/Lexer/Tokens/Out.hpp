#ifndef TOKEN_OUT_HPP
#define TOKEN_OUT_HPP

#include "Token.hpp"

namespace Lexer {
namespace Token {
class Out : public Token {
public:
  Out(std::string t, size_t tLine, size_t tColumn, const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile,
              Requirements(1, {Match(TokenType::LABEL)})) {}

  Out(const Out &) = default;
  Out(Out &&) = default;

  Out &operator=(const Out &) = default;
  Out &operator=(Out &&) = default;

  TokenType tokenType() const final { return OUT; }

  void assemble(int16_t &programCounter, size_t width,
                const std::map<std::string, Symbol> &symbols) override {
    auto sym = std::find_if(symbols.begin(), symbols.end(),
                            [programCounter](const auto &sym) {
                              return sym.second.address() == programCounter;
                            });

    const auto &symbol = symbols.find(operands().front()->getToken());
    const uint16_t bin = 0xE000 | (symbol->second.address() & 0xFFF);

    setAssembled(AssembledToken(
        bin,
        fmt::format(
            "({0:0>4X}) {1:0>4X} {1:0>16b} ({2: >4d}) {3: <{4}s} OUT {5:s}",
            programCounter++, bin, line(),
            sym == symbols.end() ? "" : sym->second.name(), width,
            symbol->first)));
  }

  word memoryRequired() const override { return 1_word; }
};
} // namespace Token
} // namespace Lexer

#endif