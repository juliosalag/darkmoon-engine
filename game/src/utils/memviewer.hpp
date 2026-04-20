#pragma once
#include <cstdint>
#include <cstdio>
#include <type_traits>

constexpr std::size_t default_width{ 16 };

struct MemoryViewer{
    template <typename T>
    MemoryViewer(T const& obj_or_ptr){
        if constexpr (std::is_pointer_v<T>){
            p = reinterpret_cast<std::uint8_t const*>(obj_or_ptr);
            size = sizeof(*obj_or_ptr);
        }
        else{
            p = reinterpret_cast<std::uint8_t const*>(&obj_or_ptr);
            size = sizeof(obj_or_ptr);
        }
    }

    void printMemory(){
        std::printf("##### MOSTRANDO OBJETO. Size: %ld bytes #####\n", static_cast<long>(size));
        showMem(p, size, default_width);
    }

private:
    void showMem(std::uint8_t const* mem, std::size_t size, std::size_t width){
        std::uint16_t cont{};
        auto const lines{ calculateLines(size, width) };
        std::printf("\033[1;48m||   DIRECCIONES   || 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ||    CARACTERES    ||\033[0m\n");
        std::printf("============================================================================================\n");
        for (std::size_t i{}; i < lines; ++i){
            showMemoryLine(mem, cont, width);
            mem += width;
        }
        std::printf("============================================================================================\n");
    }

    std::size_t calculateLines(std::size_t const size, std::size_t const width){
        auto const lines{ size / width };
        if ((size & 0xF) != 0){
            return lines + 1;
        }
        return lines;
    }

    void showMemoryLine(std::uint8_t const* mem, std::uint16_t& cont, std::size_t width){
        std::uint16_t cont2{ cont };
        std::printf("||\033[1;35m%16p\033[0m || ", (void*)mem);
        for (std::size_t i{}; i < width; ++i){
            if (cont < size)
                std::printf("\033[1;32m%02X \033[0m", mem[i]);
            else
                std::printf("\033[1;32m-- \033[0m");
            cont += 1;
        }
        std::printf("|| ");
        for (std::size_t i{}; i < width; ++i){
            auto c = (mem[i] > 31 && mem[i] < 128) ? mem[i] : '.';
            if (cont2 < size)
                std::printf("\033[1;34m%c\033[0m", c);
            else
                std::printf(" ");

            cont2 += 1;
        }
        std::printf(" ||\n");
    }

    std::uint8_t const* p{};
    std::size_t size{};
};