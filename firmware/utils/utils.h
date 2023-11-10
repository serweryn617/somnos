#ifndef UTILS_H
#define UTILS_H

#include <type_traits>

namespace utils {

template<typename T>
constexpr auto underlying_type(T v)
{
    return static_cast<std::underlying_type_t<T>>(v);
}

}  // namespace utils

#endif  // UTILS_H