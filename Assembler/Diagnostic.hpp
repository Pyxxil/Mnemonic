#ifndef DIAGNOSTIC_HPP
#define DIAGNOSTIC_HPP

#include <iostream>
#include <memory>
#include <utility>

#include "Console.hpp"

namespace Diagnostics {

class DiagnosticHighlighter {
public:
  DiagnosticHighlighter(size_t tColumn, size_t tLength, std::string tContext)
      : mContext(std::move(tContext)), mColumn(tColumn), mLength(tLength) {
    if (!mContext.empty()) {
      mHighlighter.push_back('^');
      if (length() > 0) {
        mHighlighter += std::string(length() - 1, '~');
      }
    }
  }

  auto column() const -> size_t { return mColumn; }
  auto length() const -> size_t { return mLength; }

  template <typename OStream>
  friend OStream &operator<<(OStream &os,
                             const DiagnosticHighlighter &Highlighter) {
    using namespace Console;
    return os << Highlighter.mContext << '\n'
              << std::string(Highlighter.column(), ' ')
              << Colour(FOREGROUND_COLOUR::MAGENTA) << Highlighter.mHighlighter
              << reset;
  }

protected:
  std::string mHighlighter{};
  std::string mContext;
  size_t mColumn;
  size_t mLength;
};

class Diagnostic {
public:
  Diagnostic() = default;
  explicit Diagnostic(std::unique_ptr<DiagnosticHighlighter> highlighter,
                      std::string t_message, const std::string &file_name,
                      size_t at_line)
      : mHighlighter(std::move(highlighter)), mMessage(std::move(t_message)),
        mFile(file_name), mLine(at_line) {}

  Diagnostic(const Diagnostic &) = default;
  Diagnostic(Diagnostic &&) noexcept = default;

  Diagnostic &operator=(const Diagnostic &) = default;
  Diagnostic &operator=(Diagnostic &&) noexcept = default;

  auto line() const { return mLine; }
  auto column() const { return mHighlighter->column(); }
  const auto &message() const { return mMessage; }
  const auto &file() const { return mFile; }

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Diagnostic &diagnostic) {
    using namespace Console;
    return os << Colour(FOREGROUND_COLOUR::GREEN) << diagnostic.file() << reset
              << ':' << diagnostic.line() << ':' << diagnostic.column() << ": "
              << diagnostic.message() << '\n'
              << *diagnostic.mHighlighter;
  }

private:
  std::unique_ptr<DiagnosticHighlighter> mHighlighter;
  std::string mMessage;
  std::string mFile;
  size_t mLine;
};
} // namespace Diagnostics

#endif
