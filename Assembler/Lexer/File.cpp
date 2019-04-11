#include "File.hpp"

namespace Lexer {

Position::Position(size_t column, size_t line) : mColumn(column), mLine(line) {}

size_t Position::column() const { return mColumn; }

size_t Position::line() const { return mLine; }

void Position::incColumn() { ++mColumn; }

void Position::setColumn(size_t column) { mColumn = column; }

void Position::incLine() { ++mLine; }

File::File(std::string file_name) : mFileName(std::move(file_name)), mFile() {
  mFile.open(mFileName);

  if (!isFailure()) {
    std::string line;
    while (std::getline(mFile, line)) {
      lines.emplace_back(std::move(line));
    }
  }
}

std::string File::line() const {
  if (lines.size() <= position().line() - 1)
    return {};
  return lines[position().line() - 1];
}

void File::setColumn(size_t column) { mPosition.setColumn(column); }

bool File::nextLine() {
  mPosition.incLine();
  return mPosition.line() <= lines.size();
}

void File::nextColumn() { mPosition.incColumn(); }

const std::string &File::line(size_t l) const { return lines[l]; }

bool File::isFailure() const {
  return (mFile.fail() && lines.empty()) || !mFile.is_open();
}

} // namespace Lexer