#pragma once

#include "unique_ptr.hpp"

namespace dsacpp
{

template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) noexcept {
   return unique_ptr<T>{ new T(std::forward<Args>(args)...) };
}

template<typename T>
unique_ptr<T> make_unique_for_overwrite() noexcept {
   return unique_ptr<T>{ new T };
}

template<typename T>
unique_ptr<T[]> make_unique(size_t size) noexcept {
   return unique_ptr<T[]>{ new T[size]{ } };
}

template<typename T>
unique_ptr<T[]> make_unique_for_overwrite(size_t size) noexcept {
   return unique_ptr<T[]>{ new T[size] };
}

} // namespace dsacpp