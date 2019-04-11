#ifndef TOKEN_LABEL_HPP
#define TOKEN_LABEL_HPP

#include "Token.hpp"

namespace Lexer {
namespace Token {
class Label : public Token {
public:
  explicit Label(std::string s, size_t tLine, size_t tColumn,
                 const std::string &tFile)
      : Token(std::move(s), tLine, tColumn, tFile) {}

  Label(const Label &) = default;
  Label(Label &&) = default;

  Label &operator=(const Label &) = default;
  Label &operator=(Label &&) = default;

  TokenType tokenType() const final { return LABEL; }

  word memoryRequired() const override { return 0_word; }

private:
};
} // namespace Token
} // namespace Lexer

#endif
