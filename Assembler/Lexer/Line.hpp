#ifndef LINE_HPP
#define LINE_HPP

#include <array>
#include <functional>
#include <string>

#include "../Debug.hpp"

namespace Lexer {
class Line {
public:
  explicit Line(std::string_view t_line) : m_line(t_line) {}

  enum IGNORES {
    RESET = 0,
    ESCAPE_SEQUENCE = 1,
  };

  /*! Ignore specific things about the file while finding something
   *
   * @param to_ignore The things to ignore.
   */
  void ignore(size_t to_ignore) {
    m_ignores = (RESET == to_ignore) ? 0 : m_ignores | to_ignore;
  }

  /*! Take a look at the next character in the line.
   *
   * @return The next character in the line, unless we're at the end of the
   * line, in which case 0.
   */
  inline auto peek() const { return at(m_index); }

  /*! Retrieve the next character from the string
   *
   * @return The character read if we're not at the end of the string, 0
   * otherwise.
   */
  auto next() {
    const auto c = peek();

    if (0 != c) {
      ++m_index;
    }

    return c;
  }

  /*! Skip the next character
   *
   */
  inline void skip() { ++m_index; }

  /*! Keep advancing until pred becomes false.
   *
   * @param func The function to call to check if we should stop.
   */
  template <typename Func> void skip_while(Func &&func) {
    while (!atEnd()) {
      if (!func(peek())) {
        return;
      }
      skip();
    }
  }

  /*! Attempt to find a character in the string
   *
   * @param needle The character to find
   *
   * @return The index of the character, or -1 if it wasn't found
   */
  auto find_next(char needle) {
    while (!atEnd()) {
      if (needle == peek()) {
        if (ignores[m_ignores](*this, needle)) {
          return m_index++;
        }
      }
      skip();
    }

    // Couldn't find it
    return static_cast<size_t>(-1u);
  }

  /*! Keep searching until pred becomes true
   *
   * @param func The function to call to see if the current character matches
   * some condition
   *
   * @returns The index of the character if pred is ever true, -1 otherwise.
   */
  template <typename Func> auto find_if(Func &&func) {
    while (!atEnd()) {
      if (func(peek())) {
        return m_index;
      }
      skip();
    }

    // Didn't find it
    return static_cast<size_t>(-1u);
  }

  /*! Grab a sub string from within the string
   *
   * @param begin The beginning column of the string
   * @param end The end column for the string
   *
   * @return The substring in the line.
   */
  auto substr(size_t begin, size_t end) const {
    if (end == -1u) {
      return m_line.substr(begin);
    }

    return m_line.substr(begin, end - begin);
  }

  /*! Check if we've reached the end of the string
   *
   * @return true if we have reached the end of the string, otherwise false.
   */
  inline bool atEnd() const { return m_index >= m_line.length(); }

  /*! Return the character at a specified index (doesn't do bounds checking)
   *
   * @param index The index of the character
   *
   * @return The character at the index
   */
  inline char at(size_t index) const {
    return static_cast<char>((index >= m_line.length()) ? 0 : (*this)[index]);
  }

  inline char operator[](size_t index) const { return m_line[index]; }

  const Line &operator>>(char &c) {
    c = next();
    return *this;
  }

  inline size_t index() const { return m_index; }

  const auto &line() const { return m_line; }

private:
  std::string m_line{};
  size_t m_index{};
  size_t m_ignores{};

  static const std::array<std::function<bool(const Line &, char)>, 2> ignores;
};

const std::array<std::function<bool(const Line &, char)>, 2> Line::ignores{
    [](const Line &, char) -> bool { return true; },
    [](const Line &l, char) -> bool { return l.at(l.index() - 1) != '\\'; }};
} // namespace Lexer

#endif
