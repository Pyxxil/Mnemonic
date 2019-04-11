#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <functional>
#include <string>
#include <vector>

#include "Console.hpp"
#include "Diagnostic.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "fmt/ostream.h"
#pragma GCC diagnostic pop

#include "fmt/printf.h"

namespace Notification {

using Callback_Name_t = std::string;
using Callback_Func_t = std::function<void(const Callback_Name_t &,
                                           const Diagnostics::Diagnostic &)>;

class Callback {
public:
  Callback(Callback_Name_t t_name, Callback_Func_t f, bool want_prev = true,
           bool update = true)
      : callback(std::make_pair<Callback_Name_t, Callback_Func_t>(
            std::move(t_name), std::move(f))),
        want_previous(want_prev), update_on_each(update) {}
  Callback(const Callback &c) = default;
  Callback(Callback &&c) noexcept = default;

  Callback &operator=(const Callback &c) = default;
  Callback &operator=(Callback &&c) noexcept = default;

  ~Callback() = default;

  void operator()(const Diagnostics::Diagnostic &diagnostic) const {
    callback.second(name(), diagnostic);
  }

  auto name() const -> const Callback_Name_t & { return callback.first; }

  auto wants_previous() const { return want_previous; }
  auto wants_updates() const { return update_on_each; }

  template <typename... Args> void warn(const char *fmt, Args &&... args) {
    fmt::print("{0:s}\n", fmt::format(fmt, args...));
  }

  template <typename... Args> void error(const char *fmt, Args &&... args) {
    fmt::print("{0:s}\n", fmt::format(fmt, args...));
  }

private:
  std::pair<Callback_Name_t, Callback_Func_t> callback;
  bool want_previous;
  bool update_on_each;
};
} // namespace Notification

#endif
