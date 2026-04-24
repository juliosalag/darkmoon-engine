#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <cassert>

template <typename T, std::size_t Capacity = 10, typename INDEXT = std::uint32_t>
struct Slotmap{
    using data_type  = T;
    using index_type = INDEXT;
    using gen_type   = index_type;
    using key_type   = struct { index_type id; gen_type gen; };
    using iterator   = data_type*;
    using const_iterator = data_type const*;

    constexpr explicit Slotmap() noexcept { clear(); }

    // Operator []
    [[nodiscard]] data_type& operator[](key_type key) noexcept{
        assert(is_valid(key));
        return m_data[m_indices[key.id].id];
    }

    [[nodiscard]] data_type const& operator[](key_type key) const noexcept{
        assert(is_valid(key));
        return m_data[m_indices[key.id].id];
    }

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

    [[nodiscard]] constexpr iterator begin() noexcept { return m_data.begin(); }
    [[nodiscard]] constexpr iterator end()   noexcept { return m_data.begin() + m_size; }
    [[nodiscard]] constexpr iterator cbegin() const noexcept { return m_data.begin(); }
    [[nodiscard]] constexpr iterator cend()   const noexcept { return m_data.begin() + m_size; }

    // Getters
    [[nodiscard]] constexpr std::size_t size()     const noexcept { return m_size; }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return Capacity; }

private:
    index_type m_size {};
    index_type m_freelist {};
    gen_type   m_generation {};

    std::array<  key_type, Capacity> m_indices {};
    std::array< data_type, Capacity> m_data {};
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
            m_data[dataID] = std::move(m_data[m_size - 1]);
            m_erase[dataID] = m_erase[m_size - 1];
            m_indices[m_erase[dataID]].id = dataID;
        }

        m_size -= 1;
        m_generation += 1;
    }
};
