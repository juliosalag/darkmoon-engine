#pragma once

#include <array>
#include <cstdint>

template <typename T, std::size_t Capacity>
struct Slotmap{
    using data_type  = T;
    using index_type = std::uint32_t;
    using gen_type   = index_type;
    using key_type   = struct { index_type id; gen_type gen; };

    [[nodiscard]] constexpr std::size_t size()     const noexcept { return m_size; }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return Capacity; }

private:
    index_type m_size {};
    index_type m_freelist {};
    gen_type   m_generation {};
    
    std::array<  key_type, Capacity> m_indices {};
    std::array< data_type, Capacity> m_data {};
    std::array<index_type, Capacity> m_erase {};
};
