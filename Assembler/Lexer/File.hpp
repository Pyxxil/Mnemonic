#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <vector>

namespace Lexer {

struct Position {
public:
  Position(size_t column = 0, size_t line = 0);

  Position(const Position &other) = default;
  Position(Position &&other) noexcept = default;

  Position &operator=(const Position &other) = default;
  Position &operator=(Position &&other) = default;

  ~Position() = default;

  size_t column() const;
  size_t line() const;

  void incColumn();
  void setColumn(size_t column);
  void incLine();

  size_t mColumn;
  size_t mLine;
};

class File {
public:
  File() = default;
  explicit File(std::string file_name);

  File(const File &other) = default;
  File(File &&other) = default;

  File &operator=(const File &other) = default;
  File &operator=(File &&other) = default;

  ~File() = default;

  const std::string &name() const { return mFileName; }
  const Position &position() const { return mPosition; }
  std::string line() const;

  void setColumn(size_t column);

  bool nextLine();

  void nextColumn();
  const std::string &line(size_t l) const;

  bool isFailure() const;

private:
  std::string mFileName;
  std::ifstream mFile;
  Position mPosition{};
  std::vector<std::string> lines;
};
} // namespace Lexer

#endif
