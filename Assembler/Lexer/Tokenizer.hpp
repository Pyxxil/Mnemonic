#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <memory>

#include "../Debug.hpp"

#include "../Diagnostic.hpp"
#include "Line.hpp"
#include "Token.hpp"
#include "Tokens.hpp"

namespace Lexer {
namespace Tokenizer {
class Tokenizer;
}
} // namespace Lexer
extern void throwError(Lexer::Tokenizer::Tokenizer *, Diagnostics::Diagnostic);
extern void throwWarning(Lexer::Tokenizer::Tokenizer *,
                         Diagnostics::Diagnostic);

static constexpr size_t hashedLetters[] = {
    100363, 99989, 97711, 97151, 92311, 80147, 82279, 72997,
    66457,  65719, 70957, 50262, 48407, 51151, 41047, 39371,
    35401,  37039, 28697, 27791, 20201, 21523, 6449,  4813,
    16333,  13337, 3571,  5519,  26783, 71471, 68371, 104729};

static constexpr char toUpper(char c) {
  return (c >= 0x61 && c <= 0x7a) ? static_cast<char>(c ^ 0x20) : c;
}

static constexpr bool isAlpha(const size_t c) {
  return (c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A);
}

/*! Create a hash from a string.
 *
 * It should be relatively safe in regards to repeat hashes, as it reduces the
 * number of things that are valid with the two if statements. However, I'm
 * still unsure whether it's smarter to also check if they're the same thing
 * afterwards.
 *
 * @param string The string to hash
 *
 * @return The hash of the string
 */
static constexpr size_t hash(const std::string_view &string) {
  if (string.length() > 9 || string.length() < 2) {
    // There are no registers, directives, or instructions which are longer than
    // 9 characters, or less than 3 characters.
    return {};
  }

  const auto firstCharacter{static_cast<size_t>(toUpper(string.front()))};

  if (firstCharacter != '.' && !isAlpha(firstCharacter)) {
    // Basically, we don't really want something that's likely to be an
    // immediate value, or a label (less likely to be caught here, but may as
    // well try).
    return {};
  }

  size_t hashed{37};

  for (const char character : string) {
    const size_t as_hashed{
        hashedLetters[(static_cast<size_t>(toUpper(character)) - 0x41u) &
                      0x1F]};
    hashed = (hashed * as_hashed) ^ (firstCharacter * as_hashed);
  }

  return hashed;
}

static bool isValidDecimalLiteral(const std::string_view &s) {
  return std::all_of(s.cbegin() + static_cast<size_t>('-' == s.front()),
                     s.cend(), ::isdigit);
}

static bool isValidLabel(const std::string_view &s) {
  return std::all_of('.' == s.front() ? s.cbegin() + 1 : s.cbegin(), s.cend(),
                     [](auto &&c) { return std::isalnum(c) || '_' == c; });
}

namespace Lexer {
class Lexer;

namespace Tokenizer {
class Tokenizer {
public:
  Tokenizer(Lexer &t_lexer, File &t_file) : m_lexer(t_lexer), file(t_file) {}

  std::unique_ptr<Token::Token> tokenizeImmediate(const std::string &s) {
    // We know this is going to be negative, as this is only ever called when
    // the next character on the line is a '-'.
    if (isValidDecimalLiteral(s)) {
      auto decimal{std::make_unique<Token::Decimal>(s, file.position().line(),
                                                    file.position().column(),
                                                    file.name(), true)};
      if (decimal->isTooBig()) {
        throwError(
            this,
            Diagnostics::Diagnostic(
                std::make_unique<Diagnostics::DiagnosticHighlighter>(
                    file.position().column() - s.length(), s.length(),
                    file.line()),
                fmt::format("Decimal literal is too big to fit inside 16 bits"),
                file.name(), file.position().line()));
        return std::make_unique<Token::Token>(
            s, file.position().line(), file.position().column(), file.name());
      }
      return std::move(decimal);
    }

    throwError(
        this,
        Diagnostics::Diagnostic(
            std::make_unique<Diagnostics::DiagnosticHighlighter>(
                file.position().column() - s.length(), s.length(), file.line()),
            fmt::format("Expected Decimal literal, but found {}", s),
            file.name(), file.position().line()));

    return std::make_unique<Token::Token>(
        s, file.position().line(), file.position().column(), file.name());
  }

  std::unique_ptr<Token::Token> tokenizeDirective(const std::string &s) {
    const auto &[column, line] = file.position();
    if (const auto _hash{hash(s)}; _hash > 0) {
      switch (_hash) {
      case ::hash(".DATA"):
        return std::make_unique<Token::Data>(s, line, column, file.name());
      case ::hash(".BEGIN"):
        return std::make_unique<Token::Begin>(s, line, column, file.name());
      case ::hash(".END"):
        return std::make_unique<Token::End>(s, line, column, file.name());
      }
    }

    if (isValidLabel(s)) {
      return std::make_unique<Token::Label>(s, line, column, file.name());
    }

    return std::make_unique<Token::Token>(s, line, column, file.name());
  }

  /*! Tokenize a single word
   *
   * @param s The word to tokenize
   * @return The token the word corresponds to
   */
  std::unique_ptr<Token::Token> tokenize(const std::string &s) {
    const auto &[column, line] = file.position();
    if (const auto _hash = hash(s); _hash > 0) {
      switch (_hash) {
      case ::hash("ADD"):
        return std::make_unique<Token::Add>(s, line, column, file.name());
      case ::hash("CLEAR"):
        return std::make_unique<Token::Clear>(s, line, column, file.name());
      case ::hash("COMPARE"):
        return std::make_unique<Token::Compare>(s, line, column, file.name());
      case ::hash("DECREMENT"):
        return std::make_unique<Token::Decrement>(s, line, column, file.name());
      case ::hash("HALT"):
        return std::make_unique<Token::Halt>(s, line, column, file.name());
      case ::hash("IN"):
        return std::make_unique<Token::In>(s, line, column, file.name());
      case ::hash("INCREMENT"):
        return std::make_unique<Token::Increment>(s, line, column, file.name());
      case ::hash("JUMP"):
        return std::make_unique<Token::Jump>(s, line, column, file.name());
      case ::hash("JUMPEQ"):
        return std::make_unique<Token::Jumpeq>(s, line, column, file.name());
      case ::hash("JUMPGT"):
        return std::make_unique<Token::Jumpgt>(s, line, column, file.name());
      case ::hash("JUMPLT"):
        return std::make_unique<Token::Jumplt>(s, line, column, file.name());
      case ::hash("JUMPNEQ"):
        return std::make_unique<Token::Jumpneq>(s, line, column, file.name());
      case ::hash("LOAD"):
        return std::make_unique<Token::Load>(s, line, column, file.name());
      case ::hash("OUT"):
        return std::make_unique<Token::Out>(s, line, column, file.name());
      case ::hash("STORE"):
        return std::make_unique<Token::Store>(s, line, column, file.name());
      case ::hash("SUBTRACT"):
        return std::make_unique<Token::Subtract>(s, line, column, file.name());
      default:
        break;
      }
    }

    if (isValidDecimalLiteral(s)) {
      return std::make_unique<Token::Decimal>(s, line, column, file.name());
    }

    if (isValidLabel(s)) {
      return std::make_unique<Token::Label>(s, line, column, file.name());
    }

    throwError(this,
               Diagnostics::Diagnostic(
                   std::make_unique<Diagnostics::DiagnosticHighlighter>(
                       column, s.length(), file.line()),
                   fmt::format("Invalid token: {}", s), file.name(), line));
    return std::make_unique<Token::Token>(s, line, column, file.name());
  }

  auto extraneous(char character) -> void {
    throwWarning(this, Diagnostics::Diagnostic(
                           std::make_unique<Diagnostics::DiagnosticHighlighter>(
                               file.position().column(), 0, file.line()),
                           fmt::format("Extraneous '{}' found.", character),
                           file.name(), file.position().line()));
  }

  std::vector<std::unique_ptr<Token::Token>> tokenizeLine(Line line) {
    std::vector<std::unique_ptr<Token::Token>> lTokens{};

    while (!line.atEnd()) {
      line.skip_while([](auto &&c) -> bool { return std::isspace(c); });

      const auto token_start{line.index()};
      const auto token_end{line.find_if([](auto &&c) -> bool {
        return !(std::isalnum(c) || '_' == c || '-' == c);
      })};

      file.setColumn(token_start);

      if (auto &&token{line.substr(token_start, token_end)};
          0 == token.size()) {
        DEBUG("FOUND '{}'", token);
        switch (const auto next{line.next()}; next) {
        case '.': {
          // Now we tokenize a directive
          // const auto dir_start = line.index();
          token = line.substr(token_end, line.find_if([](auto &&c) -> bool {
            return !(std::isalnum(c));
          }));
          lTokens.emplace_back(tokenizeDirective(token));
          break;
        }
        case '-': { // Negative immediate (hopefully)
          token = line.substr(line.index(), line.find_if([](auto &&c) -> bool {
            return !(std::isalnum(c));
          }));
          if (token.size() > 0) {
            auto &&tok{tokenizeImmediate(token)};
            lTokens.emplace_back(std::move(tok));
          } else {
            throwError(this,
                       Diagnostics::Diagnostic(
                           std::make_unique<Diagnostics::DiagnosticHighlighter>(
                               file.position().column(), 0, file.line()),
                           "Extraneous '-' found.", file.name(),
                           file.position().line()));
          }
          break;
        } break;
        case '/':
          // Both mean a comment (though, technically, a '//' is a
          // comment, but '/' is not used for anything.)
          file.setColumn(line.index() - 1);
          if ('/' != line.next()) {
            throwWarning(
                this, Diagnostics::Diagnostic(
                          std::make_unique<Diagnostics::DiagnosticHighlighter>(
                              file.position().column(), 0, file.line()),
                          "Found '/', acting as if it's supposed to be '//'",
                          file.name(), file.position().line()));
          }
          [[fallthrough]];
        case ';':
#ifdef KEEP_COMMENTS
          lTokens.emplace_back(std::make_unique<Token::Comment>(
              line.substr(line.index(), -1), file.position().line(),
              file.position().column(), file.name()));
#endif
          line.skip_while([](auto &&) -> bool { return true; });
          break;
        case ':': {
          break;
        }
        default:
          break;
        }
      } else {
        if (auto &&tToken{tokenize(token)};
            tToken->tokenType() == TokenType::DECIMAL && tToken->isTooBig()) {
          throwError(
              this,
              Diagnostics::Diagnostic(
                  std::make_unique<Diagnostics::DiagnosticHighlighter>(
                      token_start, tToken->getToken().length(), file.line()),
                  fmt::format("Immediate literal requires more than 16 "
                              "bits to represent"),
                  file.name(), file.position().line()));
          lTokens.emplace_back(std::make_unique<Token::Token>(
              token, file.position().line(), file.position().column(),
              file.name()));
        } else {
          lTokens.emplace_back(std::move(tToken));
        }
      }
    }

    return lTokens;
  }

  Lexer &lexer() const { return m_lexer; }

private:
  Lexer &m_lexer;
  File &file;
}; // namespace Tokenizer
} // namespace Tokenizer
} // namespace Lexer

#endif
