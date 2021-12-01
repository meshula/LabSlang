
// inspired by https://github.com/mlogan/header-only-abseil
// as a workaround to the incredibly slow abseil build

#define WIN32_LEAN_AND_MEAN

#include "../abseil-cpp/absl/strings/substitute.cc"
#include "../abseil-cpp/absl/strings/numbers.cc"
#include "../abseil-cpp/absl/strings/ascii.cc"
#include "../abseil-cpp/absl/strings/internal/memutil.cc"
#include "../abseil-cpp/absl/strings/internal/utf8.cc"
#include "../abseil-cpp/absl/strings/escaping.cc"
#include "../abseil-cpp/absl/strings/internal/escaping.cc"
#include "../abseil-cpp/absl/base/internal/raw_logging.cc"
