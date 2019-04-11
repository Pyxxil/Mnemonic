#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <functional>
#include <vector>

namespace Algorithm {
template <class InputIterator, class T>
T accumulate2d(InputIterator first, InputIterator last, T init) {
  T sum = init;
  for (; first != last; ++first) {
    for (auto iter = first.begin(); iter != first.end(); ++iter) {
      sum += *iter;
    }
  }
  return sum;
}
template <class InputIterator, class T, class Function>
T accumulate2d(InputIterator first, InputIterator last, T init, Function f) {
  T sum = init;
  for (; first != last; ++first) {
    for (auto iter = first->begin(); iter != first->end(); ++iter) {
      sum = f(sum, *iter);
    }
  }
  return sum;
}

template <class InputIterator, class Function>
Function enumerate(InputIterator first, InputIterator last, Function f) {
  for (size_t pos = 0; first != last; ++first, ++pos) {
    f(*first, pos);
  }

  return f;
}

template <class InputIterator, class Function, class _Function>
_Function first(InputIterator first, InputIterator last, Function s,
                _Function f) {
  s(*first, 0);
  ++first;

  for (size_t pos = 1; first != last; ++first, ++pos) {
    f(*first, pos);
  }

  return f;
}

template <class InputIterator, class Function, class _Function>
_Function nth(size_t idx, InputIterator first, InputIterator last, Function s,
              _Function f) {
  for (size_t pos = 0; first != last; ++first, ++pos) {
    if (idx == pos) {
      s(*first, pos);
    }
    f(*first, pos);
  }

  return f;
}

template <class InputIterator, class Function, class _Function>
_Function after(size_t idx, InputIterator first, InputIterator last, Function s,
                _Function f) {
  size_t pos = 0;
  for (; pos <= idx && first != last; ++first, ++pos) {
    f(*first, pos);
  }

  for (; first != last; ++first, ++pos) {
    s(*first, pos);
    f(*first, pos);
  }

  return f;
}

template <class InputIterator> class Enumerator {
public:
  Enumerator(InputIterator f, InputIterator l)
      : m_first{f}, m_last{l}, m_length{m_first - m_last} {}

  Enumerator(const Enumerator &) = default;
  Enumerator(Enumerator &&) noexcept = default;

  Enumerator &operator=(const Enumerator &) = default;
  Enumerator &operator=(Enumerator &&) noexcept = default;

  ~Enumerator() = default;

  template <typename Function> Enumerator &nth(size_t idx, Function f) {
    (void)idx;
    (void)f;
    return *this;
  }

  template <typename Function> Enumerator &first(Function f) {
    (void)f;
    return *this;
  }

  template <typename Function> Enumerator &every(size_t idx, Function f) {
    for (InputIterator iter = m_first; iter < m_last; iter += idx) {
      f(*iter);
    }
    return *this;
  }

private:
  InputIterator m_first;
  InputIterator m_last;
  typename InputIterator::difference_type m_length;
};
} // namespace Algorithm

#endif
