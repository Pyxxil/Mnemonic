#ifndef TOKEN_BEGIN_HPP
#define TOKEN_BEGIN_HPP

#include "Token.hpp"

namespace Lexer {
namespace Token {
class Begin : public Token {
public:
  explicit Begin(std::string t, size_t tLine, size_t tColumn,
                 const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile, Requirements(0)) {}

  Begin(const Begin &) = default;
  Begin(Begin &&) = default;

  Begin &operator=(const Begin &) = default;
  Begin &operator=(Begin &&) = default;

  TokenType tokenType() const final { return BEGIN; }

  void assemble(int16_t &programCounter, size_t width,
                const std::map<std::string, Symbol> &symbols) override {
    setAssembled(AssembledToken(
        programCounter,
        fmt::format(
            "(0000) {0:0>4X} {0:0>16b} ({1: >4d}) {2: <{3}s} .BEGIN 0x{0:0>4X}",
            programCounter, line(), std::string{}, width)));
  }

  word memoryRequired() const override { return 1_word; }
};
} // namespace Token
} // namespace Lexer

#endif