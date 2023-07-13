
// inspired by https://github.com/mlogan/header-only-abseil
// as a workaround to the incredibly slow abseil build

#define WIN32_LEAN_AND_MEAN

#include "../abseil-cpp/absl/strings/substitute.cc"
#include "../abseil-cpp/absl/strings/numbers.cc"
#include "../abseil-cpp/absl/strings/ascii.cc"
#include "../abseil-cpp/absl/strings/internal/memutil.cc"
#include "../abseil-cpp/absl/strings/internal/utf8.cc"
#include "../abseil-cpp/absl/strings/escaping.cc"
#include "../abseil-cpp/absl/strings/internal/damerau_levenshtein_distance.cc"
#include "../abseil-cpp/absl/strings/internal/escaping.cc"

#include "../abseil-cpp/absl/base/internal/raw_logging.cc"
#include "../abseil-cpp/absl/crc/internal/crc_cord_state.cc"
#include "../abseil-cpp/absl/random/internal/randen_hwaes.cc"

#include "../abseil-cpp/absl/synchronization/internal/pthread_waiter.cc"
#include "../abseil-cpp/absl/synchronization/internal/waiter_base.cc"
#include "../abseil-cpp/absl/strings/internal/stringify_sink.cc"
#include "../abseil-cpp/absl/crc/crc32c.cc"
#include "../abseil-cpp/absl/crc/internal/crc.cc"
#include "../abseil-cpp/absl/crc/internal/crc_x86_arm_combined.cc"
#include "../abseil-cpp/absl/crc/internal/crc_memcpy_fallback.cc"
#include "../abseil-cpp/absl/crc/internal/crc_memcpy_x86_64.cc"
#include "../abseil-cpp/absl/crc/internal/cpu_detect.cc"
