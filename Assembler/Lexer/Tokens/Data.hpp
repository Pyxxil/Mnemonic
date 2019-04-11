#ifndef TOKEN_DATA_HPP
#define TOKEN_DATA_HPP

#include "Decimal.hpp"

namespace Lexer {
namespace Token {
class Data : public Token {
public:
  explicit Data(std::string t, size_t tLine, size_t tColumn,
                const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile,
              Requirements(1, {Match(TokenType::DECIMAL)})) {}

  Data(const Data &) = default;
  Data(Data &&) = default;

  Data &operator=(const Data &) = default;
  Data &operator=(Data &&) = default;

  TokenType tokenType() const final { return DATA; }

  void assemble(int16_t &programCounter, size_t width,
                const std::map<std::string, Symbol> &symbols) override {
    const uint16_t bin =
        static_cast<Decimal *>(operands().front().get())->value();

    auto sym = std::find_if(symbols.begin(), symbols.end(),
                            [programCounter](const auto &sym) {
                              return sym.second.address() == programCounter;
                            });

    setAssembled(AssembledToken(
        bin, fmt::format("({0:0>4X}) {1:0>4X} {1:0>16b} ({2: >4d}) {3: <{4}s} "
                         ".DATA {5:d}",
                         programCounter++, bin, line(),
                         sym == symbols.end() ? "" : sym->second.name(), width,
                         bin)));
  }

  word memoryRequired() const override { return 1_word; }

private:
}; // namespace Token
} // namespace Token
} // namespace Lexer

#endif