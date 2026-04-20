#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <cassert>

template <typename T, std::size_t Capacity>
struct Slotmap{
    using data_type  = T;
    using index_type = std::uint64_t;
    using gen_type   = index_type;
    using key_type   = struct { index_type id; gen_type gen; };

    constexpr explicit Slotmap() noexcept { clear(); }

    // Add new value to slotmap
    [[nodiscard]] constexpr key_type push_back(data_type&& newVal){
        auto reservedID = allocate();
        auto& slot = m_indices[reservedID];

        // Move data
        m_data[slot.id] = std::move(newVal);
        m_erase[slot.id] = reservedID;

        // Key for the user
        auto key { slot };
        key.id = reservedID;

        return key;
    }

    [[nodiscard]] constexpr key_type push_back(data_type const& newVal){
        return push_back(data_type{ newVal });
    }

    // Erase item at index
    constexpr bool erase(key_type key) noexcept{
        if(!is_valid(key)) return false;
        free(key);

        return true;
    }

    // Check if key is valid index
    [[nodiscard]] constexpr bool is_valid(key_type key) const noexcept{
        if(key.id >= Capacity || m_indices[key.id].gen != key.gen)
            return false;
        
        return true;
    }

    // Clear slotmap
    constexpr void clear() noexcept { freelist_init(); };

    // Getters
    [[nodiscard]] constexpr std::size_t size()     const noexcept { return m_size; }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return Capacity; }

private:
    char const t1[8] = "#SIZE##";
    index_type m_size {};
    char const t2[8] = "#FREEL#";
    index_type m_freelist {};
    char const t3[8] = "#GENER#";
    gen_type   m_generation {};

    char const t4[16] = "#INDEX#########";
    std::array<  key_type, Capacity> m_indices {};
    char const t5[16] = "#DATA##########";
    std::array< data_type, Capacity> m_data {};
    char const t6[16] = "#ERASE#########";
    std::array<index_type, Capacity> m_erase {};

    // Initialize m_indices and m_freelist
    constexpr void freelist_init() noexcept {
        for(index_type i{}; i < m_indices.size();  i++)
            m_indices[i].id = i + 1;

        m_freelist = 0;
    }

    // Return free id of the slotmap
    [[nodiscard]] constexpr index_type allocate(){
        if(m_size >= Capacity) throw std::runtime_error("No more space in the slotmap");
        assert(m_freelist < Capacity);

        // Reserve slot
        auto slotID = m_freelist;
        m_freelist = m_indices[slotID].id;

        // Initialize slot
        auto& slot = m_indices[slotID];
        slot.id = m_size;
        slot.gen = m_generation;

        // Update m_size and m_generation
        m_size += 1;
        m_generation += 1;

        return slotID;
    }

    // Free item from the slotmap
    constexpr void free(key_type key) noexcept{
        assert(is_valid(key));

        auto& slot = m_indices[key.id];
        auto dataID = slot.id;

        // Update  m_freelist
        slot.id = m_freelist;
        slot.gen = m_generation;
        m_freelist = key.id;

        // Update array (data slot is not last, copy last here)
        if(dataID != m_size - 1){
            m_data[dataID] = m_data[m_size - 1];
            m_erase[dataID] = m_erase[m_size - 1];
            m_indices[m_erase[dataID]].id = dataID;
        }

        m_size -= 1;
        m_generation += 1;
    }
};
