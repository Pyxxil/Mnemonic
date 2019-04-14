#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>

#include "Lexer.hpp"
#include "Tokens.hpp"

#include <iostream>

namespace Parser {
class Parser {
public:
  Parser(std::vector<std::unique_ptr<Lexer::Token::Token>> tokens)
      : mTokens(std::move(tokens)) {}

  void parse() {
    uint16_t currentAddress{0};
    bool originSeen{false};
    bool endSeen{false};

    for (auto &token : mTokens) {
      switch (token->tokenType()) {
      case Lexer::TokenType::LABEL: {
        if (!originSeen) {
          error();
          break;
        } else if (endSeen) {
          Notification::warning_notifications << Diagnostics::Diagnostic(
              std::make_unique<Diagnostics::DiagnosticHighlighter>(
                  token->column(), token->getToken().length(), std::string{}),
              "Label found after .END directive, ignoring.", token->file(),
              token->line());
          break;
        }
        for (auto &&[_, symbol] : mSymbols) {
          if (symbol.address() == currentAddress) {
            Notification::error_notifications << Diagnostics::Diagnostic(
                std::make_unique<Diagnostics::DiagnosticHighlighter>(
                    token->column(), token->getToken().length(), std::string{}),
                "Multiple labels found for address", token->file(),
                token->line());
            Notification::error_notifications << Diagnostics::Diagnostic(
                std::make_unique<Diagnostics::DiagnosticHighlighter>(
                    symbol.column(), symbol.name().length(), std::string{}),
                "Previous label found here", symbol.file(), symbol.line());
          }
        }

        auto &&[symbol, inserted] = mSymbols.try_emplace(
            token->getToken(),
            Lexer::Symbol(token->getToken(), currentAddress, token->file(),
                          token->column(), token->line()));
        if (!inserted) {
          // TODO: Fix the way these are handled. At the moment, any errors
          // TODO: thrown here from labels that have been included (from the
          // TODO: same file) won't actually be useful due to the fact that it
          // TODO: doesn't tell the user where the .include was found.
          auto &&sym = symbol->second;
          Notification::error_notifications
              << Diagnostics::Diagnostic(
                     std::make_unique<Diagnostics::DiagnosticHighlighter>(
                         token->column(), token->getToken().length(),
                         std::string{}),
                     "Multiple definitions of label", token->file(),
                     token->line())
              << Diagnostics::Diagnostic(
                     std::make_unique<Diagnostics::DiagnosticHighlighter>(
                         sym.column(), sym.name().length(), std::string{}),
                     "Previous definition found here", sym.file(), sym.line());
        } else {
          longestSymbolLength =
              std::max(longestSymbolLength,
                       static_cast<int>(token->getToken().length()));
        }
        break;
      }
      case Lexer::TokenType::BEGIN:
        if (originSeen) {
          error();
        } else {
          originSeen = true;
        }
        break;
      case Lexer::TokenType::END:
        break;
      default:
        if (!originSeen) {
          error();
        } else if (endSeen) {
          Notification::warning_notifications << Diagnostics::Diagnostic(
              std::make_unique<Diagnostics::DiagnosticHighlighter>(
                  token->column(), token->getToken().length(), std::string{}),
              "Extra .END directive found.", token->file(), token->line());
        } else {
          word memoryRequired = token->memoryRequired();
          if (memoryRequired == -1) {
            error();
          } else if (memoryRequired > 0) {
            currentAddress += static_cast<uint16_t>(memoryRequired);
          }
        }
        break;
      }
    }
  }

  void error() { ++errorCount; }
  void warning() {}

  const auto &tokens() const { return mTokens; }
  const auto &symbols() const { return mSymbols; }

  auto isOkay() const { return errorCount == 0; }

private:
  std::vector<std::unique_ptr<Lexer::Token::Token>> mTokens;
  std::map<std::string, Lexer::Symbol> mSymbols{};
  size_t errorCount{0};
  int longestSymbolLength{20};
}; // namespace Parser
} // namespace Parser

#endif
