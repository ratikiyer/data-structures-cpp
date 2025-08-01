#pragma once

#include <climits>
#include <cstddef>
#include <array>
#include <cstdint>

namespace dsacpp
{

template<size_t N>
class bitset {
public:

   /**
    * Type declarations
    */
   
   

   /**
    * Constructors
    */

private:

   static constexpr size_t BITS_PER_WORD = CHAR_BIT * sizeof(uint64_t);
   static constexpr size_t WORD_COUNT = (N + BITS_PER_WORD - 1) / BITS_PER_WORD;

   std::array<uint64_t, WORD_COUNT> arr;
};

} // namespace dsacpp