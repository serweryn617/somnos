#ifndef MISC_H
#define MISC_H

#include <type_traits>

namespace misc {

template<typename T>
constexpr auto to_underlying_type(T v) {
    return static_cast<std::underlying_type_t<T>>(v);
}

}  // namespace misc

#endif  // MISC_H