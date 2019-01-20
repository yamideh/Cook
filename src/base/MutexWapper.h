#include <mutex>

struct MutexWrapper : std::mutex
{
  MutexWrapper() = default;
  MutexWrapper(MutexWrapper const&) noexcept : std::mutex() {}
  bool operator==(MutexWrapper const&other) noexcept { return this==&other; }
};