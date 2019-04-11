#ifndef TOKEN_END_HPP
#define TOKEN_END_HPP

#include "Token.hpp"

namespace Lexer {
namespace Token {
class End : public Token {
public:
  explicit End(std::string t, size_t tLine, size_t tColumn,
               const std::string &tFile)
      : Token(std::move(t), tLine, tColumn, tFile) {}

  End(const End &) = default;
  End(End &&) = default;

  End &operator=(const End &) = default;
  End &operator=(End &&) = default;

  TokenType tokenType() const final { return END; }

  word memoryRequired() const override { return 1_word; }

private:
};
} // namespace Token
} // namespace Lexer

#endif