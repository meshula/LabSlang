
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "../abseil-cpp/absl/strings/match.cc"
#include "../abseil-cpp/absl/strings/internal/charconv_bigint.cc"
#include "../abseil-cpp/absl/strings/internal/cord_rep_btree_reader.cc"
#include "../abseil-cpp/absl/strings/internal/cordz_info.cc"
#include "../abseil-cpp/absl/strings/internal/cord_internal.cc"
#include "../abseil-cpp/absl/strings/internal/cordz_sample_token.cc"
#include "../abseil-cpp/absl/strings/internal/cord_rep_consume.cc"
#include "../abseil-cpp/absl/strings/internal/cord_rep_crc.cc"

#include "../abseil-cpp/absl/strings/internal/str_format/arg.cc"
#include "../abseil-cpp/absl/strings/internal/str_format/float_conversion.cc"
#include "../abseil-cpp/absl/strings/internal/str_format/output.cc"
#include "../abseil-cpp/absl/strings/internal/str_format/bind.cc"
#include "../abseil-cpp/absl/strings/internal/str_format/parser.cc"
#include "../abseil-cpp/absl/strings/internal/str_format/extension.cc"

#include "../abseil-cpp/absl/strings/internal/cord_rep_ring.cc"
#include "../abseil-cpp/absl/strings/internal/cordz_handle.cc"
#include "../abseil-cpp/absl/strings/internal/ostringstream.cc"
#include "../abseil-cpp/absl/strings/internal/pow10_helper.cc"
#include "../abseil-cpp/absl/strings/internal/cordz_functions.cc"
#include "../abseil-cpp/absl/strings/internal/cord_rep_btree_navigator.cc"
#include "../abseil-cpp/absl/strings/internal/cord_rep_btree.cc"
#include "../abseil-cpp/absl/strings/string_view.cc"
#include "../abseil-cpp/absl/strings/str_cat.cc"
#include "../abseil-cpp/absl/strings/cord.cc"
#include "../abseil-cpp/absl/strings/charconv.cc"
#include "../abseil-cpp/absl/strings/str_split.cc"
#include "../abseil-cpp/absl/strings/str_replace.cc"
#include "../abseil-cpp/absl/types/bad_any_cast.cc"
#include "../abseil-cpp/absl/types/bad_optional_access.cc"
#include "../abseil-cpp/absl/types/bad_variant_access.cc"
#include "../abseil-cpp/absl/flags/parse.cc"
#include "../abseil-cpp/absl/flags/usage.cc"
#include "../abseil-cpp/absl/flags/internal/private_handle_accessor.cc"
#include "../abseil-cpp/absl/flags/internal/usage.cc"
#include "../abseil-cpp/absl/flags/internal/program_name.cc"
#include "../abseil-cpp/absl/flags/internal/flag.cc"
#include "../abseil-cpp/absl/flags/internal/commandlineflag.cc"
#include "../abseil-cpp/absl/flags/reflection.cc"
#include "../abseil-cpp/absl/flags/usage_config.cc"
#include "../abseil-cpp/absl/flags/flag.cc"
#include "../abseil-cpp/absl/flags/marshalling.cc"
#include "../abseil-cpp/absl/flags/commandlineflag.cc"
#include "../abseil-cpp/absl/synchronization/blocking_counter.cc"
#include "../abseil-cpp/absl/synchronization/mutex.cc"
#include "../abseil-cpp/absl/synchronization/internal/per_thread_sem.cc"
#include "../abseil-cpp/absl/synchronization/internal/create_thread_identity.cc"
#include "../abseil-cpp/absl/synchronization/internal/waiter.cc"
#include "../abseil-cpp/absl/synchronization/barrier.cc"
#include "../abseil-cpp/absl/synchronization/notification.cc"
#include "../abseil-cpp/absl/hash/internal/low_level_hash.cc"
#include "../abseil-cpp/absl/hash/internal/hash.cc"
#include "../abseil-cpp/absl/hash/internal/city.cc"
#include "../abseil-cpp/absl/debugging/symbolize.cc"
#include "../abseil-cpp/absl/debugging/failure_signal_handler.cc"
#include "../abseil-cpp/absl/debugging/leak_check_disable.cc"
#include "../abseil-cpp/absl/debugging/internal/examine_stack.cc"
#include "../abseil-cpp/absl/debugging/internal/vdso_support.cc"
#include "../abseil-cpp/absl/debugging/internal/stack_consumption.cc"
#include "../abseil-cpp/absl/debugging/internal/address_is_readable.cc"
#include "../abseil-cpp/absl/debugging/internal/elf_mem_image.cc"
#include "../abseil-cpp/absl/debugging/internal/demangle.cc"
#include "../abseil-cpp/absl/debugging/leak_check.cc"
#include "../abseil-cpp/absl/debugging/stacktrace.cc"
#include "../abseil-cpp/absl/status/status_payload_printer.cc"
#include "../abseil-cpp/absl/status/status.cc"
#include "../abseil-cpp/absl/status/statusor.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_format.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_impl.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_lookup.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_info.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_if.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_fixed.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/zone_info_source.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/time_zone_libc.cc"
#include "../abseil-cpp/absl/time/internal/cctz/src/civil_time_detail.cc"
#include "../abseil-cpp/absl/time/clock.cc"
#include "../abseil-cpp/absl/time/civil_time.cc"
#include "../abseil-cpp/absl/time/format.cc"
#include "../abseil-cpp/absl/numeric/int128.cc"
#include "../abseil-cpp/absl/random/gaussian_distribution.cc"
#include "../abseil-cpp/absl/random/discrete_distribution.cc"
#include "../abseil-cpp/absl/random/seed_gen_exception.cc"
#include "../abseil-cpp/absl/random/internal/seed_material.cc"
#include "../abseil-cpp/absl/random/internal/randen_slow.cc"
#include "../abseil-cpp/absl/random/internal/randen.cc"
#include "../abseil-cpp/absl/random/internal/randen_detect.cc"
#include "../abseil-cpp/absl/random/internal/randen_round_keys.cc"
#include "../abseil-cpp/absl/random/internal/randen_hwaes.cc"
#include "../abseil-cpp/absl/random/internal/pool_urbg.cc"
#include "../abseil-cpp/absl/random/seed_sequences.cc"
#include "../abseil-cpp/absl/base/internal/spinlock_wait.cc"
#include "../abseil-cpp/absl/base/log_severity.cc"
